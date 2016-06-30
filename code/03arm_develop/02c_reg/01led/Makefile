all:
	arm-linux-gcc -c led.c
	arm-linux-ld led.o
	arm-linux-objcopy -O binary a.out boot.bin

clean:
	$(RM) led.o a.out boot.bin
