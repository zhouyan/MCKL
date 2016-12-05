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

default rel

%macro encfirst 1 ; {{{
    pxor xmm0, %1
    pxor xmm1, %1
    pxor xmm2, %1
    pxor xmm3, %1
    pxor xmm4, %1
    pxor xmm5, %1
    pxor xmm6, %1
    pxor xmm7, %1
%endmacro ; }}}

%macro enc 1 ; {{{
    aesenc xmm0, %1
    aesenc xmm1, %1
    aesenc xmm2, %1
    aesenc xmm3, %1
    aesenc xmm4, %1
    aesenc xmm5, %1
    aesenc xmm6, %1
    aesenc xmm7, %1
%endmacro ; }}}

%macro enclast 1 ; {{{
    aesenclast xmm0, %1
    aesenclast xmm1, %1
    aesenclast xmm2, %1
    aesenclast xmm3, %1
    aesenclast xmm4, %1
    aesenclast xmm5, %1
    aesenclast xmm6, %1
    aesenclast xmm7, %1
%endmacro ; }}}

%macro generate 1 ; rounds {{{
    movdqa xmm0, xmm8
    movdqa xmm1, xmm8
    movdqa xmm2, xmm8
    movdqa xmm3, xmm8
    movdqa xmm4, xmm8
    movdqa xmm5, xmm8
    movdqa xmm6, xmm8
    movdqa xmm7, xmm8
    paddq xmm0, [increment + 0x00]
    paddq xmm1, [increment + 0x10]
    paddq xmm2, [increment + 0x20]
    paddq xmm3, [increment + 0x30]
    paddq xmm4, [increment + 0x40]
    paddq xmm5, [increment + 0x50]
    paddq xmm6, [increment + 0x60]
    paddq xmm7, [increment + 0x70]
    paddq xmm8, [increment + 0x80]
    encfirst xmm10
    enc xmm11
    enc xmm12
    enc xmm13
    enc xmm14
%if %1 > 5
%assign r 0
%rep %1 - 5
    movdqa xmm9, [rsp + r * 0x10]
    enc xmm9
    %assign r r + 1
%endrep
%endif
    enclast xmm15
%endmacro ; }}}

; rdi:ctr.data()
; rsi:n
; rdx:r
; rcx:ks.get().data()/weyl:key
%macro kernel 1 ; rounds {{{
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
    movdqu xmm8, [r8]
    add [r8], rax

%if %1 == 5 ; ARS: load weyl and key, compute round_key
    movdqu xmm9,  [r9 + 0x00] ; weyl
    movdqu xmm10, [r9 + 0x10] ; round_key[0]
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
%else ; AES: load round_key
    movdqu xmm10, [r9 + 0x00] ; round_key[0]
    movdqu xmm11, [r9 + 0x10] ; round_key[1]
    movdqu xmm12, [r9 + 0x20] ; round_key[2]
    movdqu xmm13, [r9 + 0x30] ; round_key[3]
    movdqu xmm14, [r9 + 0x40] ; round_key[4]
    movdqu xmm15, [r9 + %1 * 0x10] ; round_key[rounds]
%if %1 > 5
    mov rcx, (%1 - 5) * 2
    lea rsi, [r9 + 0x50]
    mov rdi, rsp
    rep movsq
%endif
%endif

    cmp rax, 8
    jl .last

.loop: align 16
    generate %1
    movdqu [rdx + 0x00], xmm0
    movdqu [rdx + 0x10], xmm1
    movdqu [rdx + 0x20], xmm2
    movdqu [rdx + 0x30], xmm3
    movdqu [rdx + 0x40], xmm4
    movdqu [rdx + 0x50], xmm5
    movdqu [rdx + 0x60], xmm6
    movdqu [rdx + 0x70], xmm7
    add rdx, 0x80
    sub rax, 8
    cmp rax, 8
    jge .loop

.last:
    test rax, rax
    jz .return
    generate %1
    movdqa [rsp - 0x80], xmm0
    movdqa [rsp - 0x70], xmm1
    movdqa [rsp - 0x60], xmm2
    movdqa [rsp - 0x50], xmm3
    movdqa [rsp - 0x40], xmm4
    movdqa [rsp - 0x30], xmm5
    movdqa [rsp - 0x20], xmm6
    movdqa [rsp - 0x10], xmm7
    lea rcx, [rax + rax]
    lea rsi, [rsp - 0x80]
    mov rdi, rdx
    rep movsq

.return:
    mov rsp, rbp
    pop rbp
    ret
%endmacro ; }}}

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

mckl_aes128_aesni_sse2_kernel: kernel 10
mckl_aes192_aesni_sse2_kernel: kernel 12
mckl_aes256_aesni_sse2_kernel: kernel 14
mckl_ars_aesni_sse2_kernel:    kernel 5

; vim:ft=nasm
