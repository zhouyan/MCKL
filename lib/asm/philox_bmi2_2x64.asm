;;============================================================================
;; MCKL/lib/asm/philox_bmi2_2x64.asm
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

global mckl_philox2x64_bmi2_kernel

%macro philox2x64_bmi2_rbox 1 ; {{{
    vmovq rax, %1
    xor rax, r11
    mulx r10, r11, r10
    xor r10, rax
%endmacro ; }}}

section .text

; rdi ctr.data()
; rsi n
; rdx r
; rcx mul:weyl:key
mckl_philox2x64_bmi2_kernel: ; {{{
    test rsi, rsi
    jz .return

    mov r8, [rdi]     ; counter[0]
    mov r9, [rdi + 8] ; counter[1]
    add [rdi], rsi
    adc qword [rdi + 8], 0

    vmovq xmm15, [rcx + 0x08] ; weyl
    vmovq xmm0,  [rcx + 0x10] ; round_key[0]
    vpaddq xmm1, xmm0, xmm15  ; round_key[1]
    vpaddq xmm2, xmm1, xmm15  ; round_key[2]
    vpaddq xmm3, xmm2, xmm15  ; round_key[3]
    vpaddq xmm4, xmm3, xmm15  ; round_key[4]
    vpaddq xmm5, xmm4, xmm15  ; round_key[5]
    vpaddq xmm6, xmm5, xmm15  ; round_key[6]
    vpaddq xmm7, xmm6, xmm15  ; round_key[7]
    vpaddq xmm8, xmm7, xmm15  ; round_key[8]
    vpaddq xmm9, xmm8, xmm15  ; round_key[9]

    mov rdi, rdx ; r
    mov rdx, [rcx] ; multiplier
    mov rcx, rsi ; n

    .generate:
        add r8, 1
        adc r9, 0
        mov r10, r8
        mov r11, r9
        philox2x64_bmi2_rbox xmm0
        philox2x64_bmi2_rbox xmm1
        philox2x64_bmi2_rbox xmm2
        philox2x64_bmi2_rbox xmm3
        philox2x64_bmi2_rbox xmm4
        philox2x64_bmi2_rbox xmm5
        philox2x64_bmi2_rbox xmm6
        philox2x64_bmi2_rbox xmm7
        philox2x64_bmi2_rbox xmm8
        philox2x64_bmi2_rbox xmm9
        mov [rdi + 0], r10
        mov [rdi + 8], r11
        sub rcx, 1
        add rdi, 0x10
        test rcx, rcx
        jnz .generate

    .return:
        ret
; mckl_philox2x64_bmi2_kernel: }}}

; vim:ft=nasm
