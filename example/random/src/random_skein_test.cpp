//============================================================================
// MCKL/example/random/src/random_skein_test.cpp
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

#include <mckl/random/skein.hpp>
#include "random_skein.hpp"

inline void random_skein_print(std::size_t n, const std::uint8_t *buf)
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

template <typename Hash>
inline bool random_skein_check(std::size_t Nm, std::size_t Nh,
    const std::uint8_t *expected, const std::uint8_t *result)
{
    std::size_t n = Nh / CHAR_BIT + (Nh % CHAR_BIT == 0 ? 0 : 1);
    if (std::memcmp(expected, result, n) == 0)
        return true;

    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Hash:    Skein-" << Hash::bits() << std::endl;
    std::cout << "Data:    Incrementing" << std::endl;
    std::cout << "Message: " << std::dec << Nm << " bits" << std::endl;
    std::cout << "Output:  " << std::dec << Nh << " bits" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Expected" << std::endl;
    random_skein_print(Nh, expected);
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Result" << std::endl;
    random_skein_print(Nh, result);
    std::cout << std::string(50, '-') << std::endl;

    return false;
}

template <typename Hash>
inline bool random_skein(std::size_t Nm, std::size_t Nh,
    const std::uint8_t *expected, const std::uint8_t *message)
{
    std::uint8_t result[1024] = {0};
    typename Hash::param_type M(Nm, message);
    Hash::hash(M, Nh, result);

    return random_skein_check<Hash>(Nm, Nh, expected, result);
}

bool random_skein256(std::size_t Nm, std::size_t Nh,
    const std::uint8_t *expected, const std::uint8_t *message)
{
    return random_skein<mckl::Skein256>(Nm, Nh, expected, message);
}

bool random_skein512(std::size_t Nm, std::size_t Nh,
    const std::uint8_t *expected, const std::uint8_t *message)
{
    return random_skein<mckl::Skein512>(Nm, Nh, expected, message);
}

bool random_skein1024(std::size_t Nm, std::size_t Nh,
    const std::uint8_t *expected, const std::uint8_t *message)
{
    return random_skein<mckl::Skein1024>(Nm, Nh, expected, message);
}
