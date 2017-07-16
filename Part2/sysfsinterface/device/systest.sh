# Pin mux 1 for IO2
echo -n "77" > /sys/class/gpio/export
# Level shifter GPIO for IO2
echo -n "34" > /sys/class/gpio/export
echo -n "out" > /sys/class/gpio/gpio34/direction
echo -n "0" > /sys/class/gpio/gpio34/value
# Level shifter GPIO for IO4
echo -n "36" > /sys/class/gpio/export
echo -n "out" > /sys/class/gpio/gpio36/direction
echo -n "1" > /sys/class/gpio/gpio36/value
# Pin mux 1 for IO9
echo -n "70" > /sys/class/gpio/export
# Level shifter GPIO for IO9
echo -n "22" > /sys/class/gpio/export
echo -n "out" > /sys/class/gpio/gpio22/direction
echo -n "1" > /sys/class/gpio/gpio22/value
# Pin mux 1 for IO10
echo -n "74" > /sys/class/gpio/export
# Level shifter GPIO for IO10
echo -n "26" > /sys/class/gpio/export
echo -n "out" > /sys/class/gpio/gpio26/direction
echo -n "0" > /sys/class/gpio/gpio26/value

#testing device1
echo -n "13" > /sys/class/HCSR/HCSRdevice1/trigger
echo -n "6" > /sys/class/HCSR/HCSRdevice1/echo
echo -n "0" > /sys/class/HCSR/HCSRdevice1/mode
echo -n "1" > /sys/class/HCSR/HCSRdevice1/enable
cat /sys/class/HCSR/HCSRdevice1/distance

#testing device2
echo -n "10" > /sys/class/HCSR/HCSRdevice1/trigger
echo -n "4" > /sys/class/HCSR/HCSRdevice1/echo
echo -n "0" > /sys/class/HCSR/HCSRdevice1/mode
echo -n "1" > /sys/class/HCSR/HCSRdevice1/enable
cat /sys/class/HCSR/HCSRdevice1/distance