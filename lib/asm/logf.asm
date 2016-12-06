;;============================================================================
;; MCKL/lib/asm/logf.asm
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

%include "/math.asm"

global mckl_vs_log
global mckl_vs_log2
global mckl_vs_log10
global mckl_vs_log1p

default rel

; register used as constants: ymm6-ymm8, ymm10, ymm12
; register used as variables: ymm1-5, ymm9, ymm11, ymm13-15

%macro log1pf_constants 0
    vmovaps ymm6, [rel sqrt2by2]
    vmovaps ymm7, [rel one]
    vmovaps ymm8, [rel two]
%endmacro

; log(1 + f) * (f + 2) / f - 2 = c9 * x^8 + ... + c5 * x^4 + c3 * x^2
%macro log1pf 3 ; implicity input ymm1, output ymm9
    vcmpltps ymm11, ymm0, %1
    vcmpgtps ymm15, ymm0, %2
    vxorps ymm13, ymm13, ymm13 ; k = 0
%if %3 == 0
    vaddps ymm1, ymm0, ymm7 ; b = a + 1
    vmovaps ymm14, ymm0     ; f = a;
%elif %3 == 1
    vmovaps ymm1, ymm0       ; b = a
    vsubps ymm14, ymm0, ymm7 ; f = a - 1;
%else
    %error
%endif
    vorps ymm11, ymm11, ymm15
    vtestps ymm11, ymm11
    jz %%skip

    ; k = exponent(b)
    vpsrld ymm2, ymm1, 23
    vorps ymm2, ymm2, [emask0]
    vsubps ymm3, ymm2, [emask1] ; exponent(b)

    ; fraction(b) / 2
    vandps ymm1, ymm1, [fmask0]
    vorps ymm4, ymm1, [fmask1] ; fraction(b) / 2

    ; fraction(b) > sqrt(2)
    vcmpgtps ymm1, ymm4, ymm6
    vandps ymm5, ymm1, ymm7
    vandnps ymm9, ymm1, ymm4
    vaddps ymm3, ymm3, ymm5
    vaddps ymm4, ymm4, ymm9

    ; f = fraction(b) - 1
    vsubps ymm4, ymm4, ymm7

    ; skip reduction if ymm0 in range
    vblendvps ymm13, ymm13, ymm3, ymm11
    vblendvps ymm14, ymm14, ymm4, ymm11

%%skip:

    ; x = f / (f + 2)
    vaddps ymm1, ymm14, ymm8
    vdivps ymm1, ymm14, ymm1

    vmovaps ymm9, [c9]
    vmovaps ymm5, [c5]

    vmulps ymm2, ymm1, ymm1 ; x^2
    vmulps ymm4, ymm2, ymm2 ; x^4

    vfmadd213ps ymm9, ymm2, [c7] ; u9 = c9 * x^2 + c7
    vfmadd213ps ymm5, ymm2, [c3] ; u5 = c5 * x^2 + c3

    vfmadd213ps ymm9, ymm4, ymm5 ; v9 = u9 * x^4 + u5

    vmulps ymm9, ymm9, ymm2 ; z9 = v9 * x^2
%endmacro

%macro select 1 ; implicit input ymm0, ymm9, output ymm9
    vcmpltps ymm1, ymm0, [%{1}_min_a] ; a < min_a
    vcmpgtps ymm2, ymm0, [%{1}_max_a] ; a > max_a
    vcmpltps ymm3, ymm0, [%{1}_nan_a] ; a < nan_a
    vcmpneqps ymm4, ymm0, ymm0        ; a != a
    vorps ymm5, ymm1, ymm2
    vorps ymm5, ymm5, ymm3
    vorps ymm5, ymm5, ymm4
    vtestps ymm5, ymm5
    jz %%skip
    vblendvps ymm9, ymm9, [%{1}_min_y], ymm1 ; min_y
    vblendvps ymm9, ymm9, [%{1}_max_y], ymm2 ; max_y
    vblendvps ymm9, ymm9, [%{1}_nan_y], ymm3 ; nan_y
    vblendvps ymm9, ymm9, ymm0, ymm4         ; a
%%skip:
%endmacro

%macro log_constants 0
    log1pf_constants
    vmovaps ymm10, [ln2]
%endmacro

%macro log 2
    vmovups ymm0, %2

    log1pf ymm6, [sqrt2], 1 ; log(1 + f) = f - x * (f - R)

    ; log(a) = k * log(2) + log(1 + f)
    vsubps ymm9, ymm14, ymm9
    vfnmadd213ps ymm9, ymm1, ymm14
    vfmadd231ps ymm9, ymm13, ymm10

    select log
    vmovups %1, ymm9
%endmacro

%macro log2_constants 0
    log1pf_constants
    vmovaps ymm10, [ln2inv]
%endmacro

%macro log2 2
    vmovups ymm0, %2

    log1pf ymm6, [sqrt2], 1 ; log(1 + f) = f - x * (f - R)

    ; log2(a) = k + log(1 + f) / log(2)
    vsubps ymm9, ymm14, ymm9
    vfnmadd213ps ymm9, ymm1, ymm14
    vfmadd213ps ymm9, ymm10, ymm13

    select log2
    vmovups %1, ymm9
%endmacro

%macro log10_constants 0
    log1pf_constants
    vmovaps ymm10, [ln10_2]
    vmovaps ymm12, [ln10inv]
%endmacro

%macro log10 2
    vmovups ymm0, %2

    log1pf ymm6, [sqrt2], 1 ; log(1 + f) = f - x * (f - R)

    ; log10(a) = k * log(10) / log(2) + log(1 + f) / log(10)
    vsubps ymm9, ymm14, ymm9
    vfnmadd213ps ymm9, ymm1, ymm14
    vmulps ymm13, ymm13, ymm10
    vfmadd213ps ymm9, ymm12, ymm13

    select log
    vmovups %1, ymm9
%endmacro

%macro log1p_constants 0
    log1pf_constants
    vmovaps ymm10, [ln2]
%endmacro

%macro log1p 2
    vmovups ymm0, %2

    log1pf [sqrt2m2], [sqrt2m1], 0 ; log(1 + f) = f - x * (f - R)

    ; log(1 + a) = k * log2 + log(1 + f)
    vsubps ymm9, ymm14, ymm9
    vfnmadd213ps ymm9, ymm1, ymm14
    vfmadd231ps ymm9, ymm13, ymm10

    select log1p
    vmovups %1, ymm9
%endmacro

section .rodata

align 32

log_min_a: times 8 dd 0x00800000 ; FLT_MIN
log_max_a: times 8 dd 0x7F7FFFFF ; FLT_MAX
log_nan_a: times 8 dd 0x00000000 ; 0.0
log_min_y: times 8 dd 0xFF800000 ; -HUGE_VALF
log_max_y: times 8 dd 0x7F800000 ; HUGE_VALF
log_nan_y: times 8 dd 0x7FC00000 ; NaN

log2_min_a: times 8 dd 0x00800000 ; FLT_MIN
log2_max_a: times 8 dd 0x7F7FFFFF ; FLT_MAX
log2_nan_a: times 8 dd 0x00000000 ; 0.0
log2_min_y: times 8 dd 0xFF800000 ; -HUGE_VALF
log2_max_y: times 8 dd 0x7F800000 ; HUGE_VALF
log2_nan_y: times 8 dd 0x7FC00000 ; NaN

log10_min_a: times 8 dd 0x00800000 ; FLT_MIN
log10_max_a: times 8 dd 0x7F7FFFFF ; FLT_MAX
log10_nan_a: times 8 dd 0x00000000 ; 0.0
log10_min_y: times 8 dd 0xFF800000 ; -HUGE_VALF
log10_max_y: times 8 dd 0x7F800000 ; HUGE_VALF
log10_nan_y: times 8 dd 0x7FC00000 ; NaN

log1p_min_a: times 8 dd 0xBF7FFFFF ; nextafter(-1.0, 0.0)
log1p_max_a: times 8 dd 0x7F7FFFFF ; FLT_MAX
log1p_nan_a: times 8 dd 0xBF800000 ; -1.0
log1p_min_y: times 8 dd 0xFF800000 ; -HUGE_VALF
log1p_max_y: times 8 dd 0x7F800000 ; HUGE_VALF
log1p_nan_y: times 8 dd 0x7FC00000 ; NaN

c3: times 8 dd 0x3F2AAAAA
c5: times 8 dd 0x3ECCCE13
c7: times 8 dd 0x3E91E9EE
c9: times 8 dd 0x3E789E26

emask0: times 8 dd 0x4B000000 ; 2^23
emask1: times 8 dd 0x4B00007F ; 2^23 + 127
fmask0: times 8 dd 0x007FFFFF ; fraction mask
fmask1: times 8 dd 0x3F000000 ; fraction(a) / 2

one:      times 8 dd 0x3F800000 ; 1.0
two:      times 8 dd 0x40000000 ; 2.0
ln2:      times 8 dd 0x3F317218 ; log(2.0l)
ln2inv:   times 8 dd 0x3FB8AA3B ; 1.0l / log(2.0l)
ln10_2:   times 8 dd 0x3E9A209B ; log10(2.0l)
ln10inv:  times 8 dd 0x3EDE5BD9 ; 1.0l / log(10.0l)
sqrt2:    times 8 dd 0x3FB504F3 ; sqrt(2.0l)
sqrt2by2: times 8 dd 0x3F3504F3 ; sqrt(2.0l) / 2.0l
sqrt2m1:  times 8 dd 0x3ED413CD ; sqrt(2.0l) - 1.0l
sqrt2m2:  times 8 dd 0xBE95F61A ; sqrt(2.0l) / 2.0l - 1.0l

section .text

mckl_vs_log:   math_kernel_a1r1 4, log
mckl_vs_log2:  math_kernel_a1r1 4, log2
mckl_vs_log10: math_kernel_a1r1 4, log10
mckl_vs_log1p: math_kernel_a1r1 4, log1p

; vim:ft=nasm
