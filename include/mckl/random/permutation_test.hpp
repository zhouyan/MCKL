//============================================================================
// MCKL/include/mckl/random/permutation_test.hpp
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

#ifndef MCKL_RANDOM_PERMUTATION_TEST_HPP
#define MCKL_RANDOM_PERMUTATION_TEST_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/chi_squared_test.hpp>

namespace mckl
{

/// \brief Permutation test
/// \ingroup RandomTest
///
/// \tparam T Length of the tuple
template <std::size_t T>
class PermutationTest : public ChiSquaredTest<PermutationTest<T>>
{
    static_assert(T > 0, "**PermutationTest** used with T equal to zero");

    static_assert(T <= 20, "**PermutationTest** used with T larger than 20");

  public:
    /// \brief Construct a Permutation test
    ///
    /// \param n The number of tuples
    PermutationTest(std::size_t n) : n_(n), np_(static_cast<double>(n) / M_) {}

    MCKL_DEFINE_RANDOM_TEST_OPERATOR(double)

    template <typename RNGType, typename DistributionType>
    double operator()(RNGType &rng, DistributionType &distribution)
    {
        std::array<typename DistributionType::result_type, T> r;
        Vector<double> count(M_);
        std::fill(count.begin(), count.end(), 0);
        for (std::size_t i = 0; i != n_; ++i) {
            rand(rng, distribution, T, r.data());
            count[internal::permutation_index<T>(r.data())] += 1;
        }

        return this->stat(M_, count.data(), np_);
    }

    double degree_of_freedom() const { return M_ - 1; }

  private:
    static constexpr std::size_t M_ =
        internal::Factorial<std::size_t, T>::value;

    std::size_t n_;
    double np_;
}; // class PermutationTest

} // namespace mckl

#endif // MCKL_RANDOM_PERMUTATION_TEST_HPP
