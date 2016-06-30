all:
	arm-linux-gcc -c key.c
	arm-linux-ld key.o
	arm-linux-objcopy -O binary a.out boot.bin

clean:
	$(RM) key.o a.out boot.bin
