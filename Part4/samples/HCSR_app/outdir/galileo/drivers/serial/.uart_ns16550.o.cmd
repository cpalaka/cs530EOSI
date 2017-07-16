cmd_drivers/serial/uart_ns16550.o := /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/bin/i586-poky-elf/i586-poky-elf-gcc -Wp,-MD,drivers/serial/.uart_ns16550.o.d  -nostdinc -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include-fixed -I/home/cpalaka/zephyr-project/kernel/unified/include -I/home/cpalaka/zephyr-project/arch/x86/include -I/home/cpalaka/zephyr-project/arch/x86/soc/intel_quark/quark_x1000 -I/home/cpalaka/zephyr-project/boards/x86/galileo  -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/samples/HCSR_app/outdir/galileo/include/generated -I/home/cpalaka/zephyr-project/samples/HCSR_app/outdir/galileo/misc/generated/sysgen -include /home/cpalaka/zephyr-project/samples/HCSR_app/outdir/galileo/include/generated/autoconf.h  -I/home/cpalaka/zephyr-project/lib/libc/minimal/include  -I/home/cpalaka/zephyr-project/drivers/serial -Idrivers/serial -DKERNEL -D__ZEPHYR__=1 -c -g -std=c99 -fno-asynchronous-unwind-tables -Wall -Wno-format-zero-length -Wno-main -ffreestanding -Os -fno-stack-protector -fno-omit-frame-pointer -ffunction-sections -fdata-sections -mpreferred-stack-boundary=2 -mno-sse -march=pentium -Wno-unused-but-set-variable -fno-reorder-functions -fno-defer-pop -Wno-pointer-sign -fno-strict-overflow -Werror=implicit-int   -I/home/cpalaka/zephyr-project/include/drivers   -I/home/cpalaka/zephyr-project/drivers    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(uart_ns16550)"  -D"KBUILD_MODNAME=KBUILD_STR(uart_ns16550)" -c -o drivers/serial/uart_ns16550.o /home/cpalaka/zephyr-project/drivers/serial/uart_ns16550.c

source_drivers/serial/uart_ns16550.o := /home/cpalaka/zephyr-project/drivers/serial/uart_ns16550.c

deps_drivers/serial/uart_ns16550.o := \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/uart/interrupt/driven.h) \
    $(wildcard include/config/uart/ns16550/dlf.h) \
    $(wildcard include/config/uart/ns16550/pci.h) \
    $(wildcard include/config/pci/enumeration.h) \
    $(wildcard include/config/uart/ns16550/line/ctrl.h) \
    $(wildcard include/config/uart/ns16550/drv/cmd.h) \
    $(wildcard include/config/uart/ns16550/port/0.h) \
    $(wildcard include/config/uart/ns16550/port/0/pci.h) \
    $(wildcard include/config/uart/ns16550/port/0/baud/rate.h) \
    $(wildcard include/config/uart/ns16550/port/0/options.h) \
    $(wildcard include/config/uart/ns16550/port/0/dlf.h) \
    $(wildcard include/config/uart/ns16550/port/0/name.h) \
    $(wildcard include/config/kernel/init/priority/device.h) \
    $(wildcard include/config/uart/ns16550/port/0/irq/pri.h) \
    $(wildcard include/config/uart/ns16550/port/1.h) \
    $(wildcard include/config/uart/ns16550/port/1/pci.h) \
    $(wildcard include/config/uart/ns16550/port/1/baud/rate.h) \
    $(wildcard include/config/uart/ns16550/port/1/options.h) \
    $(wildcard include/config/uart/ns16550/port/1/dlf.h) \
    $(wildcard include/config/uart/ns16550/port/1/name.h) \
    $(wildcard include/config/uart/ns16550/port/1/irq/pri.h) \
  /home/cpalaka/zephyr-project/lib/libc/minimal/include/errno.h \
  /home/cpalaka/zephyr-project/include/nanokernel.h \
  /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include/stddef.h \
  /home/cpalaka/zephyr-project/lib/libc/minimal/include/stdint.h \
  /home/cpalaka/zephyr-project/include/toolchain.h \
  /home/cpalaka/zephyr-project/include/toolchain/gcc.h \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/isa/thumb.h) \
    $(wildcard include/config/isa/thumb2.h) \
    $(wildcard include/config/isa/arm.h) \
    $(wildcard include/config/nios2.h) \
    $(wildcard include/config/arc.h) \
    $(wildcard include/config/x86.h) \
  /home/cpalaka/zephyr-project/include/toolchain/common.h \
  /home/cpalaka/zephyr-project/include/sections.h \
    $(wildcard include/config/gdb/info.h) \
    $(wildcard include/config/sw/isr/table.h) \
  /home/cpalaka/zephyr-project/include/section_tags.h \
  /home/cpalaka/zephyr-project/include/kernel_version.h \
  /home/cpalaka/zephyr-project/include/sys_clock.h \
    $(wildcard include/config/sys/clock/exists.h) \
    $(wildcard include/config/sys/clock/hw/cycles/per/sec.h) \
    $(wildcard include/config/sys/clock/ticks/per/sec.h) \
    $(wildcard include/config/timer/reads/its/frequency/at/runtime.h) \
  /home/cpalaka/zephyr-project/include/drivers/rand32.h \
  /home/cpalaka/zephyr-project/include/misc/slist.h \
  /home/cpalaka/zephyr-project/lib/libc/minimal/include/stdbool.h \
  /home/cpalaka/zephyr-project/include/misc/dlist.h \
  /home/cpalaka/zephyr-project/include/kernel.h \
    $(wildcard include/config/kernel/debug.h) \
    $(wildcard include/config/num/coop/priorities.h) \
    $(wildcard include/config/num/preempt/priorities.h) \
    $(wildcard include/config/debug/tracing/kernel/objects.h) \
    $(wildcard include/config/errno.h) \
    $(wildcard include/config/object/monitor.h) \
    $(wildcard include/config/num/mbox/async/msgs.h) \
    $(wildcard include/config/cplusplus.h) \
  /home/cpalaka/zephyr-project/include/atomic.h \
    $(wildcard include/config/atomic/operations/builtin.h) \
  /home/cpalaka/zephyr-project/include/misc/__assert.h \
    $(wildcard include/config/assert.h) \
    $(wildcard include/config/assert/level.h) \
  /home/cpalaka/zephyr-project/include/legacy.h \
    $(wildcard include/config/semaphore/groups.h) \
    $(wildcard include/config/num/pipe/async/msgs.h) \
    $(wildcard include/config/num/dynamic/timers.h) \
    $(wildcard include/config/num/timer/packets.h) \
    $(wildcard include/config/fp/sharing.h) \
  /home/cpalaka/zephyr-project/lib/libc/minimal/include/limits.h \
  /home/cpalaka/zephyr-project/include/arch/cpu.h \
  /home/cpalaka/zephyr-project/include/arch/x86/arch.h \
    $(wildcard include/config/sse.h) \
    $(wildcard include/config/x86/fixed/irq/mapping.h) \
    $(wildcard include/config/debug/info.h) \
    $(wildcard include/config/int/latency/benchmark.h) \
  /home/cpalaka/zephyr-project/include/irq.h \
  /home/cpalaka/zephyr-project/include/arch/x86/irq_controller.h \
    $(wildcard include/config/mvic.h) \
  /home/cpalaka/zephyr-project/include/drivers/sysapic.h \
    $(wildcard include/config/ioapic/num/rtes.h) \
    $(wildcard include/config/eoi/forwarding/bug.h) \
    $(wildcard include/config/loapic/base/address.h) \
    $(wildcard include/config/eoi/formwarding/bug.h) \
  /home/cpalaka/zephyr-project/include/drivers/ioapic.h \
  /home/cpalaka/zephyr-project/include/drivers/loapic.h \
  /home/cpalaka/zephyr-project/include/device.h \
    $(wildcard include/config/kernel/init/priority/default.h) \
    $(wildcard include/config/device/power/management.h) \
  /home/cpalaka/zephyr-project/include/arch/x86/asm_inline.h \
  /home/cpalaka/zephyr-project/include/arch/x86/asm_inline_gcc.h \
    $(wildcard include/config/cmov.h) \
  /home/cpalaka/zephyr-project/include/sys_io.h \
  /home/cpalaka/zephyr-project/include/arch/x86/addr_types.h \
  /home/cpalaka/zephyr-project/boards/x86/galileo/board.h \
    $(wildcard include/config/gpio/pcal9535a/0/dev/name.h) \
    $(wildcard include/config/gpio/pcal9535a/1/dev/name.h) \
    $(wildcard include/config/gpio/pcal9535a/2/dev/name.h) \
    $(wildcard include/config/pwm/pca9685/0/dev/name.h) \
    $(wildcard include/config/gpio/dw/0/name.h) \
    $(wildcard include/config/gpio/sch/0/dev/name.h) \
    $(wildcard include/config/gpio/sch/1/dev/name.h) \
  /home/cpalaka/zephyr-project/arch/x86/soc/intel_quark/quark_x1000/soc.h \
    $(wildcard include/config/ioapic.h) \
  /home/cpalaka/zephyr-project/include/misc/util.h \
  /home/cpalaka/zephyr-project/include/init.h \
  /home/cpalaka/zephyr-project/include/uart.h \
    $(wildcard include/config/uart/line/ctrl.h) \
    $(wildcard include/config/uart/drv/cmd.h) \
  /home/cpalaka/zephyr-project/include/drivers/pci/pci.h \
    $(wildcard include/config/pci/debug.h) \
  /home/cpalaka/zephyr-project/include/drivers/pci/pci_mgr.h \
  /home/cpalaka/zephyr-project/include/drivers/pci/pci.h \
  /home/cpalaka/zephyr-project/include/drivers/pci/pci_mgr.h \
  /home/cpalaka/zephyr-project/drivers/serial/uart_ns16550.h \

drivers/serial/uart_ns16550.o: $(deps_drivers/serial/uart_ns16550.o)

$(deps_drivers/serial/uart_ns16550.o):
