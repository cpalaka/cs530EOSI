cmd_drivers/interrupt_controller/built-in.o :=  /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/bin/i586-poky-elf/i586-poky-elf-ld -nostartfiles -nodefaultlibs -nostdlib -static   -r -o drivers/interrupt_controller/built-in.o drivers/interrupt_controller/loapic_intr.o drivers/interrupt_controller/system_apic.o drivers/interrupt_controller/ioapic_intr.o 