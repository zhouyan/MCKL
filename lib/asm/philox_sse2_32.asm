;;============================================================================
;; MCKL/lib/asm/philox_sse2_32.asm
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

; xmm8 counter
; xmm9 multiplier
; xmm14 mask
; xmm15 round key

%include "/common.asm"

global mckl_philox2x32_sse2_kernel
global mckl_philox4x32_sse2_kernel

%macro philox_sse2_32_prologue 0
    prologue 4, 0xA0
%endmacro

%macro philox_sse2_32_round_key 1
    %if %1 == 0x08
        movq xmm0, [rdx + 0x08]
        movq xmm1, [rdx + 0x10]
        pshufd xmm0, xmm0, 0x44
        pshufd xmm1, xmm1, 0x44
    %elif %1 == 0x10
        movdqu xmm0, [rdx + 0x10]
        movdqu xmm1, [rdx + 0x20]
    %else
        %error
    %endif
    movdqa [rsp], xmm1
    %assign r 1
    %rep 9
        paddq xmm1, xmm0
        movdqa [rsp + r * 0x10], xmm1
        %assign r r + 1
    %endrep
%endmacro

%macro philox_sse2_32_rbox 2
    movdqa xmm15, [rsp + %1 * 0x10]

    movdqa xmm10, xmm0
    movdqa xmm11, xmm1
    movdqa xmm12, xmm2
    movdqa xmm13, xmm3
    pmuludq xmm10, xmm9
    pmuludq xmm11, xmm9
    pmuludq xmm12, xmm9
    pmuludq xmm13, xmm9
    pand xmm0, xmm14
    pand xmm1, xmm14
    pand xmm2, xmm14
    pand xmm3, xmm14
    pxor xmm0, xmm15
    pxor xmm1, xmm15
    pxor xmm2, xmm15
    pxor xmm3, xmm15
    pxor xmm0, xmm10
    pxor xmm1, xmm11
    pxor xmm2, xmm12
    pxor xmm3, xmm13

    movdqa xmm10, xmm4
    movdqa xmm11, xmm5
    movdqa xmm12, xmm6
    movdqa xmm13, xmm7
    pmuludq xmm10, xmm9
    pmuludq xmm11, xmm9
    pmuludq xmm12, xmm9
    pmuludq xmm13, xmm9
    pand xmm4, xmm14
    pand xmm5, xmm14
    pand xmm6, xmm14
    pand xmm7, xmm14
    pxor xmm4, xmm15
    pxor xmm5, xmm15
    pxor xmm6, xmm15
    pxor xmm7, xmm15
    pxor xmm4, xmm10
    pxor xmm5, xmm11
    pxor xmm6, xmm12
    pxor xmm7, xmm13

    pshufd xmm0, xmm0, %2
    pshufd xmm1, xmm1, %2
    pshufd xmm2, xmm2, %2
    pshufd xmm3, xmm3, %2
    pshufd xmm4, xmm4, %2
    pshufd xmm5, xmm5, %2
    pshufd xmm6, xmm6, %2
    pshufd xmm7, xmm7, %2
%endmacro

%macro philox_sse2_32_generate 4
    %if %1 == 0x08
        movq xmm8, [rsi]
        movq xmm9, [rdx]
        pshufd xmm8, xmm8, 0x44
        pshufd xmm9, xmm9, 0x44
    %elif %1 == 0x10
        movdqu xmm8, [rsi]
        movdqu xmm9, [rdx]
    %else
        %error
    %endif
    add [rsi], rcx

    movdqa xmm14, [rel philox_sse2_32_mask]

    align 16
    .generate:
        increment_sse2_xmm xmm8, %1
        %if %2 != 0xE3
            pshufd xmm0, xmm0, %2
            pshufd xmm1, xmm1, %2
            pshufd xmm2, xmm2, %2
            pshufd xmm3, xmm3, %2
            pshufd xmm4, xmm4, %2
            pshufd xmm5, xmm5, %2
            pshufd xmm6, xmm6, %2
            pshufd xmm7, xmm7, %2
        %endif
        %assign r 0
        %rep 9
            philox_sse2_32_rbox r, %3
            %assign r r + 1
        %endrep
        philox_sse2_32_rbox 9, %4

        cmp rcx, 0x80 / %1
        jl .storen

        movdqu [rdi + 0x00], xmm0
        movdqu [rdi + 0x10], xmm1
        movdqu [rdi + 0x20], xmm2
        movdqu [rdi + 0x30], xmm3
        movdqu [rdi + 0x40], xmm4
        movdqu [rdi + 0x50], xmm5
        movdqu [rdi + 0x60], xmm6
        movdqu [rdi + 0x70], xmm7
        sub rcx, 0x80 / %1
        add rdi, 0x80

        test rcx, rcx
        jnz .generate

        .storen:
            test rcx, rcx,
            jz .return
            movdqa [rsp + 0x00], xmm0
            movdqa [rsp + 0x10], xmm1
            movdqa [rsp + 0x20], xmm2
            movdqa [rsp + 0x30], xmm3
            movdqa [rsp + 0x40], xmm4
            movdqa [rsp + 0x50], xmm5
            movdqa [rsp + 0x60], xmm6
            movdqa [rsp + 0x70], xmm7
            mov rsi, rsp
            imul rcx, %1 / 0x04
            rep movsd
%endmacro

section .rodata

align 16
philox_sse2_32_mask:
dq 0xFFFFFFFF00000000
dq 0xFFFFFFFF00000000

def_increment_xmm_data_1
def_increment_xmm_data_2

section .text

mckl_philox2x32_sse2_kernel:
    philox_sse2_32_prologue
    philox_sse2_32_round_key 0x08
    philox_sse2_32_generate 0x08, 0xE3, 0xB1, 0xB1
    epilogue
; mckl_philox2x32_sse2_kernel:

mckl_philox4x32_sse2_kernel:
    philox_sse2_32_prologue
    philox_sse2_32_round_key 0x10
    philox_sse2_32_generate 0x10, 0xC6, 0x93, 0xB1
    epilogue
; mckl_philox4x32_avx2_kernel:

; vim:ft=nasm
