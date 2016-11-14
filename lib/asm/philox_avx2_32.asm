;;============================================================================
;; MCKL/lib/asm/philox_avx2_32.asm
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

; rdi ctr.data()
; rsi n
; rdx r
; rcx mul:weyl:key

; ymm8 counter
; ymm9 multiplier
; ymm14 mask
; ymm15 round key

%include "/common.asm"

global mckl_philox2x32_avx2_kernel
global mckl_philox4x32_avx2_kernel

%macro philox_avx2_32_rk 0
    vmovdqa [rsp + 0x040], ymm0 ; round key 0
    vpaddd ymm0, ymm0, ymm1
    vmovdqa [rsp + 0x060], ymm0 ; round key 1
    vpaddd ymm0, ymm0, ymm1
    vmovdqa [rsp + 0x080], ymm0 ; round key 2
    vpaddd ymm0, ymm0, ymm1
    vmovdqa [rsp + 0x0A0], ymm0 ; round key 3
    vpaddd ymm0, ymm0, ymm1
    vmovdqa [rsp + 0x0C0], ymm0 ; round key 4
    vpaddd ymm0, ymm0, ymm1
    vmovdqa [rsp + 0x0E0], ymm0 ; round key 5
    vpaddd ymm0, ymm0, ymm1
    vmovdqa [rsp + 0x100], ymm0 ; round key 6
    vpaddd ymm0, ymm0, ymm1
    vmovdqa [rsp + 0x120], ymm0 ; round key 7
    vpaddd ymm0, ymm0, ymm1
    vmovdqa [rsp + 0x140], ymm0 ; round key 8
    vpaddd ymm0, ymm0, ymm1
    vmovdqa [rsp + 0x160], ymm0 ; round key 9
%endmacro

%macro philox_avx2_32_rbox 2
    vmovdqa ymm15, [rsp + 0x40 + %1 * 0x20]
    vpmuludq ymm10, ymm0, ymm9
    vpmuludq ymm11, ymm1, ymm9
    vpmuludq ymm12, ymm2, ymm9
    vpmuludq ymm13, ymm3, ymm9
    vpand ymm0, ymm0, ymm14
    vpand ymm1, ymm1, ymm14
    vpand ymm2, ymm2, ymm14
    vpand ymm3, ymm3, ymm14
    vpxor ymm0, ymm0, ymm15
    vpxor ymm1, ymm1, ymm15
    vpxor ymm2, ymm2, ymm15
    vpxor ymm3, ymm3, ymm15
    vpxor ymm0, ymm0, ymm10
    vpxor ymm1, ymm1, ymm11
    vpxor ymm2, ymm2, ymm12
    vpxor ymm3, ymm3, ymm13

    vpmuludq ymm10, ymm4, ymm9
    vpmuludq ymm11, ymm5, ymm9
    vpmuludq ymm12, ymm6, ymm9
    vpmuludq ymm13, ymm7, ymm9
    vpand ymm4, ymm4, ymm14
    vpand ymm5, ymm5, ymm14
    vpand ymm6, ymm6, ymm14
    vpand ymm7, ymm7, ymm14
    vpxor ymm4, ymm4, ymm15
    vpxor ymm5, ymm5, ymm15
    vpxor ymm6, ymm6, ymm15
    vpxor ymm7, ymm7, ymm15
    vpxor ymm4, ymm4, ymm10
    vpxor ymm5, ymm5, ymm11
    vpxor ymm6, ymm6, ymm12
    vpxor ymm7, ymm7, ymm13

    vpshufd ymm0, ymm0, %2
    vpshufd ymm1, ymm1, %2
    vpshufd ymm2, ymm2, %2
    vpshufd ymm3, ymm3, %2
    vpshufd ymm4, ymm4, %2
    vpshufd ymm5, ymm5, %2
    vpshufd ymm6, ymm6, %2
    vpshufd ymm7, ymm7, %2
%endmacro

%macro philox_avx2_32_store 1
    cmp rsi, %1
    jl .storen

    vmovdqu [rdx + 0x00], ymm0
    vmovdqu [rdx + 0x20], ymm1
    vmovdqu [rdx + 0x40], ymm2
    vmovdqu [rdx + 0x60], ymm3
    vmovdqu [rdx + 0x80], ymm4
    vmovdqu [rdx + 0xA0], ymm5
    vmovdqu [rdx + 0xC0], ymm6
    vmovdqu [rdx + 0xE0], ymm7
    sub rsi, %1
    add rdx, 0x100

    test rsi, rsi
    jnz .generate

.storen:
    test rsi, rsi,
    jz .return
    vmovdqa [rsp + 0x00], ymm0
    vmovdqa [rsp + 0x20], ymm1
    vmovdqa [rsp + 0x40], ymm2
    vmovdqa [rsp + 0x60], ymm3
    vmovdqa [rsp + 0x80], ymm4
    vmovdqa [rsp + 0xA0], ymm5
    vmovdqa [rsp + 0xC0], ymm6
    vmovdqa [rsp + 0xE0], ymm7
%endmacro

section .data

align 32
philox_avx2_32_mask:
DD 0
DD 0xFFFFFFFF
DD 0
DD 0xFFFFFFFF
DD 0
DD 0xFFFFFFFF
DD 0
DD 0xFFFFFFFF

section .text

mckl_philox2x32_avx2_kernel:
    prologue 5, 0x180

    vpbroadcastq ymm8, [rdi]
    add [rdi], rsi

    vpbroadcastq ymm9, [rcx]
    vpbroadcastq ymm1, [rcx + 0x08]
    vpbroadcastq ymm0, [rcx + 0x10]
    philox_avx2_32_rk

    vmovdqa ymm14, [rel philox_avx2_32_mask]

    align 16
    .generate:
        increment_ymm_64_1
        philox_avx2_32_rbox 0, 0xB1
        philox_avx2_32_rbox 1, 0xB1
        philox_avx2_32_rbox 2, 0xB1
        philox_avx2_32_rbox 3, 0xB1
        philox_avx2_32_rbox 4, 0xB1
        philox_avx2_32_rbox 5, 0xB1
        philox_avx2_32_rbox 6, 0xB1
        philox_avx2_32_rbox 7, 0xB1
        philox_avx2_32_rbox 8, 0xB1
        philox_avx2_32_rbox 9, 0xB1
        philox_avx2_32_store 32

    align 16
    .store1:
        mov r8, [rsp]
        mov [rdx], r8
        sub rsi, 1
        add rsp, 0x08
        add rdx, 0x08
        test rsi, rsi
        jnz .store1

    epilogue
; mckl_philox2x32_avx2_kernel:

mckl_philox4x32_avx2_kernel:
    prologue 5, 0x180

    vbroadcasti128 ymm8, [rdi]
    add [rdi], rsi

    vbroadcasti128 ymm9, [rcx]
    vbroadcasti128 ymm1, [rcx + 0x10]
    vbroadcasti128 ymm0, [rcx + 0x20]
    philox_avx2_32_rk

    vmovdqa ymm14, [rel philox_avx2_32_mask]

    align 16
    .generate:
        increment_ymm_64_2
        vpshufd ymm0, ymm0, 0xC6
        vpshufd ymm1, ymm1, 0xC6
        vpshufd ymm2, ymm2, 0xC6
        vpshufd ymm3, ymm3, 0xC6
        vpshufd ymm4, ymm4, 0xC6
        vpshufd ymm5, ymm5, 0xC6
        vpshufd ymm6, ymm6, 0xC6
        vpshufd ymm7, ymm7, 0xC6
        philox_avx2_32_rbox 0, 0x93
        philox_avx2_32_rbox 1, 0x93
        philox_avx2_32_rbox 2, 0x93
        philox_avx2_32_rbox 3, 0x93
        philox_avx2_32_rbox 4, 0x93
        philox_avx2_32_rbox 5, 0x93
        philox_avx2_32_rbox 6, 0x93
        philox_avx2_32_rbox 7, 0x93
        philox_avx2_32_rbox 8, 0x93
        philox_avx2_32_rbox 9, 0xB1
        philox_avx2_32_store 16

    align 16
    .store1:
        vmovdqa xmm0, [rsp]
        vmovdqu [rdx], xmm0
        sub rsi, 1
        add rsp, 0x10
        add rdx, 0x10
        test rsi, rsi
        jnz .store1

    epilogue
; mckl_philox4x32_avx2_kernel:

; vim:ft=nasm
