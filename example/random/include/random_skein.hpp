//============================================================================
// MCKL/example/random/include/random_skein.hpp
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
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#ifndef MCKL_EXAMPLE_RANDOM_SKEIN_HPP
#define MCKL_EXAMPLE_RANDOM_SKEIN_HPP

#include <mckl/random/skein.hpp>

inline void random_skein_output(std::size_t n, const std::uint8_t *buf)
{
    n /= 8;
    std::cout << std::hex << std::uppercase;
    for (std::size_t i = 0; i != n; ++i, ++buf) {
        unsigned m = *buf;
        if (m <= 0xFU)
            std::cout << '0';
        std::cout << m;
        if ((i + 1) % 16 == 0 || i == n - 1)
            std::cout << '\n';
        else if ((i + 1) % 4 == 0)
            std::cout << "  ";
        else
            std::cout << ' ';
    }
}

#include "random_skein_inc.hpp"
#include "random_skein_rand.hpp"
#include "random_skein_zero.hpp"

inline void random_skein()
{
    bool pass = true;

    pass = pass && random_skein_inc();
    pass = pass && random_skein_rand();
    pass = pass && random_skein_zero();

    if (pass)
        std::cout << "All tests passed" << std::endl;
}

#endif // MCKL_EXAMPLE_RANDOM_SKEIN_HPP
