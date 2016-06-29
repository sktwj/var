TGT := boot.bin
SRCS := flow_led.o

CROSS_COMPILER := arm-linux-
CC := $(CROSS_COMPILER)gcc
OBJCOPY := $(CROSS_COMPILER)objcopy

CFLAGS := -ffreestanding
LDFLAGS := -static -nostartfiles -Wl,-Ttext,0x40000000

all:
	$(CC) $(CFLAGS) -c $(SRCS:.o=.c)
	$(CC) $(LDFLAGS) $(SRCS)
	$(OBJCOPY) -O binary a.out $(TGT)

clean:
	$(RM) a.out $(SRCS) $(TGT)
