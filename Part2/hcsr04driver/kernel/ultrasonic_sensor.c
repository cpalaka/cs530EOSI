#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kfifo.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/types.h>

#include <linux/kthread.h>
#include <linux/gpio.h>
#include <asm/gpio.h>

#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>

MODULE_DESCRIPTION("Misc Device Driver for Ultrasonic sensor HCSR-04");
MODULE_AUTHOR("Vigneshwer Vaidyanathan and Chaitanya Palaka");
MODULE_LICENSE("GPL");

#define SETPINS 0
#define SETMODE 1

// Delay (in seconds) for how long thread should sleep in while loops
#define WHILE_DELAY 1
#define BUFFER_LEN 5
#define BUFFER_TYPE s64

static long sec_to_usec = 1000000;

static int ultrasonic_open(struct inode *inode, struct file *file);
static ssize_t ultrasonic_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos);
static ssize_t ultrasonic_read(struct file *file, char *buf, size_t len, loff_t *ppos);
static int ultrasonic_release(struct inode *inode, struct file *file);
static long ultrasonic_ioctl(struct file *file, unsigned int ioctlnum, unsigned long arg);
static irq_handler_t echo_handler(int irq, void *dev_id, struct pt_regs *regs);
int take_measurement(void * data);

static struct file_operations ultrasonic_fops = {
	.owner = THIS_MODULE,
	.write = ultrasonic_write,
	.open = ultrasonic_open,
	.read = ultrasonic_read,
	.release = ultrasonic_release,
	.unlocked_ioctl = ultrasonic_ioctl
};

static struct miscdevice ultrasonic_dev_one = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "HCSR_1",
	.fops = &ultrasonic_fops
};

static struct miscdevice ultrasonic_dev_two = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "HCSR_2",
	.fops = &ultrasonic_fops
};

typedef enum { NOT_SET, ONE_SHOT, PERIODIC_SAMPLING } sensor_mode;

struct device_data {
	// ECHO GPIO pin
	int echo_pin;
	// TRIGGER GPIO pin
	int trigger_pin;
	// Frequency (used only in PERIODIC_SAMPLING mode)
	long frequency;
	// Is the module making a measurement currently?
	int is_measuring;
	// For measurement purposes
	ktime_t start_time;
	ktime_t end_time;
	// GPIO interrupt handler
	unsigned int irq_handler;
	// Sensor mode
	sensor_mode mode;
	// For multithreading during PERIODIC_SAMPLING
	struct task_struct * sampling_thread;
	// Buffer to hold measurement
	STRUCT_KFIFO(long long, roundup_pow_of_two(5 * sizeof(long long))) buffer;
	// Spin lock for using the buffer
	spinlock_t spin_lock;
};

static int ultrasonic_open(struct inode *inode, struct file *file)
{
	struct device_data * dev_data = (struct device_data *) kmalloc(sizeof(struct device_data), GFP_KERNEL);
	dev_data->echo_pin = 5;
	dev_data->trigger_pin = -1;
	dev_data->frequency = -1;
	dev_data->is_measuring = 0;
	dev_data->mode = NOT_SET;
	dev_data->sampling_thread = NULL;
	INIT_KFIFO(dev_data->buffer);
	spin_lock_init(&dev_data->spin_lock);
	file->private_data = dev_data;
	printk(KERN_DEBUG "HCSR: Device has been opened.");
	return 0;
}

static ssize_t ultrasonic_write(struct file *file, const char __user *buf,
			size_t len, loff_t *ppos)
{
	int arg[2], u_ret;
	struct device_data * dev_data;
	u_ret = copy_from_user(&arg, (int *) buf, sizeof(int) * 2);
	dev_data = (struct device_data *) file->private_data;
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
	long long distance_measurement, unused_object;
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
		distance_measurement = ktime_to_ns(ktime_sub(dev_data->end_time, dev_data->start_time));
		spin_lock(&dev_data->spin_lock);
		if (kfifo_len(&dev_data->buffer) == BUFFER_LEN) {
			printk(KERN_DEBUG "HCSR: Buffer is at MAX_LEN. Removing the first element from the buffer.");
			ret = kfifo_out(&dev_data->buffer, &unused_object, 1);
		}
		kfifo_in(&dev_data->buffer, &distance_measurement, 1);
		spin_unlock(&dev_data->spin_lock);
		printk(KERN_DEBUG "HCSR: Measurement is %llu", distance_measurement);
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
	int retval;
	long long ret;
	struct device_data * dev_data;
	dev_data = (struct device_data *) file->private_data;
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
	struct device_data * dev_data = (struct device_data *) file->private_data;
	printk(KERN_DEBUG "HCSR: Releasing device file.");
	// if (dev_data->sampling_thread) kthread_stop(dev_data->sampling_thread);
	free_irq(dev_data->irq_handler, (void *) (dev_data));
	gpio_set_value_cansleep(dev_data->trigger_pin, 0);
	gpio_unexport(dev_data->echo_pin);
	gpio_unexport(dev_data->trigger_pin);
	gpio_free(dev_data->echo_pin);
	gpio_free(dev_data->trigger_pin);
	kfifo_free(&dev_data->buffer);
	kfree(dev_data);
	return 0;
}

static long ultrasonic_ioctl(struct file *file, unsigned int ioctlnum, unsigned long arg)
{
	int * args;
	struct device_data * dev_data;
	dev_data = (struct device_data *) file->private_data;
	if (dev_data->sampling_thread) {
		printk(KERN_DEBUG "HCSR: Waiting for sampling thread to stop execution.");
		kthread_stop(dev_data->sampling_thread);
	}
	while(dev_data->is_measuring) msleep(WHILE_DELAY * 1000);
	switch(ioctlnum)
	{
	case SETPINS:
		printk(KERN_DEBUG "HCSR: Executing SETPINS ioctl command.");
		args = (int *) arg;
		dev_data->echo_pin = args[0];
		dev_data->trigger_pin = args[1];
		if (gpio_request(dev_data->trigger_pin, "trigger_pin") < 0) {
			// errno = EINVAL;
			return -1;
		}
		if (gpio_request(dev_data->echo_pin, "echo_pin") < 0) {
			// errno = EINVAL;
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

static int __init ultrasonic_init(void)
{
	int retval;
	retval = misc_register(&ultrasonic_dev_one);
	retval = misc_register(&ultrasonic_dev_two);
	if (retval) return retval;
	printk(KERN_DEBUG "HCSR: Registered device. Got minor number.");
	return 0;
}

static void __exit ultrasonic_exit(void)
{
	int error;
	error = misc_deregister(&ultrasonic_dev_one);
	error = misc_deregister(&ultrasonic_dev_two);
}

module_init(ultrasonic_init);
module_exit(ultrasonic_exit);
