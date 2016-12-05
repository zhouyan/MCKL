//============================================================================
// MCKL/include/mckl/random/uniform_int_distribution.hpp
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

#ifndef MCKL_RANDOM_UNIFORM_INT_DISTRIBUTION_HPP
#define MCKL_RANDOM_UNIFORM_INT_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01_distribution.hpp>

namespace mckl
{

namespace internal
{

template <typename IntType>
inline bool uniform_int_distribution_use_double_big(
    IntType a, IntType b, std::true_type)
{
    constexpr IntType imax = const_one<IntType>() << 32;

    return a > -imax && b < imax;
}

template <typename IntType>
inline bool uniform_int_distribution_use_double_big(
    IntType, IntType b, std::false_type)
{
    constexpr IntType imax = const_one<IntType>() << 32;

    return b < imax;
}

template <typename IntType>
inline bool uniform_int_distribution_use_double(
    IntType, IntType, std::true_type)
{
    return true;
}

template <typename IntType>
inline bool uniform_int_distribution_use_double(
    IntType a, IntType b, std::false_type)
{
    return uniform_int_distribution_use_double_big(
        a, b, std::is_signed<IntType>());
}

template <typename IntType>
inline bool uniform_int_distribution_use_double(IntType a, IntType b)
{
    return uniform_int_distribution_use_double(
        a, b, std::integral_constant<bool,
                  std::numeric_limits<IntType>::digits <= 32>());
}

template <typename IntType>
inline bool uniform_int_distribution_check_param(IntType a, IntType b)
{
    return a <= b;
}

template <std::size_t K, typename IntType, typename RNGType>
inline void uniform_int_distribution_impl(RNGType &rng, std::size_t n,
    IntType *r, IntType a, IntType b, std::true_type)
{
    alignas(32) std::array<double, K> s;
    double ra = static_cast<double>(a);
    double rb = static_cast<double>(b);
    double *const u = s.data();
    u01_co_distribution(rng, n, u);
    muladd(n, u, rb - ra + const_one<double>(), ra, u);
    floor(n, u, u);
    for (std::size_t i = 0; i != n; ++i)
        r[i] = static_cast<IntType>(u[i]);
}

template <std::size_t, typename IntType, typename RNGType>
inline void uniform_int_distribution_impl(RNGType &rng, std::size_t n,
    IntType *r, IntType a, IntType b, std::false_type)
{
    std::uniform_int_distribution<IntType> uniform(a, b);
    rand(rng, uniform, n, r);
}

template <std::size_t K, typename IntType, typename RNGType>
inline void uniform_int_distribution_impl(
    RNGType &rng, std::size_t n, IntType *r, IntType a, IntType b)
{
    if (a == b) {
        std::fill_n(r, n, a);
        return;
    }

    uniform_int_distribution_use_double(a, b) ?
        uniform_int_distribution_impl<K>(rng, n, r, a, b, std::true_type()) :
        uniform_int_distribution_impl<K>(rng, n, r, a, b, std::false_type());
}

} // namespace mckl::internal

MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_2(
    UniformInt, uniform_int, IntType, IntType, a, IntType, b)

/// \brief Uniform integer distribution
/// \ingroup Distribution
template <typename IntType>
class UniformIntDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_INT_TYPE(UniformInt, short)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_2(UniformInt, uniform_int, IntType,
        result_type, a, 0, result_type, b,
        std::numeric_limits<result_type>::max())
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0

    public:
    result_type min() const { return a(); }

    result_type max() const { return b(); }

    void reset() {}

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        using UIntType = typename std::make_unsigned<result_type>::type;

        constexpr result_type imin = std::numeric_limits<result_type>::min();
        constexpr result_type imax = std::numeric_limits<result_type>::max();

        if (param.a() == param.b())
            return param.a();

        if (param.a() == imin && param.b() == imax) {
            UniformBitsDistribution<UIntType> ubits;
            union {
                UIntType u;
                result_type r;
            } buf;
            buf.u = ubits(rng);

            return buf.r;
        }

        return internal::uniform_int_distribution_use_double(
                   param.a(), param.b()) ?
            generate(rng, param, std::true_type()) :
            generate(rng, param, std::false_type());
    }

    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param, std::true_type)
    {
        U01CODistribution<double> u01;
        const double ra = static_cast<double>(param.a());
        const double rb = static_cast<double>(param.b());
        const double u = ra + (rb - ra + const_one<double>()) * u01(rng);

        return static_cast<result_type>(std::floor(u));
    }

    template <typename RNGType>
    result_type generate(
        RNGType &rng, const param_type &param, std::false_type)
    {
        std::uniform_int_distribution<result_type> uniform(
            param.a(), param.b());

        return uniform(rng);
    }
}; // class UniformIntDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(UniformInt, IntType)

} // namespace mckl

#endif // MCKL_RANDOM_UNIFORM_INT_DISTRIBUTION_HPP
