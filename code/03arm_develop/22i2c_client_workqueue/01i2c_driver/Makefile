obj-m := ldm_drv.o
$(obj-m:.o=)-objs := test.o

KERN_ROOT := ~/work/4412/linux/android/linux-3.5/
#KERN_ROOT := /lib/modules/`uname -r`/build/
NFSROOT := ~/work/nfsroot/

ccflags-y += -Wno-declaration-after-statement

all: build

build:
	make M=`pwd` -C $(KERN_ROOT) modules

mv:
	mv $(obj-m:.o=.ko) $(NFSROOT)

clean:
	make M=`pwd` -C $(KERN_ROOT) clean

.PHONY: all build mv clean
