
all: test

test: memblock.h test.c
	gcc -Wall -ggdb -o test test.c

exp:
	gcc -o exploit exploit.c

exp32:
	gcc -m32 -o exploit32 exploit32.c

clean:
	rm -f test exploit exploit32
