CFLAGS=-Wall -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

cccc: $(OBJS)
	$(CC) -o cccc $(OBJS) $(LDFLAGS)

$(OBJS): cccc.h

test: cccc
	./cccc --test
	./test.sh

clean:
	rm -f cccc *.o *~ *.swp tmp*
