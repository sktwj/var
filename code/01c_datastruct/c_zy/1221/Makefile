TGT = a.out
SRCS = main.c stack.c calc.c

$(TGT): $(SRCS:.c=.o)
	gcc $^

%.o: %.c
	gcc $< -c

clean:
	-rm -vf $(TGT) $(SRCS:.c=.o)

.PHONY: clean
