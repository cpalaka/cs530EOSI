# Kprobes test for HT530 hashtable 

Tests HT530 kernel hashtable using Kprobes. The test program (test.c) will take as user input a request consisting of the location (offset) of a source line of code in ht530, the location (offset) of a local variable in the probed function’s stack, and the location (offset) of a global variable in .data or .bss sections of ht530_drv module. The kprobe handler will then return as output the address of the kprobe, the pid of the running process that hits the probe, time stamp (x86 TSC), the value (4 bytes) of the local variable, and the value (4 bytes) of the global variable.

## Usage
```bash
make
sudo insmod ./Mprobe.ko
sudo insmod ./ht530_drv.ko
gcc test.c -o test
```

### Test program usage

This program must take 4 values as input: symbol name, offset into function (in hex), local variable location (in hex), and global variable location (in hex) 