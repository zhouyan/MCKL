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

global mckl_philox2x32_sse2_kernel
global mckl_philox4x32_sse2_kernel

%macro philox_sse2_32_rbox 2 ; {{{
    movdqa xmm15, [rsp + %1 * 0x10] ; round key

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
%endmacro ; }}}

; rdi ctr.data()
; rsi n
; rdx r
; rcx mul:weyl:key
%macro philox_sse2_32_kernel 4 ; block size, permute constants {{{
    push rbp
    mov rbp, rsp
    sub rsp, 0xA0

    ; load counter
    %if %1 == 0x08
        movq xmm8, [rdi]
        pshufd xmm8, xmm8, 0x44
    %elif %1 == 0x10
        movdqu xmm8, [rdi]
    %else
        %error
    %endif
    add [rdi], rsi

    ; load multiplier
    %if %1 == 0x08
        movq xmm9, [rcx]
        pshufd xmm9, xmm9, 0x44
    %elif %1 == 0x10
        movdqu xmm9, [rcx]
    %else
        %error
    %endif

    ; compute round keys
    %if %1 == 0x08
        movq xmm0, [rcx + 0x08] ; weyl
        movq xmm1, [rcx + 0x10] ; key
        pshufd xmm0, xmm0, 0x44
        pshufd xmm1, xmm1, 0x44
    %elif %1 == 0x10
        movdqu xmm0, [rcx + 0x10] ; weyl
        movdqu xmm1, [rcx + 0x20] ; key
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

    ; load mask
    movq xmm14, [rel philox_sse2_32_mask]
    pshufd xmm14, xmm14, 0x44
    align 16

    mov rcx, rsi ; n
    mov rdi, rdx ; r

    ; generate a new batch of (0x80 / %1) blocks
    .generate:
        movdqa xmm0, xmm8
        movdqa xmm1, xmm8
        movdqa xmm2, xmm8
        movdqa xmm3, xmm8
        movdqa xmm4, xmm8
        movdqa xmm5, xmm8
        movdqa xmm6, xmm8
        movdqa xmm7, xmm8
        %if %1 == 0x08
            paddq xmm0, [rel philox2x32_sse2_increment + 0x00]
            paddq xmm1, [rel philox2x32_sse2_increment + 0x10]
            paddq xmm2, [rel philox2x32_sse2_increment + 0x20]
            paddq xmm3, [rel philox2x32_sse2_increment + 0x30]
            paddq xmm4, [rel philox2x32_sse2_increment + 0x40]
            paddq xmm5, [rel philox2x32_sse2_increment + 0x50]
            paddq xmm6, [rel philox2x32_sse2_increment + 0x60]
            paddq xmm7, [rel philox2x32_sse2_increment + 0x70]
            paddq xmm8, [rel philox2x32_sse2_increment + 0x80]
        %elif %1 == 0x10
            paddq xmm0, [rel philox4x32_sse2_increment + 0x00]
            paddq xmm1, [rel philox4x32_sse2_increment + 0x10]
            paddq xmm2, [rel philox4x32_sse2_increment + 0x20]
            paddq xmm3, [rel philox4x32_sse2_increment + 0x30]
            paddq xmm4, [rel philox4x32_sse2_increment + 0x40]
            paddq xmm5, [rel philox4x32_sse2_increment + 0x50]
            paddq xmm6, [rel philox4x32_sse2_increment + 0x60]
            paddq xmm7, [rel philox4x32_sse2_increment + 0x70]
            movdqa xmm8, xmm7
        %else
            %error
        %endif
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

    ; check if this is the last batch
    cmp rcx, 0x80 / %1
    jl .last

    ; store the current batch directly to r
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

    ; check if a new batch is needed
    test rcx, rcx
    jnz .generate

    ; store the last batch on the stack and then move to r
    .last:
        test rcx, rcx
        jz .return
        movdqa [rsp + 0x00], xmm0
        movdqa [rsp + 0x10], xmm1
        movdqa [rsp + 0x20], xmm2
        movdqa [rsp + 0x30], xmm3
        movdqa [rsp + 0x40], xmm4
        movdqa [rsp + 0x50], xmm5
        movdqa [rsp + 0x60], xmm6
        movdqa [rsp + 0x70], xmm7
        %if %1 == 0x08
            ; do nothing
        %elif %1 == 0x10
            shl rcx, 1
        %else
            %error
        %endif
        mov rsi, rsp
        cld
        rep movsq

    .return:
        mov rsp, rbp
        pop rbp
        ret
%endmacro ; }}}

section .rodata

align 16
philox2x32_sse2_increment:
dq 0x01, 0x02
dq 0x03, 0x04
dq 0x05, 0x06
dq 0x07, 0x08
dq 0x09, 0x0A
dq 0x0B, 0x0C
dq 0x0D, 0x0E
dq 0x0F, 0x10
dq 0x10, 0x10

align 16
philox4x32_sse2_increment:
dq 0x01, 0x00
dq 0x02, 0x00
dq 0x03, 0x00
dq 0x04, 0x00
dq 0x05, 0x00
dq 0x06, 0x00
dq 0x07, 0x00
dq 0x08, 0x00
dq 0x08, 0x00

philox_sse2_32_mask:
dq 0xFFFF_FFFF_0000_0000

section .text

mckl_philox2x32_sse2_kernel: philox_sse2_32_kernel 0x08, 0xE3, 0xB1, 0xB1
mckl_philox4x32_sse2_kernel: philox_sse2_32_kernel 0x10, 0xC6, 0x93, 0xB1

; vim:ft=nasm
