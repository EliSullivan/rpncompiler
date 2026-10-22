CC      = gcc
CFLAGS  = -Wall -Wextra -Wno-unused-parameter

.PHONY: all clean

#compiles parser and generates assembly code for each test case
all: test1 test2 test3 test4 testbad

parser: parser.c
	$(CC) $(CFLAGS) -o parser parser.c

test1: parser
	./parser test1.txt

test2: parser
	./parser test2.txt

test3: parser
	./parser test3.txt

test4: parser
	./parser test4.txt

#these should fail
testbad: parser
	./parser testbad.txt

clean:
	rm -f parser
	rm -f *.s