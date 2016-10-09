//============================================================================
// MCKL/example/random/src/random_threefish.cpp
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

#include <mckl/random/threefry.hpp>

#define MCKL_EXAMPLE_RANDOM_THREEFISH(N)                                      \
    typename mckl::Threefish##N::generator_type generator##N;                 \
    typename mckl::Threefish##N::key_type test_key##N;                        \
    typename mckl::Threefish##N::ctr_type test_ctr##N;                        \
    typename mckl::Threefish##N::ctr_type test_res##N;                        \
    typename mckl::Threefish##N::ctr_type test_buf##N;                        \
    std::memcpy(test_key##N.data(), key##N, sizeof(test_key##N));             \
    std::memcpy(test_ctr##N.data(), ctr##N, sizeof(test_ctr##N));             \
    std::memcpy(test_res##N.data(), res##N, sizeof(test_res##N));             \
    generator##N.reset(test_key##N);                                          \
    generator##N.tweak(tweak##N[0], tweak##N[1]);                             \
    generator##N.enc(test_ctr##N, test_buf##N);                               \
    std::cout << std::setw(16) << std::left                                   \
              << std::string("Threefish-" #N ":")                             \
              << (test_buf##N == test_res##N ? "Passed" : "Failed")           \
              << std::endl;

static const std::uint64_t key256[] = {0x1716151413121110, 0x1F1E1D1C1B1A1918,
    0x2726252423222120, 0x2F2E2D2C2B2A2928};

static const std::uint64_t key512[] = {0x1716151413121110, 0x1F1E1D1C1B1A1918,
    0x2726252423222120, 0x2F2E2D2C2B2A2928, 0x3736353433323130,
    0x3F3E3D3C3B3A3938, 0x4746454443424140, 0x4F4E4D4C4B4A4948};

static const std::uint64_t key1024[] = {0x1716151413121110, 0x1F1E1D1C1B1A1918,
    0x2726252423222120, 0x2F2E2D2C2B2A2928, 0x3736353433323130,
    0x3F3E3D3C3B3A3938, 0x4746454443424140, 0x4F4E4D4C4B4A4948,
    0x5756555453525150, 0x5F5E5D5C5B5A5958, 0x6766656463626160,
    0x6F6E6D6C6B6A6968, 0x7776757473727170, 0x7F7E7D7C7B7A7978,
    0x8786858483828180, 0x8F8E8D8C8B8A8988};

static const std::uint64_t tweak256[] = {
    0x0706050403020100, 0x0F0E0D0C0B0A0908};

static const std::uint64_t tweak512[] = {
    0x0706050403020100, 0x0F0E0D0C0B0A0908};

static const std::uint64_t tweak1024[] = {
    0x0706050403020100, 0x0F0E0D0C0B0A0908};

static const std::uint64_t ctr256[] = {0xF8F9FAFBFCFDFEFF, 0xF0F1F2F3F4F5F6F7,
    0xE8E9EAEBECEDEEEF, 0xE0E1E2E3E4E5E6E7};

static const std::uint64_t ctr512[] = {0xF8F9FAFBFCFDFEFF, 0xF0F1F2F3F4F5F6F7,
    0xE8E9EAEBECEDEEEF, 0xE0E1E2E3E4E5E6E7, 0xD8D9DADBDCDDDEDF,
    0xD0D1D2D3D4D5D6D7, 0xC8C9CACBCCCDCECF, 0xC0C1C2C3C4C5C6C7};

static const std::uint64_t ctr1024[] = {0xF8F9FAFBFCFDFEFF, 0xF0F1F2F3F4F5F6F7,
    0xE8E9EAEBECEDEEEF, 0xE0E1E2E3E4E5E6E7, 0xD8D9DADBDCDDDEDF,
    0xD0D1D2D3D4D5D6D7, 0xC8C9CACBCCCDCECF, 0xC0C1C2C3C4C5C6C7,
    0xB8B9BABBBCBDBEBF, 0xB0B1B2B3B4B5B6B7, 0xA8A9AAABACADAEAF,
    0xA0A1A2A3A4A5A6A7, 0x98999A9B9C9D9E9F, 0x9091929394959697,
    0x88898A8B8C8D8E8F, 0x8081828384858687};

static const std::uint64_t res256[] = {0xDF8FEA0EFF91D0E0, 0xD50AD82EE69281C9,
    0x76F48D58085D869D, 0xDF975E95B5567065};

static const std::uint64_t res512[] = {0x2C5AD426964304E3, 0x9A2436D6D8CA01B4,
    0xDD456DB00E333863, 0x794725970EB9368B, 0x043546998D0A2A27,
    0x25A7C918EA204478, 0x346201A1FEDF11AF, 0x3DAF1C5C3D672789};

static const std::uint64_t res1024[] = {0xB0C33CD7DB4D65A6, 0xBC49A85A1077D75D,
    0x6855FCAFEA7293E4, 0x1C5385AB1B7754D2, 0x30E4AAFFE780F794,
    0xE1BBEE708CAFD8D5, 0x9CA837B7423B0F76, 0xBD1403670D4963B3,
    0x451F2E3CE61EA48A, 0xB360832F9277D4FB, 0x0AAFC7A65E12D688,
    0xC8906E79016D05D7, 0xB316570A15F41333, 0x74E98A2869F5D50E,
    0x57CE6F9247432BCE, 0xDE7CDD77215144DE};

int main()
{
    MCKL_EXAMPLE_RANDOM_THREEFISH(256)
    MCKL_EXAMPLE_RANDOM_THREEFISH(512)
    MCKL_EXAMPLE_RANDOM_THREEFISH(1024)

    return 0;
}
