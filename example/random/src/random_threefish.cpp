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
int main()
{

    mckl::Threefish256::ctr_type cipher_256 = {
        {UINT64_C(0x94EEEA8B1F2ADA84), UINT64_C(0xADF103313EAE6670),
            UINT64_C(0x952419A1F4B16D53), UINT64_C(0xD83F13E63C9F6B11)}};

    mckl::Threefish512::ctr_type cipher_512 = {
        {UINT64_C(0xBC2560EFC6BBA2B1), UINT64_C(0xE3361F162238EB40),
            UINT64_C(0xFB8631EE0ABBD175), UINT64_C(0x7B9479D4C5479ED1),
            UINT64_C(0xCFF0356E58F8C27B), UINT64_C(0xB1B7B08430F0E7F7),
            UINT64_C(0xE9A380A56139ABF1), UINT64_C(0xBE7B6D4AA11EB47E)}};

    mckl::Threefish1024::ctr_type cipher_1024 = {
        {UINT64_C(0x04B3053D0A3D5CF0), UINT64_C(0x0136E0D1C7DD85F7),
            UINT64_C(0x067B212F6EA78A5C), UINT64_C(0x0DA9C10B4C54E1C6),
            UINT64_C(0x0F4EC27394CBACF0), UINT64_C(0x32437F0568EA4FD5),
            UINT64_C(0xCFF56D1D7654B49C), UINT64_C(0xA2D5FB14369B2E7B),
            UINT64_C(0x540306B460472E0B), UINT64_C(0x71C18254BCEA820D),
            UINT64_C(0xC36B4068BEAF32C8), UINT64_C(0xFA4329597A360095),
            UINT64_C(0xC4A36C28434A5B9A), UINT64_C(0xD54331444B1046CF),
            UINT64_C(0xDF11834830B2A460), UINT64_C(0x1E39E8DFE1F7EE4F)}};

    mckl::Threefish256::generator_type threefish256;
    mckl::Threefish512::generator_type threefish512;
    mckl::Threefish1024::generator_type threefish1024;

    mckl::Threefish256::key_type key256;
    mckl::Threefish512::key_type key512;
    mckl::Threefish1024::key_type key1024;
    std::fill(key256.begin(), key256.end(), 0);
    std::fill(key512.begin(), key512.end(), 0);
    std::fill(key1024.begin(), key1024.end(), 0);

    mckl::Threefish256::ctr_type ctr256;
    mckl::Threefish512::ctr_type ctr512;
    mckl::Threefish1024::ctr_type ctr1024;
    std::fill(ctr256.begin(), ctr256.end(), 0);
    std::fill(ctr512.begin(), ctr512.end(), 0);
    std::fill(ctr1024.begin(), ctr1024.end(), 0);

    mckl::Threefish256::ctr_type buf256;
    mckl::Threefish512::ctr_type buf512;
    mckl::Threefish1024::ctr_type buf1024;

    threefish256.reset(key256);
    threefish512.reset(key512);
    threefish1024.reset(key1024);

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
