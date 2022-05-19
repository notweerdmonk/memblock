
all: test

test: memblock.h test.c
	gcc -Wall -ggdb -o test test.c

exp:
	gcc -fno-stack-protector -o exploit exploit.c

exp32:
	gcc -m32 -fno-stack-protector -o exploit32 exploit32.c

clean:
	rm -f test exploit exploit32
