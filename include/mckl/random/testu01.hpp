//============================================================================
// MCKL/include/mckl/random/testu01.hpp
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

#ifndef MCKL_RANDOM_TESTU01_HPP
#define MCKL_RANDOM_TESTU01_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/seed.hpp>

extern "C" {

#include <TestU01.h>

} // extern "C"

namespace mckl {

/// \brief Perform TestU01 tests on different RNG and U01 types
/// \ingroup RandomTest
class TestU01
{
  public:
    TestU01(const TestU01 &) = delete;
    TestU01 &operator=(const TestU01 &) = delete;

    static TestU01 &instance()
    {
        static TestU01 testu01;

        return testu01;
    }

    /// \brief Return true if the TestU01 generator is not set or released
    bool empty() const { return gen_ == nullptr; }

    /// \brief Reset the TestU01 generator to specified RNG and distribution
    template <typename RNGType, typename U01Type>
    void reset(const std::string &name, bool parallel = false)
    {
        parallel ? reset(name, u01_mt<RNGType, U01Type, 1024, 8>) :
                   reset(name, u01_st<RNGType, U01Type, 4096>);
    }

    /// \brief Reset the TestU01 generator to specified RNG and distribution
    template <typename RNGType, typename U01Type, std::size_t N, std::size_t M>
    void reset(const std::string &name, bool parallel = false)
    {
        parallel ? reset(name, u01_mt<RNGType, U01Type, N, M>) :
                   reset(name, u01_st<RNGType, U01Type, N>);
    }

    /// \brief Reset the TestU01 generator to specified distribution
    void reset(const std::string &name, double (*u01)())
    {
        release();
        gen_ =
            ::unif01_CreateExternGen01(const_cast<char *>(name.c_str()), u01);
    }

    /// \brief Release the TestU01 generator
    void release()
    {
        if (gen_ != nullptr)
            ::unif01_DeleteExternGen01(gen_);
        gen_ = nullptr;
    }

    /// \brief Get the TestU01 generator
    ::unif01_Gen *gen() const { return gen_; }

    /// \brief Apply a battery that accepts a `unif01_Gen` pointer as input
    template <typename Battery>
    void operator()(Battery &&battery)
    {
        battery(gen_);
    }

    /// \brief Apply a battery that accepts a `unif01_Gen` pointer as the first
    /// input, and an array of type `int` that specifies that replication
    /// number
    ///
    /// \details
    /// Each element in the array is set to `n`. This currently only work with
    /// `bbattery_RepeatSmallCrush` due to TestU01's internal limits of at most
    /// 200 p-values can be returned by a single battery.
    template <typename BatteryRepeat>
    void operator()(BatteryRepeat &&battery_repeat, int n)
    {
        Vector<int> rep(repeat(n));
        battery_repeat(gen_, rep.data());
    }

    /// \brief Apply a battery that accepts a `unif01_Gen` pointer as the first
    /// input, and an array of type `int` that specifies that replication
    /// number
    ///
    /// \details
    /// The range [`first`, `last`) contains enumeration numbers of tests to be
    /// repeated. Each will be repeated `n` times.
    template <typename BatteryRepeat, typename InputIter>
    void operator()(
        BatteryRepeat &&battery_repeat, InputIter first, InputIter last, int n)
    {
        Vector<int> rep(repeat(first, last, n));
        battery_repeat(gen_, rep.data());
    }

  private:
    TestU01() : gen_(nullptr) {}

    ::unif01_Gen *gen_;

    template <typename RNGType, typename U01Type, std::size_t N, std::size_t M>
    static double u01_mt()
    {
        MCKL_PUSH_CLANG_WARNING("-Wexit-time-destructors")
        static std::size_t index = N * M;
        static Vector<RNGType> rs(M);
        static Vector<double> result(N * M);
        static bool init = false;
        MCKL_POP_CLANG_WARNING

        if (index == N * M) {
            if (!init) {
                for (auto &rng : rs)
                    rng.seed(Seed<RNGType>::instance().get());
                init = true;
            }
            double *r = result.data();
            for (std::size_t i = 0; i != M; ++i, r += N) {
                RNGType rng(std::move(rs[i]));
                U01Type dist;
                mckl::rand(rng, dist, N, r);
                rs[i] = std::move(rng);
            }
            index = 0;
        }

        return result[index++];
    }

    template <typename RNGType, typename U01Type, std::size_t N>
    static double u01_st()
    {
        MCKL_PUSH_CLANG_WARNING("-Wexit-time-destructors")
        static std::size_t index = N;
        static RNGType rng(mckl::Seed<RNGType>::instance().get());
        static mckl::Vector<double> result(N);
        MCKL_POP_CLANG_WARNING

        if (index == N) {
            U01Type dist;
            mckl::rand(rng, dist, N, result.data());
            index = 0;
        }

        return result[index++];
    }

    static Vector<int> repeat(int n) { return Vector<int>(128, n); }

    template <typename InputIter>
    static Vector<int> repeat(InputIter first, InputIter last, int n)
    {
        Vector<int> rep(128, 0);
        for (InputIter iter = first; iter != last; ++iter) {
            if (*iter < 128)
                rep[static_cast<std::size_t>(*iter)] += n;
        }

        return rep;
    }
}; // class TestU01

} // namespace mckl

#endif // MCKL_RANDOM_TESTU01_HPP
