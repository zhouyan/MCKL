# ============================================================================
#  MCKL/lib/CMakeLists.txt
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
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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

project(MCKLLibrary C)

include_directories(${PROJECT_SOURCE_DIR}/asm)

set(MCKL_LIB_ASM ${MCKL_LIB_ASM}
    ${PROJECT_SOURCE_DIR}/asm/sqrtf.asm
    ${PROJECT_SOURCE_DIR}/asm/sqrt.asm
    ${PROJECT_SOURCE_DIR}/asm/expf.asm
    ${PROJECT_SOURCE_DIR}/asm/exp.asm
    ${PROJECT_SOURCE_DIR}/asm/logf.asm
    ${PROJECT_SOURCE_DIR}/asm/log.asm
    ${PROJECT_SOURCE_DIR}/asm/sincos.asm
    ${PROJECT_SOURCE_DIR}/asm/fma.asm
    ${PROJECT_SOURCE_DIR}/asm/aes_aesni_avx2.asm
    ${PROJECT_SOURCE_DIR}/asm/aes_aesni_sse2.asm
    ${PROJECT_SOURCE_DIR}/asm/philox_avx2_32.asm
    ${PROJECT_SOURCE_DIR}/asm/philox_bmi2_2x64.asm
    ${PROJECT_SOURCE_DIR}/asm/philox_bmi2_4x64.asm
    ${PROJECT_SOURCE_DIR}/asm/philox_sse2_32.asm)

set_source_files_properties(${MCKL_LIB_ASM}
    PROPERTIES OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/asm/math.asm)

if (APPLE)
    add_compile_options(--prefix _)
endif (APPLE)

add_library(libmckl STATIC ${MCKL_LIB_ASM})
set_target_properties(libmckl PROPERTIES OUTPUT_NAME mckl)
add_dependencies(lib libmckl)

if(NOT DEFINED MCKL_INSTALL_LIB_DIR)
    SET(MCKL_INSTALL_LIB_DIR lib)
endif(NOT DEFINED MCKL_INSTALL_LIB_DIR)
install(TARGETS libmckl DESTINATION ${MCKL_INSTALL_LIB_DIR})
