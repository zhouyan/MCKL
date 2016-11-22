;;============================================================================
;; MCKL/lib/asm/philox_bmi2_4x64.asm
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

global mckl_philox4x64_bmi2_kernel

%macro philox4x64_bmi2_rbox 1 ; {{{
    vmovq rax, %1 ; k0
    xor rax, r11 ; t1
    vmovq rdx, xmm10 ; m2
    mulx rsi, r11, r12 ; t2

    vpextrq rbx, %1, 1 ; k1
    xor rbx, r13 ; t3
    vmovq rdx, xmm11 ; m0
    mulx r12, r13, r10 ; t0

    mov r10, rsi
    xor r12, rbx
    xor r10, rax
%endmacro ; }}}

section .text

; rdi r

; rdi ctr.data()
; rsi n
; rdx r
; rcx mul:weyl:key
mckl_philox4x64_bmi2_kernel: ; {{{
    push rbx
    push r12
    push r13
    push r14
    push r15

    test rsi, rsi
    jz .return

    mov r8,  [rdi + 0x00] ; counter[0]
    mov r9,  [rdi + 0x08] ; counter[1]
    mov r14, [rdi + 0x10] ; counter[2]
    mov r15, [rdi + 0x18] ; counter[3]
    add [rdi], rsi
    adc qword [rdi + 0x08], 0
    adc qword [rdi + 0x10], 0
    adc qword [rdi + 0x18], 0

    vmovdqu xmm15, [rcx + 0x10] ; weyl
    vmovdqu xmm0,  [rcx + 0x20] ; round_key[0]
    vpaddq xmm1, xmm0, xmm15    ; round_key[1]
    vpaddq xmm2, xmm1, xmm15    ; round_key[2]
    vpaddq xmm3, xmm2, xmm15    ; round_key[3]
    vpaddq xmm4, xmm3, xmm15    ; round_key[4]
    vpaddq xmm5, xmm4, xmm15    ; round_key[5]
    vpaddq xmm6, xmm5, xmm15    ; round_key[6]
    vpaddq xmm7, xmm6, xmm15    ; round_key[7]
    vpaddq xmm8, xmm7, xmm15    ; round_key[8]
    vpaddq xmm9, xmm8, xmm15    ; round_key[9]

    mov rdi, rdx ; r
    vmovdqu xmm10, [rcx]       ; multiplier[0]
    vpshufd xmm11, xmm10, 0x4E ; multiplier[1]
    mov rcx, rsi ; n

    .generate:
        add r8, 1
        jnc .nocarry
        adc r9, 0
        adc r14, 0
        adc r15, 0
        .nocarry:
        mov r10, r8  ; s0
        mov r11, r9  ; s1
        mov r12, r14 ; s2
        mov r13, r15 ; s3
        philox4x64_bmi2_rbox xmm0
        philox4x64_bmi2_rbox xmm1
        philox4x64_bmi2_rbox xmm2
        philox4x64_bmi2_rbox xmm3
        philox4x64_bmi2_rbox xmm4
        philox4x64_bmi2_rbox xmm5
        philox4x64_bmi2_rbox xmm6
        philox4x64_bmi2_rbox xmm7
        philox4x64_bmi2_rbox xmm8
        philox4x64_bmi2_rbox xmm9
        mov [rdi + 0x00], r10
        mov [rdi + 0x08], r11
        mov [rdi + 0x10], r12
        mov [rdi + 0x18], r13
        sub rcx, 1
        add rdi, 0x20
        test rcx, rcx
        jnz .generate

    .return:
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbx
        ret
; mckl_philox4x64_bmi2_kernel: }}}

; vim:ft=nasm
