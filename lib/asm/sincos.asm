;;============================================================================
;; MCKL/lib/asm/sincos.asm
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
;; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;; ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
;; LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
;; CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;; SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;; INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;; CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
;; ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;; POSSIBILITY OF SUCH DAMAGE.
;;============================================================================

global mckl_vd_sin
global mckl_vd_cos
global mckl_vd_sincos
global mckl_vd_tan

%macro sincos 1 ; {{{ implicit input ymm0, output ymm13, ymm14, ymm15
    ; b = abs(a)
    vpand ymm1, ymm0, [rel pmask]

    ; n = trunc(4 * b / pi)
    vmulpd ymm11, ymm1, [rel pi4inv]
    vcvttpd2dq xmm11, ymm11

    ; k = floor((n + 1) / 2) * 2
    vpaddd xmm11, xmm11, [rel ddone]
    vpand xmm11, xmm11, [rel ddmask]

    ; x = a - k * pi / 4
    vcvtdq2pd ymm2, xmm11
    vfnmadd231pd ymm1, ymm2, [rel pi4dp1]
    vfnmadd231pd ymm1, ymm2, [rel pi4dp2]
    vfnmadd231pd ymm1, ymm2, [rel pi4dp3]

    ; sin(x) = c13 * x^13 + ... + c3 * x^3 + x
    ; cos(x) = c14 * x^14 + ... + c2 * x^2 + 1

    vmulpd ymm2, ymm1, ymm1 ; x2 = x^2
    vmulpd ymm3, ymm2, ymm1 ; x3 = x^3
    vmulpd ymm4, ymm2, ymm2 ; x4 = x^4
    vmulpd ymm7, ymm4, ymm3 ; x7 = x^7
    vmulpd ymm8, ymm4, ymm4 ; x8 = x^8

    vmovapd ymm14, [rel c14]
    vmovapd ymm13, [rel c13]
    vmovapd ymm10, [rel c10]
    vmovapd ymm9,  [rel c9]
    vmovapd ymm6,  [rel c6]
    vmovapd ymm5,  [rel c5]
    vmovapd ymm12, [rel c2]

    vfmadd213pd ymm14, ymm2, [rel c12] ; u14 = c14 * x^2 + c12
    vfmadd213pd ymm13, ymm2, [rel c11] ; u13 = c13 * x^2 + c11
    vfmadd213pd ymm10, ymm2, [rel c8]  ; u10 = c10 * x^2 + c8
    vfmadd213pd ymm9,  ymm2, [rel c7]  ; u9  = c9  * x^2 + c7
    vfmadd213pd ymm6,  ymm2, [rel c4]  ; u6  = c6  * x^2 + c4
    vfmadd213pd ymm5,  ymm2, [rel c3]  ; u5  = c5  * x^2 + c3
    vfmadd213pd ymm12, ymm2, [rel c0]  ; u5  = c2  * x^2 + c0

    vfmadd213pd ymm14, ymm4, ymm10 ; v14 = u14 * x^4 + u10
    vfmadd213pd ymm13, ymm4, ymm9  ; v13 = u13 * x^4 + u9
    vfmadd213pd ymm6,  ymm4, ymm12 ; v6  = u6  * x^4 + u5
    vfmadd213pd ymm5,  ymm3, ymm1  ; v5  = u5  * x^3 + x

    vfmadd213pd ymm14, ymm8, ymm6 ; z14 = v14 * x^8 + v6
    vfmadd213pd ymm13, ymm7, ymm5 ; z13 = v13 * x^7 + v5

    ; swap
    vpmovsxdq ymm11, xmm11
    vpsllq ymm1, ymm11, 62
    %if (%1 & 0x1) != 0 ; sin(a)
        vblendvpd ymm3, ymm13, ymm14, ymm1
    %endif
    %if (%1 & 0x2) != 0 ; cos(a)
        vblendvpd ymm4, ymm14, ymm13, ymm1
    %endif

    ; signs
    %if (%1 & 0x1) != 0 ; sin(a)
        vpsllq ymm1, ymm11, 61
        vpxor ymm1, ymm1, ymm0
        vpand ymm1, ymm1, [rel smask]
        vpxor ymm13, ymm3, ymm1
    %endif
    %if (%1 & 0x2) != 0 ; cos(a)
        vpaddq ymm1, ymm11, [rel dqtwo]
        vpsllq ymm1, ymm1, 61
        vpand ymm1, ymm1, [rel smask]
        vpxor ymm14, ymm4, ymm1
    %endif
    %if (%1 & 0x4) != 0 ; tan(a)
        vdivpd ymm15, ymm13, ymm14
    %endif

    vcmpltpd ymm1, ymm0, [rel min_a]
    vcmpgtpd ymm2, ymm0, [rel max_a]
    vcmpneqpd ymm3, ymm0, ymm0
    vpor ymm4, ymm1, ymm2
    vpor ymm4, ymm4, ymm3
    vtestpd ymm4, ymm4
    jz %%skip
    %if (%1 & 0x1) != 0 ; sin(a)
        vblendvpd ymm13, ymm13, [rel min_y], ymm1
        vblendvpd ymm13, ymm13, [rel max_y], ymm2
        vblendvpd ymm13, ymm13, ymm0, ymm3
    %endif
    %if (%1 & 0x2) != 0 ; cos(a)
        vblendvpd ymm14, ymm14, [rel min_y], ymm1
        vblendvpd ymm14, ymm14, [rel max_y], ymm2
        vblendvpd ymm14, ymm14, ymm0, ymm3
    %if (%1 & 0x4) != 0 ; tan(a)
    %endif
        vblendvpd ymm15, ymm15, [rel min_y], ymm1
        vblendvpd ymm15, ymm15, [rel max_y], ymm2
        vblendvpd ymm15, ymm15, ymm0, ymm3
    %endif
    %%skip:
%endmacro ; }}}


%macro sincos_loop 1 ; rdi:n, rsi:a, rdx:y, rcx:z {{{
    test rdi, rdi
    jz .return

    mov rax, rdi
    mov r8,  rsi
    mov r9,  rcx

    cmp rax, 4
    jl .last

    .loop:
        vmovupd ymm0, [r8]
        sincos %1
        %if %1 == 0x1 ; vd_sin
            vmovupd [rdx], ymm13
        %elif %1 == 0x2 ; vd_cos
            vmovupd [rdx], ymm14
        %elif %1 == 0x3 ; vd_sincos
            vmovupd [rdx], ymm13
            vmovupd [r9],  ymm14
        %elif %1 == 0x7
            vmovupd [rdx], ymm15
        %else
            %error
        %endif
        add r8, 0x20
        add rdx, 0x20
        %if %1 == 0x3 ; vd_sincos
            add r9, 0x20
        %endif
        sub rax, 4
        cmp rax, 4
        jge .loop

    .last:
        test rax, rax
        jz .return
        cld
        mov rcx, rax
        mov rsi, r8
        mov rdi, rsp
        sub rdi, 0x28
        rep movsq
        vmovupd ymm0, [rsp - 0x28]
        sincos %1
        %if %1 == 0x1 ; vd_sin
            vmovupd [rsp - 0x28], ymm13
        %elif %1 == 0x2 ; vd_cos
            vmovupd [rsp - 0x28], ymm14
        %elif %1 == 0x3 ; vd_sincos
            vmovupd [rsp - 0x28], ymm13
            vmovupd [rsp - 0x48], ymm14
        %elif %1 == 0x7
            vmovupd [rsp - 0x28], ymm15
        %else
            %error
        %endif
        mov rcx, rax
        mov rsi, rsp
        sub rsi, 0x28
        mov rdi, rdx
        rep movsq
        %if %1 == 0x3 ; vd_sincos
            mov rcx, rax
            mov rsi, rsp
            sub rsi, 0x48
            mov rdi, r9
            rep movsq
        %endif

    .return:
        ret
%endmacro ; }}}

section .rodata

align 32

min_a: times 4 dq 0xC1D0000000000000 ; -2^30
max_a: times 4 dq 0x41D0000000000000 ; 2^30
min_y: times 4 dq 0x7FF8000000000000 ; NaN
max_y: times 4 dq 0x7FF8000000000000 ; NaN

c0:  times 4 dq 0x3FF0000000000000
c1:  times 4 dq 0xBFE0000000000000
c2:  times 4 dq 0xBFE0000000000000
c3:  times 4 dq 0xBFC5555555555549
c4:  times 4 dq 0x3FA555555555554C
c5:  times 4 dq 0x3F8111111110F8A6
c6:  times 4 dq 0xBF56C16C16C15177
c7:  times 4 dq 0xBF2A01A019C161D5
c8:  times 4 dq 0x3EFA01A019CB1590
c9:  times 4 dq 0x3EC71DE357B1FE7D
c10: times 4 dq 0xBE927E4F809C52AD
c11: times 4 dq 0xBE5AE5E68A2B9CEB
c12: times 4 dq 0x3E21EE9EBDB4B1C4
c13: times 4 dq 0x3DE5D93A5ACFD57C
c14: times 4 dq 0xBDA8FAE9BE8838D4

ddmask: times 8 dd 0xFFFFFFFE ; ~1
ddone:  times 8 dd 0x00000001 ; 1
dqtwo:  times 4 dq 0x0000000000000002 ; 2
pmask:  times 4 dq 0x7FFFFFFFFFFFFFFF ; abs(x)  = x & pmask
smask:  times 4 dq 0x8000000000000000 ; sign(x) = x & smask
pi4dp1: times 4 dq 0x3FE921FB50000000
pi4dp2: times 4 dq 0x3E4110B460000000
pi4dp3: times 4 dq 0x3C81A62633145C07
pi4inv: times 4 dq 0x3FF45F306DC9C883 ; 4.0l / pi

section .text

mckl_vd_sin:    sincos_loop 0x1
mckl_vd_cos:    sincos_loop 0x2
mckl_vd_sincos: sincos_loop 0x3
mckl_vd_tan:    sincos_loop 0x7

; vim:ft=nasm
