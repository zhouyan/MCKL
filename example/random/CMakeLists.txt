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

PROJECT(MCKLExample-random CXX)

MCKL_ADD_EXAMPLE(random)

MCKL_ADD_TEST(random distribution)
MCKL_ADD_TEST(random distribution_perf)
MCKL_ADD_TEST(random rng)
MCKL_ADD_TEST(random test)
MCKL_ADD_TEST(random u01)
MCKL_ADD_TEST(random uniform_bits)

IF(MKL_FOUND)
    MCKL_ADD_TEST(random mkl_brng)
ENDIF(MKL_FOUND)

MCKL_ADD_FILE(random random_Philox2x32.txt)
MCKL_ADD_FILE(random random_Philox2x64.txt)
MCKL_ADD_FILE(random random_Philox4x32.txt)
MCKL_ADD_FILE(random random_Philox4x64.txt)

MCKL_ADD_FILE(random random_Threefry2x32.txt)
MCKL_ADD_FILE(random random_Threefry2x64.txt)
MCKL_ADD_FILE(random random_Threefry4x32.txt)
MCKL_ADD_FILE(random random_Threefry4x64.txt)
MCKL_ADD_FILE(random random_Threefry8x64.txt)
MCKL_ADD_FILE(random random_Threefry16x64.txt)

IF(AESNI_FOUND)
    MCKL_ADD_TEST(random aes)
    MCKL_ADD_FILE(random random_AES128.txt)
    MCKL_ADD_FILE(random random_AES192.txt)
    MCKL_ADD_FILE(random random_AES256.txt)
    MCKL_ADD_FILE(random random_ARS.txt)
ENDIF(AESNI_FOUND)
