CC = gcc
ARCH = x86

APP1 = user_test_one
APP2 = user_test_two

all: user-test_one.o user-test_two.o Gpio_func.o
	$(CC) user-test_one.o Gpio_func.o -o $(APP1) 
	$(CC) user-test_two.o Gpio_func.o -o $(APP2)

Gpio_func.o: Gpio_func.c 
	$(CC) -c -o -Wall Gpio_func.o Gpio_func.c 

user-test_one.o: user-test_one.c
	$(CC) -c -o -Wall user-test_one.o user-test_one.c  

user-test_two.o: user-test_two.c
	$(CC) -c -o -Wall user-test_two.o user-test_two.c

clean:
	rm -f *.o
	rm -f $(APP1) 
	rm -f $(APP2)
