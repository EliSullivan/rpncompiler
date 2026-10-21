CC      = gcc
CFLAGS  = -Wall -Wextra -Wno-unused-parameter

.PHONY: all clean

#compiles parser and generates assembly code for each test case
all: test1 test2

parser: parser.c
	$(CC) $(CFLAGS) -o parser parser.c


test1: parser
	./parser test1.txt

test2: parser
	./parser test2.txt

clean:
	rm -f parser
	rm -f *.s