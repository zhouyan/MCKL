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

project(MCKLExample-random CXX)

mckl_add_example(random)

function(MCKL_ADD_TEST_RNG testname)
    add_custom_target(random_${testname})
    add_custom_target(random_${testname}-check)
    if(EXISTS ${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp)
        add_library(librandom_${testname}
            ${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp)
        set_target_properties(librandom_${testname}
            PROPERTIES OUTPUT_NAME random_${testname})
        target_link_libraries(librandom_${testname} ${MCKL_LINK_LIBRARIES})
    endif(EXISTS ${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp)
    foreach(RNG ${MCKL_RNG})
        string(TOLOWER ${RNG} rng)
        add_custom_target(random_${testname}_${rng})
        add_custom_target(random_${testname}_${rng}-check)
        add_dependencies(random_${testname} random_${testname}_${rng})
        add_dependencies(random_${testname}-check
            random_${testname}_${rng}-check)
        set(RNGHeader ${MCKL_RNG_${RNG}_HEADER})
        foreach(alg ${MCKL_RNG_${RNG}})
            set(RNGType ::${MCKL_RNG_${RNG}_NAMESPACE}::${alg})
            set(RNGName ${alg})
            string(TOLOWER ${RNGName} rngname)
            string(TOUPPER ${RNGName} RNGNAME)
            configure_file(${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp.in
                ${PROJECT_BINARY_DIR}/src/random_${testname}_${rngname}.cpp)
            mckl_add_test(random ${testname}_${rngname} "BIN")
            if(EXISTS ${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp)
                target_link_libraries(random_${testname}_${rngname}
                    librandom_${testname})
                target_link_libraries(random_${testname}_${rngname}
                    librandom_rng_u01_${rngname})
            endif(EXISTS ${PROJECT_SOURCE_DIR}/src/random_${testname}.cpp)
            add_dependencies(random_${testname}_${rng}
                random_${testname}_${rngname})
            add_dependencies(random_${testname}_${rng}-check
                random_${testname}_${rngname}-check)
        endforeach(alg ${MCKL_RNG_${RNG}})
    endforeach(RNG ${MCKL_RNG})
endfunction(MCKL_ADD_TEST_RNG)

set(MCKL_RNG_STD_HEADER "random")
set(MCKL_RNG_STD_NAMESPACE "std")
set(MCKL_RNG_STD
    mt19937
    mt19937_64
    minstd_rand0
    minstd_rand
    ranlux24_base
    ranlux48_base
    ranlux24
    ranlux48
    knuth_b)

set(MCKL_RNG_AES_HEADER "mckl/random/aes.hpp")
set(MCKL_RNG_AES_NAMESPACE "mckl")
set(MCKL_RNG_AES
    AES128 AES128_64
    AES192 AES192_64
    AES256 AES256_64
    ARS    ARS_64)

set(MCKL_RNG_PHILOX_HEADER "mckl/random/philox.hpp")
set(MCKL_RNG_PHILOX_NAMESPACE "mckl")
set(MCKL_RNG_PHILOX
    Philox2x32 Philox2x32_64
    Philox4x32 Philox4x32_64
    Philox2x64 Philox2x64_64
    Philox4x64 Philox4x64_64)

set(MCKL_RNG_THREEFRY_HEADER "mckl/random/threefry.hpp")
set(MCKL_RNG_THREEFRY_NAMESPACE "mckl")
set(MCKL_RNG_THREEFRY
    Threefry2x32  Threefry2x32_64
    Threefry4x32  Threefry4x32_64
    Threefry2x64  Threefry2x64_64
    Threefry4x64  Threefry4x64_64
    Threefry8x64  Threefry8x64_64
    Threefry16x64 Threefry16x64_64
    Threefish256  Threefish256_64
    Threefish512  Threefish512_64
    Threefish1024 Threefish1024_64)

if(Random123_FOUND)
    set(MCKL_RNG_R123_HEADER "random_r123.hpp")
    set(MCKL_RNG_R123_NAMESPACE "mckl")
    set(MCKL_RNG_R123
        R123_Philox2x32
        R123_Philox4x32
        R123_Philox2x64
        R123_Philox4x64
        R123_Threefry2x32
        R123_Threefry4x32
        R123_Threefry2x64
        R123_Threefry4x64)
    if(AESNI_FOUND)
        set(MCKL_RNG_R123 ${MCKL_RNG_R123} R123_AESNI4x32 R123_ARS4x32)
    endif(AESNI_FOUND)
endif(Random123_FOUND)

if(MKL_FOUND)
    set(MCKL_RNG_MKL_HEADER "mckl/random/mkl.hpp")
    set(MCKL_RNG_MKL_NAMESPACE "mckl")
    set(MCKL_RNG_MKL
        MKL_MCG59          MKL_MCG59_64
        MKL_MT19937        MKL_MT19937_64
        MKL_MT2203         MKL_MT2203_64
        MKL_SFMT19937      MKL_SFMT19937_64
        MKL_PHILOX4X32X10  MKL_PHILOX4X32X10_64)
endif(MKL_FOUND)
if(AESNI_FOUND AND MKL_FOUND)
    set(MCKL_RNG_MKL ${MCKL_RNG_MKL} MKL_ARS5 MKL_ARS5_64)
endif(AESNI_FOUND AND MKL_FOUND)
if(RDRAND_FOUND AND MKL_FOUND)
    set(MCKL_RNG_MKL ${MCKL_RNG_MKL} MKL_NONDETERM MKL_NONDETERM_64)
endif(RDRAND_FOUND AND MKL_FOUND)

if(RDRAND_FOUND)
    set(MCKL_RNG_RDRAND_HEADER "mckl/random/rdrand.hpp")
    set(MCKL_RNG_RDRAND_NAMESPACE "mckl")
    set(MCKL_RNG_RDRAND RDRAND16 RDRAND32 RDRAND64)
endif(RDRAND_FOUND)

set(MCKL_RNG STD AES PHILOX THREEFRY)
if(Random123_FOUND)
    set(MCKL_RNG ${MCKL_RNG} R123)
endif(Random123_FOUND)
if(MKL_FOUND)
    set(MCKL_RNG ${MCKL_RNG} MKL)
endif(MKL_FOUND)
if(RDRAND_FOUND)
    set(MCKL_RNG ${MCKL_RNG} RDRAND)
endif(RDRAND_FOUND)

set(MCKL_DISTRIBUTION Arcsine Beta Cauchy ChiSquared Exponential ExtremeValue
    FisherF Gamma Laplace Levy Logistic Lognormal Normal Pareto Rayleigh Stable
    StudentT U01Canonical U01CC U01CO U01OC U01OO UniformReal Weibull Geometric
    UniformInt)

add_custom_target(librandom_rng_u01)
foreach(RNG ${MCKL_RNG})
    string(TOLOWER ${RNG} rng)
    set(RNGHeader ${MCKL_RNG_${RNG}_HEADER})
    foreach(alg ${MCKL_RNG_${RNG}})
        set(RNGType ::${MCKL_RNG_${RNG}_NAMESPACE}::${alg})
        set(RNGName ${alg})
        string(TOLOWER ${RNGName} rngname)
        string(TOUPPER ${RNGName} RNGNAME)
        configure_file(${PROJECT_SOURCE_DIR}/src/random_rng_u01.cpp.in
            ${PROJECT_BINARY_DIR}/src/random_rng_u01_${rngname}.cpp)
        add_library(librandom_rng_u01_${rngname}
            ${PROJECT_BINARY_DIR}/src/random_rng_u01_${rngname}.cpp)
        set_target_properties(librandom_rng_u01_${rngname}
            PROPERTIES OUTPUT_NAME random_rng_u01_${rngname})
        target_link_libraries(librandom_rng_u01_${rngname}
            ${MCKL_LINK_LIBRARIES})
        add_dependencies(librandom_rng_u01 librandom_rng_u01_${rngname})
    endforeach(alg ${MCKL_RNG_${RNG}})
endforeach(RNG ${MCKL_RNG})

add_custom_target(random_distribution)
add_custom_target(random_distribution-check)
foreach(Dist ${MCKL_DISTRIBUTION})
    set(DistName ${Dist})
    set(DistType ::mckl::${Dist}Distribution)
    if(${DistName} MATCHES "U01Canonical")
        set(DISTNAME "U01_CANONICAL")
        set(distname "u01_canonical")
        set(DistHeader "mckl/random/u01_distribution.hpp")
    elseif(${DistName} MATCHES "U01")
        set(DISTNAME ${DistName})
        string(REGEX REPLACE "U01(..)" "U01_\\1" Dist ${Dist})
        string(REGEX REPLACE "^_" "" Dist ${Dist})
        string(TOLOWER ${Dist} distname)
        set(DistHeader "mckl/random/u01_distribution.hpp")
    else(${DistName} MATCHES "U01Canonical")
        string(REGEX REPLACE "([A-Z])" "_\\1" Dist ${Dist})
        string(REGEX REPLACE "^_" "" Dist ${Dist})
        string(TOLOWER ${Dist} distname)
        string(TOUPPER ${Dist} DISTNAME)
        set(DistHeader "mckl/random/${distname}_distribution.hpp")
    endif(${DistName} MATCHES "U01Canonical")
    configure_file(${PROJECT_SOURCE_DIR}/src/random_distribution.cpp.in
        ${PROJECT_BINARY_DIR}/src/random_distribution_${distname}.cpp)
    mckl_add_test(random distribution_${distname} "BIN")
    add_dependencies(random_distribution
        random_distribution_${distname})
    add_dependencies(random_distribution-check
        random_distribution_${distname}-check)
    if(MKL_FOUND)
        configure_file(${PROJECT_SOURCE_DIR}/src/random_distribution.cpp.in
            ${PROJECT_BINARY_DIR}/src/random_distribution_${distname}_novml.cpp)
        mckl_add_test(random distribution_${distname}_novml "BIN")
        add_dependencies(random_distribution
            random_distribution_${distname}_novml)
        add_dependencies(random_distribution-check
            random_distribution_${distname}_novml-check)
        set_target_properties(random_distribution_${distname}_novml
            PROPERTIES COMPILE_FLAGS "-DMCKL_USE_MKL_VML=0")
    endif(MKL_FOUND)
endforeach(Dist ${MCKL_DISTRIBUTION})

mckl_add_test(random aes)
mckl_add_test(random seed)
mckl_add_test(random skein)
mckl_add_test(random threefish)
mckl_add_test(random u01)
if(MKL_FOUND)
    mckl_add_test(random mkl_brng)
endif(MKL_FOUND)

mckl_add_test_rng(rng)
mckl_add_test_rng(test)
if(TestU01_FOUND)
    mckl_add_test_rng(testu01_bigcrush)
    mckl_add_test_rng(testu01_crush)
    mckl_add_test_rng(testu01_smallcrush)
    add_custom_target(random_testu01)
    add_custom_target(random_testu01-check)
    add_dependencies(random_testu01 random_testu01_bigcrush)
    add_dependencies(random_testu01 random_testu01_crush)
    add_dependencies(random_testu01 random_testu01_smallcrush)
    add_dependencies(random_testu01-check random_testu01_bigcrush-check)
    add_dependencies(random_testu01-check random_testu01_crush-check)
    add_dependencies(random_testu01-check random_testu01_smallcrush-check)
endif(TestU01_FOUND)

mckl_add_file(random random_aes128.txt)
mckl_add_file(random random_aes192.txt)
mckl_add_file(random random_aes256.txt)
mckl_add_file(random random_ars.txt)
mckl_add_file(random random_aes128_64.txt)
mckl_add_file(random random_aes192_64.txt)
mckl_add_file(random random_aes256_64.txt)
mckl_add_file(random random_ars_64.txt)

mckl_add_file(random random_philox2x32.txt)
mckl_add_file(random random_philox2x64.txt)
mckl_add_file(random random_philox4x32.txt)
mckl_add_file(random random_philox4x64.txt)
mckl_add_file(random random_philox2x32_64.txt)
mckl_add_file(random random_philox2x64_64.txt)
mckl_add_file(random random_philox4x32_64.txt)
mckl_add_file(random random_philox4x64_64.txt)

mckl_add_file(random random_threefry2x32.txt)
mckl_add_file(random random_threefry2x64.txt)
mckl_add_file(random random_threefry4x32.txt)
mckl_add_file(random random_threefry4x64.txt)
mckl_add_file(random random_threefry8x64.txt)
mckl_add_file(random random_threefry16x64.txt)
mckl_add_file(random random_threefish256.txt)
mckl_add_file(random random_threefish512.txt)
mckl_add_file(random random_threefish1024.txt)
mckl_add_file(random random_threefry2x32_64.txt)
mckl_add_file(random random_threefry2x64_64.txt)
mckl_add_file(random random_threefry4x32_64.txt)
mckl_add_file(random random_threefry4x64_64.txt)
mckl_add_file(random random_threefry8x64_64.txt)
mckl_add_file(random random_threefry16x64_64.txt)
mckl_add_file(random random_threefish256_64.txt)
mckl_add_file(random random_threefish512_64.txt)
mckl_add_file(random random_threefish1024_64.txt)

mckl_add_file(random random_seed_1x32.txt)
mckl_add_file(random random_seed_2x32.txt)
mckl_add_file(random random_seed_3x32.txt)
mckl_add_file(random random_seed_4x32.txt)
mckl_add_file(random random_seed_5x32.txt)
mckl_add_file(random random_seed_6x32.txt)
mckl_add_file(random random_seed_7x32.txt)
mckl_add_file(random random_seed_8x32.txt)
mckl_add_file(random random_seed_9x32.txt)
mckl_add_file(random random_seed_10x32.txt)
mckl_add_file(random random_seed_11x32.txt)
mckl_add_file(random random_seed_12x32.txt)
mckl_add_file(random random_seed_13x32.txt)
mckl_add_file(random random_seed_14x32.txt)
mckl_add_file(random random_seed_15x32.txt)
mckl_add_file(random random_seed_16x32.txt)
mckl_add_file(random random_seed_1x64.txt)
mckl_add_file(random random_seed_2x64.txt)
mckl_add_file(random random_seed_3x64.txt)
mckl_add_file(random random_seed_4x64.txt)
mckl_add_file(random random_seed_5x64.txt)
mckl_add_file(random random_seed_6x64.txt)
mckl_add_file(random random_seed_7x64.txt)
mckl_add_file(random random_seed_8x64.txt)
mckl_add_file(random random_seed_9x64.txt)
mckl_add_file(random random_seed_10x64.txt)
mckl_add_file(random random_seed_11x64.txt)
mckl_add_file(random random_seed_12x64.txt)
mckl_add_file(random random_seed_13x64.txt)
mckl_add_file(random random_seed_14x64.txt)
mckl_add_file(random random_seed_15x64.txt)
mckl_add_file(random random_seed_16x64.txt)
