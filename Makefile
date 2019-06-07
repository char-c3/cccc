cccc: cccc.c

test: cccc
	./cccc --test
	./test.sh

clean:
	rm -f cccc *.o *~ *.swp tmp*
