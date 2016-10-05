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
inline UIntType random_uniform_bits_rotate(UIntType u, std::false_type)
{
    return u;
}

template <typename RNGType, typename UIntType>
inline UIntType random_uniform_bits_rotate(UIntType u, std::true_type)
{
    static constexpr int w = std::numeric_limits<UIntType>::digits;
    static constexpr int r = mckl::RNGTraits<RNGType>::bits;
    static constexpr int n = w / r;
    static constexpr int m = w % r;
    static constexpr UIntType mask = std::numeric_limits<UIntType>::max() >>
        (std::numeric_limits<UIntType>::digits - r);

    UIntType v = 0;
    for (int i = 0; i != n; ++i) {
        v = (v << r) + (u & mask);
        u >>= r;
    }
    v = (v << m) + u;

    return v;
}

template <typename RNGType, typename UIntType>
inline UIntType random_uniform_bits_rotate(UIntType u)
{
    static constexpr int w = std::numeric_limits<UIntType>::digits;
    static constexpr int r = mckl::RNGTraits<RNGType>::bits;
    static constexpr bool patch =
        mckl::RNGTraits<RNGType>::is_full_range && w > r && w % r == 0;

    return random_uniform_bits_rotate<RNGType>(
        u, std::integral_constant<bool, patch>());
}

template <typename RNGType, typename UIntType>
inline void random_uniform_bits(std::size_t N, std::size_t M, int nwid,
    int swid, int twid, const std::string &name)
{
    std::uniform_int_distribution<std::size_t> runif(N / 2, N);
    mckl::UniformBitsDistribution<UIntType> rubits;
    RNGType rng;
    bool pass = true;
    bool iseq = true;
    bool isro = false;

    RNGType rng1;
    RNGType rng2;
    RNGType rng3;

    mckl::Vector<UIntType> r1(N);
    mckl::Vector<UIntType> r2(N);
    mckl::Vector<UIntType> r3(N);

    double c1 = std::numeric_limits<double>::max();
    double c2 = std::numeric_limits<double>::max();
    double c3 = std::numeric_limits<double>::max();
    for (std::size_t k = 0; k != 10; ++k) {
        std::size_t n = 0;
        mckl::StopWatch watch1;
        mckl::StopWatch watch2;
        mckl::StopWatch watch3;
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = runif(rng);
            n += K;
            r1.resize(K);
            r2.resize(K);
            r3.resize(K);

            std::independent_bits_engine<RNGType,
                std::numeric_limits<UIntType>::digits, UIntType>
                eng(std::move(rng1));
            watch1.start();
            for (std::size_t j = 0; j != K; ++j)
                r1[j] = eng();
            watch1.stop();
            rng1 = std::move(eng.base());

            watch2.start();
            for (std::size_t j = 0; j != K; ++j)
                r2[j] = rubits(rng2);
            watch2.stop();

            watch3.start();
            mckl::rand(rng3, rubits, K, r3.data());
            watch3.stop();

            pass = pass && (r2 == r3 || rng != rng);
            if (rng == rng) {
                if (r1 == r2) {
                    iseq = iseq && true;
                } else {
                    for (std::size_t j = 0; j != K; ++j)
                        r1[j] = random_uniform_bits_rotate<RNGType>(r1[j]);
                    if (r1 == r2) {
                        iseq = iseq && true;
                        isro = true;
                    } else {
                        iseq = iseq && false;
                    }
                }
            }
        }
        std::size_t bytes = sizeof(UIntType) * n;
        c1 = std::min(c1, 1.0 * watch1.cycles() / bytes);
        c2 = std::min(c2, 1.0 * watch2.cycles() / bytes);
        c3 = std::min(c3, 1.0 * watch3.cycles() / bytes);
    }

    bool full = mckl::RNGTraits<RNGType>::is_full_range;
    int rbits = mckl::RNGTraits<RNGType>::bits;
    int tbits = std::numeric_limits<typename RNGType::result_type>::digits;
    int ubits = std::numeric_limits<UIntType>::digits;

    std::cout << std::setw(nwid) << std::left << name;
    std::cout << std::setw(swid) << std::right << (full ? "Yes" : "No");
    std::cout << std::setw(swid) << std::right << rbits;
    std::cout << std::setw(swid) << std::right << tbits;
    std::cout << std::setw(swid) << std::right << ubits;
    std::cout << std::setw(swid) << std::right << c1;
    std::cout << std::setw(swid) << std::right << c2;
    std::cout << std::setw(swid) << std::right << c3;
    std::cout << std::setw(twid) << std::right << (isro ? "Yes" : "No");
    std::cout << std::setw(twid) << std::right << random_pass(iseq);
    std::cout << std::setw(twid) << std::right << random_pass(pass);
    std::cout << std::endl;
}

template <typename RNGType>
inline void random_uniform_bits(
    std::size_t N, std::size_t M, const std::string &name)
{
    const int nwid = 20;
    const int swid = 8;
    const int twid = 15;
    const std::size_t lwid = nwid + swid * 7 + twid * 3;

    std::cout << std::string(lwid, '=') << std::endl;
    std::cout << std::setw(nwid) << std::left << "RNGType";
    std::cout << std::setw(swid) << std::right << "Full";
    std::cout << std::setw(swid) << std::right << "R";
    std::cout << std::setw(swid) << std::right << "T";
    std::cout << std::setw(swid) << std::right << "U";
    std::cout << std::setw(swid) << std::right << "I";
    std::cout << std::setw(swid) << std::right << "S";
    std::cout << std::setw(swid) << std::right << "B";
    std::cout << std::setw(twid) << std::right << "Rotate";
    std::cout << std::setw(twid) << std::right << "Equivalency";
    std::cout << std::setw(twid) << std::right << "Deterministics";
    std::cout << std::endl;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::string(lwid, '-') << std::endl;
    random_uniform_bits<RNGType, std::uint16_t>(N, M, nwid, swid, twid, name);
    random_uniform_bits<RNGType, std::uint32_t>(N, M, nwid, swid, twid, name);
    random_uniform_bits<RNGType, std::uint64_t>(N, M, nwid, swid, twid, name);
    std::cout << std::string(lwid, '-') << std::endl;
}
