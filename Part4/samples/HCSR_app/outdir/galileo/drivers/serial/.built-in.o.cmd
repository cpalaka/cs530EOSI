cmd_drivers/serial/built-in.o :=  /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/bin/i586-poky-elf/i586-poky-elf-ld -nostartfiles -nodefaultlibs -nostdlib -static   -r -o drivers/serial/built-in.o drivers/serial/uart_ns16550.o 