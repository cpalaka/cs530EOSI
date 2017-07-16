# CSE 530 Embedded Operation Systems Internals Project 

## HCSr04 Sensor Configuration

1. Device 1
- ECHO pin: IO pin 4
- TRIGGER pin: IO pin 2

2. Device 2
- ECHO pin: IO pin 9
- TRIGGER pin: IO pin 10

## Usage
```bash
cd kernel
make
insmod ultrasonic_sensor.ko

cd ../user
make all
./user_test_one
./user_test_two
```