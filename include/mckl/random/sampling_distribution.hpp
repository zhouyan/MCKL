//============================================================================
// MCKL/include/mckl/random/sampling_distribution.hpp
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

#ifndef MCKL_RANDOM_SAMPLING_DISTRIBUTION_HPP
#define MCKL_RANDOM_SAMPLING_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/sampling_distribution.hpp>
#include <mckl/random/u01_distribution.hpp>

namespace mckl
{

/// \brief Drawing a subset without replacement
/// \ingroup Distribution
template <typename IntType>
class SamplingDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_INT_TYPE(Sampling, 16)
    public:
    using result_type = IntType;
    using distribution_type = SamplingDistribution<IntType>;

    class param_type
    {
        public:
        using result_type = IntType;
        using distribution_type = SamplingDistribution<IntType>;

        explicit param_type(result_type n = 1, result_type k = 1)
            : n_(n), k_(k)
        {
        }

        result_type n() const { return n_; }
        result_type k() const { return k_; }

        friend bool operator==(
            const param_type &param1, const param_type &param2)
        {
            return param1.n_ == param2.n_ && param1.k_ == param2.k_;
        }

        friend bool operator!=(
            const param_type &param1, const param_type &param2)
        {
            return !(param1 == param2);
        }

        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits> &operator<<(
            std::basic_ostream<CharT, Traits> &os, const param_type &param)
        {
            if (!os)
                return os;

            os << param.n_ << ' ';
            os << param.k_;

            return os;
        }

        template <typename CharT, typename Traits>
        friend std::basic_istream<CharT, Traits> &operator>>(
            std::basic_istream<CharT, Traits> &is, param_type &param)
        {
            if (!is)
                return is;

            result_type n;
            result_type k;
            is >> std::ws >> n >> k;

            if (is) {
                if (n >= k && k > 0) {
                    param.n_ = n;
                    param.k_ = k;
                } else {
                    is.setstate(std::ios_base::failbit);
                }
            }

            return is;
        }

        private:
        result_type n_;
        result_type k_;

        friend distribution_type;
    }; // class param_type

    explicit SamplingDistribution(result_type n = 1, result_type k = 1)
        : param_(n, k)
    {
    }

    result_type n() const { return param_.n_; }
    result_type k() const { return param_.k_; }

    template <typename RNGType>
    void operator()(RNGType &rng, result_type *r) const
    {
        const result_type n = param_.n_;
        const result_type k = param_.k_;
        result_type t = 0;
        result_type m = 0;
        U01CODistribution<double> u01;

        while (true) {
            double u = u01(rng);
            if ((n - t) * u >= k - m) {
                ++t;
                continue;
            }
            *r++ = t;
            ++t;
            ++m;
            if (m >= k)
                return;
        }
    }

    friend bool operator==(
        const distribution_type &dist1, const distribution_type &dist2)
    {
        return dist1.param_ == dist2.param_;
    }

    friend bool operator!=(
        const distribution_type &dist1, const distribution_type &dist2)
    {
        return !(dist1 == dist2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os, const distribution_type &dist)
    {
        os << dist.param_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is, distribution_type &dist)
    {
        is >> std::ws >> dist.param_;
        if (is)
            dist.reset();

        return is;
    }

    private:
    param_type param_;
}; // class SamplingDistribution

} // namespace mckl

#endif // MCKL_RANDOM_SAMPLING_DISTRIBUTION_HPP
