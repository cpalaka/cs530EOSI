cmd_drivers/built-in.o :=  /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/bin/i586-poky-elf/i586-poky-elf-ld -nostartfiles -nodefaultlibs -nostdlib -static   -r -o drivers/built-in.o drivers/console/built-in.o drivers/random/built-in.o drivers/serial/built-in.o drivers/timer/built-in.o drivers/interrupt_controller/built-in.o drivers/pci/built-in.o drivers/shared_irq/built-in.o drivers/spi/built-in.o drivers/gpio/built-in.o drivers/i2c/built-in.o drivers/pwm/built-in.o drivers/adc/built-in.o drivers/pinmux/built-in.o 
