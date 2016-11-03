# ============================================================================
#  MCKL/example/mckl/CMakeLists.txt
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

PROJECT(MCKLExample-mckl CXX)

FUNCTION(MCKL_ADD_TEST_HEADER header_path cond)
    IF(${cond})
        STRING(REPLACE "/" "_" header "${header_path}")
        STRING(REPLACE "mckl_" "" header "${header}")
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/mckl.cpp.in
            ${PROJECT_BINARY_DIR}/src/mckl_${header}.cpp)
        MCKL_ADD_TEST(mckl ${header} "BIN" ${ARGN})
    ENDIF(${cond})
ENDFUNCTION(MCKL_ADD_TEST_HEADER)

MCKL_ADD_EXAMPLE(mckl)

MCKL_ADD_TEST(mckl config)

MCKL_ADD_TEST_HEADER(mckl/mckl TRUE "OpenMP")

MCKL_ADD_TEST_HEADER(mckl/algorithm TRUE)
MCKL_ADD_TEST_HEADER(mckl/algorithm/mh TRUE)

MCKL_ADD_TEST_HEADER(mckl/core TRUE)
MCKL_ADD_TEST_HEADER(mckl/core/monitor      TRUE)
MCKL_ADD_TEST_HEADER(mckl/core/particle     TRUE)
MCKL_ADD_TEST_HEADER(mckl/core/sampler      TRUE)
MCKL_ADD_TEST_HEADER(mckl/core/state_matrix TRUE)
MCKL_ADD_TEST_HEADER(mckl/core/weight       TRUE)

MCKL_ADD_TEST_HEADER(mckl/internal/aesni      ${AESNI_FOUND})
MCKL_ADD_TEST_HEADER(mckl/internal/assert     TRUE)
MCKL_ADD_TEST_HEADER(mckl/internal/avx2       ${AVX2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/internal/avx2_op    ${AVX2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/internal/byte_order TRUE)
MCKL_ADD_TEST_HEADER(mckl/internal/cblas      TRUE)
MCKL_ADD_TEST_HEADER(mckl/internal/common     TRUE)
MCKL_ADD_TEST_HEADER(mckl/internal/const_math TRUE)
MCKL_ADD_TEST_HEADER(mckl/internal/defines    TRUE)
MCKL_ADD_TEST_HEADER(mckl/internal/fma        ${FMA_FOUND})
MCKL_ADD_TEST_HEADER(mckl/internal/forward    TRUE)
MCKL_ADD_TEST_HEADER(mckl/internal/iostream   TRUE)
MCKL_ADD_TEST_HEADER(mckl/internal/sse2       ${SSE2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/internal/sse2_op    ${SSE2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/internal/traits     TRUE)

MCKL_ADD_TEST_HEADER(mckl/math TRUE)
MCKL_ADD_TEST_HEADER(mckl/math/beta      TRUE)
MCKL_ADD_TEST_HEADER(mckl/math/constants TRUE)
MCKL_ADD_TEST_HEADER(mckl/math/erf       TRUE)
MCKL_ADD_TEST_HEADER(mckl/math/gamma     TRUE)
MCKL_ADD_TEST_HEADER(mckl/math/vmath     TRUE)

MCKL_ADD_TEST_HEADER(mckl/random/internal/aes_aesni           ${AESNI_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/aes_constants       TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/internal/aes_generic         TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/internal/common              TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/internal/increment_avx2_64   ${AVX2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/increment_avx2_64_4 ${AVX2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/increment_avx2_64_8 ${AVX2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/increment_avx2_64_f ${AVX2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/increment_sse2_64   ${SSE2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/increment_sse2_64_4 ${SSE2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/increment_sse2_64_8 ${SSE2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/increment_sse2_64_f ${SSE2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/philox_avx2_32      ${AVX2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/philox_constants    TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/internal/philox_generic      TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/internal/philox_sse2_32      ${SSE2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/threefry_avx2_32    ${AVX2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/threefry_avx2_64    ${AVX2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/threefry_constants  TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/internal/threefry_generic    TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/internal/threefry_sse2_32    ${SSE2_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/internal/threefry_sse2_64    ${SSE2_FOUND})

MCKL_ADD_TEST_HEADER(mckl/random TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/rng_set  TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/seed     TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/u01      TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/testu01  ${TestU01_FOUND})

MCKL_ADD_TEST_HEADER(mckl/random/distribution TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/arcsine_distribution       TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/beta_distribution          TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/cauchy_distribution        TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/chi_squared_distribution   TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/dirichlet_distribution     TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/discrete_distribution      TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/exponential_distribution   TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/extreme_value_distribution TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/fisher_f_distribution      TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/gamma_distribution         TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/geometric_distribution     TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/laplace_distribution       TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/levy_distribution          TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/logistic_distribution      TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/lognormal_distribution     TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/normal_distribution        TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/normal_mv_distribution     TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/pareto_distribution        TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/rayleigh_distribution      TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/stable_distribution        TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/student_t_distribution     TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/u01_distribution           TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/uniform_bits_distribution  TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/uniform_int_distribution   TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/uniform_real_distribution  TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/weibull_distribution       TRUE)

MCKL_ADD_TEST_HEADER(mckl/random/rng TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/aes       TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/counter   TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/increment TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/mkl       ${MKL_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/philox    TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/rdrand    ${RDRAND_FOUND})
MCKL_ADD_TEST_HEADER(mckl/random/skein     TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/threefry  TRUE)

MCKL_ADD_TEST_HEADER(mckl/random/test TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/birthday_spacings_test TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/chi_squared_test       TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/collision_test         TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/coupon_collector_test  TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/gap_test               TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/maximum_of_t_test      TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/permutation_test       TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/poisson_test           TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/poker_test             TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/run_test               TRUE)
MCKL_ADD_TEST_HEADER(mckl/random/serial_test            TRUE)

MCKL_ADD_TEST_HEADER(mckl/resample TRUE)
MCKL_ADD_TEST_HEADER(mckl/resample/algorithm    TRUE)
MCKL_ADD_TEST_HEADER(mckl/resample/index        TRUE)
MCKL_ADD_TEST_HEADER(mckl/resample/transform    TRUE)
MCKL_ADD_TEST_HEADER(mckl/resample/u01_sequence TRUE)

MCKL_ADD_TEST_HEADER(mckl/smp TRUE "OpenMP")
MCKL_ADD_TEST_HEADER(mckl/smp/backend_base TRUE)
MCKL_ADD_TEST_HEADER(mckl/smp/backend_omp  TRUE "OpenMP")
MCKL_ADD_TEST_HEADER(mckl/smp/backend_seq  TRUE)
MCKL_ADD_TEST_HEADER(mckl/smp/backend_tbb  ${TBB_FOUND})

MCKL_ADD_TEST_HEADER(mckl/utility TRUE)
MCKL_ADD_TEST_HEADER(mckl/utility/aligned_memory TRUE)
MCKL_ADD_TEST_HEADER(mckl/utility/covariance     TRUE)
MCKL_ADD_TEST_HEADER(mckl/utility/hdf5           ${HDF5_FOUND})
MCKL_ADD_TEST_HEADER(mckl/utility/stop_watch     TRUE)
