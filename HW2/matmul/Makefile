CC=gcc
CFLAGS=-c -g -O3

all: matmul

matmul: main.o matmul.o
	$(CC) main.o matmul.o -o matmul
	
matmul.o: matmul.c
	$(CC) $(CFLAGS) matmul.c
	
main.o: main.c
	$(CC) $(CFLAGS) main.c
	
clean:
	rm -f *.o all

