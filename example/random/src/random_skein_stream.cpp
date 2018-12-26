//============================================================================
// MCKL/example/random/src/random_skein_stream.cpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
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
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#include <mckl/random/skein.hpp>
#include <iomanip>
#include <iostream>

int main()
{
    ::mckl::Threefish256 rng;
    std::size_t n1 = 1 << 4;
    std::size_t n2 = 1 << 4;
    ::mckl::Vector<::mckl::Threefish256::result_type> buffer(n1 + n2);
    ::mckl::rand(rng, buffer.size(), buffer.data());

    ::mckl::Skein256::stream_hasher hasher(256);
    auto state1 = hasher.init();
    auto state2 = hasher.init();
    std::array<std::uint64_t, 4> ret1;
    std::array<std::uint64_t, 4> ret2;

    hasher.update(state1,
        ::mckl::Skein256::stream_hasher::param_type(
            n1 * CHAR_BIT * sizeof(*buffer.data()), buffer.data()),
        false);
    hasher.update(state1,
        ::mckl::Skein256::stream_hasher::param_type(
            n2 * CHAR_BIT * sizeof(*buffer.data()), buffer.data() + n1),
        true);
    hasher.output(state1, ret1.data());

    hasher.update(state2,
        ::mckl::Skein256::stream_hasher::param_type(
            (n1 + n2) * CHAR_BIT * sizeof(*buffer.data()), buffer.data()),
        true);
    hasher.output(state2, ret2.data());

    std::cout << (ret1 == ret2 ? "Passed" : "Failed") << std::endl;
}
