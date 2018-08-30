;;============================================================================
;; MCKL/lib/asm/fma.asm
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

global mckl_fmadd512_vvv_ps
global mckl_fmadd512_vvs_ps
global mckl_fmadd512_vsv_ps
global mckl_fmadd512_svv_ps
global mckl_fmadd512_ssv_ps
global mckl_fmadd512_svs_ps
global mckl_fmadd512_vss_ps
global mckl_fmadd512_vvv_pd
global mckl_fmadd512_vvs_pd
global mckl_fmadd512_vsv_pd
global mckl_fmadd512_svv_pd
global mckl_fmadd512_ssv_pd
global mckl_fmadd512_svs_pd
global mckl_fmadd512_vss_pd

global mckl_fmsub512_vvv_ps
global mckl_fmsub512_vvs_ps
global mckl_fmsub512_vsv_ps
global mckl_fmsub512_svv_ps
global mckl_fmsub512_ssv_ps
global mckl_fmsub512_svs_ps
global mckl_fmsub512_vss_ps
global mckl_fmsub512_vvv_pd
global mckl_fmsub512_vvs_pd
global mckl_fmsub512_vsv_pd
global mckl_fmsub512_svv_pd
global mckl_fmsub512_ssv_pd
global mckl_fmsub512_svs_pd
global mckl_fmsub512_vss_pd

global mckl_fnmadd512_vvv_ps
global mckl_fnmadd512_vvs_ps
global mckl_fnmadd512_vsv_ps
global mckl_fnmadd512_svv_ps
global mckl_fnmadd512_ssv_ps
global mckl_fnmadd512_svs_ps
global mckl_fnmadd512_vss_ps
global mckl_fnmadd512_vvv_pd
global mckl_fnmadd512_vvs_pd
global mckl_fnmadd512_vsv_pd
global mckl_fnmadd512_svv_pd
global mckl_fnmadd512_ssv_pd
global mckl_fnmadd512_svs_pd
global mckl_fnmadd512_vss_pd

global mckl_fnmsub512_vvv_ps
global mckl_fnmsub512_vvs_ps
global mckl_fnmsub512_vsv_ps
global mckl_fnmsub512_svv_ps
global mckl_fnmsub512_ssv_ps
global mckl_fnmsub512_svs_ps
global mckl_fnmsub512_vss_ps
global mckl_fnmsub512_vvv_pd
global mckl_fnmsub512_vvs_pd
global mckl_fnmsub512_vsv_pd
global mckl_fnmsub512_svv_pd
global mckl_fnmsub512_ssv_pd
global mckl_fnmsub512_svs_pd
global mckl_fnmsub512_vss_pd

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
    and rax, (0x40 / %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

align 16
.loop:
    vmovups zmm0, [rsi]
    vmovups zmm1, [rdx]
    v%{3}213p%2 zmm0, zmm1, [rcx]
    vmovups [r8], zmm0
    add rsi, 0x40
    add rdx, 0x40
    add rcx, 0x40
    add r8,  0x40
    sub rdi, 0x40 / %1
    jnz .loop

.last:
    test rax, rax
    jz .return
    vmovaps zmm4, [mask%1]
%if %1 == 4
    vpbroadcastd zmm3, eax
    vpcmpd k1, zmm4, zmm3, 1 ; _MM_CMPINT_LT
%elif %1 == 8
    vpbroadcastq zmm3, rax
    vpcmpq k1, zmm4, zmm3, 1 ; _MM_CMPINT_LT
%else
    %error
%endif
    vmovap%2 zmm0{k1}, [rsi]
    vmovap%2 zmm1{k1}, [rdx]
    vmovap%2 zmm2{k1}, [rcx]
    v%{3}213p%2 zmm0, zmm1, zmm2
    vmovap%2 [r8]{k1}, zmm0

.return:
    ret
%endmacro

%macro fma2 7
    test rdi, rdi
    jz .return

    vbroadcasts%2 zmm0, xmm0

    mov rax, rdi
    and rax, (0x40 / %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

align 16
.loop:
    vmovups zmm1, [rsi]
    vmovups zmm2, [rdx]
    v%{3}%{4}p%2 %5, %6, %7
    vmovups [rcx], %5
    add rsi, 0x40
    add rdx, 0x40
    add rcx, 0x40
    sub rdi, 0x40 / %1
    jnz .loop

.last:
    test rax, rax
    jz .return
    vmovaps zmm4, [mask%1]
%if %1 == 4
    vpbroadcastd zmm3, eax
    vpcmpd k1, zmm4, zmm3, 1 ; _MM_CMPINT_LT
%elif %1 == 8
    vpbroadcastq zmm3, rax
    vpcmpq k1, zmm4, zmm3, 1 ; _MM_CMPINT_LT
%else
    %error
%endif
    vmovap%2 zmm1{k1}, [rsi]
    vmovap%2 zmm2{k1}, [rdx]
    v%{3}%{4}p%2 %5, %6, %7
    vmovap%2 [rcx]{k1}, %5

.return:
    ret
%endmacro

; rdi:n
; rsi:a  -> zmm1
; rdx:b  -> zmm2
; xmm0:c -> zmm0
; rcx:y
%macro fma_vvs 3
    fma2 %1, %2, %3, 213, zmm1, zmm2, zmm0
%endmacro

; rdi:n
; rsi:a  -> zmm1
; xmm0:b -> zmm0
; rdx:c  -> zmm2
; rcx:y
%macro fma_vsv 3
    fma2 %1, %2, %3, 213, zmm1, zmm0, zmm2
%endmacro

; rdi:n
; xmm0:a -> zmm0
; rsi:b  -> zmm1
; rdx:c  -> zmm2
; rcx:y
%macro fma_svv 3
    fma2 %1, %2, %3, 213, zmm1, zmm0, zmm2
%endmacro

%macro fma3 7
    test rdi, rdi
    jz .return

    vbroadcasts%2 zmm0, xmm0
    vbroadcasts%2 zmm1, xmm1

    mov rax, rdi
    and rax, (0x40 / %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

align 16
.loop:
    vmovups zmm2, [rsi]
    v%{3}%{4}p%2 %5, %6, %7
    vmovups [rdx], %5
    add rsi, 0x40
    add rdx, 0x40
    sub rdi, 0x40 / %1
    jnz .loop

.last:
    test rax, rax
    jz .return
    vmovaps zmm4, [mask%1]
%if %1 == 4
    vpbroadcastd zmm3, eax
    vpcmpd k1, zmm4, zmm3, 1 ; _MM_CMPINT_LT
%elif %1 == 8
    vpbroadcastq zmm3, rax
    vpcmpq k1, zmm4, zmm3, 1 ; _MM_CMPINT_LT
%else
    %error
%endif
    vmovap%2 zmm2{k1}, [rsi]
    v%{3}%{4}p%2 %5, %6, %7
    vmovap%2 [rdx]{k1}, %5

.return:
    ret
%endmacro

; rdi:n
; xmm0:a -> zmm0
; xmm1:b -> zmm1
; rsi:c  -> zmm2
; rdx:y
%macro fma_ssv 3
    fma3 %1, %2, %3, 231, zmm2, zmm0, zmm1
%endmacro

; rdi:n
; xmm0:a -> zmm0
; rsi:b  -> zmm2
; xmm1:c -> zmm1
; rdx:y
%macro fma_svs 3
    fma3 %1, %2, %3, 213, zmm2, zmm0, zmm1
%endmacro

; rdi:n
; rsi:a  -> zmm2
; xmm0:b -> zmm0
; xmm1:c -> zmm1
; rdx:y
%macro fma_vss 3
    fma3 %1, %2, %3, 213, zmm2, zmm0, zmm1
%endmacro

section .rodata

align 64

mask4:
dd 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7
dd 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF

mask8:
dq 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7

section .text

mckl_fmadd512_vvv_ps: fma_vvv 4, s, fmadd
mckl_fmadd512_vvs_ps: fma_vvs 4, s, fmadd
mckl_fmadd512_vsv_ps: fma_vsv 4, s, fmadd
mckl_fmadd512_svv_ps: fma_svv 4, s, fmadd
mckl_fmadd512_ssv_ps: fma_ssv 4, s, fmadd
mckl_fmadd512_svs_ps: fma_svs 4, s, fmadd
mckl_fmadd512_vss_ps: fma_vss 4, s, fmadd
mckl_fmadd512_vvv_pd: fma_vvv 8, d, fmadd
mckl_fmadd512_vvs_pd: fma_vvs 8, d, fmadd
mckl_fmadd512_vsv_pd: fma_vsv 8, d, fmadd
mckl_fmadd512_svv_pd: fma_svv 8, d, fmadd
mckl_fmadd512_ssv_pd: fma_ssv 8, d, fmadd
mckl_fmadd512_svs_pd: fma_svs 8, d, fmadd
mckl_fmadd512_vss_pd: fma_vss 8, d, fmadd

mckl_fmsub512_vvv_ps: fma_vvv 4, s, fmsub
mckl_fmsub512_vvs_ps: fma_vvs 4, s, fmsub
mckl_fmsub512_vsv_ps: fma_vsv 4, s, fmsub
mckl_fmsub512_svv_ps: fma_svv 4, s, fmsub
mckl_fmsub512_ssv_ps: fma_ssv 4, s, fmsub
mckl_fmsub512_svs_ps: fma_svs 4, s, fmsub
mckl_fmsub512_vss_ps: fma_vss 4, s, fmsub
mckl_fmsub512_vvv_pd: fma_vvv 8, d, fmsub
mckl_fmsub512_vvs_pd: fma_vvs 8, d, fmsub
mckl_fmsub512_vsv_pd: fma_vsv 8, d, fmsub
mckl_fmsub512_svv_pd: fma_svv 8, d, fmsub
mckl_fmsub512_ssv_pd: fma_ssv 8, d, fmsub
mckl_fmsub512_svs_pd: fma_svs 8, d, fmsub
mckl_fmsub512_vss_pd: fma_vss 8, d, fmsub

mckl_fnmadd512_vvv_ps: fma_vvv 4, s, fnmadd
mckl_fnmadd512_vvs_ps: fma_vvs 4, s, fnmadd
mckl_fnmadd512_vsv_ps: fma_vsv 4, s, fnmadd
mckl_fnmadd512_svv_ps: fma_svv 4, s, fnmadd
mckl_fnmadd512_ssv_ps: fma_ssv 4, s, fnmadd
mckl_fnmadd512_svs_ps: fma_svs 4, s, fnmadd
mckl_fnmadd512_vss_ps: fma_vss 4, s, fnmadd
mckl_fnmadd512_vvv_pd: fma_vvv 8, d, fnmadd
mckl_fnmadd512_vvs_pd: fma_vvs 8, d, fnmadd
mckl_fnmadd512_vsv_pd: fma_vsv 8, d, fnmadd
mckl_fnmadd512_svv_pd: fma_svv 8, d, fnmadd
mckl_fnmadd512_ssv_pd: fma_ssv 8, d, fnmadd
mckl_fnmadd512_svs_pd: fma_svs 8, d, fnmadd
mckl_fnmadd512_vss_pd: fma_vss 8, d, fnmadd

mckl_fnmsub512_vvv_ps: fma_vvv 4, s, fnmsub
mckl_fnmsub512_vvs_ps: fma_vvs 4, s, fnmsub
mckl_fnmsub512_vsv_ps: fma_vsv 4, s, fnmsub
mckl_fnmsub512_svv_ps: fma_svv 4, s, fnmsub
mckl_fnmsub512_ssv_ps: fma_ssv 4, s, fnmsub
mckl_fnmsub512_svs_ps: fma_svs 4, s, fnmsub
mckl_fnmsub512_vss_ps: fma_vss 4, s, fnmsub
mckl_fnmsub512_vvv_pd: fma_vvv 8, d, fnmsub
mckl_fnmsub512_vvs_pd: fma_vvs 8, d, fnmsub
mckl_fnmsub512_vsv_pd: fma_vsv 8, d, fnmsub
mckl_fnmsub512_svv_pd: fma_svv 8, d, fnmsub
mckl_fnmsub512_ssv_pd: fma_ssv 8, d, fnmsub
mckl_fnmsub512_svs_pd: fma_svs 8, d, fnmsub
mckl_fnmsub512_vss_pd: fma_vss 8, d, fnmsub

; vim:ft=nasm
