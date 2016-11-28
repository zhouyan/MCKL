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

; register used as constants: ymm6, ymm8-10, ymm12
; register used as variables: ymm1-5, ymm7, ymm11, ymm13-15

; log(1 + f) * (f + 2) / f - 2 = c15 * x^14 + ... + c5 * x^4 + c3 * x^2
%macro log1pf 0 ; implicity input ymm1, output ymm15 {{{
    ; k = exponent(b)
    vpsrlq ymm13, ymm1, 52
    vpor ymm13, ymm13, [rel pow252]
    vsubpd ymm13, ymm13, [rel bias]

    ; 1 + f = 0.5 * fraction(b)
    vpand ymm14, ymm1, [rel fmask]
    vpor ymm14, ymm14, [rel emask]

    ; 1 + f > sqrt(2) / 2
    vcmpgtpd ymm1, ymm14, ymm6
    vaddpd ymm3, ymm13, ymm8
    vaddpd ymm4, ymm14, ymm14
    vblendvpd ymm13, ymm13, ymm3, ymm1
    vblendvpd ymm14, ymm4, ymm14, ymm1

    ; f = a - 1
    vsubpd ymm14, ymm14, ymm8

    ; x = f / (f + 2)
    vaddpd ymm1, ymm14, ymm9
    vdivpd ymm1, ymm14, ymm1

    vmovapd ymm15, [rel c15]
    vmovapd ymm11, [rel c11]
    vmovapd ymm7,  [rel c7]

    vmulpd ymm2, ymm1, ymm1 ; x^2
    vmulpd ymm4, ymm2, ymm2 ; x^4

    vfmadd213pd ymm15, ymm2, [rel c13] ; u15 = c15 * x^2 + c13
    vfmadd213pd ymm11, ymm2, [rel c9]  ; u11 = c11 * x^2 + c9
    vfmadd213pd ymm7,  ymm2, [rel c5]  ; u7  = c7  * x^2 + c5
    vmulpd      ymm3,  ymm2, [rel c3]  ; u3  = c3  * x^2

    vfmadd213pd ymm15, ymm4, ymm11 ; v15 = u15 * x^4 + u11
    vfmadd213pd ymm7,  ymm4, ymm3  ; v7  = u7  * x^4 + u3

    vmulpd ymm4, ymm4, ymm4
    vfmadd213pd ymm15, ymm4, ymm7 ; z15 = v15 * x^8 + v7
%endmacro ; }}}

%macro log_constants 0 ; {{{
    vmovapd ymm10, [rel log2lo]
    vmovapd ymm12, [rel log2hi]
%endmacro ; }}}

%macro log_compute 0 ; implicit input ymm0, output ymm15 {{{
    vmovapd ymm1, ymm0

    log1pf ; R = log(1 + f)

    ; log(a) = k * log2 + log(1 + f)
    ;        = k * log2hi - ((x * (f - R) - k * log2lo) - f)
    vsubpd ymm15, ymm14, ymm15
    vmulpd ymm2, ymm13, ymm10
    vfmsub213pd ymm15, ymm1, ymm2
    vsubpd ymm15, ymm15, ymm14
    vfmsub231pd ymm15, ymm13, ymm12
%endmacro ; }}}

%macro log2_constants 0 ; {{{
    vmovapd ymm10, [rel log2inv]
%endmacro ; }}}

%macro log2_compute 0 ; implicit input ymm0, output ymm15 {{{
    vmovapd ymm1, ymm0

    log1pf; R = log(1 + f)

    ; log2(a) = k + log(1 + f) / log(2) = k + (f - x * (f - R)) * log2inv
    vsubpd ymm15, ymm14, ymm15
    vfnmadd213pd ymm15, ymm1, ymm14
    vfmadd132pd ymm15, ymm13, ymm10
%endmacro ; }}}

%macro log10_constants 0 ; {{{
    vmovapd ymm10, [rel log10_2]
    vmovapd ymm12, [rel log10inv]
%endmacro ; }}}

%macro log10_compute 0 ; implicit input ymm0, output ymm15 {{{
    vmovapd ymm1, ymm0

    log1pf; R = log(1 + f)

    ; log10 = k * log(10) / log(2) + log(1 + f) / log(1)
    ;       = k * log10_2 + (f - x * (f - R)) * log10inv
    vsubpd ymm15, ymm14, ymm15
    vfnmadd213pd ymm15, ymm1, ymm14
    vmulpd ymm13, ymm13, ymm10
    vfmadd132pd ymm15, ymm13, ymm12
%endmacro ; }}}

%macro log1p_constants 0 ; {{{
    vmovapd ymm10, [rel log2lo]
    vmovapd ymm12, [rel log2hi]
%endmacro ; }}}

%macro log1p_compute 0 ; implicit input ymm0, output ymm15 {{{
    ; b = a + 1
    vaddpd ymm1, ymm0, ymm8

    log1pf; R = log(1 + f)

    ; log(1 + a) = k * log2 + log(1 + f)
    ;            = k * log2hi - ((s * (f - R) - k * log2lo) - f)
    vsubpd ymm15, ymm14, ymm15
    vmulpd ymm2, ymm13, ymm10
    vfmsub213pd ymm15, ymm1, ymm2
    vsubpd ymm15, ymm15, ymm14
    vfmsub231pd ymm15, ymm13, ymm12
%endmacro ; }}}

%macro select 1 ; implicit input ymm0, ymm15, output ymm15 {{{
    vcmpltpd ymm1, ymm0, [rel %{1}_min_a] ; a < min_a
    vcmpgtpd ymm2, ymm0, [rel %{1}_max_a] ; a > max_a
    vcmpltpd ymm3, ymm0, [rel %{1}_nan_a] ; a < nan_a
    vcmpneqpd ymm4, ymm0, ymm0            ; a != a
    vpor ymm5, ymm1, ymm2
    vpor ymm5, ymm5, ymm3
    vpor ymm5, ymm5, ymm4
    vtestpd ymm5, ymm5
    jz %%skip
    vblendvpd ymm15, ymm15, [rel %{1}_min_y], ymm1 ; min_y
    vblendvpd ymm15, ymm15, [rel %{1}_max_y], ymm2 ; max_y
    vblendvpd ymm15, ymm15, [rel %{1}_nan_y], ymm3 ; nan_y
    vblendvpd ymm15, ymm15, ymm0, ymm4             ; a
%%skip:
%endmacro ; }}}

; rdi:n
; rsi:a
; rdx:y
%macro kernel 1 ; {{{
    push rbp
    mov rbp, rsp
    sub rsp, 0x20
    cld

    test rdi, rdi
    jz .return

    mov rax, rdi
    mov r8,  rsi
    mov r9,  rax

    shr rax, 2
    and r9,  0x3

    vmovapd ymm6,  [rel sqrt2by2]
    vmovapd ymm8,  [rel one]
    vmovapd ymm9,  [rel two]
    %{1}_constants

    test rax, rax
    jz .last

.loop: align 16
    vmovupd ymm0, [r8]
    %{1}_compute
    select %1
    vmovupd [rdx], ymm15
    add r8,  0x20
    add rdx, 0x20
    dec rax
    jnz .loop

.last:
    test r9, r9
    jz .return
    mov rcx, r9
    mov rsi, r8
    mov rdi, rsp
    rep movsq
    vmovupd ymm0, [rsp]
    %{1}_compute
    select %1
    vmovupd [rsp], ymm15
    mov rcx, r9
    mov rsi, rsp
    mov rdi, rdx
    rep movsq

.return:
    mov rsp, rbp
    pop rbp
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

log1p_min_a: times 4 dq 0xBFF0000000000000 ; -1.0
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

pow252: times 4 dq 0x4330000000000000 ; 2^52
bias:   times 4 dq 0x43300000000003FF ; 2^52 + 1023.0
fmask:  times 4 dq 0x000FFFFFFFFFFFFF ; fraction mask
emask:  times 4 dq 0x3FE0000000000000 ; exponent mask

one:      times 4 dq 0x3FF0000000000000 ; 1.0
two:      times 4 dq 0x4000000000000000 ; 2.0
log2hi:   times 4 dq 0x3FE62E42FEE00000
log2lo:   times 4 dq 0x3DEA39EF35793C76
log2inv:  times 4 dq 0x3FF71547652B82FE ; 1.0l / log(2.0l)
log10_2:  times 4 dq 0x3FD34413509F79FF ; log10(2.0l)
log10inv: times 4 dq 0x3FDBCB7B1526E50E ; 1.0l / log(10.0l)
sqrt2by2: times 4 dq 0x3FE6A09E667F3BCD ; sqrt(2.0l) / 2.0l

section .text

mckl_vd_log:   kernel log
mckl_vd_log2:  kernel log2
mckl_vd_log10: kernel log10
mckl_vd_log1p: kernel log1p

; vim:ft=nasm
