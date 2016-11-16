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
    and rsp, ((0xFFFFFFFFFFFFFFFF >> %1) << %1)
    sub rsp, %2
%endmacro

%macro epilogue 0
    .return:
        vzeroupper
        mov rsp, rbp
        pop rbp
        ret
%endmacro

%macro increment_xmm 2
    %if %2 == 0x08
        vpaddq xmm0, %1, [rel increment_xmm_data_1 + 0x00]
        vpaddq xmm1, %1, [rel increment_xmm_data_1 + 0x10]
        vpaddq xmm2, %1, [rel increment_xmm_data_1 + 0x20]
        vpaddq xmm3, %1, [rel increment_xmm_data_1 + 0x30]
        vpaddq xmm4, %1, [rel increment_xmm_data_1 + 0x40]
        vpaddq xmm5, %1, [rel increment_xmm_data_1 + 0x50]
        vpaddq xmm6, %1, [rel increment_xmm_data_1 + 0x60]
        vpaddq xmm7, %1, [rel increment_xmm_data_1 + 0x70]
        vpaddq %1,   %1, [rel increment_xmm_data_1 + 0x80]
    %elif %2 == 0x10
        vpaddq xmm0, %1, [rel increment_xmm_data_2 + 0x00]
        vpaddq xmm1, %1, [rel increment_xmm_data_2 + 0x10]
        vpaddq xmm2, %1, [rel increment_xmm_data_2 + 0x20]
        vpaddq xmm3, %1, [rel increment_xmm_data_2 + 0x30]
        vpaddq xmm4, %1, [rel increment_xmm_data_2 + 0x40]
        vpaddq xmm5, %1, [rel increment_xmm_data_2 + 0x50]
        vpaddq xmm6, %1, [rel increment_xmm_data_2 + 0x60]
        vpaddq xmm7, %1, [rel increment_xmm_data_2 + 0x70]
        vpaddq %1,   %1, [rel increment_xmm_data_2 + 0x80]
    %else
        %error
    %endif
%endmacro

%macro increment_ymm 2
    %if %2 == 0x08
        vpaddq ymm0, %1, [rel increment_ymm_data_1 + 0x000]
        vpaddq ymm1, %1, [rel increment_ymm_data_1 + 0x020]
        vpaddq ymm2, %1, [rel increment_ymm_data_1 + 0x040]
        vpaddq ymm3, %1, [rel increment_ymm_data_1 + 0x060]
        vpaddq ymm4, %1, [rel increment_ymm_data_1 + 0x080]
        vpaddq ymm5, %1, [rel increment_ymm_data_1 + 0x0A0]
        vpaddq ymm6, %1, [rel increment_ymm_data_1 + 0x0C0]
        vpaddq ymm7, %1, [rel increment_ymm_data_1 + 0x0E0]
        vpaddq %1,   %1, [rel increment_ymm_data_1 + 0x100]
    %elif %2 == 0x10
        vpaddq ymm0, %1, [rel increment_ymm_data_2 + 0x000]
        vpaddq ymm1, %1, [rel increment_ymm_data_2 + 0x020]
        vpaddq ymm2, %1, [rel increment_ymm_data_2 + 0x040]
        vpaddq ymm3, %1, [rel increment_ymm_data_2 + 0x060]
        vpaddq ymm4, %1, [rel increment_ymm_data_2 + 0x080]
        vpaddq ymm5, %1, [rel increment_ymm_data_2 + 0x0A0]
        vpaddq ymm6, %1, [rel increment_ymm_data_2 + 0x0C0]
        vpaddq ymm7, %1, [rel increment_ymm_data_2 + 0x0E0]
        vpaddq %1,   %1, [rel increment_ymm_data_2 + 0x100]
    %elif %2 == 0x20
        vpaddq ymm0, %1, [rel increment_ymm_data_4 + 0x000]
        vpaddq ymm1, %1, [rel increment_ymm_data_4 + 0x020]
        vpaddq ymm2, %1, [rel increment_ymm_data_4 + 0x040]
        vpaddq ymm3, %1, [rel increment_ymm_data_4 + 0x060]
        vpaddq ymm4, %1, [rel increment_ymm_data_4 + 0x080]
        vpaddq ymm5, %1, [rel increment_ymm_data_4 + 0x0A0]
        vpaddq ymm6, %1, [rel increment_ymm_data_4 + 0x0C0]
        vpaddq ymm7, %1, [rel increment_ymm_data_4 + 0x0E0]
        vpaddq %1,   %1, [rel increment_ymm_data_4 + 0x100]
    %elif %2 == 0x40
        vpaddq ymm0, %1, [rel increment_ymm_data_8 + 0x000]
        vpaddq ymm2, %1, [rel increment_ymm_data_8 + 0x020]
        vpaddq ymm4, %1, [rel increment_ymm_data_8 + 0x040]
        vpaddq ymm6, %1, [rel increment_ymm_data_8 + 0x060]
        vpaddq %1,   %1, [rel increment_ymm_data_8 + 0x080]
    %else
        %error
    %endif
%endmacro

section .data

align 16
increment_xmm_data_1:
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
increment_xmm_data_2:
dq 0x01, 0x00
dq 0x02, 0x00
dq 0x03, 0x00
dq 0x04, 0x00
dq 0x05, 0x00
dq 0x06, 0x00
dq 0x07, 0x00
dq 0x08, 0x00
dq 0x08, 0x00

align 32
increment_ymm_data_1:
dq 0x01, 0x02, 0x03, 0x04
dq 0x05, 0x06, 0x07, 0x08
dq 0x09, 0x0A, 0x0B, 0x0C
dq 0x0D, 0x0E, 0x0F, 0x10
dq 0x11, 0x12, 0x13, 0x14
dq 0x15, 0x16, 0x17, 0x18
dq 0x19, 0x1A, 0x1B, 0x1C
dq 0x1D, 0x1E, 0x1F, 0x20
dq 0x20, 0x20, 0x20, 0x20

align 32
increment_ymm_data_2:
dq 0x01, 0x00, 0x02, 0x00
dq 0x03, 0x00, 0x04, 0x00
dq 0x05, 0x00, 0x06, 0x00
dq 0x07, 0x00, 0x08, 0x00
dq 0x09, 0x00, 0x0A, 0x00
dq 0x0B, 0x00, 0x0C, 0x00
dq 0x0D, 0x00, 0x0E, 0x00
dq 0x0F, 0x00, 0x10, 0x00
dq 0x10, 0x00, 0x10, 0x00

align 32
increment_ymm_data_4:
dq 0x01, 0x00, 0x00, 0x00
dq 0x02, 0x00, 0x00, 0x00
dq 0x03, 0x00, 0x00, 0x00
dq 0x04, 0x00, 0x00, 0x00
dq 0x05, 0x00, 0x00, 0x00
dq 0x06, 0x00, 0x00, 0x00
dq 0x07, 0x00, 0x00, 0x00
dq 0x08, 0x00, 0x00, 0x00
dq 0x08, 0x00, 0x00, 0x00

align 32
increment_ymm_data_8:
dq 0x01, 0x00, 0x00, 0x00
dq 0x02, 0x00, 0x00, 0x00
dq 0x03, 0x00, 0x00, 0x00
dq 0x04, 0x00, 0x00, 0x00
dq 0x04, 0x00, 0x00, 0x00

; vim:ft=nasm
