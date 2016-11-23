;;============================================================================
;; MCKL/lib/asm/exp2.asm
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

global mckl_exp2_vv_pd

%macro exp2 1 ; implicit input ymm0 {{{
    ; r = round(a)
    vroundpd ymm14, ymm0, 0x8

    ; x = (a  - r) * log(2)
    vsubpd ymm1, ymm0, ymm14
    vmulpd ymm1, ymm1, [rel exp2_log2d]

    ; s = 2^r
    vaddpd ymm15, ymm14, [rel exp2_bias]
    vpsllq ymm15, 52

    ; z13 = c13 * x^13 + ... + c2 * x^2 + x
    vmulpd ymm2, ymm1, ymm1 ; x2 = x^2
    vmulpd ymm4, ymm2, ymm2 ; x4 = x^4
    vmulpd ymm8, ymm4, ymm4 ; x8 = x^8
    vmovapd ymm3,  [rel exp2_c3]
    vmovapd ymm5,  [rel exp2_c5]
    vmovapd ymm7,  [rel exp2_c7]
    vmovapd ymm9,  [rel exp2_c9]
    vmovapd ymm11, [rel exp2_c11]
    vmovapd ymm13, [rel exp2_c13]
    vfmadd213pd ymm3,  ymm1, [rel exp2_c2]  ; u3  = c3  * x + c2
    vfmadd213pd ymm5,  ymm1, [rel exp2_c4]  ; u5  = c5  * x + c4
    vfmadd213pd ymm7,  ymm1, [rel exp2_c6]  ; u7  = c7  * x + c6
    vfmadd213pd ymm9,  ymm1, [rel exp2_c8]  ; u9  = c9  * x + c8
    vfmadd213pd ymm11, ymm1, [rel exp2_c10] ; u11 = c11 * x + c10
    vfmadd213pd ymm13, ymm1, [rel exp2_c12] ; u13 = c13 * x + c12
    vfmadd213pd ymm3,  ymm2, ymm1  ; v3  = u3  * x^2 + x
    vfmadd213pd ymm7,  ymm2, ymm5  ; v7  = u7  * x^2 + u5
    vfmadd213pd ymm11, ymm2, ymm9  ; v11 = u11 * x^2 + u9
    vfmadd213pd ymm7,  ymm4, ymm3  ; w7  = v7  * x^4 + v3
    vfmadd213pd ymm13, ymm4, ymm11 ; w13 = v13 * x^4 + v11
    vfmadd213pd ymm13, ymm8, ymm7  ; z13 = w13 * x^8 + w7

    ; z13 = z13 * s + s
    vfmadd213pd ymm13, ymm15, ymm15

    ; FIXME overflow, underflow, infinity, NaN

    vmovupd %1, ymm13
%endmacro ; }}}

section .rodata

align 32

exp2_one:   times 4 dq 0x3FF0000000000000 ; 1.0
exp2_bias:  times 4 dq 0x43300000000003FF ; 2^52 + 1023
exp2_log2f: times 4 dq 0x3FE62E4300000000 ; log(2.0f)
exp2_log2l: times 4 dq 0xBE205C610CA80000 ; log(2.0l) - log(2.0f)
exp2_log2d: times 4 dq 0x3FE62E42FEFA39EF ; log(2.0l)

exp2_c2:  times 4 dq 0x3FE0000000000000 ; 1 / 2!
exp2_c3:  times 4 dq 0x3FC5555555555555 ; 1 / 3!
exp2_c4:  times 4 dq 0x3FA5555555555555 ; 1 / 4!
exp2_c5:  times 4 dq 0x3F81111111111111 ; 1 / 5!
exp2_c6:  times 4 dq 0x3F56C16C16C16C17 ; 1 / 6!
exp2_c7:  times 4 dq 0x3F2A01A01A01A01A ; 1 / 7!
exp2_c8:  times 4 dq 0x3EFA01A01A01A01A ; 1 / 8!
exp2_c9:  times 4 dq 0x3EC71DE3A556C734 ; 1 / 9!
exp2_c10: times 4 dq 0x3E927E4FB7789F5C ; 1 / 10!
exp2_c11: times 4 dq 0x3E5AE64567F544E4 ; 1 / 11!
exp2_c12: times 4 dq 0x3E21EED8EFF8D898 ; 1 / 12!
exp2_c13: times 4 dq 0x3DE6124613A86D09 ; 1 / 13!

section .text

; rdi n
; rsi a
; rdx y
mckl_exp2_vv_pd: ; {{{
    cmp rdi, 4
    jl .last

    .loop:
        vmovupd ymm0, [rsi]
        exp2 [rdx]
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
        exp2 [rsp - 0x28]
        mov rcx, rax
        mov rsi, rsp
        sub rsi, 0x28
        mov rdi, rdx
        cld
        rep movsq

    .return:
        ret
; }}} mckl_exp2_vv_pd

; vim:ft=nasm
