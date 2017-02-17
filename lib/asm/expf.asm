;;============================================================================
;; MCKL/lib/asm/expf.asm
;;----------------------------------------------------------------------------
;; MCKL: Monte Carlo Kernel Library
;;----------------------------------------------------------------------------
;; Copyright (c) 2013-2017, Yan Zhou
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

global mckl_vs_exp
global mckl_vs_exp2
global mckl_vs_expm1

default rel

; register used as constants: ymm6, ymm8, ymm10, ymm12, ymm13, ymm14
; register used as variables: ymm1-5, ymm7, ymm9, ymm11, ymm15

%macro expm1x_constants 0
    vmovaps ymm6,  [ln2inv]
    vmovaps ymm8,  [ln2hi]
    vmovaps ymm10, [ln2lo]
    vmovaps ymm12, [bias]
%endmacro

; exp(x) - 1 = c7 * x^7 + ... + c2 * x^2 + x
%macro expm1x 1 ; implicity input ymm1, ymm15, output ymm1-4, ymm7, ymm15
    vmovaps ymm7, [c7]
    vmovaps ymm5, [c5]
    vmovaps ymm3, [c3]

    vmulps ymm2, ymm1, ymm1 ; x^2
    vmulps ymm4, ymm2, ymm2 ; x^4

    vfmadd213ps ymm7, ymm1, [c6] ; u7 = c7 * x + c6
    vfmadd213ps ymm5, ymm1, [c4] ; u5 = c5 * x + c4
    vfmadd213ps ymm3, ymm1, [c2] ; u3 = c3 * x + c2

    vfmadd213ps ymm7, ymm2, ymm5 ; v7 = u7 * x^2 + u5
    vfmadd213ps ymm3, ymm2, ymm1 ; v3 = u3 * x^2 + x

    vcmpltps ymm1, ymm0, [%{1}_min_a] ; a < min_a
    vcmpgtps ymm2, ymm0, [%{1}_max_a] ; a > max_a
    vaddps ymm15, ymm15, ymm12 ; 2^k
    vpslld ymm15, 23

    vfmadd213ps ymm7, ymm4, ymm3 ; z7 = v7 * x^4 + v3

    vcmpneqps ymm3, ymm0, ymm0 ; a != a
    vorps ymm4, ymm2, ymm1
    vorps ymm4, ymm4, ymm3
%endmacro

%macro select 1 ; implicit input ymm1-4, ymm7, output ymm7
    vtestps ymm4, ymm4
    jz %%skip
    vblendvps ymm7, ymm7, [%{1}_min_y], ymm1 ; min_y
    vblendvps ymm7, ymm7, [%{1}_max_y], ymm2 ; max_y
    vblendvps ymm7, ymm7, ymm0, ymm3         ; a
%%skip:
%endmacro

%macro exp_constants 0
    expm1x_constants
%endmacro

%macro exp 2
    vmovups ymm0, %2

    ; k = round(a / log(2))
    vmulps ymm15, ymm0, ymm6
    vroundps ymm15, ymm15, 0x8

    ; x = a - k * log(2)
    vmovaps ymm1, ymm0
    vfnmadd231ps ymm1, ymm15, ymm8
    vfnmadd231ps ymm1, ymm15, ymm10

    expm1x exp ; exp(x) = R + 1

    ; exp(a) = exp(x) * 2^k = R * 2^k + 2^k
    vfmadd213ps ymm7, ymm15, ymm15

    select exp
    vmovups %1, ymm7
%endmacro

%macro exp2_constants 0
    expm1x_constants
    vmovaps ymm14, [ln2]
%endmacro

%macro exp2 2
    vmovups ymm0, %2

    ; k = round(a)
    vroundps ymm15, ymm0, 0x8

    ; x = (a - k) * log(2)
    vsubps ymm1, ymm0, ymm15
    vmulps ymm1, ymm1, ymm14

    expm1x exp2 ; exp(x) = R + 1

    ; 2^a = exp(x) * 2^k = R * 2^k + 2^k
    vfmadd213ps ymm7, ymm15, ymm15

    select exp2
    vmovups %1, ymm7
%endmacro

%macro expm1_constants 0
    expm1x_constants
%endmacro

%macro expm1 2
    vmovups ymm0, %2

    ; log(2) / 2 <= abs(a) <= log(2)
    vxorps ymm15, ymm15, ymm15 ; k = 0
    vmulps ymm1, ymm0, [half]  ; x = 0.5 * a
    vandps ymm2, ymm0, [pmask] ; abs(a)
    vcmpltps ymm14, ymm2, [ln2by2] ; abs(a) < log(2) / 2
    vcmpgtps ymm7,  ymm2, [ln2]    ; abs(a) > log(2)
    vorps ymm14, ymm14, ymm7 ; abs(a) < log(2) / 2 || abs(a) > log(2)
    vtestps ymm14, ymm14 ; log(2) / 2 <= abs(a) <= log(2)
    jz %%skip

    ; k = round(a / log(2))
    vmulps ymm15, ymm0, ymm6
    vroundps ymm15, ymm15, 0x8

    ; x = a - k * log(2)
    vmovaps ymm2, ymm0
    vfnmadd231ps ymm2, ymm15, ymm8
    vfnmadd231ps ymm2, ymm15, ymm10

    vblendvps ymm1, ymm1, ymm2, ymm14

%%skip:

    expm1x expm1 ; exp(x) = R + 1

    ; exp(a) - 1 = exp(x) * 2^k - 1 = R * 2^k + (2^k - 1)
    vsubps ymm5, ymm15, [one] ; 2^k - 1
    vaddps ymm9, ymm7, ymm7 ; 2 * R
    vmovaps ymm11, ymm7
    vfmadd213ps ymm7,  ymm15, ymm5 ; R * 2^k + (2^k - 1)
    vfmadd213ps ymm11, ymm11, ymm9 ; R * R + 2 * R
    vblendvps ymm7, ymm11, ymm7, ymm14

    select expm1
    vmovups %1, ymm7
%endmacro

section .rodata

align 32

exp_min_a: times 8 dd 0xC2AEAC4F ; -87.33654
exp_max_a: times 8 dd 0x42B0C0A5 ; 88.37626
exp_min_y: times 8 dd 0x00000000 ; 0.0
exp_max_y: times 8 dd 0x7F800000 ; HUGE_VALF

exp2_min_a: times 8 dd 0xC2FC0000 ; -126
exp2_max_a: times 8 dd 0x42FE0000 ; 127
exp2_min_y: times 8 dd 0x00000000 ; 0.0
exp2_max_y: times 8 dd 0x7F800000 ; HUGE_VALF

expm1_min_a: times 8 dd 0xC2AEAC4F ; -87.33654
expm1_max_a: times 8 dd 0x42B0C0A5 ; 88.37626
expm1_min_y: times 8 dd 0xBF800000 ; -1.0
expm1_max_y: times 8 dd 0x7F800000 ; HUGE_VALF

c2: times 8 dd 0x3F000000
c3: times 8 dd 0x3E2AAAAB
c4: times 8 dd 0x3D2AAAAB
c5: times 8 dd 0x3C088889
c6: times 8 dd 0x3AB60B61
c7: times 8 dd 0x39500D01

bias:   times 8 dd 0x4B00007F ; 2^23 + 127
one:    times 8 dd 0x3F800000 ; 1.0
half:   times 8 dd 0x3F000000 ; 0.5
pmask:  times 8 dd 0x7FFFFFFF ; abs(x) = x & pmask
ln2:    times 8 dd 0x3F317218 ; log(2.0l)
ln2hi:  times 8 dd 0x3F318000
ln2lo:  times 8 dd 0xB95E8083
ln2inv: times 8 dd 0x3FB8AA3B ; 1.0l / log(2.0l)
ln2by2: times 8 dd 0x3EB17218 ; log(2.0l) / 2.0l

section .text

mckl_vs_exp:   math_kernel_a1r1 4, exp
mckl_vs_exp2:  math_kernel_a1r1 4, exp2
mckl_vs_expm1: math_kernel_a1r1 4, expm1

; vim:ft=nasm
