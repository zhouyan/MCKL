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

global mckl_aes128_aesni_avx2_kernel
global mckl_aes192_aesni_avx2_kernel
global mckl_aes256_aesni_avx2_kernel
global mckl_ars_aesni_avx2_kernel

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

; rdi ctr.data()
; rsi n
; rdx r
; rcx ks.get().data()/weyl:key
%macro aes_aesni_avx2_kernel 1 ; rounds {{{
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
    vmovdqu xmm8, [rdi]
    add [rdi], rsi

    ; load/compute round keys
    %if %1 == 5 ; ARS
        vmovdqu xmm9,  [rcx + 0x00] ; weyl
        vmovdqu xmm10, [rcx + 0x10] ; round_key[0]
        vpaddq  xmm11, xmm10, xmm9  ; round_key[1]
        vpaddq  xmm12, xmm11, xmm9  ; round_key[2]
        vpaddq  xmm13, xmm12, xmm9  ; round_key[3]
        vpaddq  xmm14, xmm13, xmm9  ; round_key[4]
        vpaddq  xmm15, xmm14, xmm9  ; round_key[5]
    %else ; AES
        vmovdqu xmm10, [rcx + 0x00] ; round_key[0]
        vmovdqu xmm11, [rcx + 0x10] ; round_key[1]
        vmovdqu xmm12, [rcx + 0x20] ; round_key[2]
        vmovdqu xmm13, [rcx + 0x30] ; round_key[3]
        vmovdqu xmm14, [rcx + 0x40] ; round_key[4]
        vmovdqu xmm15, [rcx + 0x50] ; round_key[5]
        %if %1 > 5
            %assign r 0
            %rep %1 - 5
                vmovdqa [rsp + r * 0x10], xmm15
                vmovdqu xmm15, [rcx + (r + 6) * 0x10] ; round_key[r + 6]
                %assign r r + 1
            %endrep
        %endif
    %endif

    mov rcx, rsi ; n
    mov rdi, rdx ; r

    ; generate a new batch of 8 blocks
    .generate:
        vpaddq xmm0, xmm8, [rel aes_aesni_avx2_increment_data + 0x00]
        vpaddq xmm1, xmm8, [rel aes_aesni_avx2_increment_data + 0x10]
        vpaddq xmm2, xmm8, [rel aes_aesni_avx2_increment_data + 0x20]
        vpaddq xmm3, xmm8, [rel aes_aesni_avx2_increment_data + 0x30]
        vpaddq xmm4, xmm8, [rel aes_aesni_avx2_increment_data + 0x40]
        vpaddq xmm5, xmm8, [rel aes_aesni_avx2_increment_data + 0x50]
        vpaddq xmm6, xmm8, [rel aes_aesni_avx2_increment_data + 0x60]
        vpaddq xmm7, xmm8, [rel aes_aesni_avx2_increment_data + 0x70]
        vmovdqa xmm8, xmm7
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

    ; check if this is the last batch
    cmp rcx, 8
    jl .last

    ; store the current batch directly to r
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

    ; check if a new batch is needed
    test rcx, rcx
    jz .return
    jmp .generate

    ; store the last batch on the stack and then move to r
    .last:
        vmovdqa [rsp + 0x00], xmm0
        vmovdqa [rsp + 0x10], xmm1
        vmovdqa [rsp + 0x20], xmm2
        vmovdqa [rsp + 0x30], xmm3
        vmovdqa [rsp + 0x40], xmm4
        vmovdqa [rsp + 0x50], xmm5
        vmovdqa [rsp + 0x60], xmm6
        vmovdqa [rsp + 0x70], xmm7
        shl rcx, 1
        mov rsi, rsp
        cld
        rep movsq

    .return:
        vzeroupper
        mov rsp, rbp
        pop rbp
        ret
%endmacro ; }}}

section .rodata

align 16
aes_aesni_avx2_increment_data:
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

mckl_aes128_aesni_avx2_kernel: aes_aesni_avx2_kernel 10
mckl_aes192_aesni_avx2_kernel: aes_aesni_avx2_kernel 12
mckl_aes256_aesni_avx2_kernel: aes_aesni_avx2_kernel 14
mckl_ars_aesni_avx2_kernel:    aes_aesni_avx2_kernel 5

; vim:ft=nasm
