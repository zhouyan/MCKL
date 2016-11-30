;;============================================================================
;; MCKL/lib/asm/sqrt.asm
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

global mckl_vd_sqrt

default rel

section .text

; rdi:n
; rsi:a
; rdx:y
mckl_vd_sqrt: ;{{{
    push rbp
    mov rbp, rsp
    sub rsp, 0x20
    cld

    cmp rdi, 4
    jl .last

.loop: align 16
    vmovupd ymm0, [rsi]
    vsqrtpd ymm0, ymm0
    vmovupd [rdx], ymm0
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
    rep movsq
    vmovupd ymm0, [rsp]
    vsqrtpd ymm0, ymm0
    vmovupd [rsp], ymm0
    mov rcx, rax
    mov rsi, rsp
    mov rdi, rdx
    rep movsq

.return:
    mov rsp, rbp
    pop rbp
    ret
; }}}

; vim:ft=nasm
