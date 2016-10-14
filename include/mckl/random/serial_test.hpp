//============================================================================
// MCKL/include/mckl/random/serial_test.hpp
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

#ifndef MCKL_RANDOM_SERIAL_TEST_HPP
#define MCKL_RANDOM_SERIAL_TEST_HPP

#include <mckl/random/chi_squared_test.hpp>

namespace mckl
{

namespace internal
{

template <std::size_t, std::size_t, bool>
class SerialTestImpl;

template <std::size_t D, std::size_t T>
class SerialTestImpl<D, T, false>
    : public ChiSquaredTest<SerialTestImpl<D, T, false>>
{
    static_assert(D > 1, "**SerialTest** used with D less than 2");

    static_assert(T > 0, "**SerialTest** used with T equal to zero");

    public:
    SerialTestImpl(std::size_t n)
        : n_(n), np_(static_cast<double>(n) / M_), count_(M_)
    {
    }

    template <typename RNGType, typename U01DistributionType>
    double operator()(RNGType &rng, U01DistributionType &u01)
    {
        using result_type = typename U01DistributionType::result_type;

        std::fill(count_.begin(), count_.end(), 0);

        const std::size_t k = BufferSize<result_type, T>::value;
        const std::size_t m = n_ / k;
        const std::size_t l = n_ % k;
        Vector<result_type> r(k * T);
        for (std::size_t i = 0; i != m; ++i)
            generate(rng, u01, k, r.data());
        generate(rng, u01, l, r.data());

        return this->stat(M_, count_.data(), np_);
    }

    double degree_of_freedom() const { return static_cast<double>(M_ - 1); }

    private:
    static constexpr std::size_t M_ = Pow<std::size_t, D, T>::value;

    std::size_t n_;
    double np_;
    Vector<double> count_;

    template <typename RNGType, typename U01DistributionType>
    void generate(RNGType &rng, U01DistributionType &u01, std::size_t n,
        typename U01DistributionType::result_type *r)
    {
        rand(rng, u01, n * T, r);
        mul(n * T, static_cast<typename U01DistributionType::result_type>(D),
            r, r);
        for (std::size_t i = 0; i != n; ++i, r += T)
            count_[serial_index<D, T>(r)] += 1;
    }
}; // class SerialTestImpl

template <std::size_t D, std::size_t T>
class SerialTestImpl<D, T, true>
    : public ChiSquaredTest<SerialTestImpl<D, T, true>>
{
    static_assert(D > 1, "**SerialOverTest** used with D less than 2");

    static_assert(T > 1, "**SerialOverTest** used with T less than 2");

    public:
    SerialTestImpl(std::size_t n)
        : n_(n)
        , np1_(static_cast<double>(n) / M1_)
        , np2_(static_cast<double>(n) / M2_)
        , count1_(M1_)
        , count2_(M2_)
    {
        runtime_assert(
            n >= T, "**SerialTest** constructed with n less then T");
    }

    template <typename RNGType, typename U01DistributionType>
    double operator()(RNGType &rng, U01DistributionType &u01)
    {
        using result_type = typename U01DistributionType::result_type;

        std::fill(count1_.begin(), count1_.end(), 0);
        std::fill(count2_.begin(), count2_.end(), 0);

        const std::size_t k = BufferSize<result_type>::value;
        Vector<result_type> r(k);
        std::array<result_type, T> rhead;
        std::array<result_type, T> rtail;

        // generate the first tuple
        rand(rng, u01, k, r.data());
        mul(k, static_cast<result_type>(D), r.data(), r.data());
        std::memcpy(rhead.data(), r.data(), sizeof(result_type) * T);
        std::memcpy(rtail.data(), r.data(), sizeof(result_type) * T);

        // generate the first n - t + 1 counts
        cidx_ = serial_index<D, T>(rhead.data());
        head_ = 0;
        tail_ = T - 1;
        std::size_t uidx = T;
        for (std::size_t i = 0; i != n_ - T + 1; ++i) {
            if (uidx == k) {
                rand(rng, u01, k, r.data());
                mul(k, static_cast<result_type>(D), r.data(), r.data());
                uidx = 0;
            }
            result_type u = r[uidx++];
            generate(u, rtail.data());
        }

        // generate the last t - 1 counts
        uidx = 1;
        for (std::size_t i = n_ - T + 1; i != n_; ++i) {
            result_type u = rhead[uidx++];
            generate(u, rtail.data());
        }

        double s1 = this->stat(M1_, count1_.data(), np1_);
        double s2 = this->stat(M2_, count2_.data(), np2_);

        return s1 - s2;
    }

    double degree_of_freedom() const { return static_cast<double>(M1_ - M2_); }

    private:
    static constexpr std::size_t M1_ = Pow<std::size_t, D, T>::value;
    static constexpr std::size_t M2_ = Pow<std::size_t, D, T - 1>::value;

    std::size_t n_;
    double np1_;
    double np2_;
    std::size_t cidx_;
    std::size_t head_;
    std::size_t tail_;
    Vector<double> count1_;
    Vector<double> count2_;

    template <typename ResultType>
    void generate(ResultType u, ResultType *rtail)
    {
        cidx_ -= Pow<std::size_t, D, T - 1>::value *
            static_cast<std::size_t>(rtail[head_]);
        count2_[cidx_] += 1;
        head_ = (head_ + 1) % T;
        tail_ = (tail_ + 1) % T;
        rtail[tail_] = u;
        cidx_ = cidx_ * D + static_cast<std::size_t>(u);
        count1_[cidx_] += 1;
    }
}; // class SerialTestImpl

} // namespace mckl::internal

/// \brief Serial test
/// \ingroup RandomTest
///
/// \tparam D Multiplier
/// \tparam T Length of the tuple
/// \tparam Overlap Using overlapping tuples
template <std::size_t D, std::size_t T, bool Overlap>
using SerialTest = internal::SerialTestImpl<D, T, Overlap && 1 < T>;

} // namespace mckl

#endif // MCKL_RANDOM_SERIAL_TEST_HPP
