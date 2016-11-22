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

%macro partial_load 4 ; {{{
    mov rcx, %4
    mov rsi, %3
    mov rdi, rsp
    sub rdi, 0x28
    cld
    %if %1 == 4
        rep movsd
    %elif %1 == 8
        rep movsq
    %else
        %error
    %endif
    vmovups %2, [rsp - 0x28]
%endmacro ; }}}

%macro partial_store 4 ; {{{
    vmovups [rsp - 0x28], %3
    mov rcx, %4
    mov rsi, rsp
    sub rsi, 0x28
    mov rdi, %2
    cld
    %if %1 == 4
        rep movsd
    %elif %1 == 8
        rep movsq
    %else
        %error
    %endif
%endmacro ; }}}

; rdi n
; rsi a
; rdx b
; rcx c
; r8  y
%macro fma_vvv 2 ; {{{
    cmp rdi, 0x20 / %1
    jl .last

    .loop:
        vmovups ymm2, [rsi]
        vmovups ymm1, [rdx]
        vfmadd213p%2 ymm2, ymm1, [rcx]
        vmovups [r8], ymm2
        add rsi, 0x20
        add rdx, 0x20
        add rcx, 0x20
        add r8,  0x20
        sub rdi, 0x20 / %1
        cmp rdi, 0x20 / %1
        jge .loop

    .last:
        test rdi, rdi
        jz .return
        mov rax, rdi ; n
        mov r9,  rsi ; a
        mov r10, rcx ; c
        partial_load %1, ymm2, r9,  rax
        partial_load %1, ymm1, rdx, rax
        partial_load %1, ymm3, r10, rax
        vfmadd213p%2 ymm2, ymm1, ymm3
        partial_store %1, r8, ymm2, rax

    .return:
        ret
%endmacro ; }}}

; rdi  n
; rsi  a
; rdx  b
; xmm0 c
; rcx  y
%macro fma_vvs 2 ; {{{
    cmp rdi, 0x20 / %1
    jl .last

    vbroadcasts%2 ymm3, xmm0 ; c
    .loop:
        vmovups ymm2, [rsi]
        vmovups ymm1, [rdx]
        vfmadd213p%2 ymm2, ymm1, ymm3
        vmovups [rcx], ymm2
        add rsi, 0x20
        add rdx, 0x20
        add rcx, 0x20
        sub rdi, 0x20 / %1
        cmp rdi, 0x20 / %1
        jge .loop

    .last:
        test rdi, rdi
        jz .return
        mov rax, rdi ; n
        mov r8,  rsi ; a
        mov r9,  rcx ; y
        partial_load %1, ymm2, r8,  rax
        partial_load %1, ymm1, rdx, rax
        vfmadd213p%2 ymm2, ymm1, ymm3
        partial_store %1, r9, ymm2, rax

    .return:
        ret
%endmacro ; }}}

; rdi  n
; rsi  a
; xmm0 b
; rdx  c
; rcx  y
%macro fma_vsv 2 ; {{{
    cmp rdi, 0x20 / %1
    jl .last

    vbroadcasts%2 ymm1, xmm0 ; b
    .loop:
        vmovups ymm2, [rsi]
        vmovups ymm3, [rdx]
        vfmadd213p%2 ymm2, ymm1, ymm3
        vmovups [rcx], ymm2
        add rsi, 0x20
        add rdx, 0x20
        add rcx, 0x20
        sub rdi, 0x20 / %1
        cmp rdi, 0x20 / %1
        jge .loop

    .last:
        test rdi, rdi
        jz .return
        mov rax, rdi ; n
        mov r8,  rsi ; a
        mov r9,  rcx ; y
        partial_load %1, ymm2, r8,  rax
        partial_load %1, ymm3, rdx, rax
        vfmadd213p%2 ymm2, ymm1, ymm3
        partial_store %1, r9, ymm2, rax

    .return:
        ret
%endmacro ; }}}

; rdi  n
; xmm0 a
; rsi  b
; rdx  c
; rcx  y
%macro fma_svv 2 ; {{{
    cmp rdi, 0x20 / %1
    jl .last

    vbroadcasts%2 ymm1, xmm0 ; a
    .loop:
        vmovups ymm2, [rsi]
        vmovups ymm3, [rdx]
        vfmadd213p%2 ymm2, ymm1, ymm3
        vmovups [rcx], ymm2
        add rsi,  0x20
        add rdx, 0x20
        add rcx, 0x20
        sub rdi, 0x20 / %1
        cmp rdi, 0x20 / %1
        jge .loop

    .last:
        test rdi, rdi
        jz .return
        mov rax, rdi ; n
        mov r8,  rsi ; b
        mov r9,  rcx ; y
        partial_load %1, ymm2, r8,  rax
        partial_load %1, ymm3, rdx, rax
        vfmadd213p%2 ymm2, ymm1, ymm3
        partial_store %1, r9, ymm2, rax

    .return:
        ret
%endmacro ; }}}

; rdi  n
; xmm0 a
; xmm1 b
; rsi  c
; rdx  y
%macro fma_ssv 2 ; {{{
    cmp rdi, 0x20 / %1
    jl .last

    vbroadcasts%2 ymm3, xmm0 ; a
    vbroadcasts%2 ymm1, xmm1 ; b
    .loop:
        vmovups ymm2, [rsi]
        vfmadd231p%2 ymm2, ymm3, ymm1
        vmovups [rdx], ymm2
        add rsi, 0x20
        add rdx, 0x20
        sub rdi, 0x20 / %1
        cmp rdi, 0x20 / %1
        jge .loop

    .last:
        test rdi, rdi
        jz .return
        mov rax, rdi ; n
        mov r8,  rsi ; c
        partial_load %1, ymm2, r8, rax
        vfmadd231p%2 ymm2, ymm3, ymm1
        partial_store %1, rdx, ymm2, rax

    .return:
        ret
%endmacro ; }}}

; rdi  n
; xmm0 a
; rsi  b
; xmm1 c
; rdx  y
%macro fma_svs 2 ; {{{
    cmp rdi, 0x20 / %1
    jl .last

    vbroadcasts%2 ymm3, xmm1 ; c
    vbroadcasts%2 ymm1, xmm0 ; a
    .loop:
        vmovups ymm2, [rsi]
        vfmadd213p%2 ymm2, ymm1, ymm3
        vmovups [rdx], ymm2
        add rsi,  0x20
        add rdx, 0x20
        sub rdi, 0x20 / %1
        cmp rdi, 0x20 / %1
        jge .loop

    .last:
        test rdi, rdi
        jz .return
        mov rax, rdi ; n
        mov r8,  rsi ; b
        partial_load %1, ymm2, r8, rax
        vfmadd213p%2 ymm2, ymm1, ymm3
        partial_store %1, rdx, ymm2, rax

    .return:
        ret
%endmacro ; }}}

; rdi  n
; rsi  a
; xmm0 b
; xmm1 c
; rdx  y
%macro fma_vss 2 ; {{{
    cmp rdi, 0x20 / %1
    jl .last

    vbroadcasts%2 ymm3, xmm1 ; c
    vbroadcasts%2 ymm1, xmm0 ; b
    .loop:
        vmovups ymm2, [rsi]
        vfmadd213p%2 ymm2, ymm1, ymm3
        vmovups [rdx], ymm2
        add rsi, 0x20
        add rdx, 0x20
        sub rdi, 0x20 / %1
        cmp rdi, 0x20 / %1
        jge .loop

    .last:
        test rdi, rdi
        jz .return
        mov rax, rdi ; n
        mov r8,  rsi ; a
        partial_load %1, ymm2, r8, rax
        vfmadd213p%2 ymm2, ymm1, ymm3
        partial_store %1, rdx, ymm2, rax

    .return:
        ret
%endmacro ; }}}

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
