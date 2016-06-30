TGT := boot.bin
OBJS := start.S
OBJC := main.c interrupt.c timer.c io.c

CROSS_COMPILER := arm-linux-
CC := $(CROSS_COMPILER)gcc
OBJCPY := $(CROSS_COMPILER)objcopy

CFLAGS := -ffreestanding -I include
LDFLAGS := -static -nostartfiles -Wl,-T,boot.lds,-Ttext,0x40000000

all:
	$(CC) $(CFLAGS) -c $(OBJS)
	$(CC) $(CFLAGS) -c $(OBJC)
	$(CC) $(LDFLAGS) $(OBJS:.S=.o) $(OBJC:.c=.o)
	$(OBJCPY) -O binary a.out $(TGT)

clean:
	$(RM) $(OBJS:.S=.o) $(OBJC:.c=.o) a.out $(TGT)
