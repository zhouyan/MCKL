;;============================================================================
;; MCKL/lib/asm/fp_classify.asm
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

global mckl_vs_find_normal
global mckl_vs_find_subnormal
global mckl_vs_find_zero
global mckl_vs_find_inf
global mckl_vs_find_nan
global mckl_vs_find_finite

global mckl_vd_find_normal
global mckl_vd_find_subnormal
global mckl_vd_find_zero
global mckl_vd_find_inf
global mckl_vd_find_nan
global mckl_vd_find_finite

global mckl_vs_count_normal
global mckl_vs_count_subnormal
global mckl_vs_count_zero
global mckl_vs_count_inf
global mckl_vs_count_nan
global mckl_vs_count_finite

global mckl_vd_count_normal
global mckl_vd_count_subnormal
global mckl_vd_count_zero
global mckl_vd_count_inf
global mckl_vd_count_nan
global mckl_vd_count_finite

default rel

section .rodata

%macro is_vs_normal_constants 0
    vmovaps ymm5, [ssign]
    vmovaps ymm6, [smin]
    vmovaps ymm7, [smax]
%endmacro

%macro is_vs_normal 1
    vandps %1, %1, ymm5
    vcmpgeps ymm8, %1, ymm6
    vcmpleps ymm9, %1, ymm7
    vandps %1, ymm8, ymm9
%endmacro

%macro is_vs_subnormal_constants 0
    vmovaps ymm5, [ssign]
    vmovaps ymm6, [smin]
    vmovaps ymm7, [zero]
%endmacro

%macro is_vs_subnormal 1
    vandps %1, %1, ymm5
    vcmpltps ymm8, %1, ymm6
    vcmpgtps ymm9, %1, ymm7
    vandps %1, ymm8, ymm9
%endmacro

%macro is_vs_zero_constants 0
    vmovaps ymm5, [zero]
%endmacro

%macro is_vs_zero 1
    vcmpeqps %1, %1, ymm5
%endmacro

%macro is_vs_inf_constants 0
    vmovaps ymm5, [ssign]
    vmovaps ymm6, [sinf]
%endmacro

%macro is_vs_inf 1
    vandps %1, %1, ymm5
    vcmpeqps %1, %1, ymm6
%endmacro

%macro is_vs_nan_constants 0
%endmacro

%macro is_vs_nan 1
   vcmpneqps %1, %1, %1
%endmacro

%macro is_vs_finite_constants 0
    vmovaps ymm5, [ssign]
    vmovaps ymm6, [sinf]
%endmacro

%macro is_vs_finite 1
    vandps %1, %1, ymm5
    vcmpltps %1, %1, ymm6
%endmacro

%macro is_vd_normal_constants 0
    vmovapd ymm5, [dsign]
    vmovapd ymm6, [dmin]
    vmovapd ymm7, [dmax]
%endmacro

%macro is_vd_normal 1
    vandpd %1, %1, ymm5
    vcmpgepd ymm8, %1, ymm6
    vcmplepd ymm9, %1, ymm7
    vandpd %1, ymm8, ymm9
%endmacro

%macro is_vd_subnormal_constants 0
    vmovapd ymm5, [dsign]
    vmovapd ymm6, [dmin]
    vmovapd ymm7, [zero]
%endmacro

%macro is_vd_subnormal 1
    vandpd %1, %1, ymm5
    vcmpltpd ymm8, %1, ymm6
    vcmpgtpd ymm9, %1, ymm7
    vandpd %1, ymm8, ymm9
%endmacro

%macro is_vd_zero_constants 0
    vmovapd ymm5, [zero]
%endmacro

%macro is_vd_zero 1
    vcmpeqpd %1, %1, ymm5
%endmacro

%macro is_vd_inf_constants 0
    vmovapd ymm5, [dsign]
    vmovapd ymm6, [dinf]
%endmacro

%macro is_vd_inf 1
    vandpd %1, %1, ymm5
    vcmpeqpd %1, %1, ymm6
%endmacro

%macro is_vd_nan_constants 0
%endmacro

%macro is_vd_nan 1
   vcmpneqpd %1, %1, %1
%endmacro

%macro is_vd_finite_constants 0
    vmovapd ymm5, [dsign]
    vmovapd ymm6, [dinf]
%endmacro

%macro is_vd_finite 1
    vandpd %1, %1, ymm5
    vcmpltpd %1, %1, ymm6
%endmacro

; rdi:n
; rsi:a
; rax:retrun
%macro find_kernel 2 ; operand size, function
    xor rcx, rcx
    test rdi, rdi
    jz .return

    %{2}_constants

    mov rax, rdi
    and rax, (0x20 /  %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

align 16
.loop:
    vmovups ymm0, [rsi]
    %2 ymm0
%if %1 == 4
    vtestps ymm0, ymm0
%endif
%if %1 == 8
    vtestpd ymm0, ymm0
%endif
    jnz .index
    add rcx, 0x20 / %1
    add rsi, 0x20
    sub rdi, 0x20 / %1
    jnz .loop

.last:
    test rax, rax
    jz .return
    lea rdi, [mask%1]
    shl rax, 5
    add rax, rdi
    vmovdqa ymm1, [rax]
    vmaskmovps ymm0, ymm1, [rsi]
    %2 ymm0
    vandps ymm0, ymm0, ymm1
%if %1 == 4
    vtestps ymm0, ymm0
%endif
%if %1 == 8
    vtestpd ymm0, ymm0
%endif

.index:
    vmovdqu [rsp - 0x20], ymm0
    vzeroupper
%if %1 == 4
    mov eax, [rsp - 4 * 8]
    test eax, eax
    jnz .return
    inc rcx
    mov eax, [rsp - 4 * 7]
    test eax, eax
    jnz .return
    inc rcx
    mov eax, [rsp - 4 * 6]
    test eax, eax
    jnz .return
    inc rcx
    mov eax, [rsp - 4 * 5]
    test eax, eax
    jnz .return
    inc rcx
    mov eax, [rsp - 4 * 4]
    test eax, eax
    jnz .return
    inc rcx
    mov eax, [rsp - 4 * 3]
    test eax, eax
    jnz .return
    inc rcx
    mov eax, [rsp - 4 * 2]
    test eax, eax
    jnz .return
    inc rcx
    mov eax, [rsp - 4 * 1]
    test eax, eax
    jnz .return
    inc rcx
%endif
%if %1 == 8
    mov rax, [rsp - 8 * 4]
    test rax, rax
    jnz .return
    inc rcx
    mov rax, [rsp - 8 * 3]
    test rax, rax
    jnz .return
    inc rcx
    mov rax, [rsp - 8 * 2]
    test rax, rax
    jnz .return
    inc rcx
    mov rax, [rsp - 8 * 1]
    test rax, rax
    jnz .return
    inc rcx
%endif
.return:
    mov rax, rcx
    ret
%endmacro

; rdi:n
; rsi:a
; rax:retrun
%macro count_kernel 2 ; operand size, function
    vpxor ymm0, ymm0, ymm0
    test rdi, rdi
    jz .return

    %{2}_constants

%if %1 == 4
    vmovdqa ymm4, [mask32]
%endif

    mov rax, rdi
    and rax, (0x20 /  %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

align 16
.loop:
    vmovups ymm1, [rsi]
    %2 ymm1
%if %1 == 4
    vpsrld ymm2, ymm1, 31
    vpsrlq ymm1, ymm1, 63
    vpand  ymm2, ymm4
    vpaddq ymm0, ymm0, ymm1
    vpaddq ymm0, ymm0, ymm2
%endif
%if %1 == 8
    vpsrlq ymm1, ymm1, 63
    vpaddq ymm0, ymm0, ymm1
%endif
    add rsi, 0x20
    sub rdi, 0x20 / %1
    jnz .loop

.last:
    test rax, rax
    jz .return
    lea rdi, [mask%1]
    shl rax, 5
    add rax, rdi
    vmovdqa ymm2, [rax]
    vmaskmovps ymm1, ymm2, [rsi]
    %2 ymm1
    vpand ymm1, ymm1, ymm2
%if %1 == 4
    vpsrld ymm2, ymm1, 31
    vpsrlq ymm1, ymm1, 63
    vpand  ymm2, ymm2, ymm4
    vpaddq ymm0, ymm0, ymm1
    vpaddq ymm0, ymm0, ymm2
%endif
%if %1 == 8
    vpsrlq ymm1, ymm1, 63
    vpaddq ymm0, ymm0, ymm1
%endif

.return:
    vmovdqu [rsp - 0x20], ymm0
    vzeroupper
    xor rax, rax
    xor rcx, rcx
    add rax, [rsp - 8 * 1]
    add rcx, [rsp - 8 * 2]
    add rax, [rsp - 8 * 3]
    add rcx, [rsp - 8 * 4]
    add rax, rcx
    ret
%endmacro

align 32

zero: times 4 dq 0
ssign times 8 dd 0x7FFFFFFF
dsign times 4 dq 0x7FFFFFFFFFFFFFFF
sinf: times 8 dd 0x7F800000
dinf: times 4 dq 0x7FF0000000000000
smin: times 8 dd 0x00800000         ; FLT_MIN
dmin: times 4 dq 0x0010000000000000 ; DBL_MIN
smax: times 8 dd 0x7F7FFFFF         ; FLT_MAX
dmax: times 4 dq 0x7FEFFFFFFFFFFFFF ; DBL_MAX

mask4:
dd  0,  0,  0,  0,  0,  0,  0,  0
dd ~0,  0,  0,  0,  0,  0,  0,  0
dd ~0, ~0,  0,  0,  0,  0,  0,  0
dd ~0, ~0, ~0,  0,  0,  0,  0,  0
dd ~0, ~0, ~0, ~0,  0,  0,  0,  0
dd ~0, ~0, ~0, ~0, ~0,  0,  0,  0
dd ~0, ~0, ~0, ~0, ~0, ~0,  0,  0
dd ~0, ~0, ~0, ~0, ~0, ~0, ~0,  0

mask8:
dq  0,  0,  0,  0
dq ~0,  0,  0,  0
dq ~0, ~0,  0,  0
dq ~0, ~0, ~0,  0

mask32: times 4 dq 0x00000000FFFFFFFF

section .text

mckl_vs_find_normal:    find_kernel 4, is_vs_normal
mckl_vs_find_subnormal: find_kernel 4, is_vs_subnormal
mckl_vs_find_zero:      find_kernel 4, is_vs_zero
mckl_vs_find_inf:       find_kernel 4, is_vs_inf
mckl_vs_find_nan:       find_kernel 4, is_vs_nan
mckl_vs_find_finite:    find_kernel 4, is_vs_finite

mckl_vd_find_normal:    find_kernel 8, is_vd_normal
mckl_vd_find_subnormal: find_kernel 8, is_vd_subnormal
mckl_vd_find_zero:      find_kernel 8, is_vd_zero
mckl_vd_find_inf:       find_kernel 8, is_vd_inf
mckl_vd_find_nan:       find_kernel 8, is_vd_nan
mckl_vd_find_finite:    find_kernel 8, is_vd_finite

mckl_vs_count_normal:    count_kernel 4, is_vs_normal
mckl_vs_count_subnormal: count_kernel 4, is_vs_subnormal
mckl_vs_count_zero:      count_kernel 4, is_vs_zero
mckl_vs_count_inf:       count_kernel 4, is_vs_inf
mckl_vs_count_nan:       count_kernel 4, is_vs_nan
mckl_vs_count_finite:    count_kernel 4, is_vs_finite

mckl_vd_count_normal:    count_kernel 8, is_vd_normal
mckl_vd_count_subnormal: count_kernel 8, is_vd_subnormal
mckl_vd_count_zero:      count_kernel 8, is_vd_zero
mckl_vd_count_inf:       count_kernel 8, is_vd_inf
mckl_vd_count_nan:       count_kernel 8, is_vd_nan
mckl_vd_count_finite:    count_kernel 8, is_vd_finite

; vim:ft=nasm
