;;============================================================================
;; MCKL/lib/asm/aes_aesni.asm
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

; rdi r
; rsi ctr.data()
; rdx ks.get().data()/weyl:key
; rcx n

%include "/common.asm"

global mckl_aes128_aesni_sse2_kernel
global mckl_aes128_aesni_avx2_kernel
global mckl_aes192_aesni_sse2_kernel
global mckl_aes192_aesni_avx2_kernel
global mckl_aes256_aesni_sse2_kernel
global mckl_aes256_aesni_avx2_kernel
global mckl_ars_aesni_sse2_kernel
global mckl_ars_aesni_avx2_kernel

%macro aes_aesni_prologue 1
    prologue 4, (%1 - 5) * 0x10
    mov rax, rsi
    mov rsi, rdi
    mov rdi, rdx
    mov rdx, rcx
    mov rcx, rax
%endmacro

%macro aes_aesni_sse2_round_key 1
    movdqu xmm10, [rdx + 0x00]
    movdqu xmm11, [rdx + 0x10]
    movdqu xmm12, [rdx + 0x20]
    movdqu xmm13, [rdx + 0x30]
    movdqu xmm14, [rdx + 0x40]
    movdqu xmm15, [rdx + 0x50]
    %assign r 0
    %rep %1 - 5
        movdqu xmm0, [rdx + (r + 6) * 0x10]
        movdqa [rsp + r * 0x10], xmm0
    %assign r r + 1
    %endrep
%endmacro

%macro aes_aesni_avx2_round_key 1
    vmovdqu xmm10, [rdx + 0x00]
    vmovdqu xmm11, [rdx + 0x10]
    vmovdqu xmm12, [rdx + 0x20]
    vmovdqu xmm13, [rdx + 0x30]
    vmovdqu xmm14, [rdx + 0x40]
    vmovdqu xmm15, [rdx + 0x50]
    %assign r 0
    %rep %1 - 5
        vmovdqu xmm0, [rdx + (r + 6) * 0x10]
        vmovdqa [rsp + r * 0x10], xmm0
    %assign r r + 1
    %endrep
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

%macro aes_aesni_avx2_encfirst 1
    vpxor xmm0, xmm0, %1
    vpxor xmm1, xmm1, %1
    vpxor xmm2, xmm2, %1
    vpxor xmm3, xmm3, %1
    vpxor xmm4, xmm4, %1
    vpxor xmm5, xmm5, %1
    vpxor xmm6, xmm6, %1
    vpxor xmm7, xmm7, %1
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

%macro aes_aesni_avx2_enc 1
    vaesenc xmm0, xmm0, %1
    vaesenc xmm1, xmm1, %1
    vaesenc xmm2, xmm2, %1
    vaesenc xmm3, xmm3, %1
    vaesenc xmm4, xmm4, %1
    vaesenc xmm5, xmm5, %1
    vaesenc xmm6, xmm6, %1
    vaesenc xmm7, xmm7, %1
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

%macro aes_aesni_avx2_enclast 1
    vaesenclast xmm0, xmm0, %1
    vaesenclast xmm1, xmm1, %1
    vaesenclast xmm2, xmm2, %1
    vaesenclast xmm3, xmm3, %1
    vaesenclast xmm4, xmm4, %1
    vaesenclast xmm5, xmm5, %1
    vaesenclast xmm6, xmm6, %1
    vaesenclast xmm7, xmm7, %1
%endmacro

%macro aes_aesni_sse2_generate 1
    movdqu xmm8, [rsi]
    add [rsi], rcx

    align 16
    .generate:
        increment_sse2_xmm xmm8, 0x10
        aes_aesni_sse2_encfirst xmm10
        aes_aesni_sse2_enc xmm11
        aes_aesni_sse2_enc xmm12
        aes_aesni_sse2_enc xmm13
        aes_aesni_sse2_enc xmm14
        %if %1 == 5
            aes_aesni_sse2_enclast xmm15
        %else
            aes_aesni_sse2_enc xmm15
            %assign r 0
            %rep %1 - 6
                vmovdqa xmm9, [rsp + r * 0x10]
                aes_aesni_sse2_enc xmm9
                %assign r r + 1
            %endrep
            vmovdqa xmm9, [rsp + (%1 - 6) * 0x10]
            aes_aesni_sse2_enclast xmm9
        %endif

        cmp rcx, 8
        jl .storen

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

        test rcx, rcx
        jnz .generate

        .storen:
            movdqa [rsp - 0x80], xmm0
            movdqa [rsp - 0x70], xmm1
            movdqa [rsp - 0x60], xmm2
            movdqa [rsp - 0x50], xmm3
            movdqa [rsp - 0x40], xmm4
            movdqa [rsp - 0x30], xmm5
            movdqa [rsp - 0x20], xmm6
            movdqa [rsp - 0x10], xmm7
            mov rsi, rsp
            sub rsi, 0x80
            imul rcx, 4
            rep movsd
%endmacro

%macro aes_aesni_avx2_generate 1
    vmovdqu xmm8, [rsi]
    add [rsi], rcx

    align 16
    .generate:
        increment_avx2_xmm xmm8, 0x10
        aes_aesni_avx2_encfirst xmm10
        aes_aesni_avx2_enc xmm11
        aes_aesni_avx2_enc xmm12
        aes_aesni_avx2_enc xmm13
        aes_aesni_avx2_enc xmm14
        %if %1 == 5
            aes_aesni_avx2_enclast xmm15
        %else
            aes_aesni_avx2_enc xmm15
            %assign r 0
            %rep %1 - 6
                vmovdqa xmm9, [rsp + r * 0x10]
                aes_aesni_avx2_enc xmm9
                %assign r r + 1
            %endrep
            vmovdqa xmm9, [rsp + (%1 - 6) * 0x10]
            aes_aesni_avx2_enclast xmm9
        %endif

        cmp rcx, 8
        jl .storen

        vmovdqu [rdi + 0x00], xmm0
        vmovdqu [rdi + 0x10], xmm1
        vmovdqu [rdi + 0x20], xmm2
        vmovdqu [rdi + 0x30], xmm3
        vmovdqu [rdi + 0x40], xmm4
        vmovdqu [rdi + 0x50], xmm5
        vmovdqu [rdi + 0x60], xmm6
        vmovdqu [rdi + 0x70], xmm7
        sub rcx, 8
        add rdi, 0x80

        test rcx, rcx
        jnz .generate

        .storen:
            vmovdqa [rsp - 0x80], xmm0
            vmovdqa [rsp - 0x70], xmm1
            vmovdqa [rsp - 0x60], xmm2
            vmovdqa [rsp - 0x50], xmm3
            vmovdqa [rsp - 0x40], xmm4
            vmovdqa [rsp - 0x30], xmm5
            vmovdqa [rsp - 0x20], xmm6
            vmovdqa [rsp - 0x10], xmm7
            mov rsi, rsp
            sub rsi, 0x80
            imul rcx, 4
            rep movsd
%endmacro

section .rodata

def_increment_xmm_data_2

section .text

mckl_aes128_aesni_sse2_kernel:
    aes_aesni_prologue 10
    aes_aesni_sse2_round_key 10
    aes_aesni_sse2_generate 10
    epilogue
; mckl_aes128_aesni_avx2_kernel:

mckl_aes128_aesni_avx2_kernel:
    aes_aesni_prologue 10
    aes_aesni_avx2_round_key 10
    aes_aesni_avx2_generate 10
    vzeroupper
    epilogue
; mckl_aes128_aesni_avx2_kernel:

mckl_aes192_aesni_sse2_kernel:
    aes_aesni_prologue 12
    aes_aesni_sse2_round_key 12
    aes_aesni_sse2_generate 12
    epilogue
; mckl_aes192_aesni_avx2_kernel:

mckl_aes192_aesni_avx2_kernel:
    aes_aesni_prologue 12
    aes_aesni_avx2_round_key 12
    aes_aesni_avx2_generate 12
    vzeroupper
    epilogue
; mckl_aes192_aesni_avx2_kernel:

mckl_aes256_aesni_sse2_kernel:
    aes_aesni_prologue 14
    aes_aesni_sse2_round_key 14
    aes_aesni_sse2_generate 14
    epilogue
; mckl_aes256_aesni_avx2_kernel:

mckl_aes256_aesni_avx2_kernel:
    aes_aesni_prologue 14
    aes_aesni_avx2_round_key 14
    aes_aesni_avx2_generate 14
    vzeroupper
    epilogue
; mckl_aes256_aesni_avx2_kernel:

mckl_ars_aesni_sse2_kernel:
    aes_aesni_prologue 5
    movdqu xmm9,  [rdx + 0x00]
    movdqu xmm10, [rdx + 0x10]
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
    aes_aesni_avx2_generate 5
    epilogue
; mckl_ars_aesni_avx2_kernel:

mckl_ars_aesni_avx2_kernel:
    aes_aesni_prologue 5
    vmovdqu xmm9,  [rdx + 0x00]
    vmovdqu xmm10, [rdx + 0x10]
    vpaddq  xmm11, xmm10, xmm9
    vpaddq  xmm12, xmm11, xmm9
    vpaddq  xmm13, xmm12, xmm9
    vpaddq  xmm14, xmm13, xmm9
    vpaddq  xmm15, xmm14, xmm9
    aes_aesni_avx2_generate 5
    vzeroupper
    epilogue
; mckl_ars_aesni_avx2_kernel:

; vim:ft=nasm
