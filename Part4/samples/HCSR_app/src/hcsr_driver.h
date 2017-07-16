#ifndef __HCSR_DRVR_H__
#define __HCSR_DRVR_H__

//#include <misc/util.h>
//#include <device.h>
#include <gpio.h>

extern int hcsr_init(struct device *dev);

//struct gpio_callback dev1_cb, dev2_cb;

typedef int (*hcsr_reset)(struct device* dev);
typedef int (*hcsr_read)(struct device* dev, int32_t timeout);
typedef int (*hcsr_write)(struct device* dev);

struct hcsr_api {
	hcsr_reset reset;
	hcsr_read read;
	hcsr_write write;
};


struct hcsr_driver_data {
	//is the device currently measuring
	bool is_measuring;
	//per device buffer to save last reading
	int distance_measure;
	//add start time/end time variables
	int64_t timestamp;
	struct k_sem sem_lock;
	struct gpio_callback gpio_cb;
	int dev_no;//0 for hcsr0 , 1 for hcsr1
};



#endif