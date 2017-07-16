#include <zephyr.h>
#include <misc/printk.h>
#include <misc/shell.h>
#include <device.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <i2c.h>
#include <stdbool.h>
#include <gpio.h>
#include "hcsr_driver.h"

#define SHELL_MODULE_NAME "hcsr"
#define I2CADDR 0x52
static int no_of_sensors;
static bool is_rom_init;
static int current_addr_loc;

static struct device* eeprom_dev;
static struct device* hcsr0_dev;
static struct device* hcsr1_dev;
struct hcsr_api* api0;
struct hcsr_api* api1;

static int init_eeprom(void) {
	union dev_config cfg;
	int ret;

	//first enable SDA, SCL pins by setting gpio60 to low
	struct device* gpio = device_get_binding("EXP2");
	gpio_pin_configure(gpio,12 ,GPIO_DIR_OUT );
	gpio_pin_write(gpio,12 ,0);

	cfg.raw = 0;
	eeprom_dev = device_get_binding("I2C0");
	if(eeprom_dev == NULL) {
		printk("eeprom device not found.\n");
		return -1;
	}
	ret = i2c_configure(eeprom_dev, cfg.raw);
	if(ret != 0) {
		printk("i2c configuration failed.\n");
		return -1;
	}
	is_rom_init=true;
	return 0;
}

static int enable(int argc, char *argv[]) {
	if(argc==2) {
		no_of_sensors = atoi(argv[1]);
	} else {
		printk("Insufficient number of arguments.\n");
		return -1;
	}
	return 0;
}

static void write_to_eeprom(int pages) {
	uint32_t* buffer = malloc(sizeof(uint32_t)*pages);
	int i, ret;
	for(i=0; i< pages; ++i) {
		api1->write(hcsr0_dev);
		k_sleep(1000);
		buffer[i] = api1->read(hcsr0_dev, 1000);
	}

	ret = i2c_burst_write(eeprom_dev, I2CADDR, current_addr_loc, buffer, sizeof(buffer));
	if(ret==-1){
		printk("burst write failed.\n");
	}
	current_addr_loc+=pages;
}

static int start(int argc, char *argv[]) {
	int p;
	if(argc==2) {
		p = atoi(argv[1]);
		if(no_of_sensors) {
			//record p pages
			//first initialize eeprom, if not done already
			if(!is_rom_init) {
				init_eeprom();
			}

			write_to_eeprom(p);
		} else {
			printk("No sensors enabled.\n");
			return -1;
		}
	} else {
		printk("Insufficient number of arguments.\n");
		return -1;
	}
	return 0;
}

static void read_from_eeprom(int from, int to) {
	uint32_t *buffer = malloc(sizeof(int)*(to-from));
	int i, ret;

	ret = i2c_burst_read(eeprom_dev, I2CADDR, from, buffer, (to-from));
	if(ret==-1){
		printk("burst read failed.\n");
	}
	printk("Printing %d measures stored in EEPROM from page %d.\n", (to-from), from);
	for(i=0; i<(to-from); ++i) {
		printk("%d\n", buffer[i]);
	}
}

static int dump(int argc, char *argv[]) {
	int from, to;
	if(argc == 3) {
		from = atoi(argv[1]);
		to = atoi(argv[2]);
		if ( from < to) {
			//dump pages from 'from' to 'to'
			read_from_eeprom(from, to);
		} else {
			printk("Error: First argument must be less than second argument.");
			return -1;
		}
	} else {
		printk("3. Insufficient number of arguments\n");
		return -1;
	}
	return 0;
}


static struct shell_cmd commands[] = {
	{"enable", enable, "enable (n) where n = 0(none), 1(hcsr0), 2(hcsr1)"},
	{"start", start, "start (p) where p<=512 "},
	{"dump", dump, "dump (p1) (p2) where p1<p2. It will print out values stored between pages p1 and p2"},
	{NULL,NULL}
};

int main(void) {
	int ret;
	no_of_sensors = 0;
	is_rom_init = false;
	current_addr_loc = 0;
	SHELL_REGISTER(SHELL_MODULE_NAME, commands);
	//shell_register_default_module(SHELL_MODULE_NAME);
	hcsr0_dev = device_get_binding("HCSR0");
	hcsr1_dev = device_get_binding("HCSR1");

	api0 = hcsr0_dev->driver_api;
	api1 = hcsr1_dev->driver_api;
}