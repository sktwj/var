TGT = a.out
SUB_DIR = test foo

export SUB_TGT = built-in.o
export TOP_DIR = $(shell pwd)
export HEAD_DIR = $(TOP_DIR)/include

#编译选项
export CFLAGS = -I $(HEAD_DIR)

#指定交叉编译环境
#export CROSS_COMPILER = arm-linux-
export CC = $(CROSS_COMPILER)gcc

#链接器
export LD = ld
#链接选项
export LDFLAGS = -g

# 终极目标所在的规则
$(TGT): $(SUB_DIR)
	$(CC) $(CFLAGS) $(^:=/$(SUB_TGT))
	
$(SUB_DIR): 
	make -C $@

clean:
	-rm -vf $(TGT) 
	for dir in $(SUB_DIR); do \
		make -C $$dir clean; \
	done

.PHONY: clean $(SUB_DIR)


