//============================================================================
// MCKL/include/mckl/random/maximum_of_t.hpp
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

#ifndef MCKL_RANDOM_MAXIMUM_OF_T_TEST_HPP
#define MCKL_RANDOM_MAXIMUM_OF_T_TEST_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/chi_squared_test.hpp>

namespace mckl
{

/// \brief Tests based on the maximum-of-t method
/// \ingroup RandomTest
///
/// \tparam D Multiplier
/// \tparam T Length of the tuple
template <std::size_t D, std::size_t T>
class MaximumOfTTest : public ChiSquaredTest<MaximumOfTTest<D, T>>
{
    static_assert(D > 1, "**MaximumOfTTest** used with D less than two");

    static_assert(T > 0, "**MaximumOfTTest** used with T equal to zero");

    public:
    MaximumOfTTest(std::size_t n) : n_(n) {}

    template <typename RNGType, typename U01DistributionType>
    double operator()(RNGType &rng, U01DistributionType &u01)
    {
        using result_type = typename U01DistributionType::result_type;

        Vector<double> count(D);
        std::fill(count.begin(), count.end(), 0);

        const std::size_t k = internal::BufferSize<result_type>::value;
        const std::size_t m = n_ / k;
        const std::size_t l = n_ % k;
        Vector<result_type> r(k * T);
        Vector<result_type> s(k);
        for (std::size_t i = 0; i != m; ++i)
            generate(rng, u01, k, r.data(), s.data(), count.data());
        generate(rng, u01, l, r.data(), s.data(), count.data());

        return this->stat(D, count.data(), static_cast<double>(n_) / D);
    }

    double degree_of_freedom() const { return D - 1; }

    private:
    std::size_t n_;

    template <typename RNGType, typename U01DistributionType>
    void generate(RNGType &rng, U01DistributionType &u01, std::size_t n,
        typename U01DistributionType::result_type *r,
        typename U01DistributionType::result_type *s, double *count)
    {
        using result_type = typename U01DistributionType::result_type;

        rand(rng, u01, n * T, r);
        for (std::size_t i = 0; i != n; ++i, r += T)
            s[i] = *std::max_element(r, r + T);
        pow(n, s, static_cast<result_type>(T), s);
        mul(n, static_cast<result_type>(D), s, s);
        for (std::size_t i = 0; i != n; ++i)
            ++count[internal::ftoi<std::size_t, D>(s[i])];
    }
}; // class MaximumOfTTest

} // namespace mckl

#endif // MCKL_RANDOM_MAXIMUM_OF_T_TEST_HPP
