cmd_arch/x86/core/sys_fatal_error_handler.o := /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/bin/i586-poky-elf/i586-poky-elf-gcc -Wp,-MD,arch/x86/core/.sys_fatal_error_handler.o.d  -nostdinc -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include-fixed -I/home/cpalaka/zephyr-project/kernel/unified/include -I/home/cpalaka/zephyr-project/arch/x86/include -I/home/cpalaka/zephyr-project/arch/x86/soc/intel_quark/quark_x1000 -I/home/cpalaka/zephyr-project/boards/x86/galileo  -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/include/generated -I/home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/misc/generated/sysgen -include /home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/include/generated/autoconf.h  -I/home/cpalaka/zephyr-project/lib/libc/minimal/include  -I/home/cpalaka/zephyr-project/arch/x86/core -Iarch/x86/core -DKERNEL -D__ZEPHYR__=1 -c -g -std=c99 -fno-asynchronous-unwind-tables -Wall -Wno-format-zero-length -Wno-main -ffreestanding -Os -fno-stack-protector -fno-omit-frame-pointer -ffunction-sections -fdata-sections -mpreferred-stack-boundary=2 -mno-sse -march=pentium -Wno-unused-but-set-variable -fno-reorder-functions -fno-defer-pop -Wno-pointer-sign -fno-strict-overflow -Werror=implicit-int   -I/home/cpalaka/zephyr-project/kernel/unified/include    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(sys_fatal_error_handler)"  -D"KBUILD_MODNAME=KBUILD_STR(sys_fatal_error_handler)" -c -o arch/x86/core/sys_fatal_error_handler.o /home/cpalaka/zephyr-project/arch/x86/core/sys_fatal_error_handler.c

source_arch/x86/core/sys_fatal_error_handler.o := /home/cpalaka/zephyr-project/arch/x86/core/sys_fatal_error_handler.c

deps_arch/x86/core/sys_fatal_error_handler.o := \
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
  /home/cpalaka/zephyr-project/lib/libc/minimal/include/errno.h \
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
  /home/cpalaka/zephyr-project/kernel/unified/include/kernel_structs.h \
    $(wildcard include/config/thread/monitor.h) \
    $(wildcard include/config/nano/timeouts.h) \
    $(wildcard include/config/thread/custom/data.h) \
    $(wildcard include/config/sys/power/management.h) \
  /home/cpalaka/zephyr-project/arch/x86/include/kernel_arch_data.h \
  /home/cpalaka/zephyr-project/arch/x86/include/asm_inline.h \
  /home/cpalaka/zephyr-project/arch/x86/include/asm_inline_gcc.h \
  /home/cpalaka/zephyr-project/arch/x86/include/exception.h \
  /home/cpalaka/zephyr-project/kernel/unified/include/nano_internal.h \
    $(wildcard include/config/xip.h) \
  /home/cpalaka/zephyr-project/include/misc/util.h \
  /home/cpalaka/zephyr-project/arch/x86/include/kernel_arch_func.h \
    $(wildcard include/config/isr/stack/size.h) \
  /home/cpalaka/zephyr-project/include/misc/printk.h \
    $(wildcard include/config/printk.h) \

arch/x86/core/sys_fatal_error_handler.o: $(deps_arch/x86/core/sys_fatal_error_handler.o)

$(deps_arch/x86/core/sys_fatal_error_handler.o):
