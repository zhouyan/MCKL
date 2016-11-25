;;============================================================================
;; MCKL/lib/asm/exp.asm
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

global mckl_vd_exp
global mckl_vd_exp2
global mckl_vd_expm1

%macro poly 0 ; {{{ implicity input ymm1, output ymm13
    vmulpd ymm2, ymm1, ymm1 ; x2 = x^2
    vmulpd ymm4, ymm2, ymm2 ; x4 = x^4
    vmulpd ymm8, ymm4, ymm4 ; x8 = x^8
    vmovapd ymm3,  [rel c3]
    vmovapd ymm5,  [rel c5]
    vmovapd ymm7,  [rel c7]
    vmovapd ymm9,  [rel c9]
    vmovapd ymm11, [rel c11]
    vmovapd ymm13, [rel c13]
    vfmadd213pd ymm3,  ymm1, [rel c2]  ; u3  = c3  * x + c2
    vfmadd213pd ymm5,  ymm1, [rel c4]  ; u5  = c5  * x + c4
    vfmadd213pd ymm7,  ymm1, [rel c6]  ; u7  = c7  * x + c6
    vfmadd213pd ymm9,  ymm1, [rel c8]  ; u9  = c9  * x + c8
    vfmadd213pd ymm11, ymm1, [rel c10] ; u11 = c11 * x + c10
    vfmadd213pd ymm13, ymm1, [rel c12] ; u13 = c13 * x + c12
    vfmadd213pd ymm3,  ymm2, ymm1      ; v3  = u3  * x^2 + x
    vfmadd213pd ymm7,  ymm2, ymm5      ; v7  = u7  * x^2 + u5
    vfmadd213pd ymm11, ymm2, ymm9      ; v11 = u11 * x^2 + u9
    vfmadd213pd ymm7,  ymm4, ymm3      ; w7  = v7  * x^4 + v3
    vfmadd213pd ymm13, ymm4, ymm11     ; w13 = v13 * x^4 + v11
    vfmadd213pd ymm13, ymm8, ymm7      ; z13 = w13 * x^8 + w7
%endmacro ; }}}

%macro exp 0 ; {{{ implicit input ymm0
    ; r = round(a / log(2))
    vmulpd ymm15, ymm0, [rel log2inv]
    vroundpd ymm15, ymm15, 0x8

    ; x = a - r * log(2)
    vmovapd ymm1, ymm0
    vfnmadd231pd ymm1, ymm15, [rel log2hi]
    vfnmadd231pd ymm1, ymm15, [rel log2lo]

    ; z13 = c13 * x^13 + ... + c2 * x^2 + x
    ; res = z13 * 2^r + 2^r
    poly
    vaddpd ymm1, ymm15, [rel bias]
    vpsllq ymm1, 52
    vfmadd213pd ymm13, ymm1, ymm1
%endmacro ; }}}

%macro exp2 0 ; implicit input ymm0 {{{
    ; r = round(a)
    vroundpd ymm15, ymm0, 0x8

    ; x = (a  - r) * log(2)
    vsubpd ymm1, ymm0, ymm15
    vmulpd ymm1, ymm1, [rel log2]

    ; z13 = c13 * x^13 + ... + c2 * x^2 + x
    ; res = z13 * 2^r + 2^r
    poly
    vaddpd ymm1, ymm15, [rel bias]
    vpsllq ymm1, 52
    vfmadd213pd ymm13, ymm1, ymm1
%endmacro ; }}}

%macro expm1 0 ; implicit input ymm0 {{{
    ; r = round(a / log(2))
    vmulpd ymm15, ymm0, [rel log2inv]
    vroundpd ymm15, ymm15, 0x8

    ; x = a - r * log(2)
    vmovapd ymm1, ymm0
    vfnmadd231pd ymm1, ymm15, [rel log2hi]
    vfnmadd231pd ymm1, ymm15, [rel log2lo]

    ; z13 = c13 * x^13 + ... + c2 * x^2 + x
    ; res = z13 * 2^r + (2^r - 1)
    poly
    vaddpd ymm1, ymm15, [rel bias]
    vpsllq ymm1, 52
    vsubpd ymm2, ymm1, [rel one]
    vfmadd213pd ymm13, ymm1, ymm2
%endmacro ; }}}

%macro vd_exp 2 ; {{{
    %1
    vcmpltpd ymm1, ymm0, [rel %{1}_min_a]
    vcmpgtpd ymm2, ymm0, [rel %{1}_max_a]
    vblendvpd ymm13, ymm13, [rel %{1}_min_y], ymm1
    vblendvpd ymm13, ymm13, [rel %{1}_max_y], ymm2
    vmovupd %2, ymm13
%endmacro ; }}}

%macro vd_exp 1 ; rdi:n, rsi:a, rdx:y {{{
    cmp rdi, 4
    jl .last

    .loop:
        vmovupd ymm0, [rsi]
        vd_exp %1, [rdx]
        add rsi, 0x20
        add rdx, 0x20
        sub rdi, 4
        cmp rdi, 4
        jge .loop

    .last:
        test rdi, rdi
        jz .return
        mov rax, rdi
        mov rcx, rdi
        mov rdi, rsp
        sub rdi, 0x28
        cld
        rep movsq
        vmovupd ymm0, [rsp - 0x28]
        vd_exp %1, [rsp - 0x28]
        mov rcx, rax
        mov rsi, rsp
        sub rsi, 0x28
        mov rdi, rdx
        cld
        rep movsq

    .return:
        ret
%endmacro ; }}}

section .rodata

align 32

exp_contants:

exp_min_a: times 4 dq 0xC086232BDD7ABCD2 ; -708.3964185322641
exp_max_a: times 4 dq 0x40862B7D369A5AA7 ; 709.4361393031039
exp_min_y: times 4 dq 0x0000000000000000 ; 0.0
exp_max_y: times 4 dq 0x7FF0000000000000 ; HUGE_VAL

exp2_min_a: times 4 dq 0xC08FF00000000000 ; -1022.0
exp2_max_a: times 4 dq 0x408FF80000000000 ; 1023
exp2_min_y: times 4 dq 0x0000000000000000 ; 0.0
exp2_max_y: times 4 dq 0x7FF0000000000000 ; HUGE_VAL

expm1_min_a: times 4 dq 0xC086232BDD7ABCD2 ; -708.3964185322641
expm1_max_a: times 4 dq 0x40862B7D369A5AA7 ; 709.4361393031039
expm1_min_y: times 4 dq 0xBFF0000000000000 ; -1.0
expm1_max_y: times 4 dq 0x7FF0000000000000 ; HUGE_VAL

one:  times 4 dq 0x3FF0000000000000 ; 1.0
bias: times 4 dq 0x43300000000003FF ; 2^52 + 1023.0

log2:    times 4 dq 0x3FE62E42FEFA39EF ; log(2.0l)
log2hi:  times 4 dq 0x3FE62E42FEE00000
log2lo:  times 4 dq 0x3DEA39EF35793C76
log2inv: times 4 dq 0x3FF71547652B82FE ; 1.0l / log(2.0l)

c2:  times 4 dq 0x3FE0000000000000 ; 1 / 2!
c3:  times 4 dq 0x3FC5555555555555 ; 1 / 3!
c4:  times 4 dq 0x3FA5555555555555 ; 1 / 4!
c5:  times 4 dq 0x3F81111111111111 ; 1 / 5!
c6:  times 4 dq 0x3F56C16C16C16C17 ; 1 / 6!
c7:  times 4 dq 0x3F2A01A01A01A01A ; 1 / 7!
c8:  times 4 dq 0x3EFA01A01A01A01A ; 1 / 8!
c9:  times 4 dq 0x3EC71DE3A556C734 ; 1 / 9!
c10: times 4 dq 0x3E927E4FB7789F5C ; 1 / 10!
c11: times 4 dq 0x3E5AE64567F544E4 ; 1 / 11!
c12: times 4 dq 0x3E21EED8EFF8D898 ; 1 / 12!
c13: times 4 dq 0x3DE6124613A86D09 ; 1 / 13!

section .text

mckl_vd_exp:   vd_exp exp
mckl_vd_exp2:  vd_exp exp2
mckl_vd_expm1: vd_exp expm1

; vim:ft=nasm
