//============================================================================
// MCKL/example/random/src/random_test.cpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2017, Yan Zhou
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

#include "random_test.hpp"

inline std::string random_test_pass(double pass)
{
    std::stringstream ss;
    if (pass < 50)
        ss << '*';
    ss << pass << '%';

    return ss.str();
}

template <typename TestType, typename U01Type>
inline void random_test(std::size_t M, int nwid, int swid, int twid,
    const TestType &test, const U01Type &u01, const std::string &name)
{
    std::array<double, 4> pass;
    pass.fill(0);
    mckl::StopWatch watch;
    watch.start();
    TestType t(test);
    for (std::size_t i = 0; i != M; ++i) {
        typename TestType::result_type s = t(u01);
        std::get<0>(pass) += t.pass(1e-4, s) ? 1 : 0;
        std::get<1>(pass) += t.pass(1e-3, s) ? 1 : 0;
        std::get<2>(pass) += t.pass(1e-2, s) ? 1 : 0;
        std::get<3>(pass) += t.pass(1e-1, s) ? 1 : 0;
    }
    watch.stop();
    mckl::mul(4, 100.0 / M, pass.data(), pass.data());
    std::cout << std::setw(nwid) << std::left << name;
    std::cout << std::setw(swid) << std::right
              << random_test_pass(std::get<0>(pass));
    std::cout << std::setw(swid) << std::right
              << random_test_pass(std::get<1>(pass));
    std::cout << std::setw(swid) << std::right
              << random_test_pass(std::get<2>(pass));
    std::cout << std::setw(swid) << std::right
              << random_test_pass(std::get<3>(pass));
    std::cout << std::setw(twid) << std::right << std::fixed
              << std::setprecision(2) << watch.seconds();
    std::cout << std::endl;
}

template <typename TestType>
inline void random_test(std::size_t M, int nwid, int swid, int twid,
    std::size_t lwid, const TestType &test, const std::string &name)
{
    std::cout << std::string(lwid, '=') << std::endl;
    std::cout << name << std::endl;
    std::cout << std::string(lwid, '-') << std::endl;
    std::cout << std::setw(nwid) << std::left << "Level";
    std::cout << std::setw(swid) << std::right << "0.01%";
    std::cout << std::setw(swid) << std::right << "0.1%";
    std::cout << std::setw(swid) << std::right << "1%";
    std::cout << std::setw(swid) << std::right << "10%";
    std::cout << std::setw(twid) << std::right << "Time (s)";
    std::cout << std::endl;
    std::cout << std::string(lwid, '-') << std::endl;
    random_test(M, nwid, swid, twid, test, random_rng_std, "STD");
    random_test(M, nwid, swid, twid, test, random_rng_u01, "U01");
    random_test(M, nwid, swid, twid, test, random_rng_u01cc, "U01CC");
    random_test(M, nwid, swid, twid, test, random_rng_u01co, "U01CO");
    random_test(M, nwid, swid, twid, test, random_rng_u01oc, "U01OC");
    random_test(M, nwid, swid, twid, test, random_rng_u01oo, "U01OO");
    std::cout << std::string(lwid, '-') << std::endl;
}

void random_test(std::size_t N, std::size_t M, const std::string &name)
{
    const int nwid = 10;
    const int swid = 10;
    const int twid = 15;
    std::size_t lwid = nwid + swid * 4 + twid;

    std::cout << std::string(lwid, '=') << std::endl;
    std::cout << name << std::endl;
    std::cout << std::string(lwid, '-') << std::endl;

    std::size_t b = 1;
    std::size_t p = 0;
    while (N % b == 0) {
        b *= 10;
        p += 1;
    }
    b /= 10;
    p -= 1;

    std::string nstr = p == 0 ?
        std::to_string(N) :
        (N == b ? std::string() : std::to_string(N / b) + " * ") + "10^" +
            std::to_string(p);

    random_test(M, nwid, swid, twid, lwid,
        mckl::BirthdaySpacingsTest<2, 40>(N),
        "BirthdaySpacings (n = " + nstr + ", d = 2, t = 40)");
    random_test(M, nwid, swid, twid, lwid,
        mckl::BirthdaySpacingsTest<2, 42>(N),
        "BirthdaySpacings (n = " + nstr + ", d = 2, t = 42)");
    random_test(M, nwid, swid, twid, lwid,
        mckl::BirthdaySpacingsTest<2, 44>(N),
        "BirthdaySpacings (n = " + nstr + ", d = 2, t = 44)");
    random_test(M, nwid, swid, twid, lwid,
        mckl::BirthdaySpacingsTest<2, 46>(N),
        "BirthdaySpacings (n = " + nstr + ", d = 2, t = 46)");

    random_test(M, nwid, swid, twid, lwid, mckl::CollisionTest<2, 20>(N),
        "Collision (n = " + nstr + ", d = 2, t = 20)");
    random_test(M, nwid, swid, twid, lwid, mckl::CollisionTest<2, 30>(N),
        "Collision (n = " + nstr + ", d = 2, t = 30)");
    random_test(M, nwid, swid, twid, lwid, mckl::CollisionTest<2, 40>(N),
        "Collision (n = " + nstr + ", d = 2, t = 40)");
    random_test(M, nwid, swid, twid, lwid, mckl::CollisionTest<2, 50>(N),
        "Collision (n = " + nstr + ", d = 2, t = 50)");

    random_test(M, nwid, swid, twid, lwid, mckl::CouponCollectorTest<2>(N),
        "CouponCollector (n = " + nstr + ", d = 2^1)");
    random_test(M, nwid, swid, twid, lwid, mckl::CouponCollectorTest<4>(N),
        "CouponCollector (n = " + nstr + ", d = 2^2)");
    random_test(M, nwid, swid, twid, lwid, mckl::CouponCollectorTest<8>(N),
        "CouponCollector (n = " + nstr + ", d = 2^3)");
    random_test(M, nwid, swid, twid, lwid, mckl::CouponCollectorTest<16>(N),
        "CouponCollector (n = " + nstr + ", d = 2^4)");

    random_test(M, nwid, swid, twid, lwid, mckl::GapTest<>(N, 0, 0.5),
        "Gap (n = " + nstr + ", alpha = 0, beta = 2^-1)");
    random_test(M, nwid, swid, twid, lwid, mckl::GapTest<>(N, 0, 0.25),
        "Gap (n = " + nstr + ", alpha = 0, beta = 2^-2)");
    random_test(M, nwid, swid, twid, lwid, mckl::GapTest<>(N, 0, 0.125),
        "Gap (n = " + nstr + ", alpha = 0, beta = 2^-3)");
    random_test(M, nwid, swid, twid, lwid, mckl::GapTest<>(N, 0, 0.0625),
        "Gap (n = " + nstr + ", alpha = 0, beta = 2^-4)");

    random_test(M, nwid, swid, twid, lwid, mckl::MaximumOfTTest<1024, 8>(N),
        "MaximumOfT (n = " + nstr + ", d = 2^10, t = 3)");
    random_test(M, nwid, swid, twid, lwid, mckl::MaximumOfTTest<2048, 16>(N),
        "MaximumOfT (n = " + nstr + ", d = 2^11, t = 4)");
    random_test(M, nwid, swid, twid, lwid, mckl::MaximumOfTTest<4096, 32>(N),
        "MaximumOfT (n = " + nstr + ", d = 2^12, t = 5)");
    random_test(M, nwid, swid, twid, lwid, mckl::MaximumOfTTest<8192, 64>(N),
        "MaximumOfT (n = " + nstr + ", d = 2^13, t = 6)");

    random_test(M, nwid, swid, twid, lwid, mckl::PermutationTest<3>(N),
        "Permutation (n = " + nstr + ", t = 3)");
    random_test(M, nwid, swid, twid, lwid, mckl::PermutationTest<5>(N),
        "Permutation (n = " + nstr + ", t = 5)");
    random_test(M, nwid, swid, twid, lwid, mckl::PermutationTest<7>(N),
        "Permutation (n = " + nstr + ", t = 7)");
    random_test(M, nwid, swid, twid, lwid, mckl::PermutationTest<11>(N),
        "Permutation (n = " + nstr + ", t = 11)");

    random_test(M, nwid, swid, twid, lwid, mckl::PokerTest<16, 16>(N),
        "Poker (n = " + nstr + ", d = 2^3, t = 2^3)");
    random_test(M, nwid, swid, twid, lwid, mckl::PokerTest<8, 16>(N),
        "Poker (n = " + nstr + ", d = 2^3, t = 2^4)");
    random_test(M, nwid, swid, twid, lwid, mckl::PokerTest<16, 8>(N),
        "Poker (n = " + nstr + ", d = 2^4, t = 2^3)");
    random_test(M, nwid, swid, twid, lwid, mckl::PokerTest<16, 16>(N),
        "Poker (n = " + nstr + ", d = 2^4, t = 2^4)");

    random_test(M, nwid, swid, twid, lwid, mckl::RunTest<false, false>(N),
        "Run (n = " + nstr + ", Independent = false, Up = false)");
    random_test(M, nwid, swid, twid, lwid, mckl::RunTest<false, true>(N),
        "Run (n = " + nstr + ", Independent = false, Up = true)");
    random_test(M, nwid, swid, twid, lwid, mckl::RunTest<true, false>(N),
        "Run (n = " + nstr + ", Independent = true, Up = false)");
    random_test(M, nwid, swid, twid, lwid, mckl::RunTest<true, true>(N),
        "Run (n = " + nstr + ", Independent = true, Up = true)");

    random_test(M, nwid, swid, twid, lwid, mckl::SerialTest<64, 2, false>(N),
        "Serial (n = " + nstr + ", d = 2^6, t = 2, overlap = false)");
    random_test(M, nwid, swid, twid, lwid, mckl::SerialTest<16, 3, false>(N),
        "Serial (n = " + nstr + ", d = 2^4, t = 3, overlap = false)");
    random_test(M, nwid, swid, twid, lwid, mckl::SerialTest<64, 2, true>(N),
        "Serial (n = " + nstr + ", d = 2^6, t = 2, overlap = true)");
    random_test(M, nwid, swid, twid, lwid, mckl::SerialTest<16, 3, true>(N),
        "Serial (n = " + nstr + ", d = 2^4, t = 3, overlap = true)");
}
