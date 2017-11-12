CC=gcc
CFLAGS= -Wall -pedantic
ASM=nasm
ASMFLAGS=-f elf64

all: main.o sepia.o functions.o code.o
	$(CC) $(CFLAGS) functions.o sepia.o main.o code.o -o main

main.o:
	$(CC) $(CFLAGS) -c main.c -o main.o

sepia.o:
	$(CC) $(CFLAGS) -c sepia.c -o sepia.o

functions.o:
	$(CC) $(CFLAGS) -c functions.c -o functions.o

code.o:
	$(ASM) $(ASMFLAGS) code.asm -o  code.o

remove:
	rm main.o functions.o sepia.o code.o main