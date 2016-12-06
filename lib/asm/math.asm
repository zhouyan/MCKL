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
%macro math_kernel_a1r1 2 ; function, operand size
    test rdi, rdi
    jz .return

    %{2}_constants

    mov rax, rdi
    and rax, (0x20 / %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

.loop: align 16
    %2 [rdx], [rsi]
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
    vmovaps ymm1, [rax]
    vmaskmovps ymm0, ymm1, [rsi]
    %2 ymm0, ymm0
    vmovaps ymm1, [rax]
    vmaskmovps [rdx], ymm1, ymm0

.return:
    ret
%endmacro

; rdi:n
; rsi:a
; rdx:y
; rcx:z
%macro math_kernel_a1r2 2 ; function, operand size
    test rdi, rdi
    jz .return

    %{2}_constants

    mov rax, rdi
    and rax, (0x20 / %1) - 1
    sub rdi, rax

    test rdi, rdi
    jz .last

.loop: align 16
    %2 [rdx], [rcx], [rsi]
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
    vmovaps ymm2, [rax]
    vmaskmovps ymm0, ymm2, [rsi]
    %2 ymm0, ymm1, ymm0
    vmovaps ymm2, [rax]
    vmaskmovps [rdx], ymm2, ymm0
    vmaskmovps [rcx], ymm2, ymm1

.return:
    ret
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

; vim:ft=nasm
