TGT = a.out
SRCS = main.c sort.c

$(TGT): $(SRCS:.c=.o)
	gcc $^

QTGT = queue
QUEUE_SRC = test_queue.c queue.c
$(QTGT): $(QUEUE_SRC:.c=.o) 
	gcc $^ -o $(QTGT)

STGT = stack 
STACK_SRC = test_stack.c stack.c
$(STGT): $(STACK_SRC:.c=.o) 
	gcc $^ -o $(STGT)

%.o: %.c
	gcc $< -c

clean:
	-rm -vf $(TGT) $(SRCS:.c=.o) \
			$(STGT) $(QTGT) \
			$(STACK_SRC:.c=.o) $(QUEUE_SRC:.c=.o) 

.PHONY: clean
