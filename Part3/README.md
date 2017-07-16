# CSE 530 A barrier mechanism in Linux kernel through the Syscall interface

## How to install and use

This folder contains the complete patch of the linux 3.8 kernel with the extended barrier functionality as syscalls.

### To patch your kernel:
1. Copy and paste all files from folder (excluding /userprogram and /barriertestmodules) to /linux kernel source
2. Run `make` and build/cross-compile the kernel: `ARCH=x86 LOCALVERSION= CROSS_COMPILE=i586-poky-linux- make -j4`
3. copy the bzImage to replace old image on boot device
4. boot into new kernel

### To test the barrier mechanism:
```
cd userprogram
make
```

and then run usertest program with integer argument for average sleep time in milliseconds.
