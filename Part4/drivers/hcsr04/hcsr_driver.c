#include <device.h>
#include <microkernel.h>
#include <gpio.h>
#include <misc/printk.h>
#include <misc/util.h>

#include <init.h>
#include <zephyr.h>
#include <errno.h>
#include <kernel.h>
#include "hcsr_driver.c"

struct device* gpio_0;
struct device* gpio_rw;
struct device* exp1;
struct device* pwm0;
struct device *exp0;

int take_reading(struct device* dev) {
	int ret;
	struct hcsr_driver_data* data = (struct hcsr_driver_data*)dev->driver_data;
	//trigger hcsr device for 10 microsec
	ret = gpio_pin_write(gpio_0, 5, 1);
	k_sleep(USEC(10)/1000);
	ret = gpio_pin_write(gpio_0, 5, 0);
	data->is_measuring = true;
	//start timer
	data->timestamp = k_uptime_get();
}

void gpio_callback(struct device* port, struct gpio_callback* cb, uint32_t pins) {
	printk("in callback fn\n");
	int64_t timeinms;
	struct hcsr_driver_data* data = CONTAINER_OF(cb, struct hcsr_driver_data, gpio_cb);
	k_sem_take(&data->sem_lock,20*1000);
	timeinms = k_uptime_delta(&data->timestamp);

	data->distance_measure = timeinms / 58;
	data->is_measuring = false;
	k_sem_give(&data->sem_lock);
}

static int _reset(struct device* dev) {
	printk("in reset\n");
	struct hcsr_driver_data* data = (struct hcsr_driver_data*) dev->driver_data;
	
	data->is_measuring = false;
	data->distance_measure = -1;
	return 1;
}

static int _read(struct device* dev, int32_t timeout) {
	printk("in read\n");
	struct hcsr_driver_data* data = (struct hcsr_driver_data*)dev->driver_data;
	int ret=-1;
	k_sem_take(&data->sem_lock, 20*1000);

	if(data->is_measuring == false) {
		if(data->distance_measure = -1) {
			//take new measurement
			take_reading(dev);
		} else {
			ret = data->distance_measure;
		}
		
	} else {
		//block until reading comes in, or timeout ms is over
		k_sleep(MSEC(timeout));
		if(data->is_measuring == false) {
			ret = data->distance_measure;
		}
	}
	//clear latest reading
	data->distance_measure = -1;
	k_sem_give(&data->sem_lock);
	return ret;
}

static int _write(struct device* dev) {
	printk("in write\n");
	struct hcsr_driver_data* data = (struct hcsr_driver_data*) dev->driver_data;
	int ret = 1;
	k_sem_take(&data->sem_lock, 20*1000);
	data->distance_measure = -1;
	if(data->is_measuring==false) {
		//take new measurement
		take_reading(dev);
	}
	k_sem_give(&data->sem_lock);
	return ret;
}

static const struct hcsr_api driver_api = {
	.reset = _reset,
	.read  = _read,
	.write = _write,
};

int hcsr_init(struct device *dev) {
	//printk("in init\n");
	struct hcsr_driver_data* data = (struct hcsr_driver_data*) dev->driver_data;
	//set api functions inside device struct
	dev->driver_api = &driver_api;

	//struct device *gpio_cw = device_get_binding("GPIO_CW");
	gpio_rw = device_get_binding("GPIO_RW");
	gpio_0 = device_get_binding("GPIO_0");
	exp0 = device_get_binding("EXP0");
	exp1 = device_get_binding("EXP1");
	///struct device *exp2 = device_get_binding("EXP2");
	pwm0 = device_get_binding("PWM0");

	if(gpio_rw == NULL || gpio_0 == NULL || exp1 == NULL || pwm0 == NULL) {
		printk("Failed to get device binding.\n");
		return -EIO;
	}

	if(data->dev_no == 0) {//for hcsr0
		//setup trigger pin for hcsr0 (IO2 - gpio13)
		gpio_pin_write(pwm0, 13, 0);//gpio 77 low
		gpio_pin_write(exp1, 2, 0);//gpio 34 low
		
		gpio_pin_configure(gpio_0,5, GPIO_DIR_OUT);

		//setup echo pin for hcsr0 (IO4 - gpio6)
		gpio_pin_write(exp1,4,1);//gpio 36 high
		//configure so triggers interrupt on rising edge
		gpio_pin_configure(gpio_rw,4, GPIO_DIR_IN | GPIO_INT | GPIO_INT_ACTIVE_HIGH |GPIO_INT_EDGE |GPIO_INT_DEBOUNCE);

		//setup callback functions
		gpio_init_callback(&data->gpio_cb, gpio_callback, BIT(4));
		gpio_add_callback(gpio_rw, &data->gpio_cb);
		gpio_pin_enable_callback(gpio_rw, 4);
	} else {//for hcsr1
		//setup trigger pin for hcsr1 (IO10 - gpio10)
		gpio_pin_write(pwm0,10,0);
		gpio_pin_write(exp0,10,0);
		gpio_pin_configure( gpio_0,2,GPIO_DIR_OUT);

		//setup echo pin for hcsr1 (IO9 - gpio4)
		gpio_pin_write(pwm0,6,0);
		gpio_pin_write(exp0, 6,0);
		gpio_pin_configure(gpio_rw,2, GPIO_DIR_IN | GPIO_INT | GPIO_INT_ACTIVE_HIGH |GPIO_INT_EDGE |GPIO_INT_DEBOUNCE);
		
		//setup callback functions
		gpio_init_callback(&data->gpio_cb, gpio_callback, BIT(2));
		gpio_add_callback(gpio_rw, &data->gpio_cb);
		gpio_pin_enable_callback(gpio_rw, 2);
	}
	//initialize semaphore
	k_sem_init(&data->sem_lock, 1,1);
}

//initialize first device

static struct hcsr_driver_data drvdata0 = {
	.is_measuring = false,
	.distance_measure = -1,
	.dev_no = 0,
};

DEVICE_AND_API_INIT(HCSR0, CONFIG_HCSR0_NAME,
				 hcsr_init, &drvdata0,NULL, APPLICATION,
				 /* CONFIG_KERNEL_INIT_PRIORITY_DEFAULT*/56, &driver_api);


//initialize second device
static struct hcsr_driver_data drvdata1 = {
	.is_measuring = false,
	.distance_measure = -1,
	.dev_no=1,
};

DEVICE_AND_API_INIT(HCSR1, CONFIG_HCSR1_NAME,
				 hcsr_init, &drvdata1,NULL, APPLICATION,
				 /*CONFIG_KERNEL_INIT_PRIORITY_DEFAULT*/56, &driver_api);

