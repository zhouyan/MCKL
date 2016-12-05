;;============================================================================
;; MCKL/lib/asm/fma.asm
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

global mckl_fma_vvv_ps
global mckl_fma_vvs_ps
global mckl_fma_vsv_ps
global mckl_fma_svv_ps
global mckl_fma_ssv_ps
global mckl_fma_svs_ps
global mckl_fma_vss_ps

global mckl_fma_vvv_pd
global mckl_fma_vvs_pd
global mckl_fma_vsv_pd
global mckl_fma_svv_pd
global mckl_fma_ssv_pd
global mckl_fma_svs_pd
global mckl_fma_vss_pd

default rel

; rdi:n
; rsi:a
; rdx:b
; rcx:c
; r8:y
%macro fma_vvv 2 ; {{{
    test rdi, rdi
    jz .return

    mov rax, rdi
    and rax, (0x20 / %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

.loop: align 16
    vmovups ymm0, [rsi]
    vmovups ymm1, [rdx]
    vfmadd213p%2 ymm0, ymm1, [rcx]
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
    vfmadd213p%2 ymm0, ymm1, ymm2
    vmaskmovps [r8], ymm3, ymm0

.return:
    ret
%endmacro ; }}}

%macro fma2 6 ; {{{
    test rdi, rdi
    jz .return

    vbroadcasts%2 ymm0, xmm0

    mov rax, rdi
    and rax, (0x20 / %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

.loop: align 16
    vmovups ymm1, [rsi]
    vmovups ymm2, [rdx]
    vfmadd%{3}p%2 %4, %5, %6
    vmovups [rcx], %4
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
    vfmadd%{3}p%2 %4, %5, %6
    vmaskmovps [rcx], ymm3, %4

.return:
    ret
%endmacro ; }}}

; rdi:n
; rsi:a  -> ymm1
; rdx:b  -> ymm2
; xmm0:c -> ymm0
; rcx:y
%macro fma_vvs 2 ; {{{
    fma2 %1, %2, 213, ymm1, ymm2, ymm0
%endmacro ; }}}

; rdi:n
; rsi:a  -> ymm1
; xmm0:b -> ymm0
; rdx:c  -> ymm2
; rcx:y
%macro fma_vsv 2 ; {{{
    fma2 %1, %2, 213, ymm1, ymm0, ymm2
%endmacro ; }}}

; rdi:n
; xmm0:a -> ymm0
; rsi:b  -> ymm1
; rdx:c  -> ymm2
; rcx:y
%macro fma_svv 2 ; {{{
    fma2 %1, %2, 213, ymm1, ymm0, ymm2
%endmacro ; }}}

%macro fma3 6 ; {{{
    test rdi, rdi
    jz .return

    vbroadcasts%2 ymm0, xmm0
    vbroadcasts%2 ymm1, xmm1

    mov rax, rdi
    and rax, (0x20 / %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

.loop: align 16
    vmovups ymm2, [rsi]
    vfmadd%{3}p%2 %4, %5, %6
    vmovups [rdx], %4
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
    vfmadd%{3}p%2 %4, %5, %6
    vmaskmovps [rdx], ymm3, %4

.return:
    ret
%endmacro ; }}}

; rdi:n
; xmm0:a -> ymm0
; xmm1:b -> ymm1
; rsi:c  -> ymm2
; rdx:y
%macro fma_ssv 2 ; {{{
    fma3 %1, %2, 231, ymm2, ymm0, ymm1
%endmacro ; }}}

; rdi:n
; xmm0:a -> ymm0
; rsi:b  -> ymm2
; xmm1:c -> ymm1
; rdx:y
%macro fma_svs 2 ; {{{
    fma3 %1, %2, 213, ymm2, ymm0, ymm1
%endmacro ; }}}

; rdi:n
; rsi:a  -> ymm2
; xmm0:b -> ymm0
; xmm1:c -> ymm1
; rdx:y
%macro fma_vss 2 ; {{{
    fma3 %1, %2, 213, ymm2, ymm0, ymm1
%endmacro ; }}}

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

mckl_fma_vvv_ps: fma_vvv 4, s
mckl_fma_vvs_ps: fma_vvs 4, s
mckl_fma_vsv_ps: fma_vsv 4, s
mckl_fma_svv_ps: fma_svv 4, s
mckl_fma_ssv_ps: fma_ssv 4, s
mckl_fma_svs_ps: fma_svs 4, s
mckl_fma_vss_ps: fma_vss 4, s

mckl_fma_vvv_pd: fma_vvv 8, d
mckl_fma_vvs_pd: fma_vvs 8, d
mckl_fma_vsv_pd: fma_vsv 8, d
mckl_fma_svv_pd: fma_svv 8, d
mckl_fma_ssv_pd: fma_ssv 8, d
mckl_fma_svs_pd: fma_svs 8, d
mckl_fma_vss_pd: fma_vss 8, d

; vim:ft=nasm
