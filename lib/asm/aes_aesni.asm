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

; rdi ctr.data()
; rsi n
; rdx r
; rcx ks.get().data()/weyl:key

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
%endmacro

%macro aes_aesni_sse2_round_key 1
    movdqu xmm10, [rcx + 0x00]
    movdqu xmm11, [rcx + 0x10]
    movdqu xmm12, [rcx + 0x20]
    movdqu xmm13, [rcx + 0x30]
    movdqu xmm14, [rcx + 0x40]
    movdqu xmm15, [rcx + 0x50]
    %assign r 0
    %rep %1 - 5
        movdqu xmm0, [rcx + (r + 6) * 0x10]
        movdqa [rsp + r * 0x10], xmm0
    %assign r r + 1
    %endrep
%endmacro

%macro aes_aesni_avx2_round_key 1
    vmovdqu xmm10, [rcx + 0x00]
    vmovdqu xmm11, [rcx + 0x10]
    vmovdqu xmm12, [rcx + 0x20]
    vmovdqu xmm13, [rcx + 0x30]
    vmovdqu xmm14, [rcx + 0x40]
    vmovdqu xmm15, [rcx + 0x50]
    %assign r 0
    %rep %1 - 5
        vmovdqu xmm0, [rcx + (r + 6) * 0x10]
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
    movdqu xmm8, [rdi]
    add [rdi], rsi

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
            cmp rsi, 1
            jl .return
            movdqu [rdx + 0x00], xmm0
            cmp rsi, 2
            jl .return
            movdqu [rdx + 0x10], xmm1
            cmp rsi, 3
            jl .return
            movdqu [rdx + 0x20], xmm2
            cmp rsi, 4
            jl .return
            movdqu [rdx + 0x30], xmm3
            cmp rsi, 5
            jl .return
            movdqu [rdx + 0x40], xmm4
            cmp rsi, 6
            jl .return
            movdqu [rdx + 0x50], xmm5
            cmp rsi, 7
            jl .return
            movdqu [rdx + 0x60], xmm6
%endmacro

%macro aes_aesni_avx2_generate 1
    vmovdqu xmm8, [rdi]
    add [rdi], rsi

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

        cmp rsi, 8
        jl .storen

        vmovdqu [rdx + 0x00], xmm0
        vmovdqu [rdx + 0x10], xmm1
        vmovdqu [rdx + 0x20], xmm2
        vmovdqu [rdx + 0x30], xmm3
        vmovdqu [rdx + 0x40], xmm4
        vmovdqu [rdx + 0x50], xmm5
        vmovdqu [rdx + 0x60], xmm6
        vmovdqu [rdx + 0x70], xmm7
        sub rsi, 8
        add rdx, 0x80

        test rsi, rsi
        jnz .generate

        .storen:
            cmp rsi, 1
            jl .return
            vmovdqu [rdx + 0x00], xmm0
            cmp rsi, 2
            jl .return
            vmovdqu [rdx + 0x10], xmm1
            cmp rsi, 3
            jl .return
            vmovdqu [rdx + 0x20], xmm2
            cmp rsi, 4
            jl .return
            vmovdqu [rdx + 0x30], xmm3
            cmp rsi, 5
            jl .return
            vmovdqu [rdx + 0x40], xmm4
            cmp rsi, 6
            jl .return
            vmovdqu [rdx + 0x50], xmm5
            cmp rsi, 7
            jl .return
            vmovdqu [rdx + 0x60], xmm6
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
    movdqu xmm9,  [rcx + 0x00]
    movdqu xmm10, [rcx + 0x10]
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
    vmovdqu xmm9,  [rcx + 0x00]
    vmovdqu xmm10, [rcx + 0x10]
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
