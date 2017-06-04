//============================================================================
// MCKL/include/mckl/random/poker_test.hpp
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

#ifndef MCKL_RANDOM_POKER_TEST_HPP
#define MCKL_RANDOM_POKER_TEST_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/chi_squared_test.hpp>
#include <bitset>

namespace mckl {

/// \brief Pocker test (simplified version)
/// \ingroup RandomTest
///
/// \tparam D Multiplier
/// \tparam T Length of the tuple
template <std::size_t D, std::size_t T>
class PokerTest : public ChiSquaredTest<PokerTest<D, T>>
{
    static_assert(D > 1, "**PockerTest** used with D less than two");

    static_assert(T > 0, "**PockerTest** used with T equal to zero");

  public:
    PokerTest(std::size_t n) : n_(n), tmin_(0), tmax_(0)
    {
        internal::StirlingMatrix2 stirling(T, K_);
        double d = D;
        double t = T;
        double mult = n * std::pow(d, -t);
        Vector<double> np_all(K_);
        for (std::size_t k = 1; k <= K_; ++k, d -= 1) {
            mult *= d;
            np_all[k - 1] = mult * stirling(T, k);
        }
        internal::group_np(static_cast<double>(n), np_all, np_, tmin_, tmax_);
    }

    MCKL_DEFINE_RANDOM_TEST_OPERATOR(double)

    template <typename RNGType, typename U01DistributionType>
    double operator()(RNGType &rng, U01DistributionType &u01)
    {
        using result_type = typename U01DistributionType::result_type;

        const std::size_t k = internal::BufferSize<result_type, T>::value;
        const std::size_t m = n_ / k;
        const std::size_t l = n_ % k;
        Vector<result_type> r(k * T);
        count_.resize(np_.size());
        std::fill(count_.begin(), count_.end(), 0);
        for (std::size_t i = 0; i != m; ++i) {
            generate(rng, u01, k, r.data());
        }
        generate(rng, u01, l, r.data());

        return this->stat(np_.size(), count_.data(), np_.data());
    }

    double degree_of_freedom() const
    {
        return static_cast<double>(np_.size() - 1);
    }

  private:
    static constexpr std::size_t K_ = D < T ? D : T;

    std::size_t n_;
    std::size_t tmin_;
    std::size_t tmax_;
    Vector<double> np_;
    Vector<double> count_;

    template <typename RNGType, typename U01DistributionType>
    void generate(RNGType &rng, U01DistributionType &u01, std::size_t n,
        typename U01DistributionType::result_type *r)
    {
        rand(rng, u01, n * T, r);
        mul(n * T, static_cast<typename U01DistributionType::result_type>(D),
            r, r);
        for (std::size_t i = 0; i != n; ++i, r += T) {
            std::size_t t = index(r);
            if (t <= tmin_) {
                count_.front() += 1;
            } else if (t >= tmax_) {
                count_.back() += 1;
            } else {
                count_[t - tmin_] += 1;
            }
        }
    }

    template <typename ResultType>
    std::size_t index(const ResultType *r) const
    {
        std::bitset<D> occurs;
        set<0>(r, occurs, std::integral_constant<bool, 0 < T>());

        return occurs.count() - 1;
    }

    template <std::size_t N, typename ResultType>
    void set(const ResultType *, std::bitset<D> &, std::false_type) const
    {
    }

    template <std::size_t N, typename ResultType>
    void set(const ResultType *r, std::bitset<D> &occurs, std::true_type) const
    {
        std::size_t u = internal::ftoi<std::size_t, D>(r[N]);
        occurs.set(u);
        set<N + 1>(r, occurs, std::integral_constant<bool, N + 1 < T>());
    }
}; // class PokerTest

} // namespace mckl

#endif // MCKL_RANDOM_POKER_TEST_HPP
