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

; rdi ctr.data()
; rsi n
; rdx r
; rcx ks.get().data()/weyl:key

; xmm8 counter
; xmm9 round key

%include "/common.asm"

global mckl_aes128_aesni_sse2_kernel
global mckl_aes192_aesni_sse2_kernel
global mckl_aes256_aesni_sse2_kernel
global mckl_ars_aesni_sse2_kernel

%macro aes_aesni_sse2_prologue 1
    prologue 4, (%1 - 5) * 0x10

    movdqu xmm8, [rdi]
    add [rdi], rsi

    %if %1 == 5 ; ARS
        movdqu xmm9,  [rcx + 0x00] ; weyl
        movdqu xmm10, [rcx + 0x10] ; key
        movdqa xmm11, xmm10
        paddq  xmm11, xmm9
        movdqa xmm12, xmm11
        paddq  xmm12, xmm9
        movdqa xmm13, xmm12
        paddq  xmm13, xmm9
        movdqa xmm14, xmm13
        paddq  xmm14, xmm9
        movdqa xmm15, xmm14
        paddq  xmm15, xmm9
    %else ; AES
        movdqu xmm10, [rcx + 0x00]
        movdqu xmm11, [rcx + 0x10]
        movdqu xmm12, [rcx + 0x20]
        movdqu xmm13, [rcx + 0x30]
        movdqu xmm14, [rcx + 0x40]
        movdqu xmm15, [rcx + 0x50]
        %if %1 > 5
            %assign r 0
            %rep %1 - 5
                movdqa [rsp + r * 0x10], xmm15
                movdqu xmm15, [rcx + (r + 6) * 0x10]
                %assign r r + 1
            %endrep
        %endif
    %endif
%endmacro

%macro aes_aesni_sse2_epilogue 0
    .return:
        epilogue
%endmacro

%macro aes_aesni_sse2_encfirst 1
    pxor xmm0, %1
    pxor xmm1, %1
    pxor xmm2, %1
    pxor xmm3, %1
    pxor xmm4, %1
    pxor xmm5, %1
    pxor xmm6, %1
    pxor xmm7, %1
%endmacro

%macro aes_aesni_sse2_enc 1
    aesenc xmm0, %1
    aesenc xmm1, %1
    aesenc xmm2, %1
    aesenc xmm3, %1
    aesenc xmm4, %1
    aesenc xmm5, %1
    aesenc xmm6, %1
    aesenc xmm7, %1
%endmacro

%macro aes_aesni_sse2_enclast 1
    aesenclast xmm0, %1
    aesenclast xmm1, %1
    aesenclast xmm2, %1
    aesenclast xmm3, %1
    aesenclast xmm4, %1
    aesenclast xmm5, %1
    aesenclast xmm6, %1
    aesenclast xmm7, %1
%endmacro

%macro aes_aesni_sse2_generate 1
    .generate:
        increment_sse2_xmm xmm8, 0x10
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

        cmp rsi, 8
        jl .storen

        movdqu [rdx + 0x00], xmm0
        movdqu [rdx + 0x10], xmm1
        movdqu [rdx + 0x20], xmm2
        movdqu [rdx + 0x30], xmm3
        movdqu [rdx + 0x40], xmm4
        movdqu [rdx + 0x50], xmm5
        movdqu [rdx + 0x60], xmm6
        movdqu [rdx + 0x70], xmm7
        sub rsi, 8
        add rdx, 0x80

        test rsi, rsi
        jnz .generate

        .storen:
            test rsi, rsi,
            jz .return
            movdqa [rsp - 0x80], xmm0
            movdqa [rsp - 0x70], xmm1
            movdqa [rsp - 0x60], xmm2
            movdqa [rsp - 0x50], xmm3
            movdqa [rsp - 0x40], xmm4
            movdqa [rsp - 0x30], xmm5
            movdqa [rsp - 0x20], xmm6
            movdqa [rsp - 0x10], xmm7
            mov rcx, rsi
            imul rcx, 2
            mov rsi, rsp
            sub rsi, 0x80
            mov rdi, rdx
            cld
            rep movsq
%endmacro

section .rodata

def_increment_xmm_data_2

section .text

mckl_aes128_aesni_sse2_kernel:
    aes_aesni_sse2_prologue 10
    aes_aesni_sse2_generate 10
    aes_aesni_sse2_epilogue
; mckl_aes128_aesni_sse2_kernel:

mckl_aes192_aesni_sse2_kernel:
    aes_aesni_sse2_prologue 12
    aes_aesni_sse2_generate 12
    aes_aesni_sse2_epilogue
; mckl_aes192_aesni_sse2_kernel:

mckl_aes256_aesni_sse2_kernel:
    aes_aesni_sse2_prologue 14
    aes_aesni_sse2_generate 14
    aes_aesni_sse2_epilogue
; mckl_aes256_aesni_sse2_kernel:

mckl_ars_aesni_sse2_kernel:
    aes_aesni_sse2_prologue 5
    aes_aesni_sse2_generate 5
    aes_aesni_sse2_epilogue
; mckl_ars_aesni_sse2_kernel:

; vim:ft=nasm
