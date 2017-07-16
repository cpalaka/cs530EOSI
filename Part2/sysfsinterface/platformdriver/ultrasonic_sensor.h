#ifndef __ULTRASONIC_SENSOR_H__
#include <linux/platform_device.h>
#include <linux/kfifo.h>
#include <linux/spinlock.h>
#include <linux/ktime.h>
#include <linux/types.h>

typedef enum { NOT_SET=-1, ONE_SHOT=0, PERIODIC_SAMPLING=1 } sensor_mode;

struct device_data {
	// ECHO GPIO pin X 
  int echo_pin;
	// TRIGGER GPIO pin X
  int trigger_pin;
	// Frequency (used only in PERIODIC_SAMPLING mode) X
  long frequency;
	// Is the module making a measurement currently? X
	int is_measuring;
	// For measurement purposes
	ktime_t start_time;
	ktime_t end_time;
	// GPIO interrupt handler
	unsigned int irq_handler;
	// Sensor mode X
	sensor_mode mode;
	// For multithreading during PERIODIC_SAMPLING
	struct task_struct * sampling_thread;
	// Buffer to hold measurement 
  STRUCT_KFIFO(long long, roundup_pow_of_two(5 * sizeof(long long))) buffer;
   // last distance measure X
   long long last_distance;
	// Spin lock for using the buffer
	spinlock_t spin_lock;
};

struct hcsr_platform_data {
	int devno;
};

struct sensor_dev {
	char * name;
	struct device_data dev_data;
	struct platform_device plf_dev;
	int device_minor_no;
};

#endif //__ULTRASONIC_SENSOR_H__