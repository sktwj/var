TGT := boot.bin
OBJS := start.o
OBJC := main.o io.o

CROSS_COMPILER := arm-linux-
CC := $(CROSS_COMPILER)gcc
OBJCOPY := $(CROSS_COMPILER)objcopy
OBJDUMP := $(CROSS_COMPILER)objdump

CFLAGS := -ffreestanding -I./include
LDFLAGS := -static -nostartfiles -Wl,-Ttext,0x40000000,-T,boot.lds

all: build

build:
	$(CC) $(CFLAGS) -c $(OBJS:.o=.S)
	$(CC) $(CFLAGS) -c $(OBJC:.o=.c)
	$(CC) $(LDFLAGS) $(OBJS) $(OBJC)
	$(OBJCOPY) -O binary a.out $(TGT)

dump:
	$(OBJDUMP) -D a.out

clean:
	$(RM) a.out $(OBJS) $(OBJC) $(TGT)
