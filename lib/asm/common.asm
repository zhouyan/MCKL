;;============================================================================
;; MCKL/lib/asm/common.asm
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

%use smartalign

%macro prologue 2
    push rbp
    mov rbp, rsp
    %if %1 > 4
        and rsp, ((0xFFFFFFFFFFFFFFFF >> %1) << %1)
    %endif
    %if %2 != 0
        sub rsp, %2
    %endif
%endmacro

%macro epilogue 0
    .return:
        mov rsp, rbp
        pop rbp
        ret
%endmacro

%macro increment_xmm_64_2 0
    vpaddq xmm0, xmm8, [rel increment_xmm_64_2_0]
    vpaddq xmm1, xmm8, [rel increment_xmm_64_2_1]
    vpaddq xmm2, xmm8, [rel increment_xmm_64_2_2]
    vpaddq xmm3, xmm8, [rel increment_xmm_64_2_3]
    vpaddq xmm4, xmm8, [rel increment_xmm_64_2_4]
    vpaddq xmm5, xmm8, [rel increment_xmm_64_2_5]
    vpaddq xmm6, xmm8, [rel increment_xmm_64_2_6]
    vpaddq xmm7, xmm8, [rel increment_xmm_64_2_7]
    vpaddq xmm8, xmm8, [rel increment_xmm_64_2_8]
%endmacro

%macro increment_ymm_64_1 0
    vpaddq ymm0, ymm8, [rel increment_ymm_64_1_0]
    vpaddq ymm1, ymm8, [rel increment_ymm_64_1_1]
    vpaddq ymm2, ymm8, [rel increment_ymm_64_1_2]
    vpaddq ymm3, ymm8, [rel increment_ymm_64_1_3]
    vpaddq ymm4, ymm8, [rel increment_ymm_64_1_4]
    vpaddq ymm5, ymm8, [rel increment_ymm_64_1_5]
    vpaddq ymm6, ymm8, [rel increment_ymm_64_1_6]
    vpaddq ymm7, ymm8, [rel increment_ymm_64_1_7]
    vpaddq ymm8, ymm8, [rel increment_ymm_64_1_8]
%endmacro

%macro increment_ymm_64_2 0
    vpaddq ymm0, ymm8, [rel increment_ymm_64_2_0]
    vpaddq ymm1, ymm8, [rel increment_ymm_64_2_1]
    vpaddq ymm2, ymm8, [rel increment_ymm_64_2_2]
    vpaddq ymm3, ymm8, [rel increment_ymm_64_2_3]
    vpaddq ymm4, ymm8, [rel increment_ymm_64_2_4]
    vpaddq ymm5, ymm8, [rel increment_ymm_64_2_5]
    vpaddq ymm6, ymm8, [rel increment_ymm_64_2_6]
    vpaddq ymm7, ymm8, [rel increment_ymm_64_2_7]
    vpaddq ymm8, ymm8, [rel increment_ymm_64_2_8]
%endmacro

section .data

align 16
increment_xmm_64_2_0:
dq 1
dq 0

align 16
increment_xmm_64_2_1:
dq 2
dq 0

align 16
increment_xmm_64_2_2:
dq 3
dq 0

align 16
increment_xmm_64_2_3:
dq 4
dq 0

align 16
increment_xmm_64_2_4:
dq 5
dq 0

align 16
increment_xmm_64_2_5:
dq 6
dq 0

align 16
increment_xmm_64_2_6:
dq 7
dq 0

align 16
increment_xmm_64_2_7:
dq 8
dq 0

align 16
increment_xmm_64_2_8:
dq 8
dq 0

align 32
increment_ymm_64_1_0:
dq 0x01
dq 0x02
dq 0x03
dq 0x04

align 32
increment_ymm_64_1_1:
dq 0x05
dq 0x06
dq 0x07
dq 0x08

align 32
increment_ymm_64_1_2:
dq 0x09
dq 0x0A
dq 0x0B
dq 0x0C

align 32
increment_ymm_64_1_3:
dq 0x0D
dq 0x0E
dq 0x0F
dq 0x10

align 32
increment_ymm_64_1_4:
dq 0x11
dq 0x12
dq 0x13
dq 0x14

align 32
increment_ymm_64_1_5:
dq 0x15
dq 0x16
dq 0x17
dq 0x18

align 32
increment_ymm_64_1_6:
dq 0x19
dq 0x1A
dq 0x1B
dq 0x1C

align 32
increment_ymm_64_1_7:
dq 0x1D
dq 0x1E
dq 0x1F
dq 0x20

align 32
increment_ymm_64_1_8:
dq 0x20
dq 0x20
dq 0x20
dq 0x20

align 32
increment_ymm_64_2_0:
dq 0x01
dq 0
dq 0x02
dq 0

align 32
increment_ymm_64_2_1:
dq 0x03
dq 0
dq 0x04
dq 0

align 32
increment_ymm_64_2_2:
dq 0x05
dq 0
dq 0x06
dq 0

align 32
increment_ymm_64_2_3:
dq 0x07
dq 0
dq 0x08
dq 0

align 32
increment_ymm_64_2_4:
dq 0x09
dq 0
dq 0x0A
dq 0

align 32
increment_ymm_64_2_5:
dq 0x0B
dq 0
dq 0x0C
dq 0

align 32
increment_ymm_64_2_6:
dq 0x0D
dq 0
dq 0x0E
dq 0

align 32
increment_ymm_64_2_7:
dq 0x0F
dq 0
dq 0x10
dq 0

align 32
increment_ymm_64_2_8:
dq 0x10
dq 0
dq 0x10
dq 0

; vim:ft=nasm
