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

; rdi r
; rsi ctr.data()
; rdx mul:weyl:key
; rcx n

; ymm8 counter
; ymm9 multiplier
; ymm14 mask
; ymm15 round key

%include "/common.asm"

global mckl_philox2x32_avx2_kernel
global mckl_philox4x32_avx2_kernel

%macro philox_avx2_32_prologue 0
    prologue 5, 0x140
%endmacro

%macro philox_avx2_32_round_key 1
    %if %1 == 0x08
        vpbroadcastq ymm0, [rdx + 0x08]
        vpbroadcastq ymm1, [rdx + 0x10]
    %elif %1 == 0x10
        vbroadcasti128 ymm0, [rdx + 0x10]
        vbroadcasti128 ymm1, [rdx + 0x20]
    %else
        %error
    %endif
    vmovdqa [rsp], ymm1
    %assign r 1
    %rep 9
        vpaddq ymm1, ymm1, ymm0
        vmovdqa [rsp + r * 0x20], ymm1
        %assign r r + 1
    %endrep
%endmacro

%macro philox_avx2_32_rbox 2
    vmovdqa ymm15, [rsp + %1 * 0x20]

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

%macro philox_avx2_32_generate 4
    %if %1 == 0x08
        vpbroadcastq ymm8, [rsi]
        vpbroadcastq ymm9, [rdx]
    %elif %1 == 0x10
        vbroadcasti128 ymm8, [rsi]
        vbroadcasti128 ymm9, [rdx]
    %else
        %error
    %endif
    add [rsi], rcx

    vmovdqa ymm14, [rel philox_avx2_32_mask]

    align 16
    .generate:
        increment_avx2_ymm ymm8, %1
        %if %2 != 0xE3
            vpshufd ymm0, ymm0, %2
            vpshufd ymm1, ymm1, %2
            vpshufd ymm2, ymm2, %2
            vpshufd ymm3, ymm3, %2
            vpshufd ymm4, ymm4, %2
            vpshufd ymm5, ymm5, %2
            vpshufd ymm6, ymm6, %2
            vpshufd ymm7, ymm7, %2
        %endif
        %assign r 0
        %rep 9
            philox_avx2_32_rbox r, %3
            %assign r r + 1
        %endrep
        philox_avx2_32_rbox 9, %4

        cmp rcx, 0x100 / %1
        jl .storen

        vmovdqu [rdi + 0x00], ymm0
        vmovdqu [rdi + 0x20], ymm1
        vmovdqu [rdi + 0x40], ymm2
        vmovdqu [rdi + 0x60], ymm3
        vmovdqu [rdi + 0x80], ymm4
        vmovdqu [rdi + 0xA0], ymm5
        vmovdqu [rdi + 0xC0], ymm6
        vmovdqu [rdi + 0xE0], ymm7
        sub rcx, 0x100 / %1
        add rdi, 0x100

        test rcx, rcx
        jnz .generate

        .storen:
            test rcx, rcx,
            jz .return
            vmovdqa [rsp + 0x00], ymm0
            vmovdqa [rsp + 0x20], ymm1
            vmovdqa [rsp + 0x40], ymm2
            vmovdqa [rsp + 0x60], ymm3
            vmovdqa [rsp + 0x80], ymm4
            vmovdqa [rsp + 0xA0], ymm5
            vmovdqa [rsp + 0xC0], ymm6
            vmovdqa [rsp + 0xE0], ymm7
            mov rsi, rsp
            imul rcx, %1 / 0x04
            rep movsd
%endmacro

section .rodata

align 32
philox_avx2_32_mask:
dq 0xFFFFFFFF00000000
dq 0xFFFFFFFF00000000
dq 0xFFFFFFFF00000000
dq 0xFFFFFFFF00000000

def_increment_ymm_data_1
def_increment_ymm_data_2

section .text

mckl_philox2x32_avx2_kernel:
    philox_avx2_32_prologue
    philox_avx2_32_round_key 0x08
    philox_avx2_32_generate 0x08, 0xE3, 0xB1, 0xB1
    vzeroupper
    epilogue
; mckl_philox2x32_avx2_kernel:

mckl_philox4x32_avx2_kernel:
    philox_avx2_32_prologue
    philox_avx2_32_round_key 0x10
    philox_avx2_32_generate 0x10, 0xC6, 0x93, 0xB1
    vzeroupper
    epilogue
; mckl_philox4x32_avx2_kernel:

; vim:ft=nasm
