;;============================================================================
;; MCKL/lib/asm/exp.asm
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

%include "/math.asm"

global mckl_vd_exp
global mckl_vd_exp2
global mckl_vd_expm1

default rel

; register used as constants: ymm6, ymm8, ymm10, ymm12, ymm14
; register used as variables: ymm1-5, ymm7, ymm9, ymm11, ymm13, ymm15

%macro expm1x_constants 0
    vmovapd ymm6,  [ln2inv]
    vmovapd ymm8,  [ln2hi]
    vmovapd ymm10, [ln2lo]
    vmovapd ymm12, [bias]
%endmacro

; exp(x) - 1 = c13 * x^13 + ... + c2 * x^2 + x
%macro expm1x 1 ; implicity input ymm1, ymm15, output ymm1-4, ymm13, ymm15
    vmovapd ymm13, [c13]
    vmovapd ymm11, [c11]
    vmovapd ymm9,  [c9]
    vmovapd ymm7,  [c7]
    vmovapd ymm5,  [c5]
    vmovapd ymm3,  [c3]

    vmulpd ymm2, ymm1, ymm1 ; x^2
    vmulpd ymm4, ymm2, ymm2 ; x^4

    vfmadd213pd ymm13, ymm1, [c12] ; u13 = c13 * x + c12
    vfmadd213pd ymm11, ymm1, [c10] ; u11 = c11 * x + c10
    vfmadd213pd ymm9,  ymm1, [c8]  ; u9  = c9  * x + c8
    vfmadd213pd ymm7,  ymm1, [c6]  ; u7  = c7  * x + c6
    vfmadd213pd ymm5,  ymm1, [c4]  ; u5  = c5  * x + c4
    vfmadd213pd ymm3,  ymm1, [c2]  ; u3  = c3  * x + c2

    vfmadd213pd ymm13, ymm2, ymm11 ; v13 = u13 * x^2 + u11
    vfmadd213pd ymm9,  ymm2, ymm7  ; v9  = u9  * x^2 + u7
    vfmadd213pd ymm5,  ymm2, ymm3  ; v5  = u5  * x^2 + u3

    vcmpneqpd ymm3, ymm0, ymm0 ; a != a
    vaddpd ymm15, ymm15, ymm12 ; 2^k
    vpsllq ymm15, 52

    vfmadd213pd ymm13, ymm4, ymm9 ; w13 = v13 * x^4 + v9
    vfmadd213pd ymm5,  ymm2, ymm1 ; w5  = v5  * x^2 + x

    vmulpd ymm4, ymm4, ymm2 ; x^6
    vcmpltpd ymm1, ymm0, [%{1}_min_a] ; a < min_a
    vcmpgtpd ymm2, ymm0, [%{1}_max_a] ; a > max_a

    vfmadd213pd ymm13, ymm4, ymm5 ; z13 = w13 * x^6 + w5

    vorpd ymm4, ymm1, ymm2
    vorpd ymm4, ymm4, ymm3
%endmacro

%macro select 1 ; implicit input ymm1-4, ymm13, output ymm13
    vtestpd ymm4, ymm4
    jz %%skip
    vblendvpd ymm13, ymm13, [%{1}_min_y], ymm1 ; min_y
    vblendvpd ymm13, ymm13, [%{1}_max_y], ymm2 ; max_y
    vblendvpd ymm13, ymm13, ymm0, ymm3         ; a
%%skip:
%endmacro

%macro exp_constants 0
    expm1x_constants
%endmacro

%macro exp 2
    vmovupd ymm0, %2

    ; k = round(a / log(2))
    vmulpd ymm15, ymm0, ymm6
    vroundpd ymm15, ymm15, 0x8

    ; x = a - k * log(2)
    vmovapd ymm1, ymm0
    vfnmadd231pd ymm1, ymm15, ymm8
    vfnmadd231pd ymm1, ymm15, ymm10

    expm1x exp ; exp(x) = R + 1

    ; exp(a) = exp(x) * 2^k = R * 2^k + 2^k
    vfmadd213pd ymm13, ymm15, ymm15

    select exp
    vmovupd %1, ymm13
%endmacro

%macro exp2_constants 0
    expm1x_constants
    vmovapd ymm14, [ln2]
%endmacro

%macro exp2 2
    vmovupd ymm0, %2

    ; k = round(a)
    vroundpd ymm15, ymm0, 0x8

    ; x = (a - k) * log(2)
    vsubpd ymm1, ymm0, ymm15
    vmulpd ymm1, ymm1, ymm14

    expm1x exp2 ; exp(x) = R + 1

    ; 2^a = exp(x) * 2^k = R * 2^k + 2^k
    vfmadd213pd ymm13, ymm15, ymm15

    select exp2
    vmovupd %1, ymm13
%endmacro

%macro expm1_constants 0
    expm1x_constants
%endmacro

%macro expm1 2
    vmovupd ymm0, %2

    ; log(2) / 2 <= abs(a) <= log(2)
    vxorpd ymm15, ymm15, ymm15 ; k = 0
    vmulpd ymm1, ymm0, [half]  ; x = 0.5 * a
    vandpd ymm2, ymm0, [pmask] ; abs(a)
    vcmpltpd ymm14, ymm2, [ln2by2] ; abs(a) < log(2) / 2
    vcmpgtpd ymm13, ymm2, [ln2]    ; abs(a) > log(2)
    vorpd ymm14, ymm14, ymm13 ; abs(a) < log(2) / 2 || abs(a) > log(2)
    vtestpd ymm14, ymm14 ; log(2) / 2 <= abs(a) <= log(2)
    jz %%skip

    ; k = round(a / log(2))
    vmulpd ymm15, ymm0, ymm6
    vroundpd ymm15, ymm15, 0x8

    ; x = a - k * log(2)
    vmovapd ymm2, ymm0
    vfnmadd231pd ymm2, ymm15, ymm8
    vfnmadd231pd ymm2, ymm15, ymm10

    vblendvpd ymm1, ymm1, ymm2, ymm14

%%skip:

    expm1x expm1 ; exp(x) = R + 1

    ; exp(a) - 1 = exp(x) * 2^k - 1 = R * 2^k + (2^k - 1)
    vsubpd ymm5, ymm15, [one] ; 2^k - 1
    vaddpd ymm9, ymm13, ymm13 ; 2 * R
    vmovapd ymm11, ymm13
    vfmadd213pd ymm13, ymm15, ymm5 ; R * 2^k + (2^k - 1)
    vfmadd213pd ymm11, ymm11, ymm9 ; R * R + 2 * R
    vblendvpd ymm13, ymm11, ymm13, ymm14

    select expm1
    vmovupd %1, ymm13
%endmacro

section .rodata

align 32

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

c2:  times 4 dq 0x3FE0000000000000
c3:  times 4 dq 0x3FC5555555555555
c4:  times 4 dq 0x3FA5555555555555
c5:  times 4 dq 0x3F81111111111111
c6:  times 4 dq 0x3F56C16C16C16C17
c7:  times 4 dq 0x3F2A01A01A01A01A
c8:  times 4 dq 0x3EFA01A01A01A01A
c9:  times 4 dq 0x3EC71DE3A556C734
c10: times 4 dq 0x3E927E4FB7789F5C
c11: times 4 dq 0x3E5AE64567F544E4
c12: times 4 dq 0x3E21EED8EFF8D898
c13: times 4 dq 0x3DE6124613A86D09

bias:   times 4 dq 0x43300000000003FF ; 2^52 + 1023
one:    times 4 dq 0x3FF0000000000000 ; 1.0
half:   times 4 dq 0x3FE0000000000000 ; 0.5
pmask:  times 4 dq 0x7FFFFFFFFFFFFFFF ; abs(x) = x & pmask
ln2:    times 4 dq 0x3FE62E42FEFA39EF ; log(2.0l)
ln2hi:  times 4 dq 0x3FE62E42FEE00000
ln2lo:  times 4 dq 0x3DEA39EF35793C76
ln2inv: times 4 dq 0x3FF71547652B82FE ; 1.0l / log(2.0l)
ln2by2: times 4 dq 0x3FD62E42FEFA39EF ; log(2.0l) / 2.0l

section .text

mckl_vd_exp:   math_kernel_a1r1 8, exp
mckl_vd_exp2:  math_kernel_a1r1 8, exp2
mckl_vd_expm1: math_kernel_a1r1 8, expm1

; vim:ft=nasm
