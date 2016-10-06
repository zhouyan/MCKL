//============================================================================
// MCKL/example/random/include/random_uniform_bits.hpp
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

#include <mckl/random/uniform_bits_distribution.hpp>
#include "random_common.hpp"

template <typename RNGType, typename UIntType>
inline bool random_uniform_bits_check(std::size_t N, std::size_t M)
{
    std::uniform_int_distribution<std::size_t> runif(N / 2, N);
    mckl::UniformBitsDistribution<UIntType> rubits;
    RNGType rng;
    bool pass = true;

    RNGType rng1;
    RNGType rng2;

    mckl::Vector<UIntType> r1(N);
    mckl::Vector<UIntType> r2(N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = runif(rng);
        r1.resize(K);
        r2.resize(K);
        for (std::size_t j = 0; j != K; ++j)
            r1[j] = rubits(rng1);
        mckl::rand(rng2, rubits, K, r2.data());
        pass = pass && (r1 == r2 || rng != rng);
    }

    return pass;
}

template <typename RNGType>
inline void random_uniform_bits(std::size_t N, std::size_t M, int nwid,
    int swid, int twid, const std::string &name)
{
    bool pass16 = random_uniform_bits_check<RNGType, std::uint16_t>(N, M);
    bool pass32 = random_uniform_bits_check<RNGType, std::uint32_t>(N, M);
    bool pass64 = random_uniform_bits_check<RNGType, std::uint64_t>(N, M);

    bool full = mckl::RNGTraits<RNGType>::is_full_range;
    int rbits = mckl::RNGTraits<RNGType>::bits;
    int tbits = std::numeric_limits<typename RNGType::result_type>::digits;

    std::cout << std::setw(nwid) << std::left << name;
    std::cout << std::setw(swid) << std::right << (full ? "Yes" : "No");
    std::cout << std::setw(swid) << std::right << rbits;
    std::cout << std::setw(swid) << std::right << tbits;
    std::cout << std::setw(twid) << std::right << random_pass(pass16);
    std::cout << std::setw(twid) << std::right << random_pass(pass32);
    std::cout << std::setw(twid) << std::right << random_pass(pass64);
    std::cout << std::endl;
}

inline void random_uniform_bits(std::size_t N, std::size_t M)
{
    const int nwid = 20;
    const int swid = 8;
    const int twid = 15;
    const std::size_t lwid = nwid + swid * 3 + twid * 3;

    std::cout << std::string(lwid, '=') << std::endl;
    std::cout << std::setw(nwid) << std::left << "RNGType";
    std::cout << std::setw(swid) << std::right << "Full";
    std::cout << std::setw(swid) << std::right << "R";
    std::cout << std::setw(swid) << std::right << "T";
    std::cout << std::setw(twid) << std::right << "uint16_t";
    std::cout << std::setw(twid) << std::right << "uint32_t";
    std::cout << std::setw(twid) << std::right << "uint64_t";
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#ifdef MCKL_RNG_DEFINE_MACRO_NA
#undef MCKL_RNG_DEFINE_MACRO_NA
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    random_uniform_bits<RNGType>(N, M, nwid, swid, twid, #name);

#include <mckl/random/internal/rng_define_macro_std.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

#include <mckl/random/internal/rng_define_macro_mkl.hpp>

    std::cout << std::string(lwid, '-') << std::endl;
}
