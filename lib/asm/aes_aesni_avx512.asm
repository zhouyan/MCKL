;;============================================================================
;; MCKL/lib/asm/aes_aesni_avx512.asm
;;----------------------------------------------------------------------------
;; MCKL: Monte Carlo Kernel Library
;;----------------------------------------------------------------------------
;; Copyright (c) 2013-2018, Yan Zhou
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

global mckl_aes128_aesni_avx512_kernel
global mckl_aes192_aesni_avx512_kernel
global mckl_aes256_aesni_avx512_kernel
global mckl_ars_aesni_avx512_kernel

default rel

%macro encfirst 1
    vpxord xmm0, xmm0, %1
    vpxord xmm1, xmm1, %1
    vpxord xmm2, xmm2, %1
    vpxord xmm3, xmm3, %1
    vpxord xmm4, xmm4, %1
    vpxord xmm5, xmm5, %1
    vpxord xmm6, xmm6, %1
    vpxord xmm7, xmm7, %1
%endmacro

%macro enc 1
    vaesenc xmm0, xmm0, %1
    vaesenc xmm1, xmm1, %1
    vaesenc xmm2, xmm2, %1
    vaesenc xmm3, xmm3, %1
    vaesenc xmm4, xmm4, %1
    vaesenc xmm5, xmm5, %1
    vaesenc xmm6, xmm6, %1
    vaesenc xmm7, xmm7, %1
%endmacro

%macro enclast 1
    vaesenclast xmm0, xmm0, %1
    vaesenclast xmm1, xmm1, %1
    vaesenclast xmm2, xmm2, %1
    vaesenclast xmm3, xmm3, %1
    vaesenclast xmm4, xmm4, %1
    vaesenclast xmm5, xmm5, %1
    vaesenclast xmm6, xmm6, %1
    vaesenclast xmm7, xmm7, %1
%endmacro

%macro generate 1 ; rounds
    vpaddq xmm0, xmm8, [increment + 0x00]
    vpaddq xmm1, xmm8, [increment + 0x10]
    vpaddq xmm2, xmm8, [increment + 0x20]
    vpaddq xmm3, xmm8, [increment + 0x30]
    vpaddq xmm4, xmm8, [increment + 0x40]
    vpaddq xmm5, xmm8, [increment + 0x50]
    vpaddq xmm6, xmm8, [increment + 0x60]
    vpaddq xmm7, xmm8, [increment + 0x70]
    vpaddq xmm8, xmm8, [increment + 0x80]
    encfirst xmm10
    enc xmm11
    enc xmm12
    enc xmm13
    enc xmm14
%if %1 > 5
%assign r 0
%rep  %1 - 5
    vmovdqa xmm9, [rsp + r * 0x10]
    enc xmm9
%assign r r + 1
%endrep
%endif
    enclast xmm15
%endmacro

; rdi:ctr.data()
; rsi:n
; rdx:r
; rcx:ks.get().data()/weyl:key
%macro kernel 1 ; rounds
    push rbp
    mov rbp, rsp
%if %1 > 5
    sub rsp, (%1 - 5) * 0x10
%endif
    cld

    ; early return
    test rsi, rsi
    jz .return

    ; parameter processing, leave rdi, rsi, rcx free for moving memory
    mov rax, rsi ; n
    mov r8,  rdi ; ctr
    mov r9,  rcx ; key.get().data()/weyl:key

    ; load counter
    vmovdqu xmm8, [r8]
    add [r8], rax

%if %1 == 5 ; ARS: load weyl and key, compute round_key
    vmovdqu xmm9,  [r9 + 0x00] ; weyl
    vmovdqu xmm10, [r9 + 0x10] ; round_key[0]
    vpaddq  xmm11, xmm10, xmm9 ; round_key[1]
    vpaddq  xmm12, xmm11, xmm9 ; round_key[2]
    vpaddq  xmm13, xmm12, xmm9 ; round_key[3]
    vpaddq  xmm14, xmm13, xmm9 ; round_key[4]
    vpaddq  xmm15, xmm14, xmm9 ; round_key[5]
%else ; AES: load round_key
    vmovdqu xmm10, [r9 + 0x00] ; round_key[0]
    vmovdqu xmm11, [r9 + 0x10] ; round_key[1]
    vmovdqu xmm12, [r9 + 0x20] ; round_key[2]
    vmovdqu xmm13, [r9 + 0x30] ; round_key[3]
    vmovdqu xmm14, [r9 + 0x40] ; round_key[4]
    vmovdqu xmm15, [r9 + %1 * 0x10] ; round_key[rounds]
%if %1 > 5
    mov rcx, (%1 - 5) * 2
    lea rsi, [r9 + 0x50]
    mov rdi, rsp
    rep movsq
%endif
%endif

    cmp rax, 8
    jl .last

align 16
.loop:
    generate %1
    vmovdqu [rdx + 0x00], xmm0
    vmovdqu [rdx + 0x10], xmm1
    vmovdqu [rdx + 0x20], xmm2
    vmovdqu [rdx + 0x30], xmm3
    vmovdqu [rdx + 0x40], xmm4
    vmovdqu [rdx + 0x50], xmm5
    vmovdqu [rdx + 0x60], xmm6
    vmovdqu [rdx + 0x70], xmm7
    add rdx, 0x80
    sub rax, 8
    cmp rax, 8
    jge .loop

.last:
    test rax, rax
    jz .return
    generate %1
    vmovdqa [rsp - 0x80], xmm0
    vmovdqa [rsp - 0x70], xmm1
    vmovdqa [rsp - 0x60], xmm2
    vmovdqa [rsp - 0x50], xmm3
    vmovdqa [rsp - 0x40], xmm4
    vmovdqa [rsp - 0x30], xmm5
    vmovdqa [rsp - 0x20], xmm6
    vmovdqa [rsp - 0x10], xmm7
    lea rcx, [rax + rax]
    lea rsi, [rsp - 0x80]
    mov rdi, rdx
    rep movsq

.return:
    vzeroupper
    mov rsp, rbp
    pop rbp
    ret
%endmacro

section .rodata

align 16

increment:
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

mckl_aes128_aesni_avx512_kernel: kernel 10
mckl_aes192_aesni_avx512_kernel: kernel 12
mckl_aes256_aesni_avx512_kernel: kernel 14
mckl_ars_aesni_avx512_kernel:    kernel 5

; vim:ft=nasm
