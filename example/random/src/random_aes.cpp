//============================================================================
// MCKL/example/random/src/random_aes.cpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2016, Yan Zhou
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION); HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE);
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#include <mckl/random/aes.hpp>

#define MCKL_EXAMPLE_RANDOM_AES(N)                                            \
    typename mckl::AES##N::generator_type generator##N;                       \
    typename mckl::AES##N::key_type test_key##N;                              \
    typename mckl::AES##N::ctr_type test_ctr##N;                              \
    typename mckl::AES##N::ctr_type test_res##N;                              \
    std::memcpy(test_key##N.data(), key##N, sizeof(test_key##N));             \
    std::memcpy(test_ctr##N.data(), ctr##N, sizeof(test_ctr##N));             \
    mckl::internal::union_le<std::uint8_t>(test_key##N);                      \
    generator##N.reset(test_key##N);                                          \
    generator##N(test_ctr##N.data(), test_res##N.data());                     \
    bool pass##N =                                                            \
        std::memcmp(test_res##N.data(), res##N, sizeof(test_res##N)) == 0;    \
    std::cout << std::setw(16) << std::left << std::string("AES-" #N ":")     \
              << (pass##N ? "Passed" : "Failed") << std::endl;

static const std::uint8_t key128[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

static const std::uint8_t key192[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
    0x13, 0x14, 0x15, 0x16, 0x17};

static const std::uint8_t key256[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
    0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
    0x1F};

static const std::uint8_t ctr128[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
    0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

static const std::uint8_t ctr192[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
    0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

static const std::uint8_t ctr256[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
    0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

static const std::uint8_t res128[] = {0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04,
    0x30, 0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A};

static const std::uint8_t res192[] = {0xDD, 0xA9, 0x7C, 0xA4, 0x86, 0x4C, 0xDF,
    0xE0, 0x6E, 0xAF, 0x70, 0xA0, 0xEC, 0x0D, 0x71, 0x91};

static const std::uint8_t res256[] = {0x8E, 0xA2, 0xB7, 0xCA, 0x51, 0x67, 0x45,
    0xBF, 0xEA, 0xFC, 0x49, 0x90, 0x4B, 0x49, 0x60, 0x89};

int main()
{
    MCKL_EXAMPLE_RANDOM_AES(128)
    MCKL_EXAMPLE_RANDOM_AES(192)
    MCKL_EXAMPLE_RANDOM_AES(256)

    return 0;
}
