CFLAGS=-std=c11 -g -static

01cc:	01cc.c

test:	01cc
		./test.sh

clean:
		rm -f 01cc *.o *~ tmp*

.PHONY: test clean