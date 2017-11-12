%define word_size 4
%define pixel_array r12
%define array_size r13
%define result r14
section .rodata
mask: db 0xfc

section .text
global sepia_asm

sepia_asm:
    mov pixel_array, rdi
    mov array_size, rsi
    and array_size, [mask]
    ret
