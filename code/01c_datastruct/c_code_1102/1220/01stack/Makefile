TGT = a.out
SRCS = test.c stack.c

$(TGT): $(SRCS:.c=.o)
	gcc $^

%.o: %.c
	gcc $< -c

clean:
	-rm -vf $(TGT) $(SRCS:.c=.o)

.PHONY: clean
