TGT := boot.bin
OBJS := start.S
OBJC := main.c led.c uart.c printk.c mmu.c bootm.c

CROSS_COMPILER := arm-linux-
CC := $(CROSS_COMPILER)gcc
LD := $(CROSS_COMPILER)ld
OBJCOPY := $(CROSS_COMPILER)objcopy
OBJDUMP := $(CROSS_COMPILER)objdump

CFLAGS := -ffreestanding
LDFLAGS := -static -nostartfiles -Wl,-Ttext,0x40000000,-T,boot.lds

all: build

build:
	$(CC) $(CFLAGS) -c $(OBJS) $(OBJC)
	$(CC) $(LDFLAGS) $(OBJS:.S=.o) $(OBJC:.c=.o)
	$(OBJCOPY) -O binary a.out $(TGT)

clean:
	$(RM) $(OBJS:.S=.o) $(OBJC:.c=.o) $(TGT) a.out

.PHONY: all clean build
