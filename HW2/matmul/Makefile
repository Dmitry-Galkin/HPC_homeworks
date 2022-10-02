CC=gcc
CFLAGS=-c -g -O3

all: matmul_exec_time

matmul_exec_time: main.o matmul.o
	$(CC) main.o matmul.o -o matmul_exec_time
	
matmul.o: matmul.c
	$(CC) $(CFLAGS) matmul.c
	
main.o: main.c
	$(CC) $(CFLAGS) main.c
	
clean:
	rm -f *.o all

