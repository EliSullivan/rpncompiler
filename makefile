CC      = gcc
CFLAGS  = -g -Wall -Werror -Wextra -Wno-unused-parameter -fno-pie -no-pie
LDFLAGS = -no-pie

.PHONY: all clean

all: test1 test2

clean: rm -f a.out
	rm -f *.s