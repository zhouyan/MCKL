;;============================================================================
;; MCKL/lib/asm/aes_aesni_avx2.asm
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

global mckl_aes128_aesni_avx2_kernel
global mckl_aes192_aesni_avx2_kernel
global mckl_aes256_aesni_avx2_kernel
global mckl_ars_aesni_avx2_kernel

%macro aes_aesni_avx2_prologue 1 ; {{{
    %if %1 < 13
        prologue 4, 0x80
    %else
        prologue 4, (%1 - 5) * 0x10
    %endif

    vmovdqu xmm8, [rdi]
    add [rdi], rsi

    %if %1 == 5 ; ARS
        vmovdqu xmm9,  [rcx + 0x00] ; weyl
        vmovdqu xmm10, [rcx + 0x10] ; key
        vpaddq  xmm11, xmm10, xmm9
        vpaddq  xmm12, xmm11, xmm9
        vpaddq  xmm13, xmm12, xmm9
        vpaddq  xmm14, xmm13, xmm9
        vpaddq  xmm15, xmm14, xmm9
    %else ; AES
        vmovdqu xmm10, [rcx + 0x00]
        vmovdqu xmm11, [rcx + 0x10]
        vmovdqu xmm12, [rcx + 0x20]
        vmovdqu xmm13, [rcx + 0x30]
        vmovdqu xmm14, [rcx + 0x40]
        vmovdqu xmm15, [rcx + 0x50]
        %if %1 > 5
            %assign r 0
            %rep %1 - 5
                vmovdqa [rsp + r * 0x10], xmm15
                vmovdqu xmm15, [rcx + (r + 6) * 0x10]
                %assign r r + 1
            %endrep
        %endif
    %endif
%endmacro ; }}}

%macro aes_aesni_avx2_epilogue 0 ; {{{
    .return:
        vzeroupper
        epilogue
%endmacro ; }}}

%macro aes_aesni_avx2_encfirst 1 ; {{{
    vpxor xmm0, xmm0, %1
    vpxor xmm1, xmm1, %1
    vpxor xmm2, xmm2, %1
    vpxor xmm3, xmm3, %1
    vpxor xmm4, xmm4, %1
    vpxor xmm5, xmm5, %1
    vpxor xmm6, xmm6, %1
    vpxor xmm7, xmm7, %1
%endmacro ; }}}

%macro aes_aesni_avx2_enc 1 ; {{{
    vaesenc xmm0, xmm0, %1
    vaesenc xmm1, xmm1, %1
    vaesenc xmm2, xmm2, %1
    vaesenc xmm3, xmm3, %1
    vaesenc xmm4, xmm4, %1
    vaesenc xmm5, xmm5, %1
    vaesenc xmm6, xmm6, %1
    vaesenc xmm7, xmm7, %1
%endmacro ; }}}

%macro aes_aesni_avx2_enclast 1 ; {{{
    vaesenclast xmm0, xmm0, %1
    vaesenclast xmm1, xmm1, %1
    vaesenclast xmm2, xmm2, %1
    vaesenclast xmm3, xmm3, %1
    vaesenclast xmm4, xmm4, %1
    vaesenclast xmm5, xmm5, %1
    vaesenclast xmm6, xmm6, %1
    vaesenclast xmm7, xmm7, %1
%endmacro ; }}}

%macro aes_aesni_avx2_generate 1 ; {{{
    align 16
    .generate:
        increment_avx2_xmm xmm8, 0x10
        aes_aesni_avx2_encfirst xmm10
        aes_aesni_avx2_enc xmm11
        aes_aesni_avx2_enc xmm12
        aes_aesni_avx2_enc xmm13
        aes_aesni_avx2_enc xmm14
        %if %1 > 5
            %assign r 0
            %rep %1 - 5
                vmovdqa xmm9, [rsp + r * 0x10]
                aes_aesni_avx2_enc xmm9
                %assign r r + 1
            %endrep
        %endif
        aes_aesni_avx2_enclast xmm15

        cmp rsi, 8
        jl .last
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

        .last:
        test rsi, rsi
        jz .return
        vmovdqa [rsp + 0x00], xmm0
        vmovdqa [rsp + 0x10], xmm1
        vmovdqa [rsp + 0x20], xmm2
        vmovdqa [rsp + 0x30], xmm3
        vmovdqa [rsp + 0x40], xmm4
        vmovdqa [rsp + 0x50], xmm5
        vmovdqa [rsp + 0x60], xmm6
        vmovdqa [rsp + 0x70], xmm7
        mov rcx, rsi
        shl rcx, 1
        mov rsi, rsp
        mov rdi, rdx
        cld
        rep movsq
%endmacro ; }}}

section .rodata

def_increment_xmm_data_2

section .text

mckl_aes128_aesni_avx2_kernel:
    aes_aesni_avx2_prologue 10
    aes_aesni_avx2_generate 10
    aes_aesni_avx2_epilogue
; mckl_aes128_aesni_avx2_kernel:

mckl_aes192_aesni_avx2_kernel:
    aes_aesni_avx2_prologue 12
    aes_aesni_avx2_generate 12
    aes_aesni_avx2_epilogue
; mckl_aes192_aesni_avx2_kernel:

mckl_aes256_aesni_avx2_kernel:
    aes_aesni_avx2_prologue 14
    aes_aesni_avx2_generate 14
    aes_aesni_avx2_epilogue
; mckl_aes256_aesni_avx2_kernel:

mckl_ars_aesni_avx2_kernel:
    aes_aesni_avx2_prologue 5
    aes_aesni_avx2_generate 5
    aes_aesni_avx2_epilogue
; mckl_ars_aesni_avx2_kernel:

; vim:ft=nasm
