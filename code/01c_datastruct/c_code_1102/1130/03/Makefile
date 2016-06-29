SRCS = foo.c test.c
OBJ = foo.o test.o
TGT = a.out
CC = gcc
RM = rm

# 指定源文件所在的当前目录
TOP_DIR = $(shell pwd)
HEAD_DIR = $(TOP_DIR)/include
CFLAGS = -I $(HEAD_DIR)

$(TGT): $(OBJ)
	@echo '$$? = ' $?
	$(CC) $(CFLAGS) $^

#$(OBJ): $(SRCS)
#	$(CC) $(CFLAGS) $^ -c
#下面的模式规则中：%匹配.c或.o前面的部分，其实就是test和foo
%.o: %.c
	@echo '$$* = ' $*
	$(CC) $(CFLAGS) $< -c

# 下面的模式规则仅仅用来告诉make，文件之间的依赖关系
# 我们的Makefile是应该能够自动判断哪些文件依赖了头文件，下面的规则用于解决因为头文件的更新，而没有使得所有依赖头文件的文件被重新生成的问题。
%.dep: %.c
	$(CC) $(CFLAGS) -MM $< > $@
include $(SRCS:.c=.dep)
#-include $(SRCS:.c=.dep)
#sinclude $(SRCS:.c=.dep) 
# 上面的三行均可，用于包含上面的依赖关系

# -rm 前面的-号可以避免因为某个错误而终止后续的继续删除操作
clean:
	-$(RM) -vf $(TGT) $(OBJ) $(SRCS:.c=.dep)

# .PHONY: 用来说明哪些规则中的目标是伪目标。伪目标的意思就是不要让make将其视为真正的目标，当执行该目标时，只是简单的执行该规则下的命令即可。
.PHONY: clean

