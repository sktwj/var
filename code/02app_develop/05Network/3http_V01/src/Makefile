TGT := uphttpd
SRCS := uphttpd.c up_conf.c up_log.c
CFLAGS += -g -D_DEBUG -Wall -I.
DEPS  := .deps


$(TGT) : $(SRCS:%.c=%.o)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< 

sinclude $(DEPS)

$(DEPS): $(SRCS)
	$(CC) -MM $^ > $@

.PHONY: clean
clean:
	$(RM) *.o $(TGT) $(DEPS)

