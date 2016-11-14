;;============================================================================
;; MCKL/asm/lib/philox_avx2_4x32.asm
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
; rax n
; rdx r
; rcx mul:weyl:key

; ymm8 counter
; ymm9 multiplier
; ymm14 mask
; ymm15 round key

%macro philox_avx2_32_increment 1
    vpaddq ymm0, ymm8, [rel inc%{1}0]
    vpaddq ymm1, ymm8, [rel inc%{1}1]
    vpaddq ymm2, ymm8, [rel inc%{1}2]
    vpaddq ymm3, ymm8, [rel inc%{1}3]
    vpaddq ymm4, ymm8, [rel inc%{1}4]
    vpaddq ymm5, ymm8, [rel inc%{1}5]
    vpaddq ymm6, ymm8, [rel inc%{1}6]
    vpaddq ymm7, ymm8, [rel inc%{1}7]
    vpaddq ymm8, ymm8, [rel inc%{1}8]
%endmacro

%macro philox_avx2_32_rk 0
    vmovdqa [rsp + 0x040], ymm0 ; round key 0
    vpaddd ymm0, ymm1
    vmovdqa [rsp + 0x060], ymm0 ; round key 1
    vpaddd ymm0, ymm1
    vmovdqa [rsp + 0x080], ymm0 ; round key 2
    vpaddd ymm0, ymm1
    vmovdqa [rsp + 0x0A0], ymm0 ; round key 3
    vpaddd ymm0, ymm1
    vmovdqa [rsp + 0x0C0], ymm0 ; round key 4
    vpaddd ymm0, ymm1
    vmovdqa [rsp + 0x0E0], ymm0 ; round key 5
    vpaddd ymm0, ymm1
    vmovdqa [rsp + 0x100], ymm0 ; round key 6
    vpaddd ymm0, ymm1
    vmovdqa [rsp + 0x120], ymm0 ; round key 7
    vpaddd ymm0, ymm1
    vmovdqa [rsp + 0x140], ymm0 ; round key 8
    vpaddd ymm0, ymm1
    vmovdqa [rsp + 0x160], ymm0 ; round key 9
%endmacro

%macro philox_avx2_32_rbox 2
    vmovdqa ymm15, [rsp + 0x40 + %1 * 0x20]
    vpmuludq ymm10, ymm0, ymm9
    vpmuludq ymm11, ymm1, ymm9
    vpmuludq ymm12, ymm2, ymm9
    vpmuludq ymm13, ymm3, ymm9
    vpand ymm0, ymm14
    vpand ymm1, ymm14
    vpand ymm2, ymm14
    vpand ymm3, ymm14
    vpxor ymm0, ymm15
    vpxor ymm1, ymm15
    vpxor ymm2, ymm15
    vpxor ymm3, ymm15
    vpxor ymm0, ymm10
    vpxor ymm1, ymm11
    vpxor ymm2, ymm12
    vpxor ymm3, ymm13

    vpmuludq ymm10, ymm4, ymm9
    vpmuludq ymm11, ymm5, ymm9
    vpmuludq ymm12, ymm6, ymm9
    vpmuludq ymm13, ymm7, ymm9
    vpand ymm4, ymm14
    vpand ymm5, ymm14
    vpand ymm6, ymm14
    vpand ymm7, ymm14
    vpxor ymm4, ymm15
    vpxor ymm5, ymm15
    vpxor ymm6, ymm15
    vpxor ymm7, ymm15
    vpxor ymm4, ymm10
    vpxor ymm5, ymm11
    vpxor ymm6, ymm12
    vpxor ymm7, ymm13

    vpshufd ymm0, ymm0, %2
    vpshufd ymm1, ymm1, %2
    vpshufd ymm2, ymm2, %2
    vpshufd ymm3, ymm3, %2
    vpshufd ymm4, ymm4, %2
    vpshufd ymm5, ymm5, %2
    vpshufd ymm6, ymm6, %2
    vpshufd ymm7, ymm7, %2
%endmacro

%macro philox_avx2_32_store 2
    cmp rax, %2
    jl philox%{1}x32_avx2_storen

    vmovdqu [rdx + 0x00], ymm0
    vmovdqu [rdx + 0x20], ymm1
    vmovdqu [rdx + 0x40], ymm2
    vmovdqu [rdx + 0x60], ymm3
    vmovdqu [rdx + 0x80], ymm4
    vmovdqu [rdx + 0xA0], ymm5
    vmovdqu [rdx + 0xC0], ymm6
    vmovdqu [rdx + 0xE0], ymm7
    sub rax, %2
    add rdx, 0x100

    test rax, rax
    jnz philox%{1}x32_avx2_generate

philox%{1}x32_avx2_storen:
    test rax, rax,
    jz philox%{1}x32_avx2_ret
    vmovdqa [rsp + 0x00], ymm0
    vmovdqa [rsp + 0x20], ymm1
    vmovdqa [rsp + 0x40], ymm2
    vmovdqa [rsp + 0x60], ymm3
    vmovdqa [rsp + 0x80], ymm4
    vmovdqa [rsp + 0xA0], ymm5
    vmovdqa [rsp + 0xC0], ymm6
    vmovdqa [rsp + 0xE0], ymm7
%endmacro

section .rodata

align 32
inc20:
DQ 0x01
DQ 0x02
DQ 0x03
DQ 0x04

align 32
inc21:
DQ 0x05
DQ 0x06
DQ 0x07
DQ 0x08

align 32
inc22:
DQ 0x09
DQ 0x0A
DQ 0x0B
DQ 0x0C

align 32
inc23:
DQ 0x0D
DQ 0x0E
DQ 0x0F
DQ 0x10

align 32
inc24:
DQ 0x11
DQ 0x12
DQ 0x13
DQ 0x14

align 32
inc25:
DQ 0x15
DQ 0x16
DQ 0x17
DQ 0x18

align 32
inc26:
DQ 0x19
DQ 0x1A
DQ 0x1B
DQ 0x1C

align 32
inc27:
DQ 0x1D
DQ 0x1E
DQ 0x1F
DQ 0x20

align 32
inc28:
DQ 0x20
DQ 0x20
DQ 0x20
DQ 0x20

align 32
inc40:
DQ 0x01
DQ 0
DQ 0x02
DQ 0

align 32
inc41:
DQ 0x03
DQ 0
DQ 0x04
DQ 0

align 32
inc42:
DQ 0x05
DQ 0
DQ 0x06
DQ 0

align 32
inc43:
DQ 0x07
DQ 0
DQ 0x08
DQ 0

align 32
inc44:
DQ 0x09
DQ 0
DQ 0x0A
DQ 0

align 32
inc45:
DQ 0x0B
DQ 0
DQ 0x0C
DQ 0

align 32
inc46:
DQ 0x0D
DQ 0
DQ 0x0E
DQ 0

align 32
inc47:
DQ 0x0F
DQ 0
DQ 0x10
DQ 0

align 32
inc48:
DQ 0x10
DQ 0
DQ 0x10
DQ 0

align 32
mask:
DD 0
DD 0xFFFFFFFF
DD 0
DD 0xFFFFFFFF
DD 0
DD 0xFFFFFFFF
DD 0
DD 0xFFFFFFFF

section .text

global _philox2x32_avx2_kernel
_philox2x32_avx2_kernel:
    push rbp
    mov rbp, rsp
    mov rax, rsi
    and rsp, 0xFFFFFFFFFFFFFFE0
    sub rsp, 0x180

    vpbroadcastq ymm8, [rdi]
    add [rdi], rax

    vpbroadcastq ymm9, [rcx]
    vpbroadcastq ymm1, [rcx + 0x08]
    vpbroadcastq ymm0, [rcx + 0x10]
    philox_avx2_32_rk

    vmovdqa ymm14, [rel mask]

philox2x32_avx2_generate:
    philox_avx2_32_increment 2

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

    philox_avx2_32_store 2, 32

philox2x32_avx2_store1:
    mov r8, [rsp]
    mov [rdx], r8
    sub rax, 1
    add rsp, 0x08
    add rdx, 0x08
    test rax, rax
    jnz philox2x32_avx2_store1

philox2x32_avx2_ret:
    vzeroupper
    mov rsp, rbp
    pop rbp
    ret

global _philox4x32_avx2_kernel
_philox4x32_avx2_kernel:
    push rbp
    mov rbp, rsp
    mov rax, rsi
    and rsp, 0xFFFFFFFFFFFFFFE0
    sub rsp, 0x180

    vbroadcasti128 ymm8, [rdi]
    add [rdi], rax

    vbroadcasti128 ymm9, [rcx]
    vbroadcasti128 ymm1, [rcx + 0x10]
    vbroadcasti128 ymm0, [rcx + 0x20]
    philox_avx2_32_rk

    vmovdqa ymm14, [rel mask]

philox4x32_avx2_generate:
    philox_avx2_32_increment 4

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

    philox_avx2_32_store 4, 16

philox4x32_avx2_store1:
    vmovdqa xmm0, [rsp]
    vmovdqu [rdx], xmm0
    sub rax, 1
    add rsp, 0x10
    add rdx, 0x10
    test rax, rax
    jnz philox4x32_avx2_store1

philox4x32_avx2_ret:
    vzeroupper
    mov rsp, rbp
    pop rbp
    ret

; vim:ft=nasm
