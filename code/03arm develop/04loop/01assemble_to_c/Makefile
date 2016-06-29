TGT := boot.bin
OBJS := start.o
OBJC := key.o

CROSS_COMPILER := arm-linux-
CC := $(CROSS_COMPILER)gcc
OBJCOPY := $(CROSS_COMPILER)objcopy
OBJDUMP := $(CROSS_COMPILER)objdump

CFLAGS := -ffreestanding
LDFLAGS := -static -nostartfiles

all: build

build:
	$(CC) $(CFLAGS) -c $(OBJS:.o=.S)
	$(CC) $(CFLAGS) -c $(OBJC:.o=.c)
	$(CC) $(LDFLAGS) $(OBJS) $(OBJC)
	$(OBJCOPY) -O binary a.out $(TGT)

dump:
	$(OBJDUMP) -d a.out

clean:
	$(RM) a.out $(OBJS) $(OBJC) $(TGT)
