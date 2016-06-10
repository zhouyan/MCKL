//============================================================================
// MCKL/include/mckl/random/collision_test.hpp
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

#ifndef MCKL_RANDOM_COLLISION_TEST_HPP
#define MCKL_RANDOM_COLLISION_TEST_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/poisson_test.hpp>

namespace mckl
{

/// \brief Collision test
/// \ingroup RandomTest
///
/// \tparam D Multiplier
/// \tparam T Length of the tuple
///
/// \details
/// The implementation use the Poisson distribution approximation instead of
/// the exact computation. This makes it faster and more easier to conduct some
/// second-level tests. The approximation is only appropriate if \f$n\f$ is
/// large and the number of categories \f$D^T\f$ is even larger.
template <std::size_t D, std::size_t T>
class CollisionTest : public PoissonTest<CollisionTest<D, T>>
{
    static_assert(D > 1, "**CollisionTest** used with D less than two");

    static_assert(T > 0, "**CollisionTest** used with T equal to zero");

    public:
    CollisionTest(std::size_t n) : n_(n), mean_(0)
    {
        double p = static_cast<double>(n);
        mean_ = K_ * (p / K_ - 1 + std::pow(1 - 1.0 / K_, p));
    }

    template <typename RNGType, typename U01Type>
    std::size_t operator()(RNGType &rng, U01Type &u01)
    {
        using result_type = typename U01Type::result_type;

        const std::size_t k = internal::BufferSize<result_type, T>::value;
        const std::size_t m = n_ / k;
        const std::size_t l = n_ % k;
        Vector<result_type> r(k * T);
        std::size_t s = 0;
        occurs_type occurs;
        reserve(occurs);
        for (std::size_t i = 0; i != m; ++i)
            generate(rng, u01, k, r.data(), s, occurs);
        generate(rng, u01, l, r.data(), s, occurs);

        return s;
    }

    double mean() const { return mean_; }

    private:
    static constexpr std::size_t K_ = internal::Pow<std::size_t, D, T>::value;

    using occurs_type = typename std::conditional<K_ <= 1U << 27,
        std::bitset<K_>, std::unordered_set<std::size_t>>::type;

    std::size_t n_;
    double mean_;

    template <typename RNGType, typename U01Type>
    void generate(RNGType &rng, U01Type &u01, std::size_t n,
        typename U01Type::result_type *r, std::size_t &s,
        occurs_type &occurs) const
    {
        rand(rng, u01, n * T, r);
        mul(n * T, static_cast<typename U01Type::result_type>(D), r, r);
        for (std::size_t i = 0; i != n; ++i, r += T)
            count(internal::serial_index<D, T>(r), s, occurs);
    }

    void reserve(std::bitset<K_> &) const {}

    void reserve(std::unordered_set<std::size_t> &occurs) const
    {
        occurs.reserve(static_cast<std::size_t>(mean_ * 2));
    }

    void count(std::size_t u, std::size_t &s, std::bitset<K_> &occurs) const
    {
        if (occurs.test(u))
            ++s;
        else
            occurs.set(u);
    }

    void count(std::size_t u, std::size_t &s,
        std::unordered_set<std::size_t> &occurs) const
    {
        if (occurs.count(u))
            ++s;
        else
            occurs.insert(u);
    }
}; // CollisionTest

} // namespace mckl

#endif // MCKL_RANDOM_COLLISION_TEST_HPP
