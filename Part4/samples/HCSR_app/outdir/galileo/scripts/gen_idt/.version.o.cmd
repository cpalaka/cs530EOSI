cmd_scripts/gen_idt/version.o := gcc -Wp,-MD,scripts/gen_idt/.version.o.d -Iscripts/gen_idt -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu89 -DKERNEL_VERSION=0 -Wall -Werror -g -c -o scripts/gen_idt/version.o /home/cpalaka/zephyr-project/scripts/gen_idt/version.c

source_scripts/gen_idt/version.o := /home/cpalaka/zephyr-project/scripts/gen_idt/version.c

deps_scripts/gen_idt/version.o := \
  /usr/include/stdc-predef.h \
  /usr/include/stdio.h \
  /usr/include/features.h \
  /usr/include/x86_64-linux-gnu/sys/cdefs.h \
  /usr/include/x86_64-linux-gnu/bits/wordsize.h \
  /usr/include/x86_64-linux-gnu/gnu/stubs.h \
  /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
  /usr/lib/gcc/x86_64-linux-gnu/4.8/include/stddef.h \
  /usr/include/x86_64-linux-gnu/bits/types.h \
  /usr/include/x86_64-linux-gnu/bits/typesizes.h \
  /usr/include/libio.h \
  /usr/include/_G_config.h \
  /usr/include/wchar.h \
  /usr/lib/gcc/x86_64-linux-gnu/4.8/include/stdarg.h \
  /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
  /usr/include/x86_64-linux-gnu/bits/sys_errlist.h \
  /usr/include/x86_64-linux-gnu/bits/stdio.h \
  /usr/include/x86_64-linux-gnu/bits/stdio2.h \
  /home/cpalaka/zephyr-project/scripts/gen_idt/version.h \

scripts/gen_idt/version.o: $(deps_scripts/gen_idt/version.o)

$(deps_scripts/gen_idt/version.o):
