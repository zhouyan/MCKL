;;============================================================================
;; MCKL/lib/asm/log.asm
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

global mckl_vd_log
global mckl_vd_log2
global mckl_vd_log10
global mckl_vd_log1p

default rel

; register used as constants: ymm6, ymm8-10, ymm12
; register used as variables: ymm1-5, ymm7, ymm11, ymm13-15

%macro log1pf_constants 0
    vmovapd ymm6, [rel sqrt2by2]
    vmovapd ymm8, [rel one]
    vmovapd ymm9, [rel two]
%endmacro

; log(1 + f) * (f + 2) / f - 2 = c15 * x^14 + ... + c5 * x^4 + c3 * x^2
%macro log1pf 3 ; implicity input ymm1, output ymm15
    vcmpltpd ymm11, ymm0, %1
    vcmpgtpd ymm15, ymm0, %2
    vxorpd ymm13, ymm13, ymm13 ; k = 0
%if %3 == 0
    vaddpd ymm1, ymm0, ymm8 ; b = a + 1
    vmovapd ymm14, ymm0     ; f = a;
%elif %3 == 1
    vmovapd ymm1, ymm0       ; b = a
    vsubpd ymm14, ymm0, ymm8 ; f = a - 1;
%else
    %error
%endif
    vorpd ymm11, ymm11, ymm15
    vtestpd ymm11, ymm11
    jz %%skip

    ; k = exponent(b)
    vpsrlq ymm2, ymm1, 52
    vorpd ymm2, ymm2, [emask0]
    vsubpd ymm3, ymm2, [emask1] ; exponent(b)

    ; fraction(b) / 2
    vandpd ymm1, ymm1, [fmask0]
    vorpd ymm4, ymm1, [fmask1] ; fraction(b) / 2

    ; fraction(b) > sqrt(2)
    vcmpgtpd ymm1, ymm4, ymm6
    vandpd ymm5, ymm1, ymm8
    vandnpd ymm7, ymm1, ymm4
    vaddpd ymm3, ymm3, ymm5
    vaddpd ymm4, ymm4, ymm7

    ; f = fraction(b) - 1
    vsubpd ymm4, ymm4, ymm8

    ; skip reduction if ymm0 in range
    vblendvpd ymm13, ymm13, ymm3, ymm11
    vblendvpd ymm14, ymm14, ymm4, ymm11

%%skip:

    ; x = f / (f + 2)
    vaddpd ymm1, ymm14, ymm9
    vdivpd ymm1, ymm14, ymm1

    vmovapd ymm15, [c15]
    vmovapd ymm11, [c11]
    vmovapd ymm7,  [c7]

    vmulpd ymm2, ymm1, ymm1 ; x^2
    vmulpd ymm3, ymm2, [c3] ; u3  = c3  * x^2
    vmulpd ymm4, ymm2, ymm2 ; x^4

    vfmadd213pd ymm15, ymm2, [c13] ; u15 = c15 * x^2 + c13
    vfmadd213pd ymm11, ymm2, [c9]  ; u11 = c11 * x^2 + c9
    vfmadd213pd ymm7,  ymm2, [c5]  ; u7  = c7  * x^2 + c5

    vfmadd213pd ymm15, ymm4, ymm11 ; v15 = u15 * x^4 + u11
    vfmadd213pd ymm7,  ymm4, ymm3  ; v7  = u7  * x^4 + u3

    vmulpd ymm4, ymm4, ymm4
    vfmadd213pd ymm15, ymm4, ymm7 ; z15 = v15 * x^8 + v7
%endmacro

%macro select 1 ; implicit input ymm0, ymm15, output ymm15
    vcmpltpd ymm1, ymm0, [%{1}_min_a] ; a < min_a
    vcmpgtpd ymm2, ymm0, [%{1}_max_a] ; a > max_a
    vcmpltpd ymm3, ymm0, [%{1}_nan_a] ; a < nan_a
    vcmpneqpd ymm4, ymm0, ymm0        ; a != a
    vorpd ymm5, ymm1, ymm2
    vorpd ymm5, ymm5, ymm3
    vorpd ymm5, ymm5, ymm4
    vtestpd ymm5, ymm5
    jz %%skip
    vblendvpd ymm15, ymm15, [%{1}_min_y], ymm1 ; min_y
    vblendvpd ymm15, ymm15, [%{1}_max_y], ymm2 ; max_y
    vblendvpd ymm15, ymm15, [%{1}_nan_y], ymm3 ; nan_y
    vblendvpd ymm15, ymm15, ymm0, ymm4         ; a
%%skip:
%endmacro

%macro log_constants 0
    log1pf_constants
    vmovapd ymm10, [ln2]
%endmacro

%macro log 2
    vmovupd ymm0, %2

    log1pf ymm6, [sqrt2], 1 ; log(1 + f) = f - x * (f - R)

    ; log(a) = k * log(2) + log(1 + f)
    vsubpd ymm15, ymm14, ymm15
    vfnmadd213pd ymm15, ymm1, ymm14
    vfmadd231pd ymm15, ymm13, ymm10

    select log
    vmovupd %1, ymm15
%endmacro

%macro log2_constants 0
    log1pf_constants
    vmovapd ymm10, [ln2inv]
%endmacro

%macro log2 2
    vmovupd ymm0, %2

    log1pf ymm6, [sqrt2], 1 ; log(1 + f) = f - x * (f - R)

    ; log2(a) = k + log(1 + f) / log(2)
    vsubpd ymm15, ymm14, ymm15
    vfnmadd213pd ymm15, ymm1, ymm14
    vfmadd213pd ymm15, ymm10, ymm13

    select log2
    vmovupd %1, ymm15
%endmacro

%macro log10_constants 0
    log1pf_constants
    vmovapd ymm10, [ln10_2]
    vmovapd ymm12, [ln10inv]
%endmacro

%macro log10 2
    vmovupd ymm0, %2

    log1pf ymm6, [sqrt2], 1 ; log(1 + f) = f - x * (f - R)

    ; log10(a) = k * log(10) / log(2) + log(1 + f) / log(10)
    vsubpd ymm15, ymm14, ymm15
    vfnmadd213pd ymm15, ymm1, ymm14
    vmulpd ymm13, ymm13, ymm10
    vfmadd213pd ymm15, ymm12, ymm13

    select log
    vmovupd %1, ymm15
%endmacro

%macro log1p_constants 0
    log1pf_constants
    vmovapd ymm10, [ln2]
%endmacro

%macro log1p 2
    vmovupd ymm0, %2

    log1pf [sqrt2m2], [sqrt2m1], 0 ; log(1 + f) = f - x * (f - R)

    ; log(1 + a) = k * log2 + log(1 + f)
    vsubpd ymm15, ymm14, ymm15
    vfnmadd213pd ymm15, ymm1, ymm14
    vfmadd231pd ymm15, ymm13, ymm10

    select log1p
    vmovupd %1, ymm15
%endmacro

section .rodata

align 32

log_min_a: times 4 dq 0x0010000000000000 ; DBL_MIN
log_max_a: times 4 dq 0x7FEFFFFFFFFFFFFF ; DBL_MAX
log_nan_a: times 4 dq 0x0000000000000000 ; 0.0
log_min_y: times 4 dq 0xFFF0000000000000 ; -HUGE_VAL
log_max_y: times 4 dq 0x7FF0000000000000 ; HUGE_VAL
log_nan_y: times 4 dq 0x7FF8000000000000 ; NaN

log2_min_a: times 4 dq 0x0010000000000000 ; DBL_MIN
log2_max_a: times 4 dq 0x7FEFFFFFFFFFFFFF ; DBL_MAX
log2_nan_a: times 4 dq 0x0000000000000000 ; 0.0
log2_min_y: times 4 dq 0xFFF0000000000000 ; -HUGE_VAL
log2_max_y: times 4 dq 0x7FF0000000000000 ; HUGE_VAL
log2_nan_y: times 4 dq 0x7FF8000000000000 ; NaN

log10_min_a: times 4 dq 0x0010000000000000 ; DBL_MIN
log10_max_a: times 4 dq 0x7FEFFFFFFFFFFFFF ; DBL_MAX
log10_nan_a: times 4 dq 0x0000000000000000 ; 0.0
log10_min_y: times 4 dq 0xFFF0000000000000 ; -HUGE_VAL
log10_max_y: times 4 dq 0x7FF0000000000000 ; HUGE_VAL
log10_nan_y: times 4 dq 0x7FF8000000000000 ; NaN

log1p_min_a: times 4 dq 0xBFEFFFFFFFFFFFFF ; nextafter(-1.0, 0.0)
log1p_max_a: times 4 dq 0x7FEFFFFFFFFFFFFF ; DBL_MAX
log1p_nan_a: times 4 dq 0xBFF0000000000000 ; -1.0
log1p_min_y: times 4 dq 0xFFF0000000000000 ; -HUGE_VAL
log1p_max_y: times 4 dq 0x7FF0000000000000 ; HUGE_VAL
log1p_nan_y: times 4 dq 0x7FF8000000000000 ; NaN

c3:  times 4 dq 0x3FE5555555555593
c5:  times 4 dq 0x3FD999999997FA04
c7:  times 4 dq 0x3FD2492494229359
c9:  times 4 dq 0x3FCC71C51D8E78AF
c11: times 4 dq 0x3FC7466496CB03DE
c13: times 4 dq 0x3FC39A09D078C69F
c15: times 4 dq 0x3FC2F112DF3E5244

emask0: times 4 dq 0x4330000000000000 ; 2^52
emask1: times 4 dq 0x43300000000003FF ; 2^52 + 1023
fmask0: times 4 dq 0x000FFFFFFFFFFFFF ; fraction mask
fmask1: times 4 dq 0x3FE0000000000000 ; fraction(a) / 2

one:      times 4 dq 0x3FF0000000000000 ; 1.0
two:      times 4 dq 0x4000000000000000 ; 2.0
ln2:      times 4 dq 0x3FE62E42FEFA39EF ; log(2.0l)
ln2inv:   times 4 dq 0x3FF71547652B82FE ; 1.0l / log(2.0l)
ln10_2:   times 4 dq 0x3FD34413509F79FF ; log10(2.0l)
ln10inv:  times 4 dq 0x3FDBCB7B1526E50E ; 1.0l / log(10.0l)
sqrt2:    times 4 dq 0x3FF6A09E667F3BCD ; sqrt(2.0l)
sqrt2by2: times 4 dq 0x3FE6A09E667F3BCD ; sqrt(2.0l) / 2.0l
sqrt2m1:  times 4 dq 0x3FDA827999FCEF32 ; sqrt(2.0l) - 1.0l
sqrt2m2:  times 4 dq 0xBFD2BEC333018867 ; sqrt(2.0l) / 2.0l - 1.0l

section .text

mckl_vd_log:   math_kernel_a1r1 8, log
mckl_vd_log2:  math_kernel_a1r1 8, log2
mckl_vd_log10: math_kernel_a1r1 8, log10
mckl_vd_log1p: math_kernel_a1r1 8, log1p

; vim:ft=nasm
