cmd_arch/x86/core/crt0.o := /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/bin/i586-poky-elf/i586-poky-elf-gcc -Wp,-MD,arch/x86/core/.crt0.o.d  -nostdinc -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include-fixed -I/home/cpalaka/zephyr-project/kernel/unified/include -I/home/cpalaka/zephyr-project/arch/x86/include -I/home/cpalaka/zephyr-project/arch/x86/soc/intel_quark/quark_x1000 -I/home/cpalaka/zephyr-project/boards/x86/galileo  -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/include/generated -I/home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/misc/generated/sysgen -include /home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/include/generated/autoconf.h  -I/home/cpalaka/zephyr-project/lib/libc/minimal/include -DKERNEL -D__ZEPHYR__=1 -c -g -xassembler-with-cpp -march=pentium -Wa,--divide   -I/home/cpalaka/zephyr-project/kernel/unified/include   -c -o arch/x86/core/crt0.o /home/cpalaka/zephyr-project/arch/x86/core/crt0.S

source_arch/x86/core/crt0.o := /home/cpalaka/zephyr-project/arch/x86/core/crt0.S

deps_arch/x86/core/crt0.o := \
    $(wildcard include/config/gdt/dynamic.h) \
    $(wildcard include/config/sse.h) \
    $(wildcard include/config/boot/time/measurement.h) \
    $(wildcard include/config/sys/power/deep/sleep.h) \
    $(wildcard include/config/set/gdt.h) \
    $(wildcard include/config/bootloader/unknown.h) \
    $(wildcard include/config/float.h) \
    $(wildcard include/config/init/stacks.h) \
    $(wildcard include/config/isr/stack/size.h) \
    $(wildcard include/config/bootloader/context/restore.h) \
    $(wildcard include/config/xip.h) \
    $(wildcard include/config/idt/num/vectors.h) \
  /home/cpalaka/zephyr-project/include/arch/x86/asm.h \
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

arch/x86/core/crt0.o: $(deps_arch/x86/core/crt0.o)

$(deps_arch/x86/core/crt0.o):
