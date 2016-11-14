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
; rax n
; rdx r
; rcx ks.get().data()/weyl:key

%macro mckl_aes_aesni_increment 0
    vpaddq xmm0, xmm8, [rel inc0]
    vpaddq xmm1, xmm8, [rel inc1]
    vpaddq xmm2, xmm8, [rel inc2]
    vpaddq xmm3, xmm8, [rel inc3]
    vpaddq xmm4, xmm8, [rel inc4]
    vpaddq xmm5, xmm8, [rel inc5]
    vpaddq xmm6, xmm8, [rel inc6]
    vpaddq xmm7, xmm8, [rel inc7]
    vpaddq xmm8, xmm8, [rel inc8]
%endmacro

%macro mckl_aes_aesni_store 1
    cmp rax, 8
    jl mckl_%{1}_aesni_storen

    vmovdqu [rdx + 0x00], xmm0
    vmovdqu [rdx + 0x10], xmm1
    vmovdqu [rdx + 0x20], xmm2
    vmovdqu [rdx + 0x30], xmm3
    vmovdqu [rdx + 0x40], xmm4
    vmovdqu [rdx + 0x50], xmm5
    vmovdqu [rdx + 0x60], xmm6
    vmovdqu [rdx + 0x70], xmm7
    sub rax, 8
    add rdx, 0x80

    test rax, rax
    jnz mckl_%{1}_aesni_generate

mckl_%{1}_aesni_storen:
    cmp rax, 1
    jl mckl_%{1}_aesni_ret
    vmovdqu [rdx + 0x00], xmm0
    cmp rax, 2
    jl mckl_%{1}_aesni_ret
    vmovdqu [rdx + 0x10], xmm1
    cmp rax, 3
    jl mckl_%{1}_aesni_ret
    vmovdqu [rdx + 0x20], xmm2
    cmp rax, 4
    jl mckl_%{1}_aesni_ret
    vmovdqu [rdx + 0x30], xmm3
    cmp rax, 5
    jl mckl_%{1}_aesni_ret
    vmovdqu [rdx + 0x40], xmm4
    cmp rax, 6
    jl mckl_%{1}_aesni_ret
    vmovdqu [rdx + 0x50], xmm5
    cmp rax, 7
    jl mckl_%{1}_aesni_ret
    vmovdqu [rdx + 0x60], xmm6
    cmp rax, 8
    jl mckl_%{1}_aesni_ret
    vmovdqu [rdx + 0x70], xmm7
%endmacro

%macro mckl_aes_aesni_encfirst 1
    vpxor xmm0, %1
    vpxor xmm1, %1
    vpxor xmm2, %1
    vpxor xmm3, %1
    vpxor xmm4, %1
    vpxor xmm5, %1
    vpxor xmm6, %1
    vpxor xmm7, %1
%endmacro

%macro mckl_aes_aesni_enc 1
    vaesenc xmm0, %1
    vaesenc xmm1, %1
    vaesenc xmm2, %1
    vaesenc xmm3, %1
    vaesenc xmm4, %1
    vaesenc xmm5, %1
    vaesenc xmm6, %1
    vaesenc xmm7, %1
%endmacro

%macro mckl_aes_aesni_enclast 1
    vaesenclast xmm0, %1
    vaesenclast xmm1, %1
    vaesenclast xmm2, %1
    vaesenclast xmm3, %1
    vaesenclast xmm4, %1
    vaesenclast xmm5, %1
    vaesenclast xmm6, %1
    vaesenclast xmm7, %1
%endmacro

section .rodata

align 16
inc0:
DQ 1
DQ 0

align 16
inc1:
DQ 2
DQ 0

align 16
inc2:
DQ 3
DQ 0

align 16
inc3:
DQ 4
DQ 0

align 16
inc4:
DQ 5
DQ 0

align 16
inc5:
DQ 6
DQ 0

align 16
inc6:
DQ 7
DQ 0

align 16
inc7:
DQ 8
DQ 0

align 16
inc8:
DQ 8
DQ 0

section .text

global mckl_aes128_aesni_kernel
mckl_aes128_aesni_kernel:
    push rbp
    mov rbp, rsp
    mov rax, rsi
    sub rsp, 0x50

    vmovdqu xmm8, [rdi]
    add [rdi], rax

    vmovdqu xmm10, [rcx + 0x00] ; round key 0x0
    vmovdqu xmm11, [rcx + 0x10] ; round key 0x1
    vmovdqu xmm12, [rcx + 0x20] ; round key 0x2
    vmovdqu xmm13, [rcx + 0x30] ; round key 0x3
    vmovdqu xmm14, [rcx + 0x40] ; round key 0x4
    vmovdqu xmm15, [rcx + 0x50] ; round key 0x5

    vmovdqu xmm0, [rcx + 0x60]
    vmovdqa [rsp + 0x00], xmm0 ; round key 0x6
    vmovdqu xmm0, [rcx + 0x70]
    vmovdqa [rsp + 0x10], xmm0 ; round key 0x7
    vmovdqu xmm0, [rcx + 0x80]
    vmovdqa [rsp + 0x20], xmm0 ; round key 0x8
    vmovdqu xmm0, [rcx + 0x90]
    vmovdqa [rsp + 0x30], xmm0 ; round key 0x9
    vmovdqu xmm0, [rcx + 0xA0]
    vmovdqa [rsp + 0x40], xmm0 ; round key 0xA

mckl_aes128_aesni_generate:
    mckl_aes_aesni_increment

    mckl_aes_aesni_encfirst xmm10
    mckl_aes_aesni_enc xmm11
    mckl_aes_aesni_enc xmm12
    mckl_aes_aesni_enc xmm13
    mckl_aes_aesni_enc xmm14
    mckl_aes_aesni_enc xmm15
    vmovdqa xmm9, [rsp + 0x00]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x10]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x20]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x30]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x40]
    mckl_aes_aesni_enclast xmm9

    mckl_aes_aesni_store aes128

mckl_aes128_aesni_ret:
    vzeroupper
    mov rsp, rbp
    pop rbp
    ret

global mckl_aes192_aesni_kernel
mckl_aes192_aesni_kernel:
    push rbp
    mov rbp, rsp
    mov rax, rsi
    sub rsp, 0x70

    vmovdqu xmm8, [rdi]
    add [rdi], rax

    vmovdqu xmm10, [rcx + 0x00] ; round key 0x0
    vmovdqu xmm11, [rcx + 0x10] ; round key 0x1
    vmovdqu xmm12, [rcx + 0x20] ; round key 0x2
    vmovdqu xmm13, [rcx + 0x30] ; round key 0x3
    vmovdqu xmm14, [rcx + 0x40] ; round key 0x4
    vmovdqu xmm15, [rcx + 0x50] ; round key 0x5

    vmovdqu xmm0, [rcx + 0x60]
    vmovdqa [rsp + 0x00], xmm0 ; round key 0x6
    vmovdqu xmm0, [rcx + 0x70]
    vmovdqa [rsp + 0x10], xmm0 ; round key 0x7
    vmovdqu xmm0, [rcx + 0x80]
    vmovdqa [rsp + 0x20], xmm0 ; round key 0x8
    vmovdqu xmm0, [rcx + 0x90]
    vmovdqa [rsp + 0x30], xmm0 ; round key 0x9
    vmovdqu xmm0, [rcx + 0xA0]
    vmovdqa [rsp + 0x40], xmm0 ; round key 0xA
    vmovdqu xmm0, [rcx + 0xB0]
    vmovdqa [rsp + 0x50], xmm0 ; round key 0xB
    vmovdqu xmm0, [rcx + 0xC0]
    vmovdqa [rsp + 0x60], xmm0 ; round key 0xC

mckl_aes192_aesni_generate:
    mckl_aes_aesni_increment

    mckl_aes_aesni_encfirst xmm10
    mckl_aes_aesni_enc xmm11
    mckl_aes_aesni_enc xmm12
    mckl_aes_aesni_enc xmm13
    mckl_aes_aesni_enc xmm14
    mckl_aes_aesni_enc xmm15
    vmovdqa xmm9, [rsp + 0x00]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x10]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x20]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x30]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x40]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x50]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x60]
    mckl_aes_aesni_enclast xmm9

    mckl_aes_aesni_store aes192

mckl_aes192_aesni_ret:
    vzeroupper
    mov rsp, rbp
    pop rbp
    ret

global mckl_aes256_aesni_kernel
mckl_aes256_aesni_kernel:
    push rbp
    mov rbp, rsp
    mov rax, rsi
    sub rsp, 0x90

    vmovdqu xmm8, [rdi]
    add [rdi], rax

    vmovdqu xmm10, [rcx + 0x00] ; round key 0x0
    vmovdqu xmm11, [rcx + 0x10] ; round key 0x1
    vmovdqu xmm12, [rcx + 0x20] ; round key 0x2
    vmovdqu xmm13, [rcx + 0x30] ; round key 0x3
    vmovdqu xmm14, [rcx + 0x40] ; round key 0x4
    vmovdqu xmm15, [rcx + 0x50] ; round key 0x5

    vmovdqu xmm0, [rcx + 0x60]
    vmovdqa [rsp + 0x00], xmm0 ; round key 0x6
    vmovdqu xmm0, [rcx + 0x70]
    vmovdqa [rsp + 0x10], xmm0 ; round key 0x7
    vmovdqu xmm0, [rcx + 0x80]
    vmovdqa [rsp + 0x20], xmm0 ; round key 0x8
    vmovdqu xmm0, [rcx + 0x90]
    vmovdqa [rsp + 0x30], xmm0 ; round key 0x9
    vmovdqu xmm0, [rcx + 0xA0]
    vmovdqa [rsp + 0x40], xmm0 ; round key 0xA
    vmovdqu xmm0, [rcx + 0xB0]
    vmovdqa [rsp + 0x50], xmm0 ; round key 0xB
    vmovdqu xmm0, [rcx + 0xC0]
    vmovdqa [rsp + 0x60], xmm0 ; round key 0xC
    vmovdqu xmm0, [rcx + 0xD0]
    vmovdqa [rsp + 0x70], xmm0 ; round key 0xD
    vmovdqu xmm0, [rcx + 0xE0]
    vmovdqa [rsp + 0x80], xmm0 ; round key 0xE

mckl_aes256_aesni_generate:
    mckl_aes_aesni_increment

    mckl_aes_aesni_encfirst xmm10
    mckl_aes_aesni_enc xmm11
    mckl_aes_aesni_enc xmm12
    mckl_aes_aesni_enc xmm13
    mckl_aes_aesni_enc xmm14
    mckl_aes_aesni_enc xmm15
    vmovdqa xmm9, [rsp + 0x00]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x10]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x20]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x30]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x40]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x50]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x60]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x70]
    mckl_aes_aesni_enc xmm9
    vmovdqa xmm9, [rsp + 0x80]
    mckl_aes_aesni_enclast xmm9

    mckl_aes_aesni_store aes256

mckl_aes256_aesni_ret:
    vzeroupper
    mov rsp, rbp
    pop rbp
    ret

global mckl_ars_aesni_kernel
mckl_ars_aesni_kernel:
    push rbp
    mov rbp, rsp
    mov rax, rsi

    vmovdqu xmm8, [rdi]
    add [rdi], rax

    vmovdqu xmm9,  [rcx + 0x00] ; wel constant
    vmovdqu xmm10, [rcx + 0x10] ; round key 0
    vpaddq xmm11, xmm10, xmm9 ; round key 1
    vpaddq xmm12, xmm11, xmm9 ; round key 2
    vpaddq xmm13, xmm12, xmm9 ; round key 3
    vpaddq xmm14, xmm13, xmm9 ; round key 4
    vpaddq xmm15, xmm14, xmm9 ; round key 5

mckl_ars_aesni_generate:
    mckl_aes_aesni_increment

    mckl_aes_aesni_encfirst xmm10
    mckl_aes_aesni_enc xmm11
    mckl_aes_aesni_enc xmm12
    mckl_aes_aesni_enc xmm13
    mckl_aes_aesni_enc xmm14
    mckl_aes_aesni_enclast xmm15

    mckl_aes_aesni_store ars

mckl_ars_aesni_ret:
    vzeroupper
    mov rsp, rbp
    pop rbp
    ret

; vim:ft=nasm
