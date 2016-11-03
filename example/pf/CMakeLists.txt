# ============================================================================
#  MCKL/example/pf/CMakeLists.txt
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

PROJECT(MCKLExample-pf CXX)

MCKL_ADD_EXAMPLE(pf)

SET(PF_BACKEND BackendSEQ BackendSTD)
IF(OPENMP_FOUND)
    SET(PF_BACKEND ${PF_BACKEND} BackendOMP)
ENDIF(OPENMP_FOUND)
IF(TBB_FOUND)
    SET(PF_BACKEND ${PF_BACKEND} BackendTBB)
ENDIF(TBB_FOUND)

SET(PF_SCHEME Multinomial Stratified Systematic Residual ResidualStratified
    ResidualSystematic)

SET(PF_LAYOUT RowMajor ColMajor)

SET(PF_RNGSET RNGSetVector)
IF(TBB_FOUND)
    SET(PF_RNGSET ${PF_RNGSET} RNGSetTBB RNGSetTBBKPI)
ENDIF(TBB_FOUND)

MCKL_ADD_TEST(pf core  "OpenMP")
MCKL_ADD_TEST(pf smp   "OpenMP")
MCKL_ADD_TEST(pf truth "OpenMP")

MCKL_ADD_FILE(pf pf.data)
MCKL_ADD_FILE(pf pf.truth)
MCKL_ADD_FILE(pf pf_core.R)
MCKL_ADD_FILE(pf pf_cv.R)

ADD_LIBRARY(libpf ${PROJECT_SOURCE_DIR}/src/pf.cpp)
SET_TARGET_PROPERTIES(libpf PROPERTIES OUTPUT_NAME pf)

ADD_CUSTOM_TARGET(pf_cv)
ADD_CUSTOM_TARGET(pf_cv-check)
FOREACH(Backend ${PF_BACKEND})
    FOREACH(Scheme ${PF_SCHEME})
        FOREACH(Layout ${PF_LAYOUT})
            FOREACH(RNGSet ${PF_RNGSET})
                STRING(TOLOWER ${Backend} smp)
                STRING(TOLOWER ${Scheme} res)
                STRING(TOLOWER ${Layout} rc)
                STRING(TOLOWER ${RNGSet} rs)
                SET(config ${smp}_${res}_${rc}_${rs})
                CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/src/pf_cv.cpp.in
                    ${PROJECT_BINARY_DIR}/src/pf_cv_${config}.cpp)
                MCKL_ADD_TEST(pf cv_${config} "BIN" "OpenMP")
                TARGET_LINK_LIBRARIES(pf_cv_${config} libpf)
                ADD_DEPENDENCIES(pf_cv pf_cv_${config})
                ADD_DEPENDENCIES(pf_cv-check pf_cv_${config}-check)
            ENDFOREACH(RNGSet ${PF_RNGSET})
        ENDFOREACH(Layout ${PF_LAYOUT})
    ENDFOREACH(Scheme ${PF_SCHEME})
ENDFOREACH(Backend ${PF_BACKEND})

ADD_CUSTOM_TARGET(pf_core-pdf
    DEPENDS pf-files pf_core-check
    COMMAND Rscript "${PROJECT_BINARY_DIR}/pf_core.R"
    COMMENT "Plotting pf_core-check results"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

ADD_CUSTOM_TARGET(pf_cv-pdf
    DEPENDS pf-files pf_cv-check
    COMMAND Rscript "${PROJECT_BINARY_DIR}/pf_cv.R"
    COMMENT "Plotting pf_cv-check results"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
