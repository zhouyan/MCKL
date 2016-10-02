# ============================================================================
#  MCKL/CMakeLists.txt
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
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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

CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)

##############################################################################
# Disable in source build
##############################################################################

IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    MESSAGE(FATAL_ERROR "In-source builds not allowed")
ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

##############################################################################
# Basics
##############################################################################

PROJECT(MCKL VERSION 0.1 LANGUAGES CXX)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

SET (MCKL_GOOD_COMPILER TRUE)
IF (${CMAKE_CXX_COMPILER_ID} MATCHES "Intel" AND APPLE)
    IF(${CMAKE_CXX_COMPILER_VERSION} MATCHES "16.0.2.20160204")
        SET(MCKL_GOOD_COMPILER FALSE)
    ENDIF(${CMAKE_CXX_COMPILER_VERSION} MATCHES "16.0.2.20160204")
ENDIF (${CMAKE_CXX_COMPILER_ID} MATCHES "Intel" AND APPLE)

##############################################################################
# Options
##############################################################################

OPTION(MCKL_ENABLE_EXAMPLE "Enable building of example" ON)

##############################################################################
# Documentations
##############################################################################

INCLUDE(FindDoxygen)
IF(DOXYGEN_FOUND)
    FILE(READ ${PROJECT_SOURCE_DIR}/README.md README)
    FILE(READ ${PROJECT_SOURCE_DIR}/NEWS.md NEWS)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/config/Doxyfile.in
        ${PROJECT_BINARY_DIR}/config/Doxyfile)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/config/main.md.in
        ${PROJECT_BINARY_DIR}/config/main.md)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/config/news.md.in
        ${PROJECT_BINARY_DIR}/config/news.md)
    ADD_CUSTOM_TARGET(doxygen
        DEPENDS
        ${PROJECT_BINARY_DIR}/config/Doxyfile
        ${PROJECT_BINARY_DIR}/config/main.md
        ${PROJECT_BINARY_DIR}/config/news.md
        COMMAND ${DOXYGEN_EXECUTABLE} config/Doxyfile
        COMMENT "Generating doxygen"
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
ENDIF(DOXYGEN_FOUND)

##############################################################################
# Installation
##############################################################################

IF(NOT DEFINED MCKL_INSTALL_INC_DIR)
    SET(MCKL_INSTALL_INC_DIR include)
ENDIF(NOT DEFINED MCKL_INSTALL_INC_DIR)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/include/mckl
    DESTINATION ${MCKL_INSTALL_INC_DIR}/mckl
    PATTERN ".DS_Store" EXCLUDE PATTERN "*.swp" EXCLUDE)

##############################################################################
# Check features
##############################################################################

IF(MCKL_ENABLE_EXAMPLE)
    INCLUDE(MCKL)
ELSE(MCKL_ENABLE_EXAMPLE)
    RETURN()
ENDIF(MCKL_ENABLE_EXAMPLE)

IF (NOT (MKL_FOUND OR BLAS_FOUND))
    RETURN()
ENDIF (NOT (MKL_FOUND OR BLAS_FOUND))

##############################################################################
# Enable backends
##############################################################################

MESSAGE(STATUS "=================== Enable backends ===================")
FOREACH(backend ${BACKENDS})
    MESSAGE(STATUS ${backend})
ENDFOREACH(backend ${BACKENDS})

##############################################################################
# Enable features
##############################################################################

MESSAGE(STATUS "=================== Enable features ===================")
FOREACH(feature ${FEATURES})
    MESSAGE(STATUS ${feature})
ENDFOREACH(feature ${FEATURES})

##############################################################################
# Example
##############################################################################

IF(MCKL_ENABLE_EXAMPLE)
    ADD_CUSTOM_TARGET(example ALL)
    ADD_CUSTOM_TARGET(check)
    ADD_SUBDIRECTORY(example)
ENDIF(MCKL_ENABLE_EXAMPLE)

##############################################################################
# Example
##############################################################################

MESSAGE(STATUS "=======================================================")
IF (NOT MCKL_GOOD_COMPILER)
    MESSAGE(STATUS "Runtime library and some examples are disabled due to")
    MESSAGE(STATUS "known issues with compilers")
    MESSAGE (STATUS
        "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
    MESSAGE(STATUS "=======================================================")
ENDIF (NOT MCKL_GOOD_COMPILER)
