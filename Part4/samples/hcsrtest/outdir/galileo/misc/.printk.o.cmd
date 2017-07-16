cmd_misc/printk.o := /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/bin/i586-poky-elf/i586-poky-elf-gcc -Wp,-MD,misc/.printk.o.d  -nostdinc -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include-fixed -I/home/cpalaka/zephyr-project/kernel/unified/include -I/home/cpalaka/zephyr-project/arch/x86/include -I/home/cpalaka/zephyr-project/arch/x86/soc/intel_quark/quark_x1000 -I/home/cpalaka/zephyr-project/boards/x86/galileo  -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/include/generated -I/home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/misc/generated/sysgen -include /home/cpalaka/zephyr-project/samples/hcsrtest/outdir/galileo/include/generated/autoconf.h  -I/home/cpalaka/zephyr-project/lib/libc/minimal/include  -I/home/cpalaka/zephyr-project/misc -Imisc -DKERNEL -D__ZEPHYR__=1 -c -g -std=c99 -fno-asynchronous-unwind-tables -Wall -Wno-format-zero-length -Wno-main -ffreestanding -Os -fno-stack-protector -fno-omit-frame-pointer -ffunction-sections -fdata-sections -mpreferred-stack-boundary=2 -mno-sse -march=pentium -Wno-unused-but-set-variable -fno-reorder-functions -fno-defer-pop -Wno-pointer-sign -fno-strict-overflow -Werror=implicit-int    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(printk)"  -D"KBUILD_MODNAME=KBUILD_STR(printk)" -c -o misc/printk.o /home/cpalaka/zephyr-project/misc/printk.c

source_misc/printk.o := /home/cpalaka/zephyr-project/misc/printk.c

deps_misc/printk.o := \
  /home/cpalaka/zephyr-project/include/misc/printk.h \
    $(wildcard include/config/printk.h) \
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
  /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include/stdarg.h \
  /home/cpalaka/zephyr-project/include/sections.h \
    $(wildcard include/config/gdb/info.h) \
    $(wildcard include/config/sw/isr/table.h) \
  /home/cpalaka/zephyr-project/include/section_tags.h \

misc/printk.o: $(deps_misc/printk.o)

$(deps_misc/printk.o):
