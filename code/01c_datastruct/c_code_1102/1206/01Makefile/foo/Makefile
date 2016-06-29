SRCS = foo.c
SUB_DIR = luck

#生成子目标的规则
$(SUB_TGT): $(SRCS:.c=.o) $(SUB_DIR)
	$(LD) -r $(SRCS:.c=.o) $(SUB_DIR:=/$(SUB_TGT)) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -c

$(SUB_DIR):
	make -C $@

%.d: %.c
	$(CC) $(CFLAGS) -MM $< > $@.$$$$; \
	sed 's/$*.o: /$*.o $*.d: /' $@.$$$$ > $@; \
	rm -f $@.$$$$
#$(CC) $(CFLAGS) -MM $< > $@
sinclude $(SRCS:.c=.d)

.PHONY: clean $(SUB_DIR)

clean:
	-rm -vf $(SUB_TGT) $(SRCS:.c=.o) $(SRCS:.c=.d)
	for dir in $(SUB_DIR); do \
		make -C $$dir clean; \
	done

