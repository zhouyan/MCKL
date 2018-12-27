;;============================================================================
;; MCKL/lib/asm/sincos.asm
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

%include "/math.asm"

global mckl_vd_sin
global mckl_vd_cos
global mckl_vd_sincos
global mckl_vd_tan

default rel

%macro sincostan 1 ; implicit input ymm0, output ymm13, ymm14, ymm15
    ; b = abs(a)
    vandpd ymm1, ymm0, [pmask]
    vcmpgtpd ymm2, ymm1, [nan_a]
    vcmpgtpd ymm5, ymm1, [max_a]
    vblendvpd ymm0, ymm0, [nan_y], ymm2
    vtestpd ymm5, ymm5
    jz %%inrange

    ; reduce ymm1 to around 2^53 if ymm1 > 2^53
    vandpd ymm2, ymm1, [max253]
    vcmpgtpd ymm3, ymm1, [pow253]
    vblendvpd ymm1, ymm1, ymm2, ymm3

    ; reduce ymm1 to ymm1 - k * 2 * pi if ymm1 > max_a
    vsubpd ymm4, ymm1, [max_a]
    vmulpd ymm4, ymm4, [pi2inv]
    vroundpd ymm4, ymm4, 0xA
    vmovapd ymm2, ymm1
    vfnmadd231pd ymm2, ymm4, [pi2]
    vblendvpd ymm1, ymm1, ymm2, ymm5

    %%inrange:

    ; n = trunc(4 * b / pi)
    vmulpd ymm11, ymm1, [pi4inv]
    vcvttpd2dq xmm11, ymm11

    ; k = floor((n + 1) / 2) * 2
    vpaddd xmm11, xmm11, [ddone]
    vandpd xmm11, xmm11, [ddmask]

    ; x = a - k * pi / 4
    vcvtdq2pd ymm2, xmm11
    vfnmadd231pd ymm1, ymm2, [pi4dp1]
    vfnmadd231pd ymm1, ymm2, [pi4dp2]
    vfnmadd231pd ymm1, ymm2, [pi4dp3]

    ; sin(x) = c13 * x^13 + ... + c3 * x^3 + x
    ; cos(x) = c14 * x^14 + ... + c2 * x^2 + 1

    vmovapd ymm14, [c14]
    vmovapd ymm13, [c13]
    vmovapd ymm10, [c10]
    vmovapd ymm9,  [c9]
    vmovapd ymm6,  [c6]
    vmovapd ymm5,  [c5]
    vmovapd ymm12, [c2]

    vmulpd ymm2, ymm1, ymm1 ; x2 = x^2
    vmulpd ymm3, ymm2, ymm1 ; x3 = x^3
    vmulpd ymm4, ymm2, ymm2 ; x4 = x^4
    vmulpd ymm7, ymm4, ymm3 ; x7 = x^7
    vmulpd ymm8, ymm4, ymm4 ; x8 = x^8

    vfmadd213pd ymm14, ymm2, [c12] ; u14 = c14 * x^2 + c12
    vfmadd213pd ymm13, ymm2, [c11] ; u13 = c13 * x^2 + c11
    vfmadd213pd ymm10, ymm2, [c8]  ; u10 = c10 * x^2 + c8
    vfmadd213pd ymm9,  ymm2, [c7]  ; u9  = c9  * x^2 + c7
    vfmadd213pd ymm6,  ymm2, [c4]  ; u6  = c6  * x^2 + c4
    vfmadd213pd ymm5,  ymm2, [c3]  ; u5  = c5  * x^2 + c3
    vfmadd213pd ymm12, ymm2, [c0]  ; u5  = c2  * x^2 + c0

    vfmadd213pd ymm14, ymm4, ymm10 ; v14 = u14 * x^4 + u10
    vfmadd213pd ymm13, ymm4, ymm9  ; v13 = u13 * x^4 + u9
    vfmadd213pd ymm6,  ymm4, ymm12 ; v6  = u6  * x^4 + u5
    vfmadd213pd ymm5,  ymm3, ymm1  ; v5  = u5  * x^3 + x

    vfmadd213pd ymm14, ymm8, ymm6 ; z14 = v14 * x^8 + v6
    vfmadd213pd ymm13, ymm7, ymm5 ; z13 = v13 * x^7 + v5

    ; swap
    vpmovsxdq ymm11, xmm11
    vpsllq ymm1, ymm11, 62
%if %1 & 0x5 ; sin(a), tan(a)
    vblendvpd ymm3, ymm13, ymm14, ymm1
%endif
%if %1 & 0x6 ; cos(a), tan(a)
    vblendvpd ymm4, ymm14, ymm13, ymm1
%endif

    ; signs
%if %1 & 0x5 ; sin(a), tan(a)
    vpsllq ymm1, ymm11, 61
    vxorpd ymm1, ymm1, ymm0
    vandpd ymm1, ymm1, [smask]
    vxorpd ymm13, ymm3, ymm1
%endif
%if %1 & 0x6 ; cos(a), tan(a)
    vpaddq ymm1, ymm11, [dqtwo]
    vpsllq ymm1, ymm1, 61
    vandpd ymm1, ymm1, [smask]
    vxorpd ymm14, ymm4, ymm1
%endif
%if %1 & 0x4 ; tan(a)
    vdivpd ymm15, ymm13, ymm14
%endif

    vcmpneqpd ymm1, ymm0, ymm0 ; a != a
    vtestpd ymm1, ymm1
    jz %%skip
%if %1 & 0x1 ; sin(a)
    vblendvpd ymm13, ymm13, ymm0, ymm1
%endif
%if %1 & 0x2 ; cos(a)
    vblendvpd ymm14, ymm14, ymm0, ymm1
%endif
%if %1 & 0x4 ; tan(a)
    vblendvpd ymm15, ymm15, ymm0, ymm1
%endif
%%skip:
%endmacro

%macro sin_constants 0
%endmacro

%macro cos_constants 0
%endmacro

%macro sincos_constants 0
%endmacro

%macro tan_constants 0
%endmacro

%macro sin 2
    vmovupd ymm0, %2
    sincostan 0x1
    vmovupd %1, ymm13
%endmacro

%macro cos 2
    vmovupd ymm0, %2
    sincostan 0x2
    vmovupd %1, ymm14
%endmacro

%macro sincos 3
    vmovupd ymm0, %3
    sincostan 0x3
    vmovupd %1, ymm13
    vmovupd %2, ymm14
%endmacro

%macro tan 2
    vmovupd ymm0, %2
    sincostan 0x4
    vmovupd %1, ymm15
%endmacro

section .rodata

align 32

nan_a: times 4 dq 0x7FEFFFFFFFFFFFFF ; DBL_MAX
max_a: times 4 dq 0x41D921FB5411E920 ; 1686629712.279854
nan_y: times 4 dq 0x7FF8000000000000 ; NaN

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
pi2:    times 4 dq 0x401921FB54442D18 ; 2 * pi
pi2inv: times 4 dq 0x3FC45F306DC9C883 ; 1.0l / (2 * pi)
pow253: times 4 dq 0x4340000000000000 ; 2^53
max253: times 4 dq 0x434FFFFFFFFFFFFF ; 2^53 * 1.1...1b

section .text

mckl_vd_sin:    math_kernel_a1r1 8, sin
mckl_vd_cos:    math_kernel_a1r1 8, cos
mckl_vd_sincos: math_kernel_a1r2 8, sincos
mckl_vd_tan:    math_kernel_a1r1 8, tan

; vim:ft=nasm
