# ============================================================================
#  MCKL/example/math/CMakeLists.txt
# ----------------------------------------------------------------------------
#  MCKL: Monte Carlo Kernel Library
# ----------------------------------------------------------------------------
#  Copyright (c) 2013-2017, Yan Zhou
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#    Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
# ============================================================================

project(MCKLExample-math CXX)

mckl_add_example(math)

mckl_add_test(math vmf)

if(AVX2_FOUND AND FMA_FOUND)
    mckl_add_test(math fma)
    if(MCKL_ENABLE_LIBRARY)
        add_custom_target(math_asm)
        add_custom_target(math_asm-check)
        set(MATH_ASM_FUNCTIONS
            sqrtf sqrt
            expf exp exp2f exp2 expm1f expm1
            logf log log2f log2 log10f log10 log1pf log1p
            sin cos sincos tan)
        foreach(f ${MATH_ASM_FUNCTIONS})
            mckl_add_test(math ${f})
            add_dependencies(math_asm math_${f})
            add_dependencies(math_asm-check math_${f}-check)
        endforeach(f ${MATH_ASM_FUNCTIONS})
    endif(MCKL_ENABLE_LIBRARY)
endif(AVX2_FOUND AND FMA_FOUND)

mckl_add_plot(math asm)
