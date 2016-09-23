# ============================================================================
#  MCKL/example/random/CMakeLists.txt
# ----------------------------------------------------------------------------
#  MCKL: Monte Carlo Kernel Library
# ----------------------------------------------------------------------------
#  Copyright (c) 2013-2016, Yan Zhou
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

PROJECT(MCKLExample-random CXX)

SET(MCKL_RNG ${MCKL_RNG} Philox2x32 Philox4x32 Philox2x64 Philox4x64)
SET(MCKL_RNG ${MCKL_RNG} Threefry2x32 Threefry4x32 Threefry2x64 Threefry4x64)
SET(MCKL_RNG ${MCKL_RNG} Threefry8x64 Threefry16x64)
SET(MCKL_RNG ${MCKL_RNG} Threefish256 Threefish512 Threefish1024)

IF(AESNI_FOUND)
    SET(MCKL_RNG ${MCKL_RNG} AES128 AES192 AES256 ARS)
ENDIF(AESNI_FOUND)

IF(MKL_FOUND)
    SET(MCKL_RNG ${MCKL_RNG} MKL_MCG59)
    SET(MCKL_RNG ${MCKL_RNG} MKL_MT19937 MKL_MT2203 MKL_SFMT19937)
    SET(MCKL_RNG ${MCKL_RNG} MKL_PHILOX4X32X10)
ENDIF(MKL_FOUND)

IF(AESNI_FOUND AND MKL_FOUND)
    SET(MCKL_RNG ${MCKL_RNG} MKL_ARS5)
ENDIF(AESNI_FOUND AND MKL_FOUND)

IF(RDRAND_FOUND AND MKL_FOUND)
    SET(MCKL_RNG ${MCKL_RNG} MKL_NONDETERM)
ENDIF(RDRAND_FOUND AND MKL_FOUND)

FOREACH(RNG ${MCKL_RNG})
    SET(MCKL_RNG_64 ${MCKL_RNG_64} ${RNG}_64)
ENDFOREACH(RNG ${MCKL_RNG})

SET(MCKL_RNG ${MCKL_RNG} ${MCKL_RNG_64})

IF(RDRAND_FOUND)
    SET(MCKL_RNG ${MCKL_RNG} RDRAND16 RDRAND32 RDRAND64)
ENDIF(RDRAND_FOUND)

SET(STD_RNG ${STD_RNG} mt19937 mt19937_64)
SET(STD_RNG ${STD_RNG} minstd_rand0 minstd_rand)
SET(STD_RNG ${STD_RNG} ranlux24_base ranlux48_base)
SET(STD_RNG ${STD_RNG} ranlux24 ranlux48)
SET(STD_RNG ${STD_RNG} knuth_b)

MCKL_ADD_EXAMPLE(random)

ADD_CUSTOM_TARGET(random_rng)
ADD_CUSTOM_TARGET(random_rng-check)
ADD_DEPENDENCIES(random random_rng)
ADD_DEPENDENCIES(random-check random_rng-check)

MCKL_ADD_TEST(random distribution)
MCKL_ADD_TEST(random distribution_perf)
MCKL_ADD_TEST(random test)
MCKL_ADD_TEST(random threefish)
MCKL_ADD_TEST(random u01)
MCKL_ADD_TEST(random uniform_bits)

IF(MKL_FOUND)
    MCKL_ADD_TEST(random mkl_brng)
ENDIF(MKL_FOUND)

FOREACH(RNG ${MCKL_RNG})
    SET(RNGType ::mckl::${RNG})
    SET(Name ${RNG})
    STRING(TOLOWER ${RNG} name)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/random_rng.cpp.in
        ${PROJECT_SOURCE_DIR}/src/random_rng_${name}.cpp)
    MCKL_ADD_TEST(random rng_${name})
    ADD_DEPENDENCIES(random_rng random_rng_${name})
    ADD_DEPENDENCIES(random_rng-check random_rng_${name}-check)
ENDFOREACH(RNG ${MCKL_RNG})

FOREACH(RNG ${STD_RNG})
    SET(RNGType ::std::${RNG})
    SET(Name ${RNG})
    STRING(TOLOWER ${RNG} name)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/random_rng.cpp.in
        ${PROJECT_SOURCE_DIR}/src/random_rng_${name}.cpp)
    MCKL_ADD_TEST(random rng_${name})
ENDFOREACH(RNG ${STD_RNG})

MCKL_ADD_FILE(random random_Philox2x32.txt)
MCKL_ADD_FILE(random random_Philox2x64.txt)
MCKL_ADD_FILE(random random_Philox4x32.txt)
MCKL_ADD_FILE(random random_Philox4x64.txt)

MCKL_ADD_FILE(random random_Threefry2x32.txt)
MCKL_ADD_FILE(random random_Threefry2x64.txt)
MCKL_ADD_FILE(random random_Threefry4x32.txt)
MCKL_ADD_FILE(random random_Threefry4x64.txt)
MCKL_ADD_FILE(random random_Threefry8x64.txt)
MCKL_ADD_FILE(random random_Threefry16x64.txt)
MCKL_ADD_FILE(random random_Threefish256.txt)
MCKL_ADD_FILE(random random_Threefish512.txt)
MCKL_ADD_FILE(random random_Threefish1024.txt)

IF(AESNI_FOUND)
    MCKL_ADD_TEST(random aes)
    MCKL_ADD_FILE(random random_AES128.txt)
    MCKL_ADD_FILE(random random_AES192.txt)
    MCKL_ADD_FILE(random random_AES256.txt)
    MCKL_ADD_FILE(random random_ARS.txt)
ENDIF(AESNI_FOUND)
