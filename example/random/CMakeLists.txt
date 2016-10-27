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

MCKL_ADD_EXAMPLE(random)

FUNCTION(MCKL_ADD_TEST_RNG testname)
    ADD_CUSTOM_TARGET(random_${testname})
    ADD_CUSTOM_TARGET(random_${testname}-check)
    IF(EXISTS ${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp)
        ADD_LIBRARY(librandom_${testname}
            ${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp)
        SET_TARGET_PROPERTIES(librandom_${testname}
            PROPERTIES OUTPUT_NAME random_${testname})
    ENDIF(EXISTS ${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp)
    FOREACH(RNG ${MCKL_RNG})
        STRING(TOLOWER ${RNG} rng)
        ADD_CUSTOM_TARGET(random_${testname}_${rng})
        ADD_CUSTOM_TARGET(random_${testname}_${rng}-check)
        ADD_DEPENDENCIES(random_${testname} random_${testname}_${rng})
        ADD_DEPENDENCIES(random_${testname}-check
            random_${testname}_${rng}-check)
        SET(RNGHeader ${MCKL_RNG_${RNG}_HEADER})
        FOREACH(alg ${MCKL_RNG_${RNG}})
            SET(RNGType ::${MCKL_RNG_${RNG}_NAMESPACE}::${alg})
            SET(RNGName ${alg})
            STRING(TOLOWER ${RNGName} rngname)
            STRING(TOUPPER ${RNGName} RNGNAME)
            CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp.in
                ${PROJECT_BINARY_DIR}/src/random_${testname}_${rngname}.cpp)
            MCKL_ADD_TEST(random ${testname}_${rngname} "BIN")
            IF(EXISTS ${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp)
                TARGET_LINK_LIBRARIES(random_${testname}_${rngname}
                    librandom_${testname})
                TARGET_LINK_LIBRARIES(random_${testname}_${rngname}
                    librandom_rng_u01_${rngname})
            ENDIF(EXISTS ${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp)
            ADD_DEPENDENCIES(random_${testname}_${rng}
                random_${testname}_${rngname})
            ADD_DEPENDENCIES(random_${testname}_${rng}-check
                random_${testname}_${rngname}-check)
        ENDFOREACH(alg ${MCKL_RNG_${RNG}})
    ENDFOREACH(RNG ${MCKL_RNG})
ENDFUNCTION(MCKL_ADD_TEST_RNG)

SET(MCKL_RNG_STD_HEADER "random")
SET(MCKL_RNG_STD_NAMESPACE "std")
SET(MCKL_RNG_STD
    mt19937
    mt19937_64
    minstd_rand0
    minstd_rand
    ranlux24_base
    ranlux48_base
    ranlux24
    ranlux48
    knuth_b)

SET(MCKL_RNG_AES_HEADER "mckl/random/aes.hpp")
SET(MCKL_RNG_AES_NAMESPACE "mckl")
SET(MCKL_RNG_AES
    AES128 AES128_64
    AES192 AES192_64
    AES256 AES256_64
    ARS    ARS_64)

SET(MCKL_RNG_PHILOX_HEADER "mckl/random/philox.hpp")
SET(MCKL_RNG_PHILOX_NAMESPACE "mckl")
SET(MCKL_RNG_PHILOX
    Philox2x32 Philox2x32_64
    Philox4x32 Philox4x32_64
    Philox2x64 Philox2x64_64
    Philox4x64 Philox4x64_64)

SET(MCKL_RNG_THREEFRY_HEADER "mckl/random/threefry.hpp")
SET(MCKL_RNG_THREEFRY_NAMESPACE "mckl")
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

IF(MKL_FOUND)
    SET(MCKL_RNG_MKL_HEADER "mckl/random/mkl.hpp")
    SET(MCKL_RNG_MKL_NAMESPACE "mckl")
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
    SET(MCKL_RNG_RDRAND_HEADER "mckl/random/rdrand.hpp")
    SET(MCKL_RNG_RDRAND_NAMESPACE "mckl")
    SET(MCKL_RNG_RDRAND RDRAND16 RDRAND32 RDRAND64)
ENDIF(RDRAND_FOUND)

SET(MCKL_RNG STD AES PHILOX THREEFRY)
IF(MKL_FOUND)
    SET(MCKL_RNG ${MCKL_RNG} MKL)
ENDIF(MKL_FOUND)
IF(RDRAND_FOUND)
    SET(MCKL_RNG ${MCKL_RNG} RDRAND)
ENDIF(RDRAND_FOUND)

SET(MCKL_DISTRIBUTION Arcsine Beta Cauchy ChiSquared Exponential ExtremeValue
    FisherF Gamma Laplace Levy Logistic Lognormal Normal Pareto Rayleigh Stable
    StudentT U01 U01CC U01CO U01OC U01OO UniformReal Weibull Geometric
    UniformInt)

ADD_CUSTOM_TARGET(librandom_rng_u01)
ADD_CUSTOM_TARGET(librandom_rng_ubits)
FOREACH(RNG ${MCKL_RNG})
    STRING(TOLOWER ${RNG} rng)
    SET(RNGHeader ${MCKL_RNG_${RNG}_HEADER})
    FOREACH(alg ${MCKL_RNG_${RNG}})
        SET(RNGType ::${MCKL_RNG_${RNG}_NAMESPACE}::${alg})
        SET(RNGName ${alg})
        STRING(TOLOWER ${RNGName} rngname)
        STRING(TOUPPER ${RNGName} RNGNAME)
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/random_rng_u01.cpp.in
            ${PROJECT_BINARY_DIR}/src/random_rng_u01_${rngname}.cpp)
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/random_rng_ubits.cpp.in
            ${PROJECT_BINARY_DIR}/src/random_rng_ubits_${rngname}.cpp)
        ADD_LIBRARY(librandom_rng_u01_${rngname}
            ${PROJECT_BINARY_DIR}/src/random_rng_u01_${rngname}.cpp)
        ADD_LIBRARY(librandom_rng_ubits_${rngname}
            ${PROJECT_BINARY_DIR}/src/random_rng_ubits_${rngname}.cpp)
        SET_TARGET_PROPERTIES(librandom_rng_u01_${rngname}
            PROPERTIES OUTPUT_NAME random_rng_u01_${rngname})
        SET_TARGET_PROPERTIES(librandom_rng_ubits_${rngname}
            PROPERTIES OUTPUT_NAME random_rng_ubits_${rngname})
        ADD_DEPENDENCIES(librandom_rng_u01 librandom_rng_u01_${rngname})
        ADD_DEPENDENCIES(librandom_rng_ubits librandom_rng_ubits_${rngname})
    ENDFOREACH(alg ${MCKL_RNG_${RNG}})
ENDFOREACH(RNG ${MCKL_RNG})

ADD_CUSTOM_TARGET(random_distribution)
ADD_CUSTOM_TARGET(random_distribution-check)
FOREACH(Dist ${MCKL_DISTRIBUTION})
    SET(DistName ${Dist})
    SET(DistType ::mckl::${Dist}Distribution)
    IF(${DistName} MATCHES "U01")
        STRING(TOLOWER ${Dist} distname)
        STRING(TOUPPER ${Dist} DISTNAME)
        SET(DistHeader "mckl/random/u01_distribution.hpp")
    ELSE(${DistName} MATCHES "U01")
        STRING(REGEX REPLACE "([A-Z])" "_\\1" Dist ${Dist})
        STRING(REGEX REPLACE "^_" "" Dist ${Dist})
        STRING(TOLOWER ${Dist} distname)
        STRING(TOUPPER ${Dist} DISTNAME)
        SET(DistHeader "mckl/random/${distname}_distribution.hpp")
    ENDIF(${DistName} MATCHES "U01")
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/random_distribution.cpp.in
        ${PROJECT_BINARY_DIR}/src/random_distribution_${distname}.cpp)
    MCKL_ADD_TEST(random distribution_${distname} "BIN")
    ADD_DEPENDENCIES(random_distribution
        random_distribution_${distname})
    ADD_DEPENDENCIES(random_distribution-check
        random_distribution_${distname}-check)
    IF(MKL_FOUND)
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/random_distribution.cpp.in
            ${PROJECT_BINARY_DIR}/src/random_distribution_${distname}_novml.cpp)
        MCKL_ADD_TEST(random distribution_${distname}_novml "BIN")
        ADD_DEPENDENCIES(random_distribution
            random_distribution_${distname}_novml)
        ADD_DEPENDENCIES(random_distribution-check
            random_distribution_${distname}_novml-check)
        SET_TARGET_PROPERTIES(random_distribution_${distname}_novml
            PROPERTIES COMPILE_FLAGS "-DMCKL_USE_MKL_VML=0")
    ENDIF(MKL_FOUND)
    IF(AESNI_FOUND)
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/random_distribution.cpp.in
            ${PROJECT_BINARY_DIR}/src/random_distribution_${distname}_noars.cpp)
        MCKL_ADD_TEST(random distribution_${distname}_noars "BIN")
        ADD_DEPENDENCIES(random_distribution
            random_distribution_${distname}_noars)
        ADD_DEPENDENCIES(random_distribution-check
            random_distribution_${distname}_noars-check)
        SET_TARGET_PROPERTIES(random_distribution_${distname}_noars
            PROPERTIES COMPILE_FLAGS "-DMCKL_EXAMPLE_RANDOM_NO_ARS")
    ENDIF(AESNI_FOUND)
ENDFOREACH(Dist ${MCKL_DISTRIBUTION})

MCKL_ADD_TEST(random aes)
MCKL_ADD_TEST(random seed)
MCKL_ADD_TEST(random skein)
MCKL_ADD_TEST(random threefish)
MCKL_ADD_TEST(random u01)
IF(MKL_FOUND)
    MCKL_ADD_TEST(random mkl_brng)
ENDIF(MKL_FOUND)

MCKL_ADD_TEST_RNG(rng)
MCKL_ADD_TEST_RNG(test)
IF(TestU01_FOUND)
    MCKL_ADD_TEST_RNG(testu01_bigcrush)
    MCKL_ADD_TEST_RNG(testu01_crush)
    MCKL_ADD_TEST_RNG(testu01_smallcrush)
    ADD_CUSTOM_TARGET(random_testu01)
    ADD_CUSTOM_TARGET(random_testu01-check)
    ADD_DEPENDENCIES(random_testu01 random_testu01_bigcrush)
    ADD_DEPENDENCIES(random_testu01 random_testu01_crush)
    ADD_DEPENDENCIES(random_testu01 random_testu01_smallcrush)
    ADD_DEPENDENCIES(random_testu01-check random_testu01_bigcrush-check)
    ADD_DEPENDENCIES(random_testu01-check random_testu01_crush-check)
    ADD_DEPENDENCIES(random_testu01-check random_testu01_smallcrush-check)
ENDIF(TestU01_FOUND)

MCKL_ADD_FILE(random random_aes128.txt)
MCKL_ADD_FILE(random random_aes192.txt)
MCKL_ADD_FILE(random random_aes256.txt)
MCKL_ADD_FILE(random random_ars.txt)
MCKL_ADD_FILE(random random_aes128_64.txt)
MCKL_ADD_FILE(random random_aes192_64.txt)
MCKL_ADD_FILE(random random_aes256_64.txt)
MCKL_ADD_FILE(random random_ars_64.txt)

MCKL_ADD_FILE(random random_philox2x32.txt)
MCKL_ADD_FILE(random random_philox2x64.txt)
MCKL_ADD_FILE(random random_philox4x32.txt)
MCKL_ADD_FILE(random random_philox4x64.txt)
MCKL_ADD_FILE(random random_philox2x32_64.txt)
MCKL_ADD_FILE(random random_philox2x64_64.txt)
MCKL_ADD_FILE(random random_philox4x32_64.txt)
MCKL_ADD_FILE(random random_philox4x64_64.txt)

MCKL_ADD_FILE(random random_threefry2x32.txt)
MCKL_ADD_FILE(random random_threefry2x64.txt)
MCKL_ADD_FILE(random random_threefry4x32.txt)
MCKL_ADD_FILE(random random_threefry4x64.txt)
MCKL_ADD_FILE(random random_threefry8x64.txt)
MCKL_ADD_FILE(random random_threefry16x64.txt)
MCKL_ADD_FILE(random random_threefish256.txt)
MCKL_ADD_FILE(random random_threefish512.txt)
MCKL_ADD_FILE(random random_threefish1024.txt)
MCKL_ADD_FILE(random random_threefry2x32_64.txt)
MCKL_ADD_FILE(random random_threefry2x64_64.txt)
MCKL_ADD_FILE(random random_threefry4x32_64.txt)
MCKL_ADD_FILE(random random_threefry4x64_64.txt)
MCKL_ADD_FILE(random random_threefry8x64_64.txt)
MCKL_ADD_FILE(random random_threefry16x64_64.txt)
MCKL_ADD_FILE(random random_threefish256_64.txt)
MCKL_ADD_FILE(random random_threefish512_64.txt)
MCKL_ADD_FILE(random random_threefish1024_64.txt)

MCKL_ADD_FILE(random random_seed_1x32.txt)
MCKL_ADD_FILE(random random_seed_2x32.txt)
MCKL_ADD_FILE(random random_seed_3x32.txt)
MCKL_ADD_FILE(random random_seed_4x32.txt)
MCKL_ADD_FILE(random random_seed_5x32.txt)
MCKL_ADD_FILE(random random_seed_6x32.txt)
MCKL_ADD_FILE(random random_seed_7x32.txt)
MCKL_ADD_FILE(random random_seed_8x32.txt)
MCKL_ADD_FILE(random random_seed_9x32.txt)
MCKL_ADD_FILE(random random_seed_10x32.txt)
MCKL_ADD_FILE(random random_seed_11x32.txt)
MCKL_ADD_FILE(random random_seed_12x32.txt)
MCKL_ADD_FILE(random random_seed_13x32.txt)
MCKL_ADD_FILE(random random_seed_14x32.txt)
MCKL_ADD_FILE(random random_seed_15x32.txt)
MCKL_ADD_FILE(random random_seed_16x32.txt)
MCKL_ADD_FILE(random random_seed_1x64.txt)
MCKL_ADD_FILE(random random_seed_2x64.txt)
MCKL_ADD_FILE(random random_seed_3x64.txt)
MCKL_ADD_FILE(random random_seed_4x64.txt)
MCKL_ADD_FILE(random random_seed_5x64.txt)
MCKL_ADD_FILE(random random_seed_6x64.txt)
MCKL_ADD_FILE(random random_seed_7x64.txt)
MCKL_ADD_FILE(random random_seed_8x64.txt)
MCKL_ADD_FILE(random random_seed_9x64.txt)
MCKL_ADD_FILE(random random_seed_10x64.txt)
MCKL_ADD_FILE(random random_seed_11x64.txt)
MCKL_ADD_FILE(random random_seed_12x64.txt)
MCKL_ADD_FILE(random random_seed_13x64.txt)
MCKL_ADD_FILE(random random_seed_14x64.txt)
MCKL_ADD_FILE(random random_seed_15x64.txt)
MCKL_ADD_FILE(random random_seed_16x64.txt)
