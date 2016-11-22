;;============================================================================
;; MCKL/lib/asm/aes_aesni_sse2.asm
;;----------------------------------------------------------------------------
;; MCKL: Monte Carlo Kernel Library
;;----------------------------------------------------------------------------
;; Copyright (c) 2013-2016, Yan Zhou
;; All rights reserved.
;;
;; Redistribution and use in source and binary forms, with or without
;; modification, are permitted provided that the following conditions are met:
;;
;;   Redistributions of source code must retain the above copyright notice,
;;   this list of conditions and the following disclaimer.
;;
;;   Redistributions in binary form must reproduce the above copyright notice,
;;   this list of conditions and the following disclaimer in the documentation
;;   and/or other materials provided with the distribution.
;;
;; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
;; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;; ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
;; LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
;; CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;; SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;; INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;; CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
;; ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;; POSSIBILITY OF SUCH DAMAGE.
;;============================================================================

global mckl_aes128_aesni_sse2_kernel
global mckl_aes192_aesni_sse2_kernel
global mckl_aes256_aesni_sse2_kernel
global mckl_ars_aesni_sse2_kernel

%macro aes_aesni_sse2_encfirst 1 ; {{{
    pxor xmm0, %1
    pxor xmm1, %1
    pxor xmm2, %1
    pxor xmm3, %1
    pxor xmm4, %1
    pxor xmm5, %1
    pxor xmm6, %1
    pxor xmm7, %1
%endmacro ; }}}

%macro aes_aesni_sse2_enc 1 ; {{{
    aesenc xmm0, %1
    aesenc xmm1, %1
    aesenc xmm2, %1
    aesenc xmm3, %1
    aesenc xmm4, %1
    aesenc xmm5, %1
    aesenc xmm6, %1
    aesenc xmm7, %1
%endmacro ; }}}

%macro aes_aesni_sse2_enclast 1 ; {{{
    aesenclast xmm0, %1
    aesenclast xmm1, %1
    aesenclast xmm2, %1
    aesenclast xmm3, %1
    aesenclast xmm4, %1
    aesenclast xmm5, %1
    aesenclast xmm6, %1
    aesenclast xmm7, %1
%endmacro ; }}}

; rdi ctr.data()
; rsi n
; rdx r
; rcx ks.get().data()/weyl:key
%macro aes_aesni_sse2_kernel 1 ; rounds {{{
    push rbp
    mov rbp, rsp

    ; early return
    test rsi, rsi
    jz .return

    ; allocate stack space of max(8, rounds - 5) * 16 bytes
    and rsp, 0xFFFF_FFFF_FFFF_FFF0
    %if %1 < 13
        sub rsp, 0x80
    %else
        sub rsp, (%1 - 5) * 0x10
    %endif

    ; load counter
    movdqu xmm8, [rdi]
    add [rdi], rsi

    ; load/compute round keys
    %if %1 == 5 ; ARS
        movdqu xmm9,  [rcx + 0x00] ; weyl
        movdqu xmm10, [rcx + 0x10] ; round_key[0]
        movdqa xmm11, xmm10
        paddq  xmm11, xmm9 ; round_key[1]
        movdqa xmm12, xmm11
        paddq  xmm12, xmm9 ; round_key[2]
        movdqa xmm13, xmm12
        paddq  xmm13, xmm9 ; round_key[3]
        movdqa xmm14, xmm13
        paddq  xmm14, xmm9 ; round_key[4]
        movdqa xmm15, xmm14
        paddq  xmm15, xmm9 ; round_key[5]
    %else ; AES
        movdqu xmm10, [rcx + 0x00] ; round_key[0]
        movdqu xmm11, [rcx + 0x10] ; round_key[1]
        movdqu xmm12, [rcx + 0x20] ; round_key[2]
        movdqu xmm13, [rcx + 0x30] ; round_key[3]
        movdqu xmm14, [rcx + 0x40] ; round_key[4]
        movdqu xmm15, [rcx + 0x50] ; round_key[5]
        %if %1 > 5
            %assign r 0
            %rep %1 - 5
                movdqa [rsp + r * 0x10], xmm15
                movdqu xmm15, [rcx + (r + 6) * 0x10] ; round_key[r + 6]
                %assign r r + 1
            %endrep
        %endif
    %endif

    mov rcx, rsi ; n
    mov rdi, rdx ; r

    ; generate a new batch of 8 blocks
    .generate:
        movdqa xmm0, xmm8
        movdqa xmm1, xmm8
        movdqa xmm2, xmm8
        movdqa xmm3, xmm8
        movdqa xmm4, xmm8
        movdqa xmm5, xmm8
        movdqa xmm6, xmm8
        movdqa xmm7, xmm8
        paddq xmm0, [rel aes_aesni_sse2_increment_data + 0x00]
        paddq xmm1, [rel aes_aesni_sse2_increment_data + 0x10]
        paddq xmm2, [rel aes_aesni_sse2_increment_data + 0x20]
        paddq xmm3, [rel aes_aesni_sse2_increment_data + 0x30]
        paddq xmm4, [rel aes_aesni_sse2_increment_data + 0x40]
        paddq xmm5, [rel aes_aesni_sse2_increment_data + 0x50]
        paddq xmm6, [rel aes_aesni_sse2_increment_data + 0x60]
        paddq xmm7, [rel aes_aesni_sse2_increment_data + 0x70]
        movdqa xmm8, xmm7
        aes_aesni_sse2_encfirst xmm10
        aes_aesni_sse2_enc xmm11
        aes_aesni_sse2_enc xmm12
        aes_aesni_sse2_enc xmm13
        aes_aesni_sse2_enc xmm14
        %if %1 > 5
            %assign r 0
            %rep %1 - 5
                movdqa xmm9, [rsp + r * 0x10]
                aes_aesni_sse2_enc xmm9
                %assign r r + 1
            %endrep
        %endif
        aes_aesni_sse2_enclast xmm15

    ; check if this is the last batch
    cmp rcx, 8
    jl .last

    ; store the current batch direclty to r
    movdqu [rdi + 0x00], xmm0
    movdqu [rdi + 0x10], xmm1
    movdqu [rdi + 0x20], xmm2
    movdqu [rdi + 0x30], xmm3
    movdqu [rdi + 0x40], xmm4
    movdqu [rdi + 0x50], xmm5
    movdqu [rdi + 0x60], xmm6
    movdqu [rdi + 0x70], xmm7
    sub rcx, 8
    add rdi, 0x80

    ; check if a new batch is needed
    test rcx, rcx
    jz .return
    jmp .generate

    ; store the last batch on the stack and then move to r
    .last:
        movdqa [rsp + 0x00], xmm0
        movdqa [rsp + 0x10], xmm1
        movdqa [rsp + 0x20], xmm2
        movdqa [rsp + 0x30], xmm3
        movdqa [rsp + 0x40], xmm4
        movdqa [rsp + 0x50], xmm5
        movdqa [rsp + 0x60], xmm6
        movdqa [rsp + 0x70], xmm7
        shl rcx, 1
        mov rsi, rsp
        cld
        rep movsq

    .return:
        mov rsp, rbp
        pop rbp
        ret
%endmacro ; }}}

section .rodata

align 16
aes_aesni_sse2_increment_data:
dq 0x01, 0x00
dq 0x02, 0x00
dq 0x03, 0x00
dq 0x04, 0x00
dq 0x05, 0x00
dq 0x06, 0x00
dq 0x07, 0x00
dq 0x08, 0x00
dq 0x08, 0x00

section .text

mckl_aes128_aesni_sse2_kernel: aes_aesni_sse2_kernel 10
mckl_aes192_aesni_sse2_kernel: aes_aesni_sse2_kernel 12
mckl_aes256_aesni_sse2_kernel: aes_aesni_sse2_kernel 14
mckl_ars_aesni_sse2_kernel:    aes_aesni_sse2_kernel 5

; vim:ft=nasm
