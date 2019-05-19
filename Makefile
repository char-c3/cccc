cccc: cccc.c

test: cccc
	./test/sh

clean:
	rm -f cccc *.o *~ *.swp tmp*
