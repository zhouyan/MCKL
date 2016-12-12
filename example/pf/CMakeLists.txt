# ============================================================================
#  MCKL/example/pf/CMakeLists.txt
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

project(MCKLExample-pf CXX)

mckl_add_example(pf)

set(PF_BACKEND BackendOMP BackendSEQ BackendSTD)
if(TBB_FOUND)
    set(PF_BACKEND ${PF_BACKEND} BackendTBB)
endif(TBB_FOUND)

set(PF_SCHEME Multinomial Stratified Systematic Residual ResidualStratified
    ResidualSystematic)

set(PF_LAYOUT RowMajor ColMajor)

set(PF_RNGSET RNGSetVector)
if(TBB_FOUND)
    set(PF_RNGSET ${PF_RNGSET} RNGSetTBB RNGSetTBBKPI)
endif(TBB_FOUND)

mckl_add_test(pf core  "OpenMP")
mckl_add_test(pf smp   "OpenMP")
mckl_add_test(pf truth "OpenMP")

mckl_add_file(pf pf.data)
mckl_add_file(pf pf.truth)
mckl_add_file(pf pf_core.R)
mckl_add_file(pf pf_cv.R)

add_library(libpf ${PROJECT_SOURCE_DIR}/src/pf.cpp)
set_target_properties(libpf PROPERTIES OUTPUT_NAME pf)

add_custom_target(pf_cv)
add_custom_target(pf_cv-check)
foreach(Backend ${PF_BACKEND})
    foreach(Scheme ${PF_SCHEME})
        foreach(Layout ${PF_LAYOUT})
            foreach(RNGSet ${PF_RNGSET})
                string(TOLOWER ${Backend} smp)
                string(TOLOWER ${Scheme} res)
                string(TOLOWER ${Layout} rc)
                string(TOLOWER ${RNGSet} rs)
                set(config ${smp}_${res}_${rc}_${rs})
                configure_file(${PROJECT_SOURCE_DIR}/src/pf_cv.cpp.in
                    ${PROJECT_BINARY_DIR}/src/pf_cv_${config}.cpp)
                mckl_add_test(pf cv_${config} "BIN" "OpenMP")
                target_link_libraries(pf_cv_${config} libpf)
                add_dependencies(pf_cv pf_cv_${config})
                add_dependencies(pf_cv-check pf_cv_${config}-check)
            endforeach(RNGSet ${PF_RNGSET})
        endforeach(Layout ${PF_LAYOUT})
    endforeach(Scheme ${PF_SCHEME})
endforeach(Backend ${PF_BACKEND})

add_custom_target(pf_core-pdf
    DEPENDS pf-files pf_core-check
    COMMAND Rscript "${PROJECT_BINARY_DIR}/pf_core.R"
    COMMENT "Plotting pf_core-check results"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

add_custom_target(pf_cv-pdf
    DEPENDS pf-files pf_cv-check
    COMMAND Rscript "${PROJECT_BINARY_DIR}/pf_cv.R"
    COMMENT "Plotting pf_cv-check results"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
