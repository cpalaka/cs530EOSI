cmd_kernel/unified/version.o := /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/bin/i586-poky-elf/i586-poky-elf-gcc -Wp,-MD,kernel/unified/.version.o.d  -nostdinc -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/i586-poky-elf/gcc/i586-poky-elf/5.2.0/include-fixed -I/home/cpalaka/zephyr-project/kernel/unified/include -I/home/cpalaka/zephyr-project/arch/x86/include -I/home/cpalaka/zephyr-project/arch/x86/soc/intel_quark/quark_x1000 -I/home/cpalaka/zephyr-project/boards/x86/galileo  -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/include -I/home/cpalaka/zephyr-project/samples/HCSR_app/outdir/galileo/include/generated -I/home/cpalaka/zephyr-project/samples/HCSR_app/outdir/galileo/misc/generated/sysgen -include /home/cpalaka/zephyr-project/samples/HCSR_app/outdir/galileo/include/generated/autoconf.h  -I/home/cpalaka/zephyr-project/lib/libc/minimal/include  -I/home/cpalaka/zephyr-project/kernel/unified -Ikernel/unified -DKERNEL -D__ZEPHYR__=1 -c -g -std=c99 -fno-asynchronous-unwind-tables -Wall -Wno-format-zero-length -Wno-main -ffreestanding -Os -fno-stack-protector -fno-omit-frame-pointer -ffunction-sections -fdata-sections -mpreferred-stack-boundary=2 -mno-sse -march=pentium -Wno-unused-but-set-variable -fno-reorder-functions -fno-defer-pop -Wno-pointer-sign -fno-strict-overflow -Werror=implicit-int   -I/home/cpalaka/zephyr-project/kernel/unified/include    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(version)"  -D"KBUILD_MODNAME=KBUILD_STR(version)" -c -o kernel/unified/version.o /home/cpalaka/zephyr-project/kernel/unified/version.c

source_kernel/unified/version.o := /home/cpalaka/zephyr-project/kernel/unified/version.c

deps_kernel/unified/version.o := \
  /home/cpalaka/zephyr-project/lib/libc/minimal/include/stdint.h \
  /home/cpalaka/zephyr-project/samples/HCSR_app/outdir/galileo/include/generated/version.h \

kernel/unified/version.o: $(deps_kernel/unified/version.o)

$(deps_kernel/unified/version.o):
