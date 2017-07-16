#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kfifo.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/cdev.h>

#include <linux/kthread.h>
#include <linux/gpio.h>
#include <asm/gpio.h>
#include <linux/string.h>

#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include "ultrasonic_sensor.h"

MODULE_DESCRIPTION("Misc Device Driver for Ultrasonic sensor HCSR-04");
MODULE_AUTHOR("Vigneshwer Vaidyanathan and Chaitanya Palaka");
MODULE_LICENSE("GPL");

#define SETPINS 0
#define SETMODE 1

// Delay (in seconds) for how long thread should sleep in while loops
#define WHILE_DELAY 1
#define BUFFER_LEN 5
#define BUFFER_TYPE s64

// sysfs interface names
#define DEVICE_NAME1 "HCSRdevice1"
#define DEVICE_NAME2 "HCSRdevice2"
#define DEV_NAME1 "HCSR_1"
#define DEV_NAME2 "HCSR_2"
#define CLASS_NAME "HCSR"

static long sec_to_usec = 1000000;
static struct cdev *hcsr_cdev;//1;, *hcsr_cdev2;
static dev_t hcsr_dev;//1 hcsr_dev2;
static struct class *hcsr_class;
static struct device *hcsr_device1;
static struct device *hcsr_device2;

/*
	FUNCTION PROTOTYPES
*/

static int __init ultrasonic_init(void);
static void __exit ultrasonic_exit(void);
static int ultrasonic_open(struct inode *inode, struct file *file);
static ssize_t ultrasonic_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos);
static ssize_t ultrasonic_read(struct file *file, char *buf, size_t len, loff_t *ppos);
static int ultrasonic_release(struct inode *inode, struct file *file);
static long ultrasonic_ioctl(struct file *file, unsigned int ioctlnum, unsigned long arg);
static irq_handler_t echo_handler(int irq, void *dev_id, struct pt_regs *regs);
int take_measurement(void * data);
static ssize_t hcsr_trigger_show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t hcsr_echo_show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t hcsr_mode_show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t hcsr_mode_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
static ssize_t hcsr_frequency_show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t hcsr_enable_show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t hcsr_enable_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
static ssize_t hcsr_distance_show(struct device *dev, struct device_attribute *attr, char *buf);

/*
	GLOBAL VARIABLES
*/

static struct file_operations ultrasonic_fops = {
	.owner = THIS_MODULE,
	.write = ultrasonic_write,
	.open = ultrasonic_open,
	.read = ultrasonic_read,
	.release = ultrasonic_release,
	.unlocked_ioctl = ultrasonic_ioctl
};

struct hcsr_platform_data sensor1data = {
	.devno = 1,
};

struct hcsr_platform_data sensor2data = {
	.devno = 2,
};

static struct sensor_dev sensor1 = {
	.name = "HCSR1",
	.plf_dev = {
		.name = "HCSR_1",
		.id = -1,
		.dev = {
			.platform_data = &sensor1data,
		},
	},
	.device_minor_no = -1,
};

static struct sensor_dev sensor2 = {
	.name = "HCSR2",
	.plf_dev = {
		.name = "HCSR_2",
		.id = -1,
		.dev = {
			.platform_data = &sensor2data,
		},
	},
	.device_minor_no = -1,
};

/*
	USERSPACE DEV INTERFACE FUNCTIONS
*/

static int ultrasonic_open(struct inode *inode, struct file *file)
{
	int minor_no;
	struct device_data * dev_data = NULL;
	minor_no = iminor(file->f_path.dentry->d_inode);
	pr_info("OPEN:minorno in open:%d", minor_no);
	pr_info("OPEN:sensor1mno:%d", sensor1.device_minor_no);
	pr_info("OPEN:sesor2mno:%d", sensor2.device_minor_no);
	if (sensor1.device_minor_no == minor_no) dev_data = &sensor1.dev_data;
	else if (sensor2.device_minor_no == minor_no) dev_data = &sensor2.dev_data;
	if (!dev_data) return -1;
	// Should technically never reach here, but just to be sure
	dev_data->echo_pin = -1;
	dev_data->trigger_pin = -1;
	dev_data->frequency = -1;
	dev_data->is_measuring = 0;
	dev_data->mode = NOT_SET;
	dev_data->sampling_thread = NULL;
	INIT_KFIFO(dev_data->buffer);
	spin_lock_init(&dev_data->spin_lock);
	file->private_data = (void*) minor_no;
	printk(KERN_DEBUG "HCSR: Device has been opened. minor num:%d\n", minor_no);

	return 0;
}

static ssize_t ultrasonic_write(struct file *file, const char __user *buf,
			size_t len, loff_t *ppos)
{
	int arg[2], u_ret, minor_no;
	struct device_data * dev_data = NULL;
	u_ret = copy_from_user(&arg, (int *) buf, sizeof(int) * 2);
	minor_no = iminor(file->f_path.dentry->d_inode);
	if (sensor1.device_minor_no == minor_no) dev_data = &sensor1.dev_data;
	else if (sensor2.device_minor_no == minor_no) dev_data = &sensor2.dev_data;
	if (!dev_data) return -1;
	switch (dev_data->mode) {
	case ONE_SHOT:
		if (arg[0] != 0) {
			// Clear the buffer
			printk(KERN_DEBUG "HCSR: Non-zero first argument in ONE_SHOT mode. Buffer is cleared.");
			kfifo_reset(&dev_data->buffer);
		}
		// If no sampling is being done, do sampling
		if (!dev_data->is_measuring) {
			take_measurement((void *) dev_data);
		}
		break;
	case PERIODIC_SAMPLING:
		if (arg[0] == 0) {
			if (dev_data->sampling_thread) {
				// Stop the sampling if argument is 0
				printk(KERN_DEBUG "HCSR: Stop sampling thread.");
				kthread_stop(dev_data->sampling_thread);
			}
		} else if (arg[0] == 1) {
			// Start the sampling if argument is 1
			printk(KERN_DEBUG "HCSR: Starting sampling thread.");
			dev_data->sampling_thread = kthread_run(&take_measurement, (void *) dev_data, "sampling_thread");
		}
		break;
	default:
		break;
	}
  return 0;
}

static irq_handler_t echo_handler(int irq, void *dev_id, struct pt_regs *regs) {
	/* Check if ECHOs value is high (rising edge) or low (falling edge) and do calculations */
	int ret;
	long long time_difference, unused_object;
	struct device_data * dev_data;
	dev_data = (struct device_data *) dev_id;
	printk(KERN_DEBUG "HCSR: Interrupt handler has been called.");
	if (gpio_get_value(dev_data->echo_pin)) {
		dev_data->start_time = ktime_get();
		irq_set_irq_type(dev_data->irq_handler, IRQF_TRIGGER_FALLING); // Set irq_echo to trigger at a falling edge      
	}
	else {
		dev_data->end_time = ktime_get();
		irq_set_irq_type(dev_data->irq_handler, IRQF_TRIGGER_RISING); // Set irq_echo to trigger at next rising edge
		time_difference = ktime_to_ns(ktime_sub(dev_data->end_time, dev_data->start_time));
		spin_lock(&dev_data->spin_lock);
		if (kfifo_len(&dev_data->buffer) == BUFFER_LEN) {
			printk(KERN_DEBUG "HCSR: Buffer is at MAX_LEN. Removing the first element from the buffer.");
			ret = kfifo_out(&dev_data->buffer, &unused_object, 1);
		}
		kfifo_in(&dev_data->buffer, &time_difference, 1);
		spin_unlock(&dev_data->spin_lock);
		dev_data->last_distance = time_difference;
		printk(KERN_DEBUG "HCSR: Measurement is %llu", time_difference);
	}
	return (irq_handler_t) IRQ_HANDLED;
}

int take_measurement(void * data)
{
  struct device_data * dev_data;
  dev_data = (struct device_data *) data;
	dev_data->is_measuring = 1;
	printk(KERN_DEBUG "HCSR: Inside measurement thread.");
	do {
		// 1. Set TRIGGER to HIGH
		gpio_set_value_cansleep(dev_data->trigger_pin, 1);
		// 2. Delay for 10 microseconds
		// Use udelay rather than usleep on smaller microseconds
		// Read: https://www.kernel.org/doc/Documentation/timers/timers-howto.txt
		udelay(10);
		// 3. Set TRIGGER to LOW again
		gpio_set_value_cansleep(dev_data->trigger_pin, 0);
		if (dev_data->mode != PERIODIC_SAMPLING) break;
		// Sleep for approximately ~dev_data->frequency
		usleep_range(dev_data->frequency, dev_data->frequency + 500);
	} while(!kthread_should_stop() && dev_data->mode == PERIODIC_SAMPLING);
	dev_data->is_measuring = 0;
  return 0;
}

static ssize_t ultrasonic_read(struct file *file, char *buf,
			size_t len, loff_t *ppos)
{
  int retval, minor_no;
	long long ret;
	struct device_data * dev_data = NULL;
  minor_no = iminor(file->f_path.dentry->d_inode);;
	if (sensor1.device_minor_no == minor_no) dev_data = &sensor1.dev_data;
	else if (sensor2.device_minor_no == minor_no) dev_data = &sensor2.dev_data;
	if (!dev_data) return -1;
  switch (dev_data->mode) {
	case ONE_SHOT:
		// Trigger reading if nothing is being read right now
		if (!dev_data->is_measuring) {
			printk(KERN_DEBUG "HCSR: No measurement being taken right now. Initiating a new read.");
			take_measurement((void *) dev_data);
		}
		while(kfifo_is_empty(&dev_data->buffer)) msleep(WHILE_DELAY * 1000);
		// Fetch reading otherwise
		retval = kfifo_out_spinlocked(&dev_data->buffer, &ret, 1, &dev_data->spin_lock);
		retval = copy_to_user((long long *) buf, &ret, sizeof(long long));
		break;
	case PERIODIC_SAMPLING:
		if (!dev_data->sampling_thread) {
			// Cannot be blocked to wait for measurement if thread hasn't been started
			return -1;
		}
		// Block till the next measurement
		while(kfifo_is_empty(&dev_data->buffer)) msleep(WHILE_DELAY * 1000);
		retval = kfifo_out_spinlocked(&dev_data->buffer, &ret, 1, &dev_data->spin_lock);
		retval = copy_to_user((long long *) buf, &ret, sizeof(long long));
	default:
		retval = -1;
		break;
  }
	return 0;
}

static int ultrasonic_release(struct inode *inode, struct file *file)
{
	int minor_no;
	struct device_data * dev_data = NULL;
	minor_no = iminor(file->f_path.dentry->d_inode);;
	if (sensor1.device_minor_no == minor_no) dev_data = &sensor1.dev_data;
	else if (sensor2.device_minor_no == minor_no) dev_data = &sensor2.dev_data;
	if (!dev_data) return -1;
	printk(KERN_DEBUG "HCSR: Releasing device file.");
	// if (dev_data->sampling_thread) kthread_stop(dev_data->sampling_thread);
	free_irq(dev_data->irq_handler, (void *) (dev_data));
	gpio_set_value_cansleep(dev_data->trigger_pin, 0);
	gpio_unexport(dev_data->echo_pin);
	gpio_unexport(dev_data->trigger_pin);
	gpio_free(dev_data->echo_pin);
	gpio_free(dev_data->trigger_pin);
	kfifo_free(&dev_data->buffer);
	//kfree(dev_data);
	return 0;
}

static long ultrasonic_ioctl(struct file *file, unsigned int ioctlnum, unsigned long arg)
{
	int * args, minor_no;
	struct device_data * dev_data = NULL;
	pr_info("IOCTL is being called.");
	minor_no = iminor(file->f_path.dentry->d_inode);;
	pr_info("IOCTL:minorno:%d\n",minor_no);
	if (sensor1.device_minor_no == minor_no) dev_data = &sensor1.dev_data;
	else if (sensor2.device_minor_no == minor_no) dev_data = &sensor2.dev_data;
	if (!dev_data) {
		pr_info("dev_data is NULL");
		return -1;
	}
	if (dev_data->sampling_thread) {
		printk(KERN_DEBUG "HCSR: Waiting for sampling thread to stop execution.");
		kthread_stop(dev_data->sampling_thread);
	}
	pr_info("debug1");
	while(dev_data->is_measuring) msleep(WHILE_DELAY * 1000);
	pr_info("debug2");
	switch(ioctlnum)
	{
	case SETPINS:
		printk(KERN_DEBUG "HCSR: Executing SETPINS ioctl command.");
		args = (int *) arg;
		dev_data->echo_pin = args[0];
		dev_data->trigger_pin = args[1];
		printk(KERN_DEBUG "HCSR: Trigger pin: %d, Echo pin: %d", dev_data->trigger_pin, dev_data->echo_pin);
		if (gpio_request(dev_data->trigger_pin, "trigger_pin") < 0) {
			// errno = EINVAL;
			pr_info("gpio trigger error.\n");
			return -1;
		}
		if (gpio_request(dev_data->echo_pin, "echo_pin") < 0) {
			// errno = EINVAL;
			pr_info("gpio echo error.\n");
			return -1;
		}
		printk(KERN_DEBUG "HCSR: Trigger pin: %d, Echo pin: %d", dev_data->trigger_pin, dev_data->echo_pin);
		gpio_direction_output(dev_data->trigger_pin, 0);
		gpio_set_value_cansleep(dev_data->trigger_pin, 1);
		gpio_set_value_cansleep(dev_data->echo_pin, 0);
		gpio_direction_input(dev_data->echo_pin);
		gpio_export(dev_data->echo_pin, true);
		gpio_export(dev_data->trigger_pin, true);
		
		dev_data->irq_handler = gpio_to_irq(dev_data->echo_pin);
		if (request_irq(dev_data->irq_handler, (irq_handler_t) echo_handler, IRQF_TRIGGER_RISING, "trigger_rising", (void *) (dev_data)) < 0) {
			printk(KERN_DEBUG "IRQ number request failed.");
			return -1;
		}
		//pr_info("debug1\n");
		break;
	case SETMODE:
		printk(KERN_DEBUG "HCSR: Executing SETMODE ioctl command.");
		args = (int *) arg;
		if (args[0] == 0) {
			printk(KERN_DEBUG "HCSR: Setting device to ONE_SHOT mode.");
			dev_data->mode = ONE_SHOT;
		}
		if (args[0] == 1) {
			dev_data->mode = PERIODIC_SAMPLING;
			dev_data->frequency = sec_to_usec / args[1];
			printk(KERN_DEBUG "HCSR: Setting device to PERIODIC_SAMPLING mode with frequency %ld.", dev_data->frequency);
		}
		break;
	default:
		break;
	}
	return 0;
}

/*
	SYSFS INTERFACE FUNCTIONS
*/
static ssize_t hcsr_trigger_show(struct device *dev, struct device_attribute *attr, char *buf) {
	struct hcsr_platform_data* pfdata = (struct hcsr_platform_data*)dev->platform_data;
	if(pfdata->devno==1) {
		return snprintf(buf, sizeof(int), "%d\n", sensor1.dev_data.trigger_pin);
	} else if(pfdata->devno==2) {
		return snprintf(buf, sizeof(int), "%d\n", sensor2.dev_data.trigger_pin);
	} else {
		return -EINVAL;
	}
	return 0;
}

static ssize_t hcsr_echo_show(struct device *dev, struct device_attribute *attr, char *buf) {
	struct hcsr_platform_data* pfdata = (struct hcsr_platform_data*)dev->platform_data;
	if(pfdata->devno==1) {
		return snprintf(buf, sizeof(int), "%d\n", sensor1.dev_data.echo_pin);
	} else if(pfdata->devno==2) {
		return snprintf(buf, sizeof(int), "%d\n", sensor2.dev_data.echo_pin);
	} else {
		return -EINVAL;
	}
	return 0;
}

static ssize_t hcsr_mode_show(struct device *dev, struct device_attribute *attr, char *buf) {
	struct hcsr_platform_data* pfdata = (struct hcsr_platform_data*)dev->platform_data;
	if(pfdata->devno==1) {
		return snprintf(buf, sizeof(int), "%d\n", sensor1.dev_data.mode);
	} else if(pfdata->devno==2){
		return snprintf(buf, sizeof(int), "%d\n", sensor2.dev_data.mode);
	} else {
		return -EINVAL;
	}
	return 0;
}

static ssize_t hcsr_mode_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
	int mode;
	int ret;
	struct hcsr_platform_data* pfdata = (struct hcsr_platform_data*)dev->platform_data;
	ret = kstrtoint(buf, 10, &mode);
	if(ret != 0) {
		printk(KERN_ERR "Error converting char* to int.");
	}

	if(pfdata->devno==1) {
		sensor1.dev_data.mode=mode;
	} else if(pfdata->devno==2){
		sensor2.dev_data.mode=mode;
	} else {
		return -EINVAL;
	}
	return 0;
}

static ssize_t hcsr_frequency_show(struct device *dev, struct device_attribute *attr, char *buf) {
	struct hcsr_platform_data* pfdata = (struct hcsr_platform_data*)dev->platform_data;
	if(pfdata->devno==1) {
		return snprintf(buf, sizeof(long), "%ld\n", sensor1.dev_data.frequency);
	} else if(pfdata->devno==2){
		return snprintf(buf, sizeof(long), "%ld\n", sensor2.dev_data.frequency);
	} else {
		return -EINVAL;
	}
	return 0;
}

static ssize_t hcsr_enable_show(struct device *dev, struct device_attribute *attr, char *buf) {
	struct hcsr_platform_data* pfdata = (struct hcsr_platform_data*)dev->platform_data;
	if(pfdata->devno==1) {
		return snprintf(buf, sizeof(int), "%d\n", sensor1.dev_data.is_measuring);
	} else if(pfdata->devno==2){
		return snprintf(buf, sizeof(int), "%d\n", sensor2.dev_data.is_measuring);
	} else {
		return -EINVAL;
	}
	return 0;
}

static ssize_t hcsr_enable_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
	struct device_data * dev_data = NULL;
	int enable;
	int ret;
	struct hcsr_platform_data* pfdata = (struct hcsr_platform_data*)dev->platform_data;
	ret = kstrtoint(buf, 10, &enable);
	if(ret != 0) {
		printk(KERN_ERR "Error converting char* to int.");
	}

	if(pfdata->devno==1) {
		sensor1.dev_data.is_measuring = enable;
		dev_data = &sensor1.dev_data;
	} else if(pfdata->devno==2){
		sensor2.dev_data.is_measuring = enable;
		dev_data = &sensor2.dev_data;
	} else {
		return -EINVAL;
	}
	// can put this entire bit in a separate function as we're reusing this from ultrasonic_write
	switch (dev_data->mode) {
	case ONE_SHOT:
		if (enable != 0) {
			// Clear the buffer
			printk(KERN_DEBUG "HCSR: Non-zero first argument in ONE_SHOT mode. Buffer is cleared.");
			kfifo_reset(&dev_data->buffer);
		}
		// If no sampling is being done, do sampling
		if (!dev_data->is_measuring) {
			take_measurement((void *) dev_data);
		}
		break;
	case PERIODIC_SAMPLING:
		if (enable == 0) {
			if (dev_data->sampling_thread) {
				// Stop the sampling if argument is 0
				printk(KERN_DEBUG "HCSR: Stop sampling thread.");
				kthread_stop(dev_data->sampling_thread);
			}
		} else if (enable == 1) {
			// Start the sampling if argument is 1
			printk(KERN_DEBUG "HCSR: Starting sampling thread.");
			dev_data->sampling_thread = kthread_run(&take_measurement, (void *) dev_data, "sampling_thread");
		}
		break;
	default:
		break;
	}
	return 0;
}

static ssize_t hcsr_distance_show(struct device *dev, struct device_attribute *attr, char *buf) {
	struct hcsr_platform_data* pfdata = (struct hcsr_platform_data*)dev->platform_data;
	if(pfdata->devno==1) {
		return snprintf(buf, sizeof(long long), "%lld\n", sensor1.dev_data.last_distance);
	} else if(pfdata->devno==2){
		return snprintf(buf, sizeof(long long), "%lld\n", sensor2.dev_data.last_distance);
	} else {
		return -EINVAL;
	}
	return 0;
}

static DEVICE_ATTR(trigger, S_IRUSR, hcsr_trigger_show, NULL);
static DEVICE_ATTR(echo, S_IRUSR, hcsr_echo_show, NULL);
static DEVICE_ATTR(frequency, S_IRUSR, hcsr_frequency_show, NULL);
static DEVICE_ATTR(distance, S_IRUSR, hcsr_distance_show, NULL);
static DEVICE_ATTR(mode, S_IRUSR | S_IWUSR, hcsr_mode_show, hcsr_mode_store);
static DEVICE_ATTR(enable, S_IRUSR | S_IWUSR, hcsr_enable_show, hcsr_enable_store);

static int __init ultrasonic_init(void)
{
	int retval;
	retval = alloc_chrdev_region(&hcsr_dev, 0, 2, DEV_NAME1);
	if(retval != 0) {
		return -EFAULT;
	}
	/*
	retval = alloc_chrdev_region(&hcsr_dev2, 1, 1, DEVICE_NAME2);
	if(retval != 0) {
		return -EFAULT;
	}
*/
	hcsr_cdev = cdev_alloc();
	if(!hcsr_cdev) {
		return -EFAULT;
	}
	hcsr_cdev->ops = &ultrasonic_fops;
	hcsr_cdev->owner = THIS_MODULE;
/*
	hcsr_cdev2 = cdev_alloc();
	if(!hcsr_cdev2) {
		return -EFAULT;
	}
	hcsr_cdev2->ops = &ultrasonic_fops;
	hcsr_cdev2->owner = THIS_MODULE;
*/
	//cdev_init(hcsr_cdev1, &ultrasonic_fops);
	//cdev_init(hcsr_cdev2, &ultrasonic_fops);

	retval = cdev_add(hcsr_cdev, hcsr_dev, 2);
	if(retval < 0) {
		cdev_del(hcsr_cdev);
	}
	/*
	retval = cdev_add(hcsr_cdev2, hcsr_dev, 2);
	if(retval < 0) {
		cdev_del(hcsr_cdev2);
	}
*/
	hcsr_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(hcsr_class)) {
        printk(KERN_ERR DEVICE_NAME1 " cant create class %s\n", CLASS_NAME);
        class_unregister(hcsr_class);
        class_destroy(hcsr_class);
    }

    hcsr_device1 = device_create(hcsr_class, NULL,MKDEV(MAJOR(hcsr_dev),MINOR(hcsr_dev)), NULL, DEVICE_NAME1);
    if (IS_ERR(hcsr_device1)) {
        printk(KERN_ERR DEVICE_NAME1 " cant create device %s\n", DEVICE_NAME1);
        device_destroy(hcsr_class, hcsr_dev);
    }
    hcsr_device2 = device_create(hcsr_class, NULL,MKDEV(MAJOR(hcsr_dev),MINOR(hcsr_dev)+1), NULL, DEVICE_NAME2);
    if (IS_ERR(hcsr_device2)) {
        printk(KERN_ERR DEVICE_NAME2 " cant create device %s\n", DEVICE_NAME2);
        device_destroy(hcsr_class, hcsr_dev);
    }

    //per device sysfs attributes
    device_create_file(hcsr_device1, &dev_attr_trigger);
    device_create_file(hcsr_device2, &dev_attr_trigger);
    device_create_file(hcsr_device1, &dev_attr_echo);
    device_create_file(hcsr_device2, &dev_attr_echo);
    device_create_file(hcsr_device1, &dev_attr_mode);
    device_create_file(hcsr_device2, &dev_attr_mode);
    device_create_file(hcsr_device1, &dev_attr_frequency);
    device_create_file(hcsr_device2, &dev_attr_frequency);
    device_create_file(hcsr_device1, &dev_attr_enable);
    device_create_file(hcsr_device2, &dev_attr_enable);
    device_create_file(hcsr_device1, &dev_attr_distance);
    device_create_file(hcsr_device2, &dev_attr_distance);

	platform_device_register(&sensor1.plf_dev);
	platform_device_register(&sensor2.plf_dev);

	sensor1.device_minor_no = MINOR(hcsr_dev);
	sensor2.device_minor_no = MINOR(hcsr_dev)+1;

	printk(KERN_ERR "sens1minorno:%d\n",sensor1.device_minor_no);
	printk(KERN_ERR "sens2minorno:%d\n",sensor2.device_minor_no);

	return 0;
}

static void __exit ultrasonic_exit(void)
{
	int i;
	for(i=0;i<2;++i) {
		device_destroy(hcsr_class, MKDEV(hcsr_dev,i));
	}
	//device_destroy(hcsr_class, hcsr_dev2);
	class_unregister(hcsr_class);
	class_destroy(hcsr_class);
	cdev_del(hcsr_cdev);
	//cdev_del(hcsr_cdev2);
	unregister_chrdev_region(hcsr_dev,2);
	//unregister_chrdev_region(hcsr_dev2,1);

	platform_device_unregister(&sensor1.plf_dev);
	platform_device_unregister(&sensor2.plf_dev);
}

module_init(ultrasonic_init);
module_exit(ultrasonic_exit);