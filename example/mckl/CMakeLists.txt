# ============================================================================
#  MCKL/example/mckl/CMakeLists.txt
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

project(MCKLExample-mckl CXX)

function(mckl_add_test_header header_path cond)
    if(${cond})
        string(REPLACE "/" "_" header "${header_path}")
        configure_file(${PROJECT_SOURCE_DIR}/src/mckl.cpp.in
            ${PROJECT_BINARY_DIR}/src/mckl_${header}.cpp)
        mckl_add_test(mckl ${header} "BIN" ${ARGN})
    endif(${cond})
endfunction(mckl_add_test_header)

mckl_add_example(mckl)

mckl_add_test(mckl config)

mckl_add_test_header(mckl TRUE "OpenMP")

mckl_add_test_header(algorithm TRUE)
mckl_add_test_header(algorithm/mcmc     TRUE)
mckl_add_test_header(algorithm/mh       TRUE)
mckl_add_test_header(algorithm/pmcmc    TRUE)
mckl_add_test_header(algorithm/resample TRUE)
mckl_add_test_header(algorithm/smc      TRUE)

mckl_add_test_header(core TRUE)
mckl_add_test_header(core/estimate_matrix TRUE)
mckl_add_test_header(core/estimator       TRUE)
mckl_add_test_header(core/iterator        TRUE)
mckl_add_test_header(core/matrix          TRUE)
mckl_add_test_header(core/memory          TRUE)
mckl_add_test_header(core/particle        TRUE)
mckl_add_test_header(core/sampler         TRUE)
mckl_add_test_header(core/state_matrix    TRUE)
mckl_add_test_header(core/weight          TRUE)

mckl_add_test_header(internal/aesni      ${AESNI_FOUND})
mckl_add_test_header(internal/assert     TRUE)
mckl_add_test_header(internal/avx2       ${AVX2_FOUND})
mckl_add_test_header(internal/avx2_op    ${AVX2_FOUND})
mckl_add_test_header(internal/byte_order TRUE)
mckl_add_test_header(internal/cblas      TRUE)
mckl_add_test_header(internal/common     TRUE)
mckl_add_test_header(internal/const_math TRUE)
mckl_add_test_header(internal/defines    TRUE)
mckl_add_test_header(internal/fma        ${FMA_FOUND})
mckl_add_test_header(internal/iostream   TRUE)
mckl_add_test_header(internal/sse2       ${SSE2_FOUND})
mckl_add_test_header(internal/sse2_op    ${SSE2_FOUND})
mckl_add_test_header(internal/traits     TRUE)

mckl_add_test_header(math TRUE)
mckl_add_test_header(math/beta      TRUE)
mckl_add_test_header(math/constants TRUE)
mckl_add_test_header(math/erf       TRUE)
mckl_add_test_header(math/gamma     TRUE)
mckl_add_test_header(math/vmf       TRUE)

mckl_add_test_header(random/internal/aes_aesni              ${AESNI_FOUND})
mckl_add_test_header(random/internal/aes_aesni_aes128       ${AESNI_FOUND})
mckl_add_test_header(random/internal/aes_aesni_aes192       ${AESNI_FOUND})
mckl_add_test_header(random/internal/aes_aesni_aes256       ${AESNI_FOUND})
mckl_add_test_header(random/internal/aes_aesni_ars          ${AESNI_FOUND})
mckl_add_test_header(random/internal/aes_constants          TRUE)
mckl_add_test_header(random/internal/aes_generic            TRUE)
mckl_add_test_header(random/internal/aes_key_seq            TRUE)
mckl_add_test_header(random/internal/common                 TRUE)
mckl_add_test_header(random/internal/increment_avx2_64      ${AVX2_FOUND})
mckl_add_test_header(random/internal/increment_avx2_64_4    ${AVX2_FOUND})
mckl_add_test_header(random/internal/increment_avx2_64_8    ${AVX2_FOUND})
mckl_add_test_header(random/internal/increment_avx2_64_f    ${AVX2_FOUND})
mckl_add_test_header(random/internal/increment_sse2_64      ${SSE2_FOUND})
mckl_add_test_header(random/internal/increment_sse2_64_4    ${SSE2_FOUND})
mckl_add_test_header(random/internal/increment_sse2_64_8    ${SSE2_FOUND})
mckl_add_test_header(random/internal/increment_sse2_64_f    ${SSE2_FOUND})
mckl_add_test_header(random/internal/philox_avx2_2x32       ${AVX2_FOUND})
mckl_add_test_header(random/internal/philox_avx2_32         ${AVX2_FOUND})
mckl_add_test_header(random/internal/philox_avx2_4x32       ${AVX2_FOUND})
mckl_add_test_header(random/internal/philox_constants       TRUE)
mckl_add_test_header(random/internal/philox_generic         TRUE)
mckl_add_test_header(random/internal/philox_sse2_2x32       ${SSE2_FOUND})
mckl_add_test_header(random/internal/philox_sse2_32         ${SSE2_FOUND})
mckl_add_test_header(random/internal/philox_sse2_4x32       ${SSE2_FOUND})
mckl_add_test_header(random/internal/threefry_avx2_16x64    ${AVX2_FOUND})
mckl_add_test_header(random/internal/threefry_avx2_2x32     ${AVX2_FOUND})
mckl_add_test_header(random/internal/threefry_avx2_2x64     ${AVX2_FOUND})
mckl_add_test_header(random/internal/threefry_avx2_32       ${AVX2_FOUND})
mckl_add_test_header(random/internal/threefry_avx2_4x32     ${AVX2_FOUND})
mckl_add_test_header(random/internal/threefry_avx2_4x64     ${AVX2_FOUND})
mckl_add_test_header(random/internal/threefry_avx2_64       ${AVX2_FOUND})
mckl_add_test_header(random/internal/threefry_avx2_8x64     ${AVX2_FOUND})
mckl_add_test_header(random/internal/threefry_common        TRUE)
mckl_add_test_header(random/internal/threefry_constants     TRUE)
mckl_add_test_header(random/internal/threefry_generic       TRUE)
mckl_add_test_header(random/internal/threefry_generic_16x64 TRUE)
mckl_add_test_header(random/internal/threefry_generic_2x32  TRUE)
mckl_add_test_header(random/internal/threefry_generic_2x64  TRUE)
mckl_add_test_header(random/internal/threefry_generic_4x32  TRUE)
mckl_add_test_header(random/internal/threefry_generic_4x64  TRUE)
mckl_add_test_header(random/internal/threefry_generic_8x64  TRUE)
mckl_add_test_header(random/internal/threefry_sse2_16x64    ${SSE2_FOUND})
mckl_add_test_header(random/internal/threefry_sse2_2x32     ${SSE2_FOUND})
mckl_add_test_header(random/internal/threefry_sse2_2x64     ${SSE2_FOUND})
mckl_add_test_header(random/internal/threefry_sse2_32       ${SSE2_FOUND})
mckl_add_test_header(random/internal/threefry_sse2_4x32     ${SSE2_FOUND})
mckl_add_test_header(random/internal/threefry_sse2_4x64     ${SSE2_FOUND})
mckl_add_test_header(random/internal/threefry_sse2_64       ${SSE2_FOUND})
mckl_add_test_header(random/internal/threefry_sse2_8x64     ${SSE2_FOUND})

mckl_add_test_header(random TRUE)
mckl_add_test_header(random/rng_set  TRUE)
mckl_add_test_header(random/seed     TRUE)
mckl_add_test_header(random/u01      TRUE)
mckl_add_test_header(random/testu01  ${TestU01_FOUND})

mckl_add_test_header(random/distribution TRUE)
mckl_add_test_header(random/arcsine_distribution       TRUE)
mckl_add_test_header(random/beta_distribution          TRUE)
mckl_add_test_header(random/cauchy_distribution        TRUE)
mckl_add_test_header(random/chi_squared_distribution   TRUE)
mckl_add_test_header(random/dirichlet_distribution     TRUE)
mckl_add_test_header(random/discrete_distribution      TRUE)
mckl_add_test_header(random/exponential_distribution   TRUE)
mckl_add_test_header(random/extreme_value_distribution TRUE)
mckl_add_test_header(random/fisher_f_distribution      TRUE)
mckl_add_test_header(random/gamma_distribution         TRUE)
mckl_add_test_header(random/geometric_distribution     TRUE)
mckl_add_test_header(random/laplace_distribution       TRUE)
mckl_add_test_header(random/levy_distribution          TRUE)
mckl_add_test_header(random/logistic_distribution      TRUE)
mckl_add_test_header(random/lognormal_distribution     TRUE)
mckl_add_test_header(random/normal_distribution        TRUE)
mckl_add_test_header(random/normal_mv_distribution     TRUE)
mckl_add_test_header(random/pareto_distribution        TRUE)
mckl_add_test_header(random/rayleigh_distribution      TRUE)
mckl_add_test_header(random/stable_distribution        TRUE)
mckl_add_test_header(random/student_t_distribution     TRUE)
mckl_add_test_header(random/u01_distribution           TRUE)
mckl_add_test_header(random/uniform_bits_distribution  TRUE)
mckl_add_test_header(random/uniform_int_distribution   TRUE)
mckl_add_test_header(random/uniform_real_distribution  TRUE)
mckl_add_test_header(random/weibull_distribution       TRUE)

mckl_add_test_header(random/rng TRUE)
mckl_add_test_header(random/aes       TRUE)
mckl_add_test_header(random/counter   TRUE)
mckl_add_test_header(random/increment TRUE)
mckl_add_test_header(random/mkl       ${MKL_FOUND})
mckl_add_test_header(random/philox    TRUE)
mckl_add_test_header(random/rdrand    ${RDRAND_FOUND})
mckl_add_test_header(random/skein     TRUE)
mckl_add_test_header(random/threefry  TRUE)

mckl_add_test_header(random/test TRUE)
mckl_add_test_header(random/birthday_spacings_test TRUE)
mckl_add_test_header(random/chi_squared_test       TRUE)
mckl_add_test_header(random/collision_test         TRUE)
mckl_add_test_header(random/coupon_collector_test  TRUE)
mckl_add_test_header(random/gap_test               TRUE)
mckl_add_test_header(random/maximum_of_t_test      TRUE)
mckl_add_test_header(random/permutation_test       TRUE)
mckl_add_test_header(random/poisson_test           TRUE)
mckl_add_test_header(random/poker_test             TRUE)
mckl_add_test_header(random/run_test               TRUE)
mckl_add_test_header(random/serial_test            TRUE)

mckl_add_test_header(smp TRUE "OpenMP")
mckl_add_test_header(smp/backend_base TRUE)
mckl_add_test_header(smp/backend_omp  TRUE "OpenMP")
mckl_add_test_header(smp/backend_seq  TRUE)
mckl_add_test_header(smp/backend_tbb  ${TBB_FOUND})

mckl_add_test_header(utility TRUE)
mckl_add_test_header(utility/covariance     TRUE)
mckl_add_test_header(utility/hdf5           ${HDF5_FOUND})
mckl_add_test_header(utility/stop_watch     TRUE)
