#include <nanokernel.h>
#include <microkernel.h>
#include <gpio.h>
#include <misc/printk.h>
#include <misc/util.h>
#include <device.h>
#include <init.h>
#include <zephyr.h>
#include <errno.h>
#include <kernel.h>
#include "hcsr_driver.h"

void main(void) {
	printk("start of application.\n");
	struct device* hcsr = device_get_binding("HCSR0");
	printk("after device get binding.\n");

	if(hcsr == NULL) {
		printk("somethings wrong here.\n");
	}
	struct hcsr_api* api = hcsr->driver_api;
	int ret;

	//api->reset(hcsr);
	api->write(hcsr);
	k_sleep(SECONDS(5));
	ret = api->read(hcsr, 1000);
	if(ret==-1) {
		printk("read failed.\n");
	}


	// 	//struct device *gpio_cw = device_get_binding("GPIO_CW");
	// int cnt=0;
	// struct device *gpio_rw = device_get_binding("GPIO_RW");
	// struct device *gpio_0 = device_get_binding("GPIO_0");
	// //struct device *exp0 = device_get_binding("EXP0");
	// struct device *exp1 = device_get_binding("EXP1");
	// ///struct device *exp2 = device_get_binding("EXP2");
	// struct device *pwm0 = device_get_binding("PWM0");

	// if(gpio_rw == NULL || gpio_0 == NULL || exp1 == NULL || pwm0 == NULL) {
	// 	printk("Failed to get device binding.\n");
	// 	return;
	// }

	// //setup trigger pin for hcsr0 (IO2 - gpio13)
	// gpio_pin_write(pwm0, 13, 0);//gpio 77 low
	// gpio_pin_write(exp1, 2, 0);//gpio 34 low
	
	// gpio_pin_configure(gpio_0,5, GPIO_DIR_OUT);

	// while (1) {
	// 	/* Set pin to HIGH/LOW every 1 second */
	// 	gpio_pin_write(gpio_0, 5, cnt % 2);
	// 	cnt++;
	// 	k_sleep(1000);
	// }

}