# Platform Device Driver and Sysfs interface for HC-SR04 

## Usage
```bash
cd device
make
sudo insmod ./ultrasonic_sensor.ko
cd ../platform_driver
make
sudo insmod ./platform_driver.ko
```

Test the dev interface for the module using test programs from part 1 (../hcsr04driver/user).
Test the sysfs interface by running:
```bash
cd device
./systest.sh
```