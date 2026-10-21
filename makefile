CC      = gcc
CFLAGS  = -g -Wall -Werror -Wextra -Wno-unused-parameter -fno-pie -no-pie
LDFLAGS = -no-pie

.PHONY: all clean

all: test1 test2

parser: parser.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o parser parser.c

test1: parser
	./gen test1.txt

test2: parser
	./parser test2.txt

clean:
	rm -f parser
	rm -f *.s