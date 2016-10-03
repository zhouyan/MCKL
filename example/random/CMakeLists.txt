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

SET(STD_RNG
    mt19937
    mt19937_64
    minstd_rand0
    minstd_rand
    ranlux24_base
    ranlux48_base
    ranlux24
    ranlux48
    knuth_b)

SET(MCKL_RNG_PHILOX
    Philox2x32 Philox2x32_64
    Philox4x32 Philox4x32_64
    Philox2x64 Philox2x64_64
    Philox4x64 Philox4x64_64)

SET(MCKL_RNG_THREEFRY
    Threefry2x32  Threefry2x32_64
    Threefry4x32  Threefry4x32_64
    Threefry2x64  Threefry2x64_64
    Threefry4x64  Threefry4x64_64
    Threefry8x64  Threefry8x64_64
    Threefry16x64 Threefry16x64_64
    Threefish256  Threefish256_64
    Threefish512  Threefish512_64
    Threefish1024 Threefish1024_64)

IF(AESNI_FOUND)
    SET(MCKL_RNG_AESNI
        AES128 AES128_64
        AES192 AES192_64
        AES256 AES256_64
        ARS    ARS_64)
ENDIF(AESNI_FOUND)

IF(MKL_FOUND)
    SET(MCKL_RNG_MKL
        MKL_MCG59          MKL_MCG59_64
        MKL_MT19937        MKL_MT19937_64
        MKL_MT2203         MKL_MT2203_64
        MKL_SFMT19937      MKL_SFMT19937_64
        MKL_PHILOX4X32X10  MKL_PHILOX4X32X10_64)
ENDIF(MKL_FOUND)

IF(AESNI_FOUND AND MKL_FOUND)
    SET(MCKL_RNG_MKL ${MCKL_RNG_MKL} MKL_ARS5 MKL_ARS5_64)
ENDIF(AESNI_FOUND AND MKL_FOUND)

IF(RDRAND_FOUND AND MKL_FOUND)
    SET(MCKL_RNG_MKL ${MCKL_RNG_MKL} MKL_NONDETERM MKL_NONDETERM_64)
ENDIF(RDRAND_FOUND AND MKL_FOUND)

IF(RDRAND_FOUND)
    SET(MCKL_RNG_RDRAND RDRAND16 RDRAND32 RDRAND64)
ENDIF(RDRAND_FOUND)

SET(MCKL_RNG
    ${MCKL_RNG_PHILOX}
    ${MCKL_RNG_THREEFRY}
    ${MCKL_RNG_AESNI}
    ${MCKL_RNG_MKL}
    ${MCKL_RNG_RDRAND})

SET(MCKL_DISTRIBUTION Arcsine Beta Cauchy ChiSquared Exponential ExtremeValue
    FisherF Gamma Laplace Levy Logistic Lognormal Normal Pareto Rayleigh Stable
    StudentT U01 UniformReal Weibull Geometric UniformInt)

FUNCTION(MCKL_ADD_TEST_RNG testname)
    ADD_CUSTOM_TARGET(random_${testname})
    ADD_CUSTOM_TARGET(random_${testname}-check)
    ADD_DEPENDENCIES(random random_${testname})
    ADD_DEPENDENCIES(random-check random_${testname}-check)

    FOREACH(RNG ${MCKL_RNG})
        SET(RNGType ::mckl::${RNG})
        SET(RNGName ${RNG})
        STRING(TOLOWER ${RNG} rng)
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp.in
            ${PROJECT_SOURCE_DIR}/src/random_${testname}_${rng}.cpp)
        MCKL_ADD_TEST(random ${testname}_${rng})
        ADD_DEPENDENCIES(random_${testname}
            random_${testname}_${rng})
        ADD_DEPENDENCIES(random_${testname}-check
            random_${testname}_${rng}-check)
    ENDFOREACH(RNG ${MCKL_RNG})

    ADD_CUSTOM_TARGET(random_${testname}_std)
    ADD_CUSTOM_TARGET(random_${testname}_std-check)
    FOREACH(RNG ${STD_RNG})
        SET(RNGType std::${RNG})
        SET(RNGName ${RNG})
        STRING(TOLOWER ${RNG} rng)
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp.in
            ${PROJECT_SOURCE_DIR}/src/random_${testname}_${rng}.cpp)
        MCKL_ADD_TEST(random ${testname}_${rng})
        ADD_DEPENDENCIES(random_${testname}
            random_${testname}_${rng})
        ADD_DEPENDENCIES(random_${testname}-check
            random_${testname}_${rng}-check)
        ADD_DEPENDENCIES(random_${testname}_std
            random_${testname}_${rng})
        ADD_DEPENDENCIES(random_${testname}_std-check
            random_${testname}_${rng}-check)
    ENDFOREACH(RNG ${MCKL_RNG})

    ADD_CUSTOM_TARGET(random_${testname}_philox)
    ADD_CUSTOM_TARGET(random_${testname}_philox-check)
    FOREACH(RNG ${MCKL_RNG_PHILOX})
        STRING(TOLOWER ${RNG} rng)
        ADD_DEPENDENCIES(random_${testname}_philox
            random_${testname}_${rng})
        ADD_DEPENDENCIES(random_${testname}_philox-check
            random_${testname}_${rng}-check)
    ENDFOREACH(RNG ${MCKL_RNG_PHILOX})

    ADD_CUSTOM_TARGET(random_${testname}_threefry)
    ADD_CUSTOM_TARGET(random_${testname}_threefry-check)
    FOREACH(RNG ${MCKL_RNG_THREEFRY})
        STRING(TOLOWER ${RNG} rng)
        ADD_DEPENDENCIES(random_${testname}_threefry
            random_${testname}_${rng})
        ADD_DEPENDENCIES(random_${testname}_threefry-check
            random_${testname}_${rng}-check)
    ENDFOREACH(RNG ${MCKL_RNG_THREEFRY})

    IF(AESNI_FOUND)
        ADD_CUSTOM_TARGET(random_${testname}_aesni)
        ADD_CUSTOM_TARGET(random_${testname}_aesni-check)
        FOREACH(RNG ${MCKL_RNG_AESNI})
            STRING(TOLOWER ${RNG} rng)
            ADD_DEPENDENCIES(random_${testname}_aesni
                random_${testname}_${rng})
            ADD_DEPENDENCIES(random_${testname}_aesni-check
                random_${testname}_${rng}-check)
        ENDFOREACH(RNG ${MCKL_RNG_AESNI})
    ENDIF(AESNI_FOUND)

    IF(MKL_FOUND)
        ADD_CUSTOM_TARGET(random_${testname}_mkl)
        ADD_CUSTOM_TARGET(random_${testname}_mkl-check)
        FOREACH(RNG ${MCKL_RNG_MKL})
            STRING(TOLOWER ${RNG} rng)
            ADD_DEPENDENCIES(random_${testname}_mkl
                random_${testname}_${rng})
            ADD_DEPENDENCIES(random_${testname}_mkl-check
                random_${testname}_${rng}-check)
        ENDFOREACH(RNG ${MCKL_RNG_MKL})
    ENDIF(MKL_FOUND)

    IF(RDRAND_FOUND)
        ADD_CUSTOM_TARGET(random_${testname}_rdrand)
        ADD_CUSTOM_TARGET(random_${testname}_rdrand-check)
        FOREACH(RNG ${MCKL_RNG_RDRAND})
            STRING(TOLOWER ${RNG} rng)
            ADD_DEPENDENCIES(random_${testname}_rdrand
                random_${testname}_${rng})
            ADD_DEPENDENCIES(random_${testname}_rdrand-check
                random_${testname}_${rng}-check)
        ENDFOREACH(RNG ${MCKL_RNG_RDRAND})
    ENDIF(RDRAND_FOUND)
ENDFUNCTION(MCKL_ADD_TEST_RNG testname)

FUNCTION(MCKL_ADD_TEST_DISTRIBUTION testname)
    ADD_CUSTOM_TARGET(random_${testname})
    ADD_CUSTOM_TARGET(random_${testname}-check)
    ADD_DEPENDENCIES(random random_${testname})
    ADD_DEPENDENCIES(random-check random_${testname}-check)

    FOREACH(Distribution ${MCKL_DISTRIBUTION})
        SET(DistributionType mckl::${Distribution}Distribution)
        STRING(TOLOWER ${Distribution} distribution)
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp.in
            ${PROJECT_SOURCE_DIR}/src/random_${testname}_${distribution}.cpp)
        MCKL_ADD_TEST(random ${testname}_${distribution})
        ADD_DEPENDENCIES(random_${testname}
            random_${testname}_${distribution})
        ADD_DEPENDENCIES(random_${testname}-check
            random_${testname}_${distribution}-check)
    ENDFOREACH(Distribution ${MCKL_DISTRIBUTION})
ENDFUNCTION(MCKL_ADD_TEST_DISTRIBUTION testname)

MCKL_ADD_EXAMPLE(random)

MCKL_ADD_TEST(random threefish)
MCKL_ADD_TEST(random u01)
IF(AESNI_FOUND)
    MCKL_ADD_TEST(random aes)
ENDIF(AESNI_FOUND)
IF(MKL_FOUND)
    MCKL_ADD_TEST(random mkl_brng)
ENDIF(MKL_FOUND)

MCKL_ADD_TEST_RNG(rng)
MCKL_ADD_TEST_RNG(seed)
MCKL_ADD_TEST_RNG(test)
MCKL_ADD_TEST_RNG(uniform_bits)

MCKL_ADD_TEST_DISTRIBUTION(distribution)
MCKL_ADD_TEST_DISTRIBUTION(distribution_perf)

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
    MCKL_ADD_FILE(random random_AES128.txt)
    MCKL_ADD_FILE(random random_AES192.txt)
    MCKL_ADD_FILE(random random_AES256.txt)
    MCKL_ADD_FILE(random random_ARS.txt)
ENDIF(AESNI_FOUND)
