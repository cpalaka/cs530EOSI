#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <poll.h>
#include <sys/ioctl.h>

#include "Gpio_func.h"

int main ()
{
  int device_one;
  int arg[2];
  long long retval;
  // Set multiplexer pins
  mux_gpio_set(74, 0);
  mux_gpio_set(26, 0);
  mux_gpio_set(70, 0);
  mux_gpio_set(22, 1);
  // Open the module
  device_one = open("/dev/HCSR_2", O_RDWR);
  arg[0] = 4;
  arg[1] = 10;
  ioctl(device_one, 0, arg);
  arg[0] = 0;
  arg[1] = 5;
  printf("Setting sensor to ONE_SHOT mode.\n");
  ioctl(device_one, 1, arg);
  printf("Making a measurement.\n");
  write(device_one, arg, sizeof(int)*2);
  printf("Sleeping for 4 seconds.");
  sleep(4);
  printf("Making a second measurement.\n");
  write(device_one, arg, sizeof(int)*2);
  sleep(4);
  printf("Making a third measurement.\n");
  write(device_one, arg, sizeof(int)*2);
  sleep(4);
  printf("Making a fourth measurement.\n");
  write(device_one, arg, sizeof(int)*2);
  sleep(4);
  printf("Making a fifth measurement.\n");
  write(device_one, arg, sizeof(int)*2);
  sleep(4);
  printf("Making a sixth measurement.\n");
  printf("This is to test the limit of the buffer.\n");
  write(device_one, arg, sizeof(int)*2);
  printf("Reading the first measurement in the queue. (Originally second)\n");
  printf("This will also trigger a measurement reading.\n");
  read(device_one, &retval, sizeof(long long));
  printf("The read value is %llu", retval);
  printf("Writing with non-zero first argument. This will reset the buffer and trigger a measurement.\n");
  arg[0] = 1;
  arg[1] = 5;
  write(device_one, arg, sizeof(int)*2);
  printf("Reading the recent measurement.\n");
  read(device_one, &retval, sizeof(long long));
  printf("The read value is %llu\n", retval);
  arg[0] = 1;
  arg[1] = 5;
  write(device_one, arg, sizeof(int)*2);
  arg[0] = 1;
  arg[1] = 10;
  printf("Setting sensor to PERIODIC_SAMPLING mode. Frequency: %d\n", arg[1]);
  ioctl(device_one, 1, arg);
  arg[0] = 1;
  arg[1] = 10;
  printf("Starting sampling.\n");
  write(device_one, arg, sizeof(int)*2);
  printf("Reading a measurement. This will block till there is a reading.\n");
  read(device_one, &retval, sizeof(long long));
  printf("The read value is %llu\n", retval);
  arg[0] = 0;
  arg[1] = 10;
  printf("Stopping the sampling.\n");
  write(device_one, arg, sizeof(int)*2);
  close(device_one);
  return 0;
}
