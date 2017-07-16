# HT530 Driver

This is a Linux kernel module for a Hash table which can be accessed using a /dev/ and /proc/ filesystem interface. A test program is also provided (user_program_test.c).

## Usage
```bash
make
sudo insmod ./ht530_drv.ko
gcc user_program_test.c -pthread -o test_program
./test_program
```