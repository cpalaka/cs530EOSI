cmd_arch/x86/core/swap.o := /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/bin/i586-poky-elf/i586-poky-elf-gcc -Wp,-MD,arch/x86/core/.swap.o.d  -nostdinc -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include-fixed -I/home/cpalaka/zephyr-project/kernel/unified/include -I/home/cpalaka/zephyr-project/arch/x86/include -I/home/cpalaka/zephyr-project/arch/x86/soc/intel_quark/quark_x1000 -I/home/cpalaka/zephyr-project/boards/x86/galileo  -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/include/generated -I/home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/misc/generated/sysgen -include /home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/include/generated/autoconf.h  -I/home/cpalaka/zephyr-project/lib/libc/minimal/include -DKERNEL -D__ZEPHYR__=1 -c -g -xassembler-with-cpp -march=pentium -Wa,--divide   -I/home/cpalaka/zephyr-project/kernel/unified/include   -c -o arch/x86/core/swap.o /home/cpalaka/zephyr-project/arch/x86/core/swap.S

source_arch/x86/core/swap.o := /home/cpalaka/zephyr-project/arch/x86/core/swap.S

deps_arch/x86/core/swap.o := \
    $(wildcard include/config/x86/iamcu.h) \
    $(wildcard include/config/kernel/event/logger/context/switch.h) \
    $(wildcard include/config/fp/sharing.h) \
    $(wildcard include/config/sse.h) \
    $(wildcard include/config/int/latency/benchmark.h) \
  /home/cpalaka/zephyr-project/kernel/unified/include/kernel_structs.h \
    $(wildcard include/config/thread/monitor.h) \
    $(wildcard include/config/nano/timeouts.h) \
    $(wildcard include/config/thread/custom/data.h) \
    $(wildcard include/config/errno.h) \
    $(wildcard include/config/sys/clock/exists.h) \
    $(wildcard include/config/sys/power/management.h) \
  /home/cpalaka/zephyr-project/arch/x86/include/kernel_arch_data.h \
    $(wildcard include/config/gdb/info.h) \
    $(wildcard include/config/debug/info.h) \
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
    $(wildcard include/config/sw/isr/table.h) \
  /home/cpalaka/zephyr-project/include/section_tags.h \
  /home/cpalaka/zephyr-project/arch/x86/include/asm_inline.h \
  /home/cpalaka/zephyr-project/arch/x86/include/asm_inline_gcc.h \
  /home/cpalaka/zephyr-project/arch/x86/include/exception.h \
  /home/cpalaka/zephyr-project/include/arch/x86/asm.h \
  /home/cpalaka/zephyr-project/kernel/unified/include/offsets_short.h \
  /home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/include/generated/offsets.h \
  /home/cpalaka/zephyr-project/arch/x86/include/offsets_short_arch.h \

arch/x86/core/swap.o: $(deps_arch/x86/core/swap.o)

$(deps_arch/x86/core/swap.o):
