
all: test

test: memblock.h test.c
	gcc -Wall -Wextra -ggdb -I. -o test test.c

clean:
	rm -f test
