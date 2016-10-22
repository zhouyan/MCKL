# ============================================================================
#  MCKL/cmake/MCKL.cmake
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

##############################################################################
# Functions
##############################################################################

FUNCTION(MCKL_LINK_TARGET target)
    IF(DEFINED MCKL_LINK_LIBRARIES)
        TARGET_LINK_LIBRARIES(${target} ${MCKL_LINK_LIBRARIES})
    ENDIF(DEFINED MCKL_LINK_LIBRARIES)

    UNSET(compile_flags)
    UNSET(link_flags)

    IF("${ARGN}" MATCHES "OpenMP" AND OPENMP_FOUND)
        SET(compile_flags "${compile_flags} ${OpenMP_CXX_FLAGS}")
        IF(NOT MSVC)
            SET(link_flags "${link_flags} ${OpenMP_CXX_FLAGS}")
        ENDIF(NOT MSVC)
    ENDIF("${ARGN}" MATCHES "OpenMP" AND OPENMP_FOUND)

    IF(NOT MKL_FOUND)
        SET(link_flags "${link_flags} ${BLAS_LINKER_FLAGS}")
    ENDIF(NOT MKL_FOUND)

    IF(compile_flags)
        SET_TARGET_PROPERTIES(${target} PROPERTIES COMPILE_FLAGS
            "${compile_flags}")
    ENDIF(compile_flags)

    IF(link_flags)
        SET_TARGET_PROPERTIES(${target} PROPERTIES LINK_FLAGS
            "${link_flags}")
    ENDIF(link_flags)
ENDFUNCTION(MCKL_LINK_TARGET)

FUNCTION(MCKL_ADD_EXECUTABLE exe src)
    ADD_EXECUTABLE(${exe} ${src})
    MCKL_LINK_TARGET(${exe} ${ARGN})
ENDFUNCTION(MCKL_ADD_EXECUTABLE)

FUNCTION(MCKL_ADD_EXAMPLE exname)
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

    ADD_CUSTOM_TARGET(${exname})
    ADD_CUSTOM_TARGET(${exname}-check)
    ADD_CUSTOM_TARGET(${exname}-files)
    ADD_DEPENDENCIES(${exname} ${exname}-files)
    ADD_DEPENDENCIES(example ${exname})
ENDFUNCTION(MCKL_ADD_EXAMPLE)

FUNCTION(MCKL_ADD_TEST exname testname)
    SET(src ${exname}_${testname})
    IF ("${ARGN}" MATCHES "BIN")
        MCKL_ADD_EXECUTABLE(${src}
            ${PROJECT_BINARY_DIR}/src/${src}.cpp ${ARGN})
    ELSE ("${ARGN}" MATCHES "BIN")
        MCKL_ADD_EXECUTABLE(${src}
            ${PROJECT_SOURCE_DIR}/src/${src}.cpp ${ARGN})
    ENDIF ("${ARGN}" MATCHES "BIN")
    ADD_DEPENDENCIES(${exname} ${src})
    ADD_CUSTOM_TARGET(${src}-check
        DEPENDS ${src} ${exname}-files
        COMMAND ${MCKL_TEST_EXEC} ${src}
        COMMENT "Checking ${src}"
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
    ADD_DEPENDENCIES(${exname}-check ${src}-check)
ENDFUNCTION(MCKL_ADD_TEST)

FUNCTION(MCKL_ADD_FILE exname filename)
    SET(src ${exname}_${filename})
    IF(UNIX)
        ADD_CUSTOM_COMMAND(
            OUTPUT  ${PROJECT_BINARY_DIR}/${filename}
            DEPENDS ${PROJECT_SOURCE_DIR}/${filename}
            COMMAND ${CMAKE_COMMAND} ARGS -E create_symlink
            ${PROJECT_SOURCE_DIR}/${filename}
            ${PROJECT_BINARY_DIR}/${filename})
    ELSE(UNIX)
        ADD_CUSTOM_COMMAND(
            OUTPUT  ${PROJECT_BINARY_DIR}/${filename}
            DEPENDS ${PROJECT_SOURCE_DIR}/${filename}
            COMMAND ${CMAKE_COMMAND} ARGS -E copy
            ${PROJECT_SOURCE_DIR}/${filename}
            ${PROJECT_BINARY_DIR}/${filename})
    ENDIF(UNIX)
    ADD_CUSTOM_TARGET(${src} DEPENDS ${PROJECT_BINARY_DIR}/${filename})
    ADD_DEPENDENCIES(${exname}-files ${src})
ENDFUNCTION(MCKL_ADD_FILE)

##############################################################################
# Essential
##############################################################################

SET(CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS}
    -D__STDC_CONSTANT_MACROS)

ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

##############################################################################
# Check features
##############################################################################

SET(FEATURES)

# SSE2
INCLUDE(FindSSE2)
IF(SSE2_FOUND)
    SET(FEATURES ${FEATURES} "SSE2")
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_SSE2=1)
ENDIF(SSE2_FOUND)

# AVX2
INCLUDE(FindAVX2)
IF(AVX2_FOUND)
    SET(FEATURES ${FEATURES} "AVX2")
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_AVX2=1)
ENDIF(AVX2_FOUND)

# AES-NI
INCLUDE(FindAESNI)
IF(AESNI_FOUND)
    SET(FEATURES ${FEATURES} "AES-NI")
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_AESNI=1)
ENDIF(AESNI_FOUND)

# RDRAND
INCLUDE(FindRDRAND)
IF(RDRAND_FOUND)
    SET(FEATURES ${FEATURES} "RDRAND")
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_RDRAND=1)
ENDIF(RDRAND_FOUND)

# BMI2
INCLUDE(FindBMI2)
IF(BMI2_FOUND)
    SET(FEATURES ${FEATURES} "BMI2")
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_BMI2=1)
ENDIF(BMI2_FOUND)

# RDTSCP
INCLUDE(FindRDTSCP)
IF(RDTSCP_FOUND)
    SET(FEATURES ${FEATURES} "RDTSCP")
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_RDTSCP=1)
ENDIF(RDTSCP_FOUND)

# 128-bit integer type
INCLUDE(FindInt128)
IF(INT128_FOUND)
    SET(FEATURES ${FEATURES} "128-bit integer type")
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_INT128=1)
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_INT128=${INT128_TYPE})
ENDIF(INT128_FOUND)

# POSIX
INCLUDE(FindPOSIX)
IF(POSIX_FOUND)
    SET(FEATURES ${FEATURES} "POSIX")
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_POSIX=1)
ENDIF(POSIX_FOUND)

# Thread
INCLUDE(FindThread)
IF(THREAD_FOUND)
    SET(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${Thread_LINK_LIBRARIES})
ENDIF(THREAD_FOUND)

# OpenMP
IF(NOT DEFINED OPENMP_FOUND)
    INCLUDE(FindOpenMP)
ENDIF(NOT DEFINED OPENMP_FOUND)
IF(OPENMP_FOUND)
    SET(FEATURES ${FEATURES} "OpenMP")
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_OMP=1)
ENDIF(OPENMP_FOUND)

# Intel TBB
INCLUDE(FindTBB)
IF(TBB_FOUND)
    SET(FEATURES ${FEATURES} "Intel TBB")
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} ${TBB_DEFINITIONS})
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_TBB=1)
    SET(MCKL_INCLUDE_DIRS ${MCKL_INCLUDE_DIRS} ${TBB_INCLUDE_DIR})
    SET(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${TBB_LINK_LIBRARIES})
ENDIF(TBB_FOUND)

# TBB malloc
IF(TBB_FOUND AND TBB_MALLOC_LINK_LIBRARIES)
    SET(FEATURES ${FEATURES} "Intel TBB malloc")
    SET(TBB_MALLOC_FOUND TRUE CACHE BOOL "Found Intel TBB malloc")
    SET(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES}
        ${TBB_MALLOC_LINK_LIBRARIES})
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_USE_TBB_MALLOC=1)
ENDIF(TBB_FOUND AND TBB_MALLOC_LINK_LIBRARIES)

# MKL or BLAS
INCLUDE(FindMKL)
IF(MKL_FOUND)
    SET(FEATURES ${FEATURES} "Intel MKL")
    SET(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${MKL_LINK_LIBRARIES})
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_MKL=1)
    SET(MCKL_INCLUDE_DIRS ${MCKL_INCLUDE_DIRS} ${MKL_INCLUDE_DIR})
ELSE(MKL_FOUND)
    IF(NOT DEFINED BLAS_FOUND)
        INCLUDE(FindBLAS)
    ENDIF(NOT DEFINED BLAS_FOUND)
    IF(BLAS_FOUND)
        SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_BLAS=1)
        SET(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${BLAS_LIBRARIES})
    ENDIF(BLAS_FOUND)
ENDIF(MKL_FOUND)

# HDF5
IF(NOT DEFINED HDF5_FOUND)
    INCLUDE(FindHDF5)
ENDIF(NOT DEFINED HDF5_FOUND)
IF(HDF5_FOUND)
    SET(FEATURES ${FEATURES} "HDF5")
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_HDF5=1)
    SET(MCKL_INCLUDE_DIRS ${MCKL_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS})
    SET(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${HDF5_LIBRARIES})
ENDIF(HDF5_FOUND)

# TestU01
INCLUDE(FindTestU01)
IF(TestU01_FOUND)
    SET(FEATURES ${FEATURES} "TestU01")
    SET(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_TESTU01=1)
    SET(MCKL_INCLUDE_DIRS ${MCKL_INCLUDE_DIRS} ${TestU01_INCLUDE_DIR})
    SET(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${TestU01_LINK_LIBRARIES})
ENDIF(TestU01_FOUND)

##############################################################################
# Summary
##############################################################################

ADD_DEFINITIONS(${MCKL_DEFINITIONS})
INCLUDE_DIRECTORIES(SYSTEM ${MCKL_INCLUDE_DIRS})

MESSAGE(STATUS "=================== Definitions =======================")
FOREACH(def ${MCKL_DEFINITIONS})
    MESSAGE(STATUS "${def}")
ENDFOREACH(def ${MCKL_DEFINITIONS})

MESSAGE(STATUS "=================== Includes ==========================")
FOREACH(inc ${MCKL_INCLUDE_DIRS})
    MESSAGE(STATUS "${inc}")
ENDFOREACH(inc ${MCKL_INCLUDE_DIRS})

MESSAGE(STATUS "=================== Libraries =========================")
FOREACH(lib ${MCKL_LINK_LIBRARIES})
    MESSAGE(STATUS "${lib}")
ENDFOREACH(lib ${MCKL_LINK_LIBRARIES})

MESSAGE(STATUS "=================== Features ==========================")
FOREACH(feature ${FEATURES})
    MESSAGE(STATUS ${feature})
ENDFOREACH(feature ${FEATURES})
