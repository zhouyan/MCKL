;;============================================================================
;; MCKL/lib/asm/math.asm
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

; rdi:n
; rsi:a
; rdx:y
%macro math_kernel_a1r1 2 ; function, operand size {{{
    push rbp
    mov rbp, rsp
    shr rsp, 5
    shl rsp, 5
    sub rsp, 0x20
    mov rax, rdi

    cmp rdi, 0x2000 / %1
    jg .skip

    sub rsp, 0x2000
    mov rcx, rax
    lea rdi, [rsp + 0x20]
%if %1 == 4
    rep movsd
%elif %1 == 8
    rep movsq
%else
    %error
%endif
    lea rsi, [rsp + 0x20]

.skip:

    test rax, rax
    jz .return

    mov r8, rax
%if %1 == 4
    shr rax, 3
    and r8,  0x7
%elif %1 == 8
    shr rax, 2
    and r8,  0x3
%else
    %error
%endif

    %{2}_constants

    test rax, rax
    jz .last

.loop: align 16
    %2 [rdx], [rsi]
    add rsi, 0x20
    add rdx, 0x20
    dec rax
    jnz .loop

.last:
    test r8, r8
    jz .return
    mov rcx, r8
    mov rdi, rsp
%if %1 == 4
    rep movsd
%elif %1 == 8
    rep movsq
%else
    %error
%endif
    vmovaps ymm0, [rsp]
    %2 ymm0, ymm0
    vmovaps [rsp], ymm0
    mov rcx, r8
    mov rsi, rsp
    mov rdi, rdx
%if %1 == 4
    rep movsd
%elif %1 == 8
    rep movsq
%else
    %error
%endif

.return:
    mov rsp, rbp
    pop rbp
    ret
%endmacro ; }}}

; rdi:n
; rsi:a
; rdx:y
; rcx:z
%macro math_kernel_a1r2 2 ; function, operand size {{{
    push rbp
    mov rbp, rsp
    sub rsp, 0x40

    test rdi, rdi
    jz .return

    mov rax, rdi
%if %1 == 4
    shr rdi, 3
    and rax, 0x7
%elif %1 == 8
    shr rdi, 2
    and rax, 0x3
%else
    %error
%endif

    %{2}_constants

    test rdi, rdi
    jz .last

.loop: align 16
    %2 [rdx], [rcx], [rsi]
    add rsi, 0x20
    add rdx, 0x20
    add rcx, 0x20
    dec rdi
    jnz .loop

.last:
    test rax, rax
    jz .return
    mov r8,  rcx
    mov rcx, rax
    mov rdi, rsp
%if %1 == 4
    rep movsd
%elif %1 == 8
    rep movsq
%else
    %error
%endif
    vmovups ymm0, [rsp]
    %2 ymm0, ymm1, ymm0
    vmovups [rsp + 0x00], ymm0
    vmovups [rsp + 0x20], ymm1
    mov rcx, rax
    mov rsi, rsp
    mov rdi, rdx
%if %1 == 4
    rep movsd
%elif %1 == 8
    rep movsq
%else
    %error
%endif
    mov rcx, rax
    lea rsi, [rsp + 0x20]
    mov rdi, r8
%if %1 == 4
    rep movsd
%elif %1 == 8
    rep movsq
%else
    %error
%endif

.return:
    mov rsp, rbp
    pop rbp
    ret
%endmacro ; }}}

; vim:ft=nasm
