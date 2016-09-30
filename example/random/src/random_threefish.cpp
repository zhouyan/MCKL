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

class Threefish256Constants : public mckl::ThreefryConstants<std::uint64_t, 4>
{
    public:
    static constexpr std::uint64_t tweaks[2] = {
        UINT64_C(0x0706050403020100), UINT64_C(0x0F0E0D0C0B0A0908)};
}; // class Threefish256Constants

class Threefish512Constants : public mckl::ThreefryConstants<std::uint64_t, 8>
{
    public:
    static constexpr std::uint64_t tweaks[2] = {
        UINT64_C(0x0706050403020100), UINT64_C(0x0F0E0D0C0B0A0908)};
}; // class Threefish512Constants

class Threefish1024Constants
    : public mckl::ThreefryConstants<std::uint64_t, 16>
{
    public:
    static constexpr std::uint64_t tweaks[2] = {
        UINT64_C(0x0706050403020100), UINT64_C(0x0F0E0D0C0B0A0908)};
}; // class Threefish1024Constants

using Threefish256 = mckl::ThreefryEngine<std::uint64_t, std::uint64_t, 4, 72,
    Threefish256Constants>;

using Threefish512 = mckl::ThreefryEngine<std::uint64_t, std::uint64_t, 8, 72,
    Threefish512Constants>;

using Threefish1024 = mckl::ThreefryEngine<std::uint64_t, std::uint64_t, 16,
    80, Threefish1024Constants>;

int main()
{
    Threefish256::ctr_type cipher_256 = {
        {UINT64_C(0xDF8FEA0EFF91D0E0), UINT64_C(0xD50AD82EE69281C9),
            UINT64_C(0x76F48D58085D869D), UINT64_C(0xDF975E95B5567065)}};

    Threefish512::ctr_type cipher_512 = {
        {UINT64_C(0x2C5AD426964304E3), UINT64_C(0x9A2436D6D8CA01B4),
            UINT64_C(0xDD456DB00E333863), UINT64_C(0x794725970EB9368B),
            UINT64_C(0x043546998D0A2A27), UINT64_C(0x25A7C918EA204478),
            UINT64_C(0x346201A1FEDF11AF), UINT64_C(0x3DAF1C5C3D672789)}};

    Threefish1024::ctr_type cipher_1024 = {
        {UINT64_C(0xB0C33CD7DB4D65A6), UINT64_C(0xBC49A85A1077D75D),
            UINT64_C(0x6855FCAFEA7293E4), UINT64_C(0x1C5385AB1B7754D2),
            UINT64_C(0x30E4AAFFE780F794), UINT64_C(0xE1BBEE708CAFD8D5),
            UINT64_C(0x9CA837B7423B0F76), UINT64_C(0xBD1403670D4963B3),
            UINT64_C(0x451F2E3CE61EA48A), UINT64_C(0xB360832F9277D4FB),
            UINT64_C(0x0AAFC7A65E12D688), UINT64_C(0xC8906E79016D05D7),
            UINT64_C(0xB316570A15F41333), UINT64_C(0x74E98A2869F5D50E),
            UINT64_C(0x57CE6F9247432BCE), UINT64_C(0xDE7CDD77215144DE)}};

    Threefish256::key_type key256 = {
        {UINT64_C(0x1716151413121110), UINT64_C(0x1F1E1D1C1B1A1918),
            UINT64_C(0x2726252423222120), UINT64_C(0x2F2E2D2C2B2A2928)}};

    Threefish512::key_type key512 = {
        {UINT64_C(0x1716151413121110), UINT64_C(0x1F1E1D1C1B1A1918),
            UINT64_C(0x2726252423222120), UINT64_C(0x2F2E2D2C2B2A2928),
            UINT64_C(0x3736353433323130), UINT64_C(0x3F3E3D3C3B3A3938),
            UINT64_C(0x4746454443424140), UINT64_C(0x4F4E4D4C4B4A4948)}};

    Threefish1024::key_type key1024 = {
        {UINT64_C(0x1716151413121110), UINT64_C(0x1F1E1D1C1B1A1918),
            UINT64_C(0x2726252423222120), UINT64_C(0x2F2E2D2C2B2A2928),
            UINT64_C(0x3736353433323130), UINT64_C(0x3F3E3D3C3B3A3938),
            UINT64_C(0x4746454443424140), UINT64_C(0x4F4E4D4C4B4A4948),
            UINT64_C(0x5756555453525150), UINT64_C(0x5F5E5D5C5B5A5958),
            UINT64_C(0x6766656463626160), UINT64_C(0x6F6E6D6C6B6A6968),
            UINT64_C(0x7776757473727170), UINT64_C(0x7F7E7D7C7B7A7978),
            UINT64_C(0x8786858483828180), UINT64_C(0x8F8E8D8C8B8A8988)}};

    Threefish256::ctr_type ctr256 = {
        {UINT64_C(0xF8F9FAFBFCFDFEFF), UINT64_C(0xF0F1F2F3F4F5F6F7),
            UINT64_C(0xE8E9EAEBECEDEEEF), UINT64_C(0xE0E1E2E3E4E5E6E7)}};

    Threefish512::ctr_type ctr512 = {
        {UINT64_C(0xF8F9FAFBFCFDFEFF), UINT64_C(0xF0F1F2F3F4F5F6F7),
            UINT64_C(0xE8E9EAEBECEDEEEF), UINT64_C(0xE0E1E2E3E4E5E6E7),
            UINT64_C(0xD8D9DADBDCDDDEDF), UINT64_C(0xD0D1D2D3D4D5D6D7),
            UINT64_C(0xC8C9CACBCCCDCECF), UINT64_C(0xC0C1C2C3C4C5C6C7)}};

    Threefish1024::ctr_type ctr1024 = {
        {UINT64_C(0xF8F9FAFBFCFDFEFF), UINT64_C(0xF0F1F2F3F4F5F6F7),
            UINT64_C(0xE8E9EAEBECEDEEEF), UINT64_C(0xE0E1E2E3E4E5E6E7),
            UINT64_C(0xD8D9DADBDCDDDEDF), UINT64_C(0xD0D1D2D3D4D5D6D7),
            UINT64_C(0xC8C9CACBCCCDCECF), UINT64_C(0xC0C1C2C3C4C5C6C7),
            UINT64_C(0xB8B9BABBBCBDBEBF), UINT64_C(0xB0B1B2B3B4B5B6B7),
            UINT64_C(0xA8A9AAABACADAEAF), UINT64_C(0xA0A1A2A3A4A5A6A7),
            UINT64_C(0x98999A9B9C9D9E9F), UINT64_C(0x9091929394959697),
            UINT64_C(0x88898A8B8C8D8E8F), UINT64_C(0x8081828384858687)}};

    Threefish256::ctr_type buf256;
    Threefish512::ctr_type buf512;
    Threefish1024::ctr_type buf1024;

    Threefish256 threefish256;
    Threefish512 threefish512;
    Threefish1024 threefish1024;

    threefish256.key(key256);
    threefish512.key(key512);
    threefish1024.key(key1024);

    threefish256.enc(ctr256, buf256);
    threefish512.enc(ctr512, buf512);
    threefish1024.enc(ctr1024, buf1024);

    bool pass256 = cipher_256 == buf256;
    bool pass512 = cipher_512 == buf512;
    bool pass1024 = cipher_1024 == buf1024;

    std::cout << "Threefish256:  " << (pass256 ? "Passed" : "Failed")
              << std::endl;
    std::cout << "Threefish512:  " << (pass512 ? "Passed" : "Failed")
              << std::endl;
    std::cout << "Threefish1024: " << (pass1024 ? "Passed" : "Failed")
              << std::endl;

    return 0;
}
