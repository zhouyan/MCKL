;;============================================================================
;; MCKL/lib/asm/philox_avx512_32.asm
;;----------------------------------------------------------------------------
;; MCKL: Monte Carlo Kernel Library
;;----------------------------------------------------------------------------
;; Copyright (c) 2013-2017, Yan Zhou
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

global mckl_philox2x32_avx512_kernel
global mckl_philox4x32_avx512_kernel

default rel

%macro rbox 2
    vmovdqa32 zmm15, [rsp + %1 * 0x40] ; round key

    vpmuludq zmm10, zmm0, zmm9
    vpmuludq zmm11, zmm1, zmm9
    vpmuludq zmm12, zmm2, zmm9
    vpmuludq zmm13, zmm3, zmm9
    vpandd zmm0, zmm0, zmm14
    vpandd zmm1, zmm1, zmm14
    vpandd zmm2, zmm2, zmm14
    vpandd zmm3, zmm3, zmm14
    vpxord zmm0, zmm0, zmm15
    vpxord zmm1, zmm1, zmm15
    vpxord zmm2, zmm2, zmm15
    vpxord zmm3, zmm3, zmm15
    vpxord zmm0, zmm0, zmm10
    vpxord zmm1, zmm1, zmm11
    vpxord zmm2, zmm2, zmm12
    vpxord zmm3, zmm3, zmm13

    vpmuludq zmm10, zmm4, zmm9
    vpmuludq zmm11, zmm5, zmm9
    vpmuludq zmm12, zmm6, zmm9
    vpmuludq zmm13, zmm7, zmm9
    vpandd zmm4, zmm4, zmm14
    vpandd zmm5, zmm5, zmm14
    vpandd zmm6, zmm6, zmm14
    vpandd zmm7, zmm7, zmm14
    vpxord zmm4, zmm4, zmm15
    vpxord zmm5, zmm5, zmm15
    vpxord zmm6, zmm6, zmm15
    vpxord zmm7, zmm7, zmm15
    vpxord zmm4, zmm4, zmm10
    vpxord zmm5, zmm5, zmm11
    vpxord zmm6, zmm6, zmm12
    vpxord zmm7, zmm7, zmm13

    vpshufd zmm0, zmm0, %2
    vpshufd zmm1, zmm1, %2
    vpshufd zmm2, zmm2, %2
    vpshufd zmm3, zmm3, %2
    vpshufd zmm4, zmm4, %2
    vpshufd zmm5, zmm5, %2
    vpshufd zmm6, zmm6, %2
    vpshufd zmm7, zmm7, %2
%endmacro

%macro generate 4 ; block size, permute constants
%if %1 == 0x08 ; Philox2x32
    vpaddq zmm0, zmm8, [increment2x32 + 0x000]
    vpaddq zmm1, zmm8, [increment2x32 + 0x040]
    vpaddq zmm2, zmm8, [increment2x32 + 0x080]
    vpaddq zmm3, zmm8, [increment2x32 + 0x0C0]
    vpaddq zmm4, zmm8, [increment2x32 + 0x100]
    vpaddq zmm5, zmm8, [increment2x32 + 0x140]
    vpaddq zmm6, zmm8, [increment2x32 + 0x180]
    vpaddq zmm7, zmm8, [increment2x32 + 0x1C0]
    vpaddq zmm8, zmm8, [increment2x32 + 0x200]
%elif %1 == 0x10 ; Philox4x32
    vpaddq zmm0, zmm8, [increment4x32 + 0x000]
    vpaddq zmm1, zmm8, [increment4x32 + 0x040]
    vpaddq zmm2, zmm8, [increment4x32 + 0x080]
    vpaddq zmm3, zmm8, [increment4x32 + 0x0C0]
    vpaddq zmm4, zmm8, [increment4x32 + 0x100]
    vpaddq zmm5, zmm8, [increment4x32 + 0x140]
    vpaddq zmm6, zmm8, [increment4x32 + 0x180]
    vpaddq zmm7, zmm8, [increment4x32 + 0x1C0]
    vpaddq zmm8, zmm8, [increment4x32 + 0x200]
%else
    %error
%endif
%if %2 != 0xE3
    vpshufd zmm0, zmm0, %2
    vpshufd zmm1, zmm1, %2
    vpshufd zmm2, zmm2, %2
    vpshufd zmm3, zmm3, %2
    vpshufd zmm4, zmm4, %2
    vpshufd zmm5, zmm5, %2
    vpshufd zmm6, zmm6, %2
    vpshufd zmm7, zmm7, %2
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
    shr rsp, 6
    shl rsp, 6
    sub rsp, 0x280 ; 10 round keys
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
    vpbroadcastq zmm8, [r8]
%elif %1 == 0x10 ; Philox4x32
    vbroadcasti64x2 zmm8, [r8]
%else
    %error
%endif
    add [r8], rax

    ; load multiplier
%if %1 == 0x08 ; Philox2x32
    vpbroadcastq zmm9, [r9]
%elif %1 == 0x10 ; Philox4x32
    vbroadcasti64x2 zmm9, [r9]
%else
    %error
%endif

    ; load mask
    vmovdqa32 zmm14, [mask]

    ; compute round keys and store to stack
%if %1 == 0x08 ; Philox2x32
    vpbroadcastq zmm0, [r9 + 0x08] ; weyl
    vpbroadcastq zmm1, [r9 + 0x10] ; key
%elif %1 == 0x10 ; Philox4x32
    vbroadcasti64x2 zmm0, [r9 + 0x10] ; weyl
    vbroadcasti64x2 zmm1, [r9 + 0x20] ; key
%else
    %error
%endif
    vmovdqa32 [rsp], zmm1
%assign r 1
%rep 9
    vpaddq zmm1, zmm1, zmm0
    vmovdqa32 [rsp + r * 0x40], zmm1
%assign r r + 1
%endrep

    cmp rax, 0x200 / %1
    jl .last

align 16
.loop:
    generate %1, %2, %3, %4
    vmovdqu32 [rdx + 0x000], zmm0
    vmovdqu32 [rdx + 0x040], zmm1
    vmovdqu32 [rdx + 0x080], zmm2
    vmovdqu32 [rdx + 0x0C0], zmm3
    vmovdqu32 [rdx + 0x100], zmm4
    vmovdqu32 [rdx + 0x140], zmm5
    vmovdqu32 [rdx + 0x180], zmm6
    vmovdqu32 [rdx + 0x1C0], zmm7
    add rdx, 0x200
    sub rax, 0x200 / %1
    cmp rax, 0x200 / %1
    jge .loop

.last:
    test rax, rax
    jz .return
    generate %1, %2, %3, %4
    vmovdqa32 [rsp + 0x000], zmm0
    vmovdqa32 [rsp + 0x040], zmm1
    vmovdqa32 [rsp + 0x080], zmm2
    vmovdqa32 [rsp + 0x0C0], zmm3
    vmovdqa32 [rsp + 0x100], zmm4
    vmovdqa32 [rsp + 0x140], zmm5
    vmovdqa32 [rsp + 0x180], zmm6
    vmovdqa32 [rsp + 0x1C0], zmm7
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

align 64

increment2x32:
dq 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
dq 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
dq 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
dq 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20
dq 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28
dq 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30
dq 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38
dq 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40
dq 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40

increment4x32:
dq 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00
dq 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08, 0x00
dq 0x09, 0x00, 0x0A, 0x00, 0x0B, 0x00, 0x0C, 0x00
dq 0x0D, 0x00, 0x0E, 0x00, 0x0F, 0x00, 0x10, 0x00
dq 0x11, 0x00, 0x12, 0x00, 0x13, 0x00, 0x14, 0x00
dq 0x15, 0x00, 0x16, 0x00, 0x17, 0x00, 0x18, 0x00
dq 0x19, 0x00, 0x1A, 0x00, 0x1B, 0x00, 0x1C, 0x00
dq 0x1D, 0x00, 0x1E, 0x00, 0x1F, 0x00, 0x20, 0x00
dq 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00

mask: times 8 dq 0xFFFF_FFFF_0000_0000

section .text

mckl_philox2x32_avx512_kernel: kernel 0x08, 0xE3, 0xB1, 0xB1
mckl_philox4x32_avx512_kernel: kernel 0x10, 0xC6, 0x93, 0xB1

; vim:ft=nasm
