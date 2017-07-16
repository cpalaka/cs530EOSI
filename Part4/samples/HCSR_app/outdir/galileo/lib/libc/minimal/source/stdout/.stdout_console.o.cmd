cmd_lib/libc/minimal/source/stdout/stdout_console.o := /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/bin/i586-poky-elf/i586-poky-elf-gcc -Wp,-MD,lib/libc/minimal/source/stdout/.stdout_console.o.d  -nostdinc -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include-fixed -I/home/cpalaka/zephyr-project/kernel/unified/include -I/home/cpalaka/zephyr-project/arch/x86/include -I/home/cpalaka/zephyr-project/arch/x86/soc/intel_quark/quark_x1000 -I/home/cpalaka/zephyr-project/boards/x86/galileo  -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/samples/HCSR_app/outdir/galileo/include/generated -I/home/cpalaka/zephyr-project/samples/HCSR_app/outdir/galileo/misc/generated/sysgen -include /home/cpalaka/zephyr-project/samples/HCSR_app/outdir/galileo/include/generated/autoconf.h  -I/home/cpalaka/zephyr-project/lib/libc/minimal/include  -I/home/cpalaka/zephyr-project/lib/libc/minimal/source/stdout -Ilib/libc/minimal/source/stdout -DKERNEL -D__ZEPHYR__=1 -c -g -std=c99 -fno-asynchronous-unwind-tables -Wall -Wno-format-zero-length -Wno-main -ffreestanding -Os -fno-stack-protector -fno-omit-frame-pointer -ffunction-sections -fdata-sections -mpreferred-stack-boundary=2 -mno-sse -march=pentium -Wno-unused-but-set-variable -fno-reorder-functions -fno-defer-pop -Wno-pointer-sign -fno-strict-overflow -Werror=implicit-int    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(stdout_console)"  -D"KBUILD_MODNAME=KBUILD_STR(stdout_console)" -c -o lib/libc/minimal/source/stdout/stdout_console.o /home/cpalaka/zephyr-project/lib/libc/minimal/source/stdout/stdout_console.c

source_lib/libc/minimal/source/stdout/stdout_console.o := /home/cpalaka/zephyr-project/lib/libc/minimal/source/stdout/stdout_console.c

deps_lib/libc/minimal/source/stdout/stdout_console.o := \
  /home/cpalaka/zephyr-project/lib/libc/minimal/include/stdio.h \
  /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include/stdarg.h \
  /home/cpalaka/zephyr-project/lib/libc/minimal/include/bits/restrict.h \
  /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include/stddef.h \

lib/libc/minimal/source/stdout/stdout_console.o: $(deps_lib/libc/minimal/source/stdout/stdout_console.o)

$(deps_lib/libc/minimal/source/stdout/stdout_console.o):
