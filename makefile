CC=arm-linux-gnueabihf-gcc
CFLAGS= -mfpu=neon -ftree-vectorize -static
ASM=nasm
ASMFLAGS=-f elf64

all: 
	$(CC) $(CFLAGS) -omain main.c functions.c
remove:
	rm main