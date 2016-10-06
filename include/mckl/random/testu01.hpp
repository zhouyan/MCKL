//============================================================================
// MCKL/include/mckl/random/testu01.hpp
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

#ifndef MCKL_RANDOM_TESTU01_HPP
#define MCKL_RANDOM_TESTU01_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/rng_set.hpp>

extern "C" {

#include <TestU01.h>

} // extern "C"

namespace mckl
{

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

    bool empty() const { return gen_ == nullptr; }

    template <typename RNGType, typename U01Type>
    void reset(const std::string &name)
    {
        release();
        gen_ = ::unif01_CreateExternGen01(
            const_cast<char *>(name.c_str()), u01<RNGType, U01Type>);
    }

    void release()
    {
        if (gen_ != nullptr)
            ::unif01_DeleteExternGen01(gen_);
        gen_ = nullptr;
    }

    ::unif01_Gen *gen() const { return gen_; }

    template <typename Battery>
    void operator()(Battery &&battery)
    {
        battery(gen_);
    }

    template <typename BatteryRepeat, typename InputIter>
    void operator()(BatteryRepeat &&battery_repeat, InputIter first,
        InputIter last, int n = 10)
    {
        Vector<int> rep(repeat(first, last, n));
        battery_repeat(gen_, rep.data());
    }

    private:
    TestU01() : gen_(nullptr) { reset<RNG, U01Distribution<double>>("U01"); }

    ::unif01_Gen *gen_;

    template <typename RNGType, typename U01Type>
    static double u01()
    {
        static const std::size_t N = internal::BufferSize<double>::value;
        static const std::size_t M = std::thread::hardware_concurrency();

        static std::size_t index = N * M;
        static RNGSetVector<RNGType> rs(M);
        static Vector<double> result(N * M);

        if (index == N * M) {
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
