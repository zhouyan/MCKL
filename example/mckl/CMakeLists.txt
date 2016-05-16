# ============================================================================
#  MCKL/example/mckl/CMakeLists.txt
# ----------------------------------------------------------------------------
#                          MCKL: Monte Carlo Kernel Library
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

FUNCTION(MCKL_ADD_HEADER_TEST basepath cond)
    IF(${cond})
        STRING(REPLACE "/" "_" basename "${basepath}")
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/mckl.cpp
            ${PROJECT_BINARY_DIR}/src/${basename}.cpp)
        MCKL_ADD_EXECUTABLE(${basename}_hpp
            ${PROJECT_BINARY_DIR}/src/${basename}.cpp)
        ADD_DEPENDENCIES(mckl ${basename}_hpp)

        ADD_CUSTOM_TARGET(${basename}_hpp-check
            DEPENDS ${basename}_hpp mckl-files
            COMMAND ${basename}_hpp
            COMMENT "Running ${basename}_hpp"
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
        ADD_DEPENDENCIES(mckl-check ${basename}_hpp-check)
    ENDIF(${cond})
ENDFUNCTION(MCKL_ADD_HEADER_TEST)

MCKL_ADD_EXAMPLE(mckl)

MCKL_ADD_HEADER_TEST(mckl/mckl TRUE)

MCKL_ADD_HEADER_TEST(mckl/algorithm/algorithm TRUE)
MCKL_ADD_HEADER_TEST(mckl/algorithm/mh TRUE)

MCKL_ADD_HEADER_TEST(mckl/core/core TRUE)
MCKL_ADD_HEADER_TEST(mckl/core/monitor      TRUE)
MCKL_ADD_HEADER_TEST(mckl/core/particle     TRUE)
MCKL_ADD_HEADER_TEST(mckl/core/sampler      TRUE)
MCKL_ADD_HEADER_TEST(mckl/core/state_matrix TRUE)
MCKL_ADD_HEADER_TEST(mckl/core/weight       TRUE)

MCKL_ADD_HEADER_TEST(mckl/internal/assert  TRUE)
MCKL_ADD_HEADER_TEST(mckl/internal/basic   TRUE)
MCKL_ADD_HEADER_TEST(mckl/internal/common  TRUE)
MCKL_ADD_HEADER_TEST(mckl/internal/defines TRUE)
MCKL_ADD_HEADER_TEST(mckl/internal/forward TRUE)
MCKL_ADD_HEADER_TEST(mckl/internal/traits  TRUE)

MCKL_ADD_HEADER_TEST(mckl/math/math TRUE)
MCKL_ADD_HEADER_TEST(mckl/math/constants TRUE)
MCKL_ADD_HEADER_TEST(mckl/math/vmath     TRUE)

MCKL_ADD_HEADER_TEST(mckl/resample/resample TRUE)
MCKL_ADD_HEADER_TEST(mckl/resample/algorithm TRUE)
MCKL_ADD_HEADER_TEST(mckl/resample/index     TRUE)
MCKL_ADD_HEADER_TEST(mckl/resample/transform TRUE)

MCKL_ADD_HEADER_TEST(mckl/rng/rng TRUE "MKL")
MCKL_ADD_HEADER_TEST(mckl/rng/rng_set         TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/seed            TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/u01             TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/u01_sequence    TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/internal/common TRUE)

MCKL_ADD_HEADER_TEST(mckl/rng/distribution TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/arcsine_distribution       TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/beta_distribution          TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/cauchy_distribution        TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/chi_squared_distribution   TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/dirichlet_distribution     TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/discrete_distribution      TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/exponential_distribution   TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/extreme_value_distribution TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/fisher_f_distribution      TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/gamma_distribution         TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/laplace_distribution       TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/levy_distribution          TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/logistic_distribution      TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/lognormal_distribution     TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/normal_distribution        TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/normal_mv_distribution     TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/pareto_distribution        TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/rayleigh_distribution      TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/student_t_distribution     TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/u01_distribution           TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/uniform_bits_distribution  TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/uniform_real_distribution  TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/weibull_distribution       TRUE)

MCKL_ADD_HEADER_TEST(mckl/rng/engine TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/aes_ni   ${AESNI_FOUND})
MCKL_ADD_HEADER_TEST(mckl/rng/counter  TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/mkl      ${MKL_FOUND})
MCKL_ADD_HEADER_TEST(mckl/rng/philox   TRUE)
MCKL_ADD_HEADER_TEST(mckl/rng/rdrand   ${RDRAND_FOUND})
MCKL_ADD_HEADER_TEST(mckl/rng/threefry TRUE)

MCKL_ADD_HEADER_TEST(mckl/smp/smp TRUE)
MCKL_ADD_HEADER_TEST(mckl/smp/backend_base TRUE)
MCKL_ADD_HEADER_TEST(mckl/smp/backend_omp  ${OPENMP_FOUND})
MCKL_ADD_HEADER_TEST(mckl/smp/backend_seq  TRUE)
MCKL_ADD_HEADER_TEST(mckl/smp/backend_tbb  ${TBB_FOUND})

MCKL_ADD_HEADER_TEST(mckl/utility/utility TRUE)
MCKL_ADD_HEADER_TEST(mckl/utility/aligned_memory TRUE)
MCKL_ADD_HEADER_TEST(mckl/utility/covariance     TRUE)
MCKL_ADD_HEADER_TEST(mckl/utility/hdf5           ${HDF5_FOUND})
MCKL_ADD_HEADER_TEST(mckl/utility/program_option TRUE)
MCKL_ADD_HEADER_TEST(mckl/utility/progress       TRUE)
MCKL_ADD_HEADER_TEST(mckl/utility/stop_watch     TRUE)
