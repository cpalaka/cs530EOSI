VDIR := /opt/iot-devkit/1.7.3/sysroots/i586-poky-linux/usr/src/kernel/

CC = i586-poky-linux-gcc -Wall 
ARCH = x86
CROSS_COMPILE = i586-poky-linux-
SROOT = /opt/iot-devkit/1.7.3/sysroots/i586-poky-linux/

obj-m := ultrasonic_sensor.o

all:
	make ARCH=x86 CROSS_COMPILE=i586-poky-linux- -C $(VDIR) M=$(PWD) modules 
clean:
	rm -f *.ko
	rm -f *.o
	rm -f *.mod.c
	rm -f *.mod.o
	rm -f Module.symvers
	rm -f modules.order
