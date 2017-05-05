;;============================================================================
;; MCKL/lib/asm/philox_sse2_32.asm
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

global mckl_philox2x32_sse2_kernel
global mckl_philox4x32_sse2_kernel

default rel

%macro rbox 2
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
%endmacro

%macro generate 4 ; block size, permute constants
    movdqa xmm0, xmm8
    movdqa xmm1, xmm8
    movdqa xmm2, xmm8
    movdqa xmm3, xmm8
    movdqa xmm4, xmm8
    movdqa xmm5, xmm8
    movdqa xmm6, xmm8
    movdqa xmm7, xmm8
%if %1 == 0x08 ; Philox2x32
    paddq xmm0, [increment2x32 + 0x00]
    paddq xmm1, [increment2x32 + 0x10]
    paddq xmm2, [increment2x32 + 0x20]
    paddq xmm3, [increment2x32 + 0x30]
    paddq xmm4, [increment2x32 + 0x40]
    paddq xmm5, [increment2x32 + 0x50]
    paddq xmm6, [increment2x32 + 0x60]
    paddq xmm7, [increment2x32 + 0x70]
    paddq xmm8, [increment2x32 + 0x80]
%elif %1 == 0x10 ; Philox4x32
    paddq xmm0, [increment4x32 + 0x00]
    paddq xmm1, [increment4x32 + 0x10]
    paddq xmm2, [increment4x32 + 0x20]
    paddq xmm3, [increment4x32 + 0x30]
    paddq xmm4, [increment4x32 + 0x40]
    paddq xmm5, [increment4x32 + 0x50]
    paddq xmm6, [increment4x32 + 0x60]
    paddq xmm7, [increment4x32 + 0x70]
    paddq xmm8, [increment4x32 + 0x80]
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
    sub rsp, 0xA0 ; 10 round keys
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
    movq xmm8, [r8]
    pshufd xmm8, xmm8, 0x44
%elif %1 == 0x10 ; Philox4x32
    movdqu xmm8, [r8]
%else
    %error
%endif
    add [r8], rax

    ; load multiplier
%if %1 == 0x08 ; Philox2x32
    movq xmm9, [r9]
    pshufd xmm9, xmm9, 0x44
%elif %1 == 0x10 ; Philox4x32
    movdqu xmm9, [r9]
%else
    %error
%endif

    ; load mask
    movdqa xmm14, [mask]

    ; compute round keys and store to stack
%if %1 == 0x08
    movq xmm0, [r9 + 0x08] ; weyl
    movq xmm1, [r9 + 0x10] ; key
    pshufd xmm0, xmm0, 0x44
    pshufd xmm1, xmm1, 0x44
%elif %1 == 0x10
    movdqu xmm0, [r9 + 0x10] ; weyl
    movdqu xmm1, [r9 + 0x20] ; key
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

    cmp rax, 0x80 / %1
    jl .last

align 16
.loop:
    generate %1, %2, %3, %4
    movdqu [rdx + 0x00], xmm0
    movdqu [rdx + 0x10], xmm1
    movdqu [rdx + 0x20], xmm2
    movdqu [rdx + 0x30], xmm3
    movdqu [rdx + 0x40], xmm4
    movdqu [rdx + 0x50], xmm5
    movdqu [rdx + 0x60], xmm6
    movdqu [rdx + 0x70], xmm7
    add rdx, 0x80
    sub rax, 0x80 / %1
    cmp rax, 0x80 / %1
    jge .loop

.last:
    test rax, rax
    jz .return
    generate %1, %2, %3, %4
    movdqa [rsp + 0x00], xmm0
    movdqa [rsp + 0x10], xmm1
    movdqa [rsp + 0x20], xmm2
    movdqa [rsp + 0x30], xmm3
    movdqa [rsp + 0x40], xmm4
    movdqa [rsp + 0x50], xmm5
    movdqa [rsp + 0x60], xmm6
    movdqa [rsp + 0x70], xmm7
%if %1 == 0x08
    mov rcx, rax
%elif %1 == 0x10
    lea rcx, [rax + rax]
%else
    %error
%endif
    mov rsi, rsp
    mov rdi, rdx
    rep movsq

.return:
    mov rsp, rbp
    pop rbp
    ret
%endmacro

section .rodata

align 16

increment2x32:
dq 0x01, 0x02
dq 0x03, 0x04
dq 0x05, 0x06
dq 0x07, 0x08
dq 0x09, 0x0A
dq 0x0B, 0x0C
dq 0x0D, 0x0E
dq 0x0F, 0x10
dq 0x10, 0x10

increment4x32:
dq 0x01, 0x00
dq 0x02, 0x00
dq 0x03, 0x00
dq 0x04, 0x00
dq 0x05, 0x00
dq 0x06, 0x00
dq 0x07, 0x00
dq 0x08, 0x00
dq 0x08, 0x00

mask: times 2 dq 0xFFFF_FFFF_0000_0000

section .text

mckl_philox2x32_sse2_kernel: kernel 0x08, 0xE3, 0xB1, 0xB1
mckl_philox4x32_sse2_kernel: kernel 0x10, 0xC6, 0x93, 0xB1

; vim:ft=nasm
