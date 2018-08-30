;;============================================================================
;; MCKL/lib/asm/philox_avx2_32.asm
;;----------------------------------------------------------------------------
;; MCKL: Monte Carlo Kernel Library
;;----------------------------------------------------------------------------
;; Copyright (c) 2013-2018, Yan Zhou
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

global mckl_philox2x32_avx2_kernel
global mckl_philox4x32_avx2_kernel

default rel

%macro rbox 2
    vmovdqa ymm15, [rsp + %1 * 0x20] ; round key

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

%macro generate 4 ; block size, permute constants
%if %1 == 0x08 ; Philox2x32
    vpaddq ymm0, ymm8, [increment2x32 + 0x000]
    vpaddq ymm1, ymm8, [increment2x32 + 0x020]
    vpaddq ymm2, ymm8, [increment2x32 + 0x040]
    vpaddq ymm3, ymm8, [increment2x32 + 0x060]
    vpaddq ymm4, ymm8, [increment2x32 + 0x080]
    vpaddq ymm5, ymm8, [increment2x32 + 0x0A0]
    vpaddq ymm6, ymm8, [increment2x32 + 0x0C0]
    vpaddq ymm7, ymm8, [increment2x32 + 0x0E0]
    vpaddq ymm8, ymm8, [increment2x32 + 0x100]
%elif %1 == 0x10 ; Philox4x32
    vpaddq ymm0, ymm8, [increment4x32 + 0x000]
    vpaddq ymm1, ymm8, [increment4x32 + 0x020]
    vpaddq ymm2, ymm8, [increment4x32 + 0x040]
    vpaddq ymm3, ymm8, [increment4x32 + 0x060]
    vpaddq ymm4, ymm8, [increment4x32 + 0x080]
    vpaddq ymm5, ymm8, [increment4x32 + 0x0A0]
    vpaddq ymm6, ymm8, [increment4x32 + 0x0C0]
    vpaddq ymm7, ymm8, [increment4x32 + 0x0E0]
    vpaddq ymm8, ymm8, [increment4x32 + 0x100]
%else
    %error
%endif
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
    rbox 0, %3
    rbox 1, %3
    rbox 2, %3
    rbox 3, %3
    rbox 4, %3
    rbox 5, %3
    rbox 6, %3
    rbox 7, %3
    rbox 8, %3
    rbox 9, %4
%endmacro

; rdi:ctr.data()
; rsi:n
; rdx:r
; rcx:mul:weyl:key
%macro kernel 4 ; block size, permute constants
    push rbp
    mov rbp, rsp
    shr rsp, 5
    shl rsp, 5
    sub rsp, 0x140 ; 10 round keys
    cld

    ; early return
    test rsi, rsi
    jz .return

    ; parameter processing, leave rdi, rsi, rcx free for moving memory
    mov rax, rsi ; n
    mov r8,  rdi ; ctr
    mov r9,  rcx ; mul:wey:key

    ; load counter
%if %1 == 0x08 ; Philox2x32
    vpbroadcastq ymm8, [r8]
%elif %1 == 0x10 ; Philox4x32
    vbroadcasti128 ymm8, [r8]
%else
    %error
%endif
    add [r8], rax

    ; load multiplier
%if %1 == 0x08 ; Philox2x32
    vpbroadcastq ymm9, [r9]
%elif %1 == 0x10 ; Philox4x32
    vbroadcasti128 ymm9, [r9]
%else
    %error
%endif

    ; load mask
    vmovdqa ymm14, [mask]

    ; compute round keys and store to stack
%if %1 == 0x08 ; Philox2x32
    vpbroadcastq ymm0, [r9 + 0x08] ; weyl
    vpbroadcastq ymm1, [r9 + 0x10] ; key
%elif %1 == 0x10 ; Philox4x32
    vbroadcasti128 ymm0, [r9 + 0x10] ; weyl
    vbroadcasti128 ymm1, [r9 + 0x20] ; key
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

    cmp rax, 0x100 / %1
    jl .last

align 16
.loop:
    generate %1, %2, %3, %4
    vmovdqu [rdx + 0x00], ymm0
    vmovdqu [rdx + 0x20], ymm1
    vmovdqu [rdx + 0x40], ymm2
    vmovdqu [rdx + 0x60], ymm3
    vmovdqu [rdx + 0x80], ymm4
    vmovdqu [rdx + 0xA0], ymm5
    vmovdqu [rdx + 0xC0], ymm6
    vmovdqu [rdx + 0xE0], ymm7
    add rdx, 0x100
    sub rax, 0x100 / %1
    cmp rax, 0x100 / %1
    jge .loop

.last:
    test rax, rax
    jz .return
    generate %1, %2, %3, %4
    vmovdqa [rsp + 0x00], ymm0
    vmovdqa [rsp + 0x20], ymm1
    vmovdqa [rsp + 0x40], ymm2
    vmovdqa [rsp + 0x60], ymm3
    vmovdqa [rsp + 0x80], ymm4
    vmovdqa [rsp + 0xA0], ymm5
    vmovdqa [rsp + 0xC0], ymm6
    vmovdqa [rsp + 0xE0], ymm7
%if %1 == 0x08 ; Philox2x32
    mov rcx, rax
%elif %1 == 0x10 ; Philox4x32
    lea rcx, [rax + rax]
%else
    %error
%endif
    mov rsi, rsp
    mov rdi, rdx
    rep movsq

.return:
    vzeroupper
    mov rsp, rbp
    pop rbp
    ret
%endmacro

section .rodata

align 32

increment2x32:
dq 0x01, 0x02, 0x03, 0x04
dq 0x05, 0x06, 0x07, 0x08
dq 0x09, 0x0A, 0x0B, 0x0C
dq 0x0D, 0x0E, 0x0F, 0x10
dq 0x11, 0x12, 0x13, 0x14
dq 0x15, 0x16, 0x17, 0x18
dq 0x19, 0x1A, 0x1B, 0x1C
dq 0x1D, 0x1E, 0x1F, 0x20
dq 0x20, 0x20, 0x20, 0x20

increment4x32:
dq 0x01, 0x00, 0x02, 0x00
dq 0x03, 0x00, 0x04, 0x00
dq 0x05, 0x00, 0x06, 0x00
dq 0x07, 0x00, 0x08, 0x00
dq 0x09, 0x00, 0x0A, 0x00
dq 0x0B, 0x00, 0x0C, 0x00
dq 0x0D, 0x00, 0x0E, 0x00
dq 0x0F, 0x00, 0x10, 0x00
dq 0x10, 0x00, 0x10, 0x00

mask: times 4 dq 0xFFFF_FFFF_0000_0000

section .text

mckl_philox2x32_avx2_kernel: kernel 0x08, 0xE3, 0xB1, 0xB1
mckl_philox4x32_avx2_kernel: kernel 0x10, 0xC6, 0x93, 0xB1

; vim:ft=nasm
