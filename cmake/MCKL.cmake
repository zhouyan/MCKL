# ============================================================================
#  MCKL/cmake/MCKL.cmake
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

##############################################################################
# Functions
##############################################################################

function(mckl_link_target target)
    if(DEFINED MCKL_LINK_LIBRARIES)
        target_link_libraries(${target} ${MCKL_LINK_LIBRARIES})
    endif(DEFINED MCKL_LINK_LIBRARIES)

    unset(compile_flags)
    unset(link_flags)

    if("${ARGN}" MATCHES "OpenMP" AND OPENMP_FOUND)
        set(compile_flags "${compile_flags} ${OpenMP_CXX_FLAGS}")
        if(NOT MSVC)
            set(link_flags "${link_flags} ${OpenMP_CXX_FLAGS}")
        endif(NOT MSVC)
    endif("${ARGN}" MATCHES "OpenMP" AND OPENMP_FOUND)

    if(NOT MKL_FOUND)
        set(link_flags "${link_flags} ${BLAS_LINKER_FLAGS}")
    endif(NOT MKL_FOUND)

    if(compile_flags)
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS
            "${compile_flags}")
    endif(compile_flags)

    if(link_flags)
        set_target_properties(${target} PROPERTIES LINK_FLAGS
            "${link_flags}")
    endif(link_flags)
endfunction(mckl_link_target)

function(mckl_add_executable exe src)
    add_executable(${exe} ${src})
    mckl_link_target(${exe} ${ARGN})
endfunction(mckl_add_executable)

function(mckl_add_example exname)
    include_directories(${PROJECT_SOURCE_DIR}/include)

    add_custom_target(${exname})
    add_custom_target(${exname}-check)
    add_custom_target(${exname}-files)
    add_custom_target(${exname}-plots)
    add_dependencies(${exname} ${exname}-files)
    add_dependencies(example ${exname})
endfunction(mckl_add_example)

function(mckl_add_test exname testname)
    set(src ${exname}_${testname})
    if("${ARGN}" MATCHES "BIN")
        mckl_add_executable(${src}
            ${PROJECT_BINARY_DIR}/src/${src}.cpp ${ARGN})
    else("${ARGN}" MATCHES "BIN")
        mckl_add_executable(${src}
            ${PROJECT_SOURCE_DIR}/src/${src}.cpp ${ARGN})
    endif("${ARGN}" MATCHES "BIN")
    add_dependencies(${exname} ${src})
    add_custom_target(${src}-check
        DEPENDS ${src} ${exname}-files
        COMMAND ${MCKL_TEST_EXEC} ${src}
        COMMENT "Checking ${src}"
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
    add_dependencies(${exname}-check ${src}-check)
endfunction(mckl_add_test)

function(mckl_add_file exname filename)
    if(UNIX)
        ADD_CUSTOM_COMMAND(
            OUTPUT  ${PROJECT_BINARY_DIR}/${exname}_${filename}
            DEPENDS ${PROJECT_SOURCE_DIR}/${exname}_${filename}
            COMMAND ${CMAKE_COMMAND} ARGS -E create_symlink
            ${PROJECT_SOURCE_DIR}/${exname}_${filename}
            ${PROJECT_BINARY_DIR}/${exname}_${filename})
    else(UNIX)
        ADD_CUSTOM_COMMAND(
            OUTPUT  ${PROJECT_BINARY_DIR}/${exname}_${filename}
            DEPENDS ${PROJECT_SOURCE_DIR}/${exname}_${filename}
            COMMAND ${CMAKE_COMMAND} ARGS -E copy
            ${PROJECT_SOURCE_DIR}/${exname}_${filename}
            ${PROJECT_BINARY_DIR}/${exname}_${filename})
    endif(UNIX)
    add_custom_target(${exname}-${exname}_${filename}
        DEPENDS ${PROJECT_BINARY_DIR}/${exname}_${filename})
    add_dependencies(${exname}-files ${exname}-${exname}_${filename})
endfunction(mckl_add_file)

function(mckl_add_plot exname testname)
    mckl_add_file(${exname} ${testname}.R)
    add_custom_target(${exname}_${testname}-plot
        DEPENDS ${exname}-files ${exname}_${testname}-check
        COMMAND Rscript "${PROJECT_BINARY_DIR}/${exname}_${testname}.R"
        COMMENT "Plotting ${exname}_${testname}-check results"
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
    add_dependencies(${exname}-plots ${exname}_${testname}-plot)
endfunction(mckl_add_plot exname testname)

##############################################################################
# Essential
##############################################################################

include_directories(${PROJECT_SOURCE_DIR}/include)

##############################################################################
# Check features
##############################################################################

set(FEATURES)

# SSE2
include(FindSSE2)
if(SSE2_FOUND)
    set(FEATURES ${FEATURES} "SSE2")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_SSE2=1)
else(SSE2_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_SSE2=0)
endif(SSE2_FOUND)

# AVX2
include(FindAVX2)
if(AVX2_FOUND)
    set(FEATURES ${FEATURES} "AVX2")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_AVX2=1)
else(AVX2_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_AVX2=0)
endif(AVX2_FOUND)

# AES-NI
include(FindAESNI)
if(AESNI_FOUND)
    set(FEATURES ${FEATURES} "AES-NI")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_AESNI=1)
else(AESNI_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_AESNI=0)
endif(AESNI_FOUND)

# RDRAND
include(FindRDRAND)
if(RDRAND_FOUND)
    set(FEATURES ${FEATURES} "RDRAND")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_RDRAND=1)
else(RDRAND_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_RDRAND=0)
endif(RDRAND_FOUND)

# BMI2
include(FindBMI2)
if(BMI2_FOUND)
    set(FEATURES ${FEATURES} "BMI2")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_BMI2=1)
else(BMI2_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_BMI2=0)
endif(BMI2_FOUND)

# FMA
include(FindFMA)
if(FMA_FOUND)
    set(FEATURES ${FEATURES} "FMA")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_FMA=1)
else(FMA_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_FMA=0)
endif(FMA_FOUND)

# NASM
enable_language(ASM_NASM)
if(CMAKE_ASM_NASM_COMPILER_LOADED)
    set(FEATURES ${FEATURES} "ASM")
endif(CMAKE_ASM_NASM_COMPILER_LOADED)

# 128-bit integer type
include(FindInt128)
if(INT128_FOUND)
    set(FEATURES ${FEATURES} "128-bit integer type")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_INT128=1)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_INT128=${INT128_TYPE})
else(INT128_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_INT128=0)
endif(INT128_FOUND)

# POSIX
include(FindPOSIX)
if(POSIX_FOUND)
    set(FEATURES ${FEATURES} "POSIX")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_POSIX=1)
else(POSIX_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_POSIX=0)
endif(POSIX_FOUND)

# Thread
include(FindThread)
if(THREAD_FOUND)
    set(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${Thread_LINK_LIBRARIES})
endif(THREAD_FOUND)

# OpenMP
if(NOT DEFINED OPENMP_FOUND)
    include(FindOpenMP)
endif(NOT DEFINED OPENMP_FOUND)
if(OPENMP_FOUND)
    set(FEATURES ${FEATURES} "OpenMP")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_OMP=1)
else(OPENMP_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_OMP=0)
endif(OPENMP_FOUND)

# jemalloc
include(FindJEMalloc)
if(JEMALLOC_FOUND)
    set(FEATURES ${FEATURES} "jemalloc")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_JEMALLOC=1)
    set(MCKL_INCLUDE_DIRS ${MCKL_INCLUDE_DIRS} ${JEMalloc_INCLUDE_DIR})
    set(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${JEMalloc_LINK_LIBRARIES})
else(JEMALLOC_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_JEMALLOC=0)
endif(JEMALLOC_FOUND)

# Intel TBB
include(FindTBB)
if(TBB_FOUND)
    set(FEATURES ${FEATURES} "Intel TBB")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_TBB=1)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DJEMALLOC_NO_DEMANGLE)
    set(MCKL_INCLUDE_DIRS ${MCKL_INCLUDE_DIRS} ${TBB_INCLUDE_DIR})
    set(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${TBB_LINK_LIBRARIES})
else(TBB_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_TBB=0)
endif(TBB_FOUND)

# TBB malloc
if(TBB_FOUND AND TBB_MALLOC_LINK_LIBRARIES)
    set(FEATURES ${FEATURES} "Intel TBB malloc")
    set(TBB_MALLOC_FOUND TRUE CACHE BOOL "Found Intel TBB malloc")
    set(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES}
        ${TBB_MALLOC_LINK_LIBRARIES})
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_USE_TBB_MALLOC=1)
else(TBB_FOUND AND TBB_MALLOC_LINK_LIBRARIES)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_USE_TBB_MALLOC=0)
endif(TBB_FOUND AND TBB_MALLOC_LINK_LIBRARIES)

# MKL or BLAS
include(FindMKL)
if(MKL_FOUND)
    set(FEATURES ${FEATURES} "Intel MKL")
    set(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${MKL_LINK_LIBRARIES})
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_MKL=1)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_BLAS=1)
    set(MCKL_INCLUDE_DIRS ${MCKL_INCLUDE_DIRS} ${MKL_INCLUDE_DIR})
else(MKL_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_MKL=0)
    if(NOT DEFINED BLAS_FOUND)
        include(FindBLAS)
    endif(NOT DEFINED BLAS_FOUND)
    if(BLAS_FOUND)
        set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_BLAS=1)
        set(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${BLAS_LIBRARIES})
    else(BLAS_FOUND)
        set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_BLAS=0)
    endif(BLAS_FOUND)
endif(MKL_FOUND)

# Boost
include(FindBoost)
if(Boost_FOUND)
    set(FEATURES ${FEATURES} "Boost")
    set(MCKL_INCLUDE_DIRS ${MCKL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_BOOST=1)
else(Boost_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_BOOST=0)
endif(Boost_FOUND)

# Random123
include(FindRandom123)
if(Random123_FOUND)
    set(FEATURES ${FEATURES} "Random123")
    set(MCKL_INCLUDE_DIRS ${MCKL_INCLUDE_DIRS} ${Random123_INCLUDE_DIR})
endif(Random123_FOUND)

# HDF5
if(NOT DEFINED HDF5_FOUND)
    include(FindHDF5)
endif(NOT DEFINED HDF5_FOUND)
if(HDF5_FOUND)
    set(FEATURES ${FEATURES} "HDF5")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_HDF5=1)
    set(MCKL_INCLUDE_DIRS ${MCKL_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS})
    set(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${HDF5_LIBRARIES})
else(HDF5_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_HDF5=0)
endif(HDF5_FOUND)

# TestU01
include(FindTestU01)
if(TestU01_FOUND)
    set(FEATURES ${FEATURES} "TestU01")
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_TESTU01=1)
    set(MCKL_INCLUDE_DIRS ${MCKL_INCLUDE_DIRS} ${TestU01_INCLUDE_DIR})
    set(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} ${TestU01_LINK_LIBRARIES})
else(TestU01_FOUND)
    set(MCKL_DEFINITIONS ${MCKL_DEFINITIONS} -DMCKL_HAS_TESTU01=0)
endif(TestU01_FOUND)

##############################################################################
# Summary
##############################################################################

add_definitions(${MCKL_DEFINITIONS})
include_directories(SYSTEM ${MCKL_INCLUDE_DIRS})

message(STATUS "=================== Definitions =======================")
foreach(def ${MCKL_DEFINITIONS})
    message(STATUS "${def}")
endforeach(def ${MCKL_DEFINITIONS})

message(STATUS "=================== Includes ==========================")
foreach(inc ${MCKL_INCLUDE_DIRS})
    message(STATUS "${inc}")
endforeach(inc ${MCKL_INCLUDE_DIRS})

message(STATUS "=================== Libraries =========================")
foreach(lib ${MCKL_LINK_LIBRARIES})
    message(STATUS "${lib}")
endforeach(lib ${MCKL_LINK_LIBRARIES})

message(STATUS "=================== Features ==========================")
foreach(feature ${FEATURES})
    message(STATUS ${feature})
endforeach(feature ${FEATURES})

message(STATUS "=======================================================")
