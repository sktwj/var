TGT = a.out
SRCS = test.c list.c

$(TGT): $(SRCS:.c=.o)
	@echo '$$^' $^
	@echo '$$<' $<
	gcc $^ 

%.o: %.c
	gcc $< -c

clean:
	-rm -vf $(TGT) $(SRCS:.c=.o)

.PHONY: clean
