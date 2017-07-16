#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kfifo.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include "ultrasonic_sensor.h"

#define DRIVER_NAME		"HCSR_of_driver"

static const struct platform_device_id P_id_table[] = {
         { "HCSR_1", 0 },
         { "HCSR_2", 0 },
	 { },
};

static int P_driver_probe(struct platform_device *dev_found)
{
	struct sensor_dev* sdev;

	sdev = container_of(dev_found, struct sensor_dev, plf_dev);

	sdev->dev_data.echo_pin = -1;
  	sdev->dev_data.trigger_pin = -1;
  	sdev->dev_data.frequency = -1;
	sdev->dev_data.is_measuring = 0;
  	sdev->dev_data.mode = NOT_SET;
  	sdev->dev_data.sampling_thread = NULL;
  	INIT_KFIFO(sdev->dev_data.buffer);
	spin_lock_init(&sdev->dev_data.spin_lock);

	if(!strcmp(sdev->name, "HCSR1")) {
		printk(KERN_DEBUG "HCSR1: Device has been found and opened.");
	} else {
		printk(KERN_DEBUG "HCSR2: Device has been found and opened.");
	}
	
	return 0;
}

static int P_driver_remove(struct platform_device *pdev)
{
	struct sensor_dev* sdev;
	sdev = container_of(pdev, struct sensor_dev, plf_dev);

	free_irq(sdev->dev_data.irq_handler, (void *) (&sdev->dev_data));
	gpio_set_value_cansleep(sdev->dev_data.trigger_pin, 0);
	gpio_unexport(sdev->dev_data.echo_pin);
	gpio_unexport(sdev->dev_data.trigger_pin);
	gpio_free(sdev->dev_data.echo_pin);
	gpio_free(sdev->dev_data.trigger_pin);
	kfifo_free(&sdev->dev_data.buffer);

	if(!strcmp(sdev->name, "HCSR1")) {
		printk(KERN_DEBUG "HCSR1: Device has been removed.");
	} else {
		printk(KERN_DEBUG "HCSR2: Device has been removed.");
	}

	return 0;
};

static struct platform_driver P_driver = {
	.driver		= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= P_driver_probe,
	.remove		= P_driver_remove,
	.id_table	= P_id_table,
};

module_platform_driver(P_driver);
MODULE_LICENSE("GPL");
