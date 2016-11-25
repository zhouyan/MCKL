;;============================================================================
;; MCKL/lib/asm/log.asm
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

global mckl_vd_log
global mckl_vd_log2
global mckl_vd_log10
global mckl_vd_log1p

%macro poly 0 ; {{{ implicity input ymm1, output ymm15
    vmulpd ymm2, ymm1, ymm1 ; x2 = x^2
    vmulpd ymm4, ymm2, ymm2 ; x4 = x^4
    vmulpd ymm8, ymm4, ymm4 ; x8 = x^8
    vmovapd ymm7,  [rel c7]
    vmovapd ymm11, [rel c11]
    vmovapd ymm15, [rel c15]
    vmulpd      ymm3,  ymm2, [rel c3]  ; u3  = c3  * x^2
    vfmadd213pd ymm7,  ymm2, [rel c5]  ; u7  = c7  * x^2 + c5
    vfmadd213pd ymm11, ymm2, [rel c9]  ; u11 = c11 * x^2 + c9
    vfmadd213pd ymm15, ymm2, [rel c13] ; u15 = c15 * x^2 + c13
    vfmadd213pd ymm7,  ymm4, ymm3      ; v7  = u7  * x^4 + u3
    vfmadd213pd ymm15, ymm4, ymm11     ; v15 = u15 * x^4 + u11
    vfmadd213pd ymm15, ymm8, ymm7      ; z15 = v15 * x^8 + v7
%endmacro ; }}}

%macro log 0 ; {{{ implicit input ymm0
    ; k = exponent(a)
    vpsrlq ymm13, ymm0, 52
    vpor ymm13, ymm13, [rel pow252]
    vsubpd ymm13, ymm13, [rel bias]

    ; 1 + f = 0.5 * fraction(a)
    vpand ymm14, ymm0, [rel fmask]
    vpor ymm14, ymm14, [rel emask]

    ; 1 + f > sqrt(2) / 2
    vcmpgtpd ymm1, ymm14, [rel sqrt2by2]
    vaddpd ymm3, ymm13, [rel one]
    vaddpd ymm4, ymm14, ymm14
    vblendvpd ymm13, ymm13, ymm3, ymm1
    vblendvpd ymm14, ymm4, ymm14, ymm1

    ; f = a - 1
    vsubpd ymm14, ymm14, [rel one]

    ; x = f / (2 + f)
    vaddpd ymm1, ymm14, [rel two]
    vdivpd ymm1, ymm14, ymm1

    ; z15 = c15 * x^14 + ... + c5 * x^4 + c3 * x^2
    ; res = k * log2hi - ((s * (f - R) - k * log2lo) - f)
    poly
    vsubpd ymm15, ymm14, ymm15
    vmulpd ymm2, ymm13, [rel log2lo]
    vfmsub213pd ymm15, ymm1, ymm2
    vsubpd ymm15, ymm15, ymm14
    vfmsub231pd ymm15, ymm13, [rel log2hi]
%endmacro ; }}}

%macro log2 0 ; {{{ implicit input ymm0
    ; k = exponent(a)
    vpsrlq ymm13, ymm0, 52
    vpor ymm13, ymm13, [rel pow252]
    vsubpd ymm13, ymm13, [rel bias]

    ; 1 + f = 0.5 * fraction(a)
    vpand ymm14, ymm0, [rel fmask]
    vpor ymm14, ymm14, [rel emask]

    ; 1 + f > sqrt(2) / 2
    vcmpgtpd ymm1, ymm14, [rel sqrt2by2]
    vaddpd ymm3, ymm13, [rel one]
    vaddpd ymm4, ymm14, ymm14
    vblendvpd ymm13, ymm13, ymm3, ymm1
    vblendvpd ymm14, ymm4, ymm14, ymm1

    ; f = a - 1
    vsubpd ymm14, ymm14, [rel one]

    ; x = f / (2 + f)
    vaddpd ymm1, ymm14, [rel two]
    vdivpd ymm1, ymm14, ymm1

    ; z15 = c15 * x^14 + ... + c5 * x^4 + c3 * x^2
    ; res = k + (f - s * (f - R)) * log2inv
    poly
    vsubpd ymm15, ymm14, ymm15
    vfnmadd213pd ymm15, ymm1, ymm14
    vfmadd132pd ymm15, ymm13, [rel log2inv]
%endmacro ; }}}

%macro log10 0 ; {{{ implicit input ymm0
    ; k = exponent(a)
    vpsrlq ymm13, ymm0, 52
    vpor ymm13, ymm13, [rel pow252]
    vsubpd ymm13, ymm13, [rel bias]

    ; 1 + f = 0.5 * fraction(a)
    vpand ymm14, ymm0, [rel fmask]
    vpor ymm14, ymm14, [rel emask]

    ; 1 + f > sqrt(2) / 2
    vcmpgtpd ymm1, ymm14, [rel sqrt2by2]
    vaddpd ymm3, ymm13, [rel one]
    vaddpd ymm4, ymm14, ymm14
    vblendvpd ymm13, ymm13, ymm3, ymm1
    vblendvpd ymm14, ymm4, ymm14, ymm1

    ; f = a - 1
    vsubpd ymm14, ymm14, [rel one]

    ; x = f / (2 + f)
    vaddpd ymm1, ymm14, [rel two]
    vdivpd ymm1, ymm14, ymm1

    ; z15 = c15 * x^14 + ... + c5 * x^4 + c3 * x^2
    ; res = k * log10_2 + (f - s * (f - R)) * log10inv
    poly
    vsubpd ymm15, ymm14, ymm15
    vfnmadd213pd ymm15, ymm1, ymm14
    vmulpd ymm13, ymm13, [rel log10_2]
    vfmadd132pd ymm15, ymm13, [rel log10inv]
%endmacro ; }}}

%macro log1p 0 ; {{{ implicit input ymm0
    ; b = a + 1
    vaddpd ymm1, ymm0, [rel one]

    ; k = exponent(b)
    vpsrlq ymm13, ymm1, 52
    vpor ymm13, ymm13, [rel pow252]
    vsubpd ymm13, ymm13, [rel bias]

    ; 1 + f = 0.5 * fraction(b)
    vpand ymm14, ymm1, [rel fmask]
    vpor ymm14, ymm14, [rel emask]

    ; 1 + f > sqrt(2) / 2
    vcmpgtpd ymm1, ymm14, [rel sqrt2by2]
    vaddpd ymm3, ymm13, [rel one]
    vaddpd ymm4, ymm14, ymm14
    vblendvpd ymm13, ymm13, ymm3, ymm1
    vblendvpd ymm14, ymm4, ymm14, ymm1

    ; f = a - 1
    vsubpd ymm14, ymm14, [rel one]

    ; x = f / (2 + f)
    vaddpd ymm1, ymm14, [rel two]
    vdivpd ymm1, ymm14, ymm1

    ; z15 = c15 * x^14 + ... + c5 * x^4 + c3 * x^2
    ; res = k * log2hi - ((s * (f - R) - k * log2lo) - f)
    poly
    vsubpd ymm15, ymm14, ymm15
    vmulpd ymm2, ymm13, [rel log2lo]
    vfmsub213pd ymm15, ymm1, ymm2
    vsubpd ymm15, ymm15, ymm14
    vfmsub231pd ymm15, ymm13, [rel log2hi]
%endmacro ; }}}

%macro vd_log 2 ; {{{
    %1
    vcmpltpd ymm1, ymm0, [rel %{1}_min_a]
    vcmpgtpd ymm2, ymm0, [rel %{1}_max_a]
    vpor ymm5, ymm1, ymm2
    vcmpltpd ymm3, ymm0, [rel %{1}_nan_a]
    vpor ymm5, ymm5, ymm3
    vcmpneqpd ymm4, ymm0, ymm0
    vpor ymm5, ymm5, ymm4
    vtestpd ymm5, ymm5
    jz %%skip
    vblendvpd ymm15, ymm15, [rel %{1}_min_y], ymm1
    vblendvpd ymm15, ymm15, [rel %{1}_max_y], ymm2
    vblendvpd ymm15, ymm15, [rel %{1}_nan_y], ymm3
    vblendvpd ymm15, ymm15, ymm0, ymm4
    %%skip:
    vmovupd %2, ymm15
%endmacro ; }}}

%macro vd_log 1 ; rdi:n, rsi:a, rdx:y {{{
    cmp rdi, 4
    jl .last

    .loop:
        vmovupd ymm0, [rsi]
        vd_log %1, [rdx]
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
        vd_log %1, [rsp - 0x28]
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

one:   times 4 dq 0x3FF0000000000000 ; 1.0
two:   times 4 dq 0x4000000000000000 ; 2.0
pow252 times 4 dq 0x4330000000000000 ; 2^52
bias:  times 4 dq 0x43300000000003FF ; 2^52 + 1023.0
emask: times 4 dq 0x3FE0000000000000 ; exponent mask
fmask: times 4 dq 0x000FFFFFFFFFFFFF ; fraction mask

log2hi:   times 4 dq 0x3FE62E42FEE00000
log2lo:   times 4 dq 0x3DEA39EF35793C76
log2inv:  times 4 dq 0x3FF71547652B82FE ; 1.0l / log(2.0l)
log10_2:  times 4 dq 0x3FD34413509F79FF ; log10(2.0l)
log10inv: times 4 dq 0x3FDBCB7B1526E50E ; 1.0l / log(10.0l)
sqrt2by2: times 4 dq 0x3FE6A09E667F3BCD ; sqrt(2.0l) / 2.0l

c3:  times 4 dq 0x3FE5555555555593
c5:  times 4 dq 0x3FD999999997FA04
c7:  times 4 dq 0x3FD2492494229359
c9:  times 4 dq 0x3FCC71C51D8E78AF
c11: times 4 dq 0x3FC7466496CB03DE
c13: times 4 dq 0x3FC39A09D078C69F
c15: times 4 dq 0x3FC2F112DF3E5244

section .text

mckl_vd_log:   vd_log log
mckl_vd_log2:  vd_log log2
mckl_vd_log10: vd_log log10
mckl_vd_log1p: vd_log log1p

; vim:ft=nasm
