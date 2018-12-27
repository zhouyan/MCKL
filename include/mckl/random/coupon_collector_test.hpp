//============================================================================
// MCKL/include/mckl/random/coupon_collector_test.hpp
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

#ifndef MCKL_RANDOM_COUPON_COLLECTOR_TEST_HPP
#define MCKL_RANDOM_COUPON_COLLECTOR_TEST_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/chi_squared_test.hpp>
#include <bitset>

namespace mckl {

/// \brief Coupon collector's test
/// \ingroup RandomTest
///
/// \tparam D Multiplier
template <std::size_t D,
    std::size_t NTrialMax = std::numeric_limits<std::size_t>::max()>
class CouponCollectorTest
    : public ChiSquaredTest<CouponCollectorTest<D, NTrialMax>>
{
    static_assert(D > 1, "**CouponCollectorTest** used with D less than two");

  public:
    CouponCollectorTest(std::size_t n) : n_(n), tmin_(0), tmax_(0)
    {
        internal::StirlingMatrix2 stirling(T_ - 2, D - 1);
        double d = D;
        double lfact = std::lgamma(d + 1);
        double ld = std::log(d);
        Vector<double> np_all(T_ - D + 1);
        for (std::size_t k = D; k < T_; ++k) {
            np_all[k - D] =
                n * std::exp(lfact - k * ld) * stirling(k - 1, D - 1);
        }
        np_all[T_ - D] =
            n - std::accumulate(np_all.data(), np_all.data() + T_ - D, 0.0);
        internal::group_np(static_cast<double>(n), np_all, np_, tmin_, tmax_);
    }

    MCKL_DEFINE_RANDOM_TEST_OPERATOR(double)

    template <typename RNGType, typename U01DistributionType>
    double operator()(RNGType &rng, U01DistributionType &u01)
    {
        using result_type = typename U01DistributionType::result_type;

        const std::size_t k = internal::BufferSize<result_type>::value;
        Vector<result_type> r(k);
        count_.resize(np_.size());
        std::fill(count_.begin(), count_.end(), 0);

        std::size_t uidx = k;
        std::size_t s = 0;
        while (s < n_) {
            std::size_t t = 0;
            std::size_t v = 0;
            std::bitset<D> occurs;
            while (v < D) {
                if (t >= NTrialMax) {
                    return 0;
                }
                if (uidx == k) {
                    rand(rng, u01, k, r.data());
                    mul(k, static_cast<double>(D), r.data(), r.data());
                    uidx = 0;
                }
                ++t;
                std::size_t u = internal::ftoi<std::size_t, D>(r[uidx++]);
                if (!occurs.test(u)) {
                    occurs.set(u);
                    ++v;
                }
                if (v == D) {
                    t -= D;
                    if (t <= tmin_) {
                        count_.front() += 1;
                    } else if (t >= tmax_) {
                        count_.back() += 1;
                    } else {
                        count_[t - tmin_] += 1;
                    }
                    ++s;
                    break;
                }
            }
        }

        return this->stat(np_.size(), count_.data(), np_.data());
    }

    double degree_of_freedom() const
    {
        return static_cast<double>(np_.size() - 1);
    }

  private:
    static constexpr std::size_t T_ = D + 256;

    std::size_t n_;
    std::size_t tmin_;
    std::size_t tmax_;
    Vector<double> np_;
    Vector<double> count_;
}; // class CouponCollectorTest

} // namespace mckl

#endif // MCKL_RANDOM_COUPON_COLLECTOR_TEST_HPP
