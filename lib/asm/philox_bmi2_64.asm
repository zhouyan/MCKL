;;============================================================================
;; MCKL/lib/asm/philox_bmi2_64.asm
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
;; AND ANY EXPRESS OR IMPLIED WARRANTIES, inc4LUDING, BUT NOT LIMITED TO, THE
;; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;; ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
;; LIABLE FOR ANY DIRECT, INDIRECT, inc4IDENTAL, SPECIAL, EXEMPLARY, OR
;; CONSEQUENTIAL DAMAGES (inc4LUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;; SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;; INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;; CONTRACT, STRICT LIABILITY, OR TORT (inc4LUDING NEGLIGENCE OR OTHERWISE)
;; ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;; POSSIBILITY OF SUCH DAMAGE.
;;============================================================================

; rdi r
; rsi ctr.data()
; rdx mul:weyl:key
; rcx n

%include "/common.asm"

global mckl_philox2x64_bmi2_kernel
global mckl_philox4x64_bmi2_kernel

%macro philox2x64_bmi2_rbox 1
    ; rdx multiplier
    ; r10 s0
    ; r11 s1
    mov r12, r11
    xor r12, [rsp + %1 * 8]
    mulx r10, r11, r10
    xor r10, r12
%endmacro

%macro philox4x64_bmi2_rbox 1
    ; rsp[0xB0] multiplier[0]
    ; rsp[0xB8] multiplier[1]
    ; r10 s0
    ; r11 s1
    ; r12 s2
    ; r13 s3
    mov r14, r11 ; t1
    mov r15, r13 ; t3
    xor r14, [rsp + %1 * 0x10]
    xor r15, [rsp + %1 * 0x10 + 8]
    mov r8, r10
    mov rdx, [rsp + 0xB0]
    mulx r10, r11, r12 ; t2
    mov rdx, [rsp + 0xB8]
    mulx r12, r13, r8 ; t0
    xor r10, r14
    xor r12, r15
%endmacro

section .text

mckl_philox2x64_bmi2_kernel:
    push r12
    push rbx
    push rbp
    mov rbp, rsp
    sub rsp, 0x50
    mov rax, rsi
    mov rsi, rdi
    mov rdi, rdx
    mov rdx, rcx
    mov rcx, rax

    test rcx, rcx
    jz .return

    mov rax, [rsi + 0] ; counter[0]
    mov rbx, [rsi + 8] ; counter[1]

    mov r8, [rdx + 0x08] ; weyl
    mov r9, [rdx + 0x10] ; key
    mov [rsp], r9
    %assign r 1
    %rep 9
        add r9, r8
        mov [rsp + r * 8], r9
        %assign r r + 1
    %endrep

    mov rdx, [rdx] ; multiplier

    .generate:
        add rax, 1
        adc rbx, 0
        mov r10, rax ; s0
        mov r11, rbx ; s1
        %assign r 0
        %rep 10
            philox2x64_bmi2_rbox r
            %assign r r + 1
        %endrep
        mov [rdi + 0], r10
        mov [rdi + 8], r11
        sub rcx, 1
        add rdi, 0x10
        test rcx, rcx
        jnz .generate

    mov [rsi + 0], rax
    mov [rsi + 8], rbx

    .return:
        mov rsp, rbp
        pop rbp
        pop rbx
        pop r12
        ret
; mckl_philox2x64_bmi2_kernel:

mckl_philox4x64_bmi2_kernel:
    push r12
    push r13
    push r14
    push r15
    push rbx
    push rbp
    mov rbp, rsp
    sub rsp, 0xC0
    mov rax, rsi
    mov rsi, rdi
    mov rdi, rdx
    mov rdx, rcx
    mov rcx, rax

    test rcx, rcx
    jz .return

    mov rax, [rsi + 0] ; counter[0]
    mov rbx, [rsi + 8] ; counter[1]
    vmovdqu xmm0, [rsi + 0x10] ; counter[3:2]
    vmovdqu [rsp + 0xA0], xmm0;

    mov r8, [rdx + 0] ; multiplier[0]
    mov r9, [rdx + 8] ; multiplier[1]
    mov [rsp + 0xB0], r8
    mov [rsp + 0xB8], r9
    vmovdqu xmm0, [rdx + 0x10]; weyl
    vmovdqu xmm1, [rdx + 0x20]; key
    vmovdqu [rsp], xmm1
    %assign r 1
    %rep 9
        vpaddq xmm1, xmm1, xmm0
        vmovdqu [rsp + r * 0x10], xmm1
        %assign r r + 1
    %endrep

    .generate:
        add rax, 1
        adc rbx, 0
        adc qword [rsp + 0xA0], 0
        adc qword [rsp + 0xA8], 0
        mov r10, rax ; s0
        mov r11, rbx ; s1
        mov r12, [rsp + 0xA0] ; s2
        mov r13, [rsp + 0xA8] ; s3
        %assign r 0
        %rep 10
            philox4x64_bmi2_rbox r
            %assign r r + 1
        %endrep
        mov [rdi + 0x00], r10
        mov [rdi + 0x08], r11
        mov [rdi + 0x10], r12
        mov [rdi + 0x18], r13
        sub rcx, 1
        add rdi, 0x20
        test rcx, rcx
        jnz .generate

    mov [rsi + 0], rax
    mov [rsi + 8], rbx
    vmovdqu xmm0, [rsp + 0xA0]
    vmovdqu [rsi + 0x10], xmm0

    .return:
        mov rsp, rbp
        pop rbp
        pop rbx
        pop r15
        pop r14
        pop r13
        pop r12
        ret
; mckl_philox4x64_bmi2_kernel:

; vim:ft=nasm
