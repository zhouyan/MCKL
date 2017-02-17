//============================================================================
// MCKL/include/mckl/random/u01_distribution.hpp
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

#ifndef MCKL_RANDOM_U01_DISTRIBUTION_HPP
#define MCKL_RANDOM_U01_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01.hpp>
#include <mckl/random/uniform_bits_distribution.hpp>

/// \brief Default U01 distribution using fixed point conversion
/// \ingroup Config
#ifndef MCKL_U01_USE_FIXED_POINT
#define MCKL_U01_USE_FIXED_POINT 0
#endif

/// \brief Use 64-bit intermediate random integers for double precison output
/// \ingroup Config
#ifndef MCKL_U01_USE_64BITS_DOUBLE
#define MCKL_U01_USE_64BITS_DOUBLE 0
#endif

#define MCKL_DEFINE_RANDOM_U01_DISTRIBUTION(Name, name)                       \
    template <typename RealType>                                              \
    class Name##Distribution                                                  \
    {                                                                         \
        MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(Name)                \
        MCKL_DEFINE_RANDOM_DISTRIBUTION_0(Name, name, RealType)               \
        MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0                              \
                                                                              \
        public:                                                               \
        result_type min() const { return 0; }                                 \
                                                                              \
        result_type max() const { return 1; }                                 \
                                                                              \
        void reset() {}                                                       \
                                                                              \
        private:                                                              \
        template <typename RNGType>                                           \
        result_type generate(RNGType &rng, const param_type &)                \
        {                                                                     \
            using UIntType =                                                  \
                typename internal::U01UIntType<RNGType, RealType>;            \
                                                                              \
            UniformBitsDistribution<UIntType> ubits;                          \
                                                                              \
            return name<UIntType, result_type>(ubits(rng));                   \
        }                                                                     \
    };

#define MCKL_DEFINE_RANDOM_U01_DISTRIBUTION_IMPL(name)                        \
    template <std::size_t K, typename RealType, typename RNGType>             \
    inline void name##_distribution_impl(                                     \
        RNGType &rng, std::size_t n, RealType *r)                             \
    {                                                                         \
        using UIntType = U01UIntType<RNGType, RealType>;                      \
                                                                              \
        alignas(32) std::array<UIntType, K> s;                                \
        uniform_bits_distribution(rng, n, s.data());                          \
        name<UIntType, RealType>(n, s.data(), r);                             \
    }

namespace mckl
{

namespace internal
{

#if MCKL_U01_USE_64BITS_DOUBLE

template <typename RNGType, typename RealType>
using U01UIntType = std::conditional_t<
    (RNGTraits<RNGType>::bits >= 64 ||
        std::is_same<typename std::remove_cv_t<RealType>,
            long double>::value ||
        std::is_same<typename std::remove_cv_t<RealType>, double>::value),
    std::uint64_t, std::uint32_t>;

#else // MCKL_U01_USE_64BITS_DOUBLE

template <typename RNGType, typename RealType>
using U01UIntType =
    std::conditional_t<(RNGTraits<RNGType>::bits >= 64 ||
                           std::is_same<RealType, long double>::value),
        std::uint64_t, std::uint32_t>;

#endif // MCKL_U01_USE_64BITS_DOUBLE

template <std::size_t K, typename RealType, typename RNGType>
inline void u01_canonical_distribution_impl(
    RNGType &rng, std::size_t n, RealType *r)
{
    using UIntType = U01UIntType<RNGType, RealType>;

    constexpr int W = std::numeric_limits<UIntType>::digits;
    constexpr int M = std::numeric_limits<RealType>::digits;
    constexpr int P = (M + W - 1) / W;
    constexpr int Q = 1 > P ? 1 : P;

    alignas(32) std::array<UIntType, K * Q> s;
    uniform_bits_distribution(rng, n * Q, s.data());
    u01_canonical<UIntType, RealType, Q>(n, s.data(), r);
}

MCKL_DEFINE_RANDOM_U01_DISTRIBUTION_IMPL(u01_cc)
MCKL_DEFINE_RANDOM_U01_DISTRIBUTION_IMPL(u01_co)
MCKL_DEFINE_RANDOM_U01_DISTRIBUTION_IMPL(u01_oc)
MCKL_DEFINE_RANDOM_U01_DISTRIBUTION_IMPL(u01_oo)

} // namespace mckl::internal

MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_0(U01Canonical, u01_canonical, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_0(U01CC, u01_cc, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_0(U01CO, u01_co, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_0(U01OC, u01_oc, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_0(U01OO, u01_oo, RealType)

/// \brief Standard uniform distribution on [0, 1)
/// \ingroup Distribution
template <typename RealType>
class U01CanonicalDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(U01Canonical)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_0(U01Canonical, u01_canonical, RealType)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0

    public:
    result_type min() const { return 0; }
    result_type max() const { return 1; }

    void reset() {}

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &)
    {
        using UIntType = internal::U01UIntType<RNGType, RealType>;

        constexpr int W = std::numeric_limits<UIntType>::digits;
        constexpr int M = std::numeric_limits<RealType>::digits;
        constexpr int P = (M + W - 1) / W;
        constexpr int Q = 1 > P ? 1 : P;

        std::array<UIntType, Q> u;
        generate<0>(rng, u, std::true_type());

        return u01_canonical<UIntType, RealType, Q>(u.data());
    }

    template <std::size_t, typename RNGType, typename UIntType, std::size_t Q>
    void generate(RNGType &, std::array<UIntType, Q> &, std::false_type)
    {
    }

    template <std::size_t N, typename RNGType, typename UIntType,
        std::size_t Q>
    void generate(RNGType &rng, std::array<UIntType, Q> &u, std::true_type)
    {
        UniformBitsDistribution<UIntType> ubits;
        std::get<N>(u) = ubits(rng);
        generate<N + 1>(rng, u, std::integral_constant<bool, N + 1 < Q>());
    }
}; // class U01CanonicalDistribution

/// \brief Standard uniform distribution on [0, 1]
/// \ingroup Distribution
MCKL_DEFINE_RANDOM_U01_DISTRIBUTION(U01CC, u01_cc)

/// \brief Standard uniform distribution on [0, 1)
/// \ingroup Distribution
MCKL_DEFINE_RANDOM_U01_DISTRIBUTION(U01CO, u01_co)

/// \brief Standard uniform distribution on (0, 1]
/// \ingroup Distribution
MCKL_DEFINE_RANDOM_U01_DISTRIBUTION(U01OC, u01_oc)

/// \brief Standard uniform distribution on (0, 1)
/// \ingroup Distribution
MCKL_DEFINE_RANDOM_U01_DISTRIBUTION(U01OO, u01_oo)

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(U01Canonical, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(U01CC, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(U01CO, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(U01OC, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(U01OO, RealType)

#if MCKL_U01_USE_FIXED_POINT

template <typename RealType>
using U01Distribution = U01CODistribution<RealType>;

template <typename RealType, typename RNGType>
inline void u01_distribution(RNGType &rng, std::size_t n, RealType *r)
{
    u01_co_distribution(rng, n, r);
}

#else // MCKL_U01_USE_FIXED_POINT

template <typename RealType>
using U01Distribution = U01CanonicalDistribution<RealType>;

template <typename RealType, typename RNGType>
inline void u01_distribution(RNGType &rng, std::size_t n, RealType *r)
{
    u01_canonical_distribution(rng, n, r);
}

#endif // MCKL_U01_USE_FIXED_POINT

} // namespace mckl

#endif // MCKL_RANDOM_U01_DISTRIBUTION_HPP
