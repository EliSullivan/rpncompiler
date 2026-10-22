CC      = gcc
CFLAGS  = -Wall -Wextra -Wno-unused-parameter

.PHONY: all clean

#compiles parser and generates assembly code for each test case
all: parser test1 test2 test3 testbad

parser: parser.c
	$(CC) $(CFLAGS) -o parser parser.c

test1: parser
	./parser test1.txt

test2: parser
	./parser test2.txt

test3: parser
	./parser test3.txt

#these should fail (except for ok)
testbad: parser
	./parser testbad.txt

clean:
	rm -f parser
	rm -f *.s