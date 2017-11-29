%define word_size 4
%define pixel_array r11
%define array_size r12
%define result r13
%define bArr r14
%define max_values r8

global sepia_asm
sepia_asm:
    push r11
    push r12
    push r13
    push r14
    mov pixel_array, rdi
    mov array_size, rsi
    and array_size, 0xfffffffffffffffc
    mov result, rdx
    mov bArr, rcx
    movaps xmm6, [max_values]
    test array_size, array_size
    jz .end
.loop:  pxor xmm0, xmm0
        pxor xmm1, xmm1
        pxor xmm2, xmm2
        pinsrb xmm0, byte[pixel_array], 0
        shufps xmm0, xmm0, 00000000
        pinsrb xmm0, byte[pixel_array +3], 12
        cvtdq2ps xmm0, xmm0
        pinsrb xmm1, byte[pixel_array+ 1], 0
        shufps xmm1, xmm1, 00000000
        pinsrb xmm1, byte[pixel_array + 4], 12
        cvtdq2ps xmm1, xmm1

        pinsrb xmm2, byte[pixel_array + 2], 0
        shufps xmm2, xmm2, 00000000
        pinsrb xmm2, byte[pixel_array + 5], 12
        cvtdq2ps xmm2, xmm2

    movaps xmm3, [bArr]
    movaps xmm4, [bArr+48]
    movaps xmm5, [bArr+96]
    mulps xmm0, xmm3
    mulps xmm1, xmm4
    mulps xmm2, xmm5
    addps xmm0, xmm1
    addps xmm0, xmm2
    cvtps2dq xmm0, xmm0
    pminsd xmm0, xmm6
        pextrb [result], xmm0, 0
        pextrb [result + 1], xmm0, 4
        pextrb [result + 2], xmm0, 8
        pextrb [result + 3], xmm0, 12
    add pixel_array, 3
    add result, 4
;Second iteration
        pxor xmm0, xmm0
        pxor xmm1, xmm1
        pxor xmm2, xmm2
            pinsrb xmm0, [pixel_array], 0
            pinsrb xmm0, [pixel_array + 3], 8
        shufps xmm0, xmm0, 0b00001010
        cvtdq2ps xmm0, xmm0
           pinsrb xmm1, [pixel_array + 1], 0
           pinsrb xmm1, [pixel_array + 4], 8
       shufps xmm1, xmm1, 0b00001010
       cvtdq2ps xmm1, xmm1
           pinsrb xmm2, [pixel_array + 2], 0
           pinsrb xmm2, [pixel_array + 5], 8
       shufps xmm2, xmm2, 0b00001010
       cvtdq2ps xmm2, xmm2

    lea bArr, [bArr + 16]
    movaps xmm3, [bArr]
    movaps xmm4, [bArr+48]
    movaps xmm5, [bArr+96]
    mulps xmm0, xmm3
    mulps xmm1, xmm4
    mulps xmm2, xmm5
    addps xmm0, xmm1
    addps xmm0, xmm2
    cvtps2dq xmm0, xmm0
    pminsd xmm0, xmm6
        pextrb [result], xmm0, 0
        pextrb [result + 1], xmm0, 4
        pextrb [result + 2], xmm0, 8
        pextrb [result + 3], xmm0, 12
    add pixel_array, 3
    add result, 4
;Third iteration
        pxor xmm0, xmm0
        pxor xmm1, xmm1
        pxor xmm2, xmm2
            pinsrb xmm0, byte[pixel_array], 0
            pinsrb xmm0, byte[pixel_array+3], 4
        shufps xmm0, xmm0, 0b00010101
        cvtdq2ps xmm0, xmm0
            pinsrb xmm1, byte[pixel_array + 1], 0
            pinsrb xmm1, byte[pixel_array + 4], 4
        shufps xmm1, xmm1, 0b00010101
        cvtdq2ps xmm1, xmm1
            pinsrb xmm2, byte[pixel_array + 2], 0
            pinsrb xmm2, byte[pixel_array + 5], 4
        shufps xmm2, xmm2, 0b00010101
        cvtdq2ps xmm2, xmm2

    lea bArr, [bArr + 16]
    movaps xmm3, [bArr]
    movaps xmm4, [bArr+48]
    movaps xmm5, [bArr+96]
    mulps xmm0, xmm3
    mulps xmm1, xmm4
    mulps xmm2, xmm5
    addps xmm0, xmm1
    addps xmm0, xmm2
    cvtps2dq xmm0, xmm0
    pminsd xmm0, xmm6
        pextrb [result], xmm0, 0
        pextrb [result + 1], xmm0, 4
        pextrb [result + 2], xmm0, 8
        pextrb [result + 3], xmm0, 12
    add pixel_array, 6
    add result, 4
    lea bArr, [bArr - 32]
    sub array_size, 4
    jnz .loop
.end:
    pop r14
    pop r13
    pop r12
    pop r11
    ret