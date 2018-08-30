//============================================================================
// MCKL/example/mckl/src/mckl_config.cpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#include <mckl/internal/common.hpp>

#define MCKL_EXAMPLE_CONFIG(Macro)                                            \
    std::cout << std::setw(40) << std::left << ("MCKL_" #Macro)               \
              << MCKL_##Macro << std::endl;

int main()
{
    MCKL_EXAMPLE_CONFIG(NO_RUNTIME_ASSERT)
    MCKL_EXAMPLE_CONFIG(RUNTIME_ASSERT_AS_EXCEPTION)
    MCKL_EXAMPLE_CONFIG(REQUIRE_ENDIANNESS_NEUTURAL)
    MCKL_EXAMPLE_CONFIG(HAS_POSIX)
    MCKL_EXAMPLE_CONFIG(HAS_OMP)
    MCKL_EXAMPLE_CONFIG(USE_OMP)
    MCKL_EXAMPLE_CONFIG(HAS_TBB)
    MCKL_EXAMPLE_CONFIG(USE_TBB)
    MCKL_EXAMPLE_CONFIG(USE_TBB_MALLOC)
    MCKL_EXAMPLE_CONFIG(USE_TBB_TLS)
    MCKL_EXAMPLE_CONFIG(HAS_MKL)
    MCKL_EXAMPLE_CONFIG(USE_MKL_CBLAS)
    MCKL_EXAMPLE_CONFIG(USE_MKL_VML)
    MCKL_EXAMPLE_CONFIG(USE_MKL_VSL)
    MCKL_EXAMPLE_CONFIG(USE_CBLAS)
    MCKL_EXAMPLE_CONFIG(HAS_HDF5)
    MCKL_EXAMPLE_CONFIG(HAS_TESTU01)
    MCKL_EXAMPLE_CONFIG(HAS_X86)
    MCKL_EXAMPLE_CONFIG(HAS_X86_64)
    MCKL_EXAMPLE_CONFIG(HAS_LITTLE_ENDIAN)
    MCKL_EXAMPLE_CONFIG(HAS_BIG_ENDIAN)
    MCKL_EXAMPLE_CONFIG(HAS_AVX2)
    MCKL_EXAMPLE_CONFIG(HAS_AVX)
    MCKL_EXAMPLE_CONFIG(HAS_SSE4_2)
    MCKL_EXAMPLE_CONFIG(HAS_SSE4_1)
    MCKL_EXAMPLE_CONFIG(HAS_SSSE3)
    MCKL_EXAMPLE_CONFIG(HAS_SSE3)
    MCKL_EXAMPLE_CONFIG(HAS_SSE2)
    MCKL_EXAMPLE_CONFIG(HAS_AESNI)
    MCKL_EXAMPLE_CONFIG(HAS_RDRAND)
    MCKL_EXAMPLE_CONFIG(HAS_BMI)
    MCKL_EXAMPLE_CONFIG(HAS_BMI2)
    MCKL_EXAMPLE_CONFIG(HAS_RDTSC)
    MCKL_EXAMPLE_CONFIG(HAS_RDTSCP)
    MCKL_EXAMPLE_CONFIG(HAS_INT128)

    std::cout << std::setw(40) << std::left << "is_little_endian()"
              << mckl::internal::is_little_endian() << std::endl;

    std::cout << std::setw(40) << std::left << "is_big_endian()"
              << mckl::internal::is_big_endian() << std::endl;

    return 0;
}
