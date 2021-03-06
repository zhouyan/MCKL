;;============================================================================
;; MCKL/lib/asm/fma.asm
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

global mckl_fmadd_vvv_ps
global mckl_fmadd_vvs_ps
global mckl_fmadd_vsv_ps
global mckl_fmadd_svv_ps
global mckl_fmadd_ssv_ps
global mckl_fmadd_svs_ps
global mckl_fmadd_vss_ps
global mckl_fmadd_vvv_pd
global mckl_fmadd_vvs_pd
global mckl_fmadd_vsv_pd
global mckl_fmadd_svv_pd
global mckl_fmadd_ssv_pd
global mckl_fmadd_svs_pd
global mckl_fmadd_vss_pd

global mckl_fmsub_vvv_ps
global mckl_fmsub_vvs_ps
global mckl_fmsub_vsv_ps
global mckl_fmsub_svv_ps
global mckl_fmsub_ssv_ps
global mckl_fmsub_svs_ps
global mckl_fmsub_vss_ps
global mckl_fmsub_vvv_pd
global mckl_fmsub_vvs_pd
global mckl_fmsub_vsv_pd
global mckl_fmsub_svv_pd
global mckl_fmsub_ssv_pd
global mckl_fmsub_svs_pd
global mckl_fmsub_vss_pd

global mckl_fnmadd_vvv_ps
global mckl_fnmadd_vvs_ps
global mckl_fnmadd_vsv_ps
global mckl_fnmadd_svv_ps
global mckl_fnmadd_ssv_ps
global mckl_fnmadd_svs_ps
global mckl_fnmadd_vss_ps
global mckl_fnmadd_vvv_pd
global mckl_fnmadd_vvs_pd
global mckl_fnmadd_vsv_pd
global mckl_fnmadd_svv_pd
global mckl_fnmadd_ssv_pd
global mckl_fnmadd_svs_pd
global mckl_fnmadd_vss_pd

global mckl_fnmsub_vvv_ps
global mckl_fnmsub_vvs_ps
global mckl_fnmsub_vsv_ps
global mckl_fnmsub_svv_ps
global mckl_fnmsub_ssv_ps
global mckl_fnmsub_svs_ps
global mckl_fnmsub_vss_ps
global mckl_fnmsub_vvv_pd
global mckl_fnmsub_vvs_pd
global mckl_fnmsub_vsv_pd
global mckl_fnmsub_svv_pd
global mckl_fnmsub_ssv_pd
global mckl_fnmsub_svs_pd
global mckl_fnmsub_vss_pd

default rel

; rdi:n
; rsi:a
; rdx:b
; rcx:c
; r8:y
%macro fma_vvv 3
    test rdi, rdi
    jz .return

    mov rax, rdi
    and rax, (0x20 / %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

align 16
.loop:
    vmovups ymm0, [rsi]
    vmovups ymm1, [rdx]
    v%{3}213p%2 ymm0, ymm1, [rcx]
    vmovups [r8], ymm0
    add rsi, 0x20
    add rdx, 0x20
    add rcx, 0x20
    add r8,  0x20
    sub rdi, 0x20 / %1
    jnz .loop

.last:
    test rax, rax
    jz .return
    lea rdi, [mask%1]
    shl rax, 5
    add rax, rdi
    vmovaps ymm3, [rax]
    vmaskmovps ymm0, ymm3, [rsi]
    vmaskmovps ymm1, ymm3, [rdx]
    vmaskmovps ymm2, ymm3, [rcx]
    v%{3}213p%2 ymm0, ymm1, ymm2
    vmaskmovps [r8], ymm3, ymm0

.return:
    ret
%endmacro

%macro fma2 7
    test rdi, rdi
    jz .return

    vbroadcasts%2 ymm0, xmm0

    mov rax, rdi
    and rax, (0x20 / %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

align 16
.loop:
    vmovups ymm1, [rsi]
    vmovups ymm2, [rdx]
    v%{3}%{4}p%2 %5, %6, %7
    vmovups [rcx], %5
    add rsi, 0x20
    add rdx, 0x20
    add rcx, 0x20
    sub rdi, 0x20 / %1
    jnz .loop

.last:
    test rax, rax
    jz .return
    lea rdi, [mask%1]
    shl rax, 5
    add rax, rdi
    vmovaps ymm3, [rax]
    vmaskmovps ymm1, ymm3, [rsi]
    vmaskmovps ymm2, ymm3, [rdx]
    v%{3}%{4}p%2 %5, %6, %7
    vmaskmovps [rcx], ymm3, %5

.return:
    ret
%endmacro

; rdi:n
; rsi:a  -> ymm1
; rdx:b  -> ymm2
; xmm0:c -> ymm0
; rcx:y
%macro fma_vvs 3
    fma2 %1, %2, %3, 213, ymm1, ymm2, ymm0
%endmacro

; rdi:n
; rsi:a  -> ymm1
; xmm0:b -> ymm0
; rdx:c  -> ymm2
; rcx:y
%macro fma_vsv 3
    fma2 %1, %2, %3, 213, ymm1, ymm0, ymm2
%endmacro

; rdi:n
; xmm0:a -> ymm0
; rsi:b  -> ymm1
; rdx:c  -> ymm2
; rcx:y
%macro fma_svv 3
    fma2 %1, %2, %3, 213, ymm1, ymm0, ymm2
%endmacro

%macro fma3 7
    test rdi, rdi
    jz .return

    vbroadcasts%2 ymm0, xmm0
    vbroadcasts%2 ymm1, xmm1

    mov rax, rdi
    and rax, (0x20 / %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

align 16
.loop:
    vmovups ymm2, [rsi]
    v%{3}%{4}p%2 %5, %6, %7
    vmovups [rdx], %5
    add rsi, 0x20
    add rdx, 0x20
    sub rdi, 0x20 / %1
    jnz .loop

.last:
    test rax, rax
    jz .return
    lea rdi, [mask%1]
    shl rax, 5
    add rax, rdi
    vmovaps ymm3, [rax]
    vmaskmovps ymm2, ymm3, [rsi]
    v%{3}%{4}p%2 %5, %6, %7
    vmaskmovps [rdx], ymm3, %5

.return:
    ret
%endmacro

; rdi:n
; xmm0:a -> ymm0
; xmm1:b -> ymm1
; rsi:c  -> ymm2
; rdx:y
%macro fma_ssv 3
    fma3 %1, %2, %3, 231, ymm2, ymm0, ymm1
%endmacro

; rdi:n
; xmm0:a -> ymm0
; rsi:b  -> ymm2
; xmm1:c -> ymm1
; rdx:y
%macro fma_svs 3
    fma3 %1, %2, %3, 213, ymm2, ymm0, ymm1
%endmacro

; rdi:n
; rsi:a  -> ymm2
; xmm0:b -> ymm0
; xmm1:c -> ymm1
; rdx:y
%macro fma_vss 3
    fma3 %1, %2, %3, 213, ymm2, ymm0, ymm1
%endmacro

section .rodata

align 32

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

section .text

mckl_fmadd_vvv_ps: fma_vvv 4, s, fmadd
mckl_fmadd_vvs_ps: fma_vvs 4, s, fmadd
mckl_fmadd_vsv_ps: fma_vsv 4, s, fmadd
mckl_fmadd_svv_ps: fma_svv 4, s, fmadd
mckl_fmadd_ssv_ps: fma_ssv 4, s, fmadd
mckl_fmadd_svs_ps: fma_svs 4, s, fmadd
mckl_fmadd_vss_ps: fma_vss 4, s, fmadd
mckl_fmadd_vvv_pd: fma_vvv 8, d, fmadd
mckl_fmadd_vvs_pd: fma_vvs 8, d, fmadd
mckl_fmadd_vsv_pd: fma_vsv 8, d, fmadd
mckl_fmadd_svv_pd: fma_svv 8, d, fmadd
mckl_fmadd_ssv_pd: fma_ssv 8, d, fmadd
mckl_fmadd_svs_pd: fma_svs 8, d, fmadd
mckl_fmadd_vss_pd: fma_vss 8, d, fmadd

mckl_fmsub_vvv_ps: fma_vvv 4, s, fmsub
mckl_fmsub_vvs_ps: fma_vvs 4, s, fmsub
mckl_fmsub_vsv_ps: fma_vsv 4, s, fmsub
mckl_fmsub_svv_ps: fma_svv 4, s, fmsub
mckl_fmsub_ssv_ps: fma_ssv 4, s, fmsub
mckl_fmsub_svs_ps: fma_svs 4, s, fmsub
mckl_fmsub_vss_ps: fma_vss 4, s, fmsub
mckl_fmsub_vvv_pd: fma_vvv 8, d, fmsub
mckl_fmsub_vvs_pd: fma_vvs 8, d, fmsub
mckl_fmsub_vsv_pd: fma_vsv 8, d, fmsub
mckl_fmsub_svv_pd: fma_svv 8, d, fmsub
mckl_fmsub_ssv_pd: fma_ssv 8, d, fmsub
mckl_fmsub_svs_pd: fma_svs 8, d, fmsub
mckl_fmsub_vss_pd: fma_vss 8, d, fmsub

mckl_fnmadd_vvv_ps: fma_vvv 4, s, fnmadd
mckl_fnmadd_vvs_ps: fma_vvs 4, s, fnmadd
mckl_fnmadd_vsv_ps: fma_vsv 4, s, fnmadd
mckl_fnmadd_svv_ps: fma_svv 4, s, fnmadd
mckl_fnmadd_ssv_ps: fma_ssv 4, s, fnmadd
mckl_fnmadd_svs_ps: fma_svs 4, s, fnmadd
mckl_fnmadd_vss_ps: fma_vss 4, s, fnmadd
mckl_fnmadd_vvv_pd: fma_vvv 8, d, fnmadd
mckl_fnmadd_vvs_pd: fma_vvs 8, d, fnmadd
mckl_fnmadd_vsv_pd: fma_vsv 8, d, fnmadd
mckl_fnmadd_svv_pd: fma_svv 8, d, fnmadd
mckl_fnmadd_ssv_pd: fma_ssv 8, d, fnmadd
mckl_fnmadd_svs_pd: fma_svs 8, d, fnmadd
mckl_fnmadd_vss_pd: fma_vss 8, d, fnmadd

mckl_fnmsub_vvv_ps: fma_vvv 4, s, fnmsub
mckl_fnmsub_vvs_ps: fma_vvs 4, s, fnmsub
mckl_fnmsub_vsv_ps: fma_vsv 4, s, fnmsub
mckl_fnmsub_svv_ps: fma_svv 4, s, fnmsub
mckl_fnmsub_ssv_ps: fma_ssv 4, s, fnmsub
mckl_fnmsub_svs_ps: fma_svs 4, s, fnmsub
mckl_fnmsub_vss_ps: fma_vss 4, s, fnmsub
mckl_fnmsub_vvv_pd: fma_vvv 8, d, fnmsub
mckl_fnmsub_vvs_pd: fma_vvs 8, d, fnmsub
mckl_fnmsub_vsv_pd: fma_vsv 8, d, fnmsub
mckl_fnmsub_svv_pd: fma_svv 8, d, fnmsub
mckl_fnmsub_ssv_pd: fma_ssv 8, d, fnmsub
mckl_fnmsub_svs_pd: fma_svs 8, d, fnmsub
mckl_fnmsub_vss_pd: fma_vss 8, d, fnmsub

; vim:ft=nasm
