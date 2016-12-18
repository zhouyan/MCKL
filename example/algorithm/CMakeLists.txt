# ============================================================================
#  MCKL/example/algorithm/CMakeLists.txt
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

project(MCKLExample-algorithm CXX)

mckl_add_example(algorithm)

mckl_add_test(algorithm resample_index)
mckl_add_test(algorithm resample_transform)
mckl_add_test(algorithm resample_u01_sequence)

set(ALGORITHMS gibbs pf pmcmc)
add_custom_target(algorithm-pdf)
foreach(alg ${ALGORITHMS})
    mckl_add_test(algorithm ${alg})
    mckl_add_file(algorithm algorithm_${alg}.data)
    mckl_add_file(algorithm algorithm_${alg}.R)
    add_custom_target(algorithm_${alg}-pdf
        DEPENDS algorithm-files algorithm_${alg}-check
        COMMAND Rscript "${PROJECT_BINARY_DIR}/algorithm_${alg}.R"
        COMMENT "Plotting algorithm_${alg}-check results"
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
    add_dependencies(algorithm-pdf algorithm_${alg}-pdf)
endforeach(alg ${ALGORITHMS})
