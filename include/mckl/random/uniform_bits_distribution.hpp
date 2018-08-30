//============================================================================
// MCKL/include/mckl/random/uniform_bits_distribution.hpp
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

#ifndef MCKL_RANDOM_UNIFORM_BITS_DISTRIBUTION_HPP
#define MCKL_RANDOM_UNIFORM_BITS_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>

namespace mckl {

namespace internal {

template <typename UIntType, typename RNGType>
inline void uniform_bits_distribution_impl(
    RNGType &rng, std::size_t n, UIntType *r, std::false_type, std::false_type)
{
    UniformBitsDistribution<UIntType> ubits;
    for (std::size_t i = 0; i != n; ++i) {
        r[i] = ubits(rng);
    }
}

template <typename UIntType, typename RNGType, bool DownCast>
inline void uniform_bits_distribution_impl(RNGType &rng, std::size_t n,
    UIntType *r, std::true_type, std::integral_constant<bool, DownCast>)
{
    constexpr int rbits = RNGTraits<RNGType>::bits;
    constexpr int ubits = std::numeric_limits<UIntType>::digits;
    constexpr std::size_t rate = ubits / rbits;
    const std::size_t m = n * rate;
    rand(rng, m, reinterpret_cast<typename RNGType::result_type *>(r));
}

template <typename UIntType, typename RNGType>
inline void uniform_bits_distribution_impl(
    RNGType &rng, std::size_t n, UIntType *r, std::false_type, std::true_type)
{
    const std::size_t k = BufferSize<UIntType>::value;
    alignas(MCKL_ALIGNMENT) std::array<typename RNGType::result_type, k> s;
    while (n >= k) {
        rand(rng, k, s.data());
        std::copy_n(s.data(), k, r);
        n -= k;
        r += k;
    }
    rand(rng, n, s.data());
    std::copy_n(s.data(), n, r);
}

} // namespace mckl::internal

template <typename UIntType, typename RNGType>
inline void uniform_bits_distribution(RNGType &rng, std::size_t n, UIntType *r)
{
    constexpr int rbits = RNGTraits<RNGType>::bits;
    constexpr int ubits = std::numeric_limits<UIntType>::digits;
    constexpr int tbits =
        std::numeric_limits<typename RNGType::result_type>::digits;
    constexpr std::size_t ualign = alignof(UIntType);
    constexpr std::size_t talign = alignof(UIntType);
    constexpr bool direct =
        // Full range, range is [0, 2^W - 1]
        RNGTraits<RNGType>::is_full_range &&
        // All ouptut of rng() are random bits
        rbits == tbits &&
        // One or multiple rng() can fill exactly one output integer
        ubits >= tbits && ubits % tbits == 0 &&
        // Alignment requirement
        ualign >= talign && ualign % talign == 0;
    constexpr bool downcast =
        // Full range, range is [0, 2^W - 1]
        RNGTraits<RNGType>::is_full_range &&
        // down cast
        ubits <= rbits;

    internal::uniform_bits_distribution_impl(rng, n, r,
        std::integral_constant<bool, direct>(),
        std::integral_constant<bool, downcast>());
}

template <typename UIntType, typename RNGType>
inline void uniform_bits_distribution(RNGType &rng, std::size_t n, UIntType *r,
    const typename UniformBitsDistribution<UIntType>::param_type &)
{
    uniform_bits_distribution(rng, n, r);
}

/// \brief Uniform bits distribution
/// \ingroup Distribution
///
/// \details
/// For a given unsigned integer type `UIntType` with \f$W\f$ bits, the output
/// will be unsigned integers on the set \f$\{0,\dots,2^W - 1\}\f$ regardless
/// of the range of the input `RNGType`
template <typename UIntType>
class UniformBitsDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_UINT_TYPE(UniformBits, 16)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_0(UniformBits, uniform_bits, UIntType)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0

  public:
    result_type min() const { return std::numeric_limits<result_type>::min(); }

    result_type max() const { return std::numeric_limits<result_type>::max(); }

    void reset() {}

  private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &)
    {
        constexpr bool patch = RNGTraits<RNGType>::is_full_range;

        return generate(rng, std::integral_constant<bool, patch>());
    }

    template <typename RNGType>
    static UIntType generate(RNGType &rng, std::false_type)
    {
        std::independent_bits_engine<RNGType,
            std::numeric_limits<UIntType>::digits, UIntType>
            eng(std::move(rng));
        UIntType u = eng();
        rng = std::move(eng.base());

        return u;
    }

    template <typename RNGType>
    static UIntType generate(RNGType &rng, std::true_type)
    {
        constexpr int w = std::numeric_limits<UIntType>::digits;
        constexpr int r = RNGTraits<RNGType>::bits;

        return generate_patch(rng, std::integral_constant<bool, (w <= r)>());
    }

    template <typename RNGType>
    static UIntType generate_patch(RNGType &rng, std::true_type)
    {
        return static_cast<UIntType>(rng() - RNGType::min());
    }

    template <typename RNGType>
    static UIntType generate_patch(RNGType &rng, std::false_type)
    {
#if MCKL_HAS_LITTLE_ENDIAN || !MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        return patch_le<0>(rng, std::true_type());
#elif MCKL_HAS_BIG_ENDIAN
        return patch_be<0>(rng, std::true_type());
#else
        return internal::is_big_endian() ? patch_be<0>(rng, std::true_type()) :
                                           patch_le<0>(rng, std::true_type());
#endif
    }

    template <int, typename RNGType>
    static UIntType patch_le(RNGType &, std::false_type)
    {
        return 0;
    }

    template <int N, typename RNGType>
    static UIntType patch_le(RNGType &rng, std::true_type)
    {
        constexpr int w = std::numeric_limits<UIntType>::digits;
        constexpr int r = RNGTraits<RNGType>::bits;
        constexpr int p = r * N;
        constexpr int q = r * N + r;

        UIntType u = static_cast<UIntType>(rng() - RNGType::min())
            << static_cast<UIntType>(p);

        return u |
            patch_le<N + 1>(rng, std::integral_constant<bool, (q < w)>());
    }

    template <int N, typename RNGType>
    static UIntType patch_be(RNGType &, std::false_type)
    {
        return 0;
    }

    template <int N, typename RNGType>
    static UIntType patch_be(RNGType &rng, std::true_type)
    {
        constexpr int w = std::numeric_limits<UIntType>::digits;
        constexpr int r = RNGTraits<RNGType>::bits;
        constexpr int q = r * N + r;
        constexpr int pl = q < w ? w - q : 0;
        constexpr int pr = q < w ? 0 : q - w;

        UIntType u = static_cast<UIntType>(rng() - RNGType::min());
        u <<= static_cast<UIntType>(pl);
        u >>= static_cast<UIntType>(pr);

        return u |
            patch_be<N + 1>(rng, std::integral_constant<bool, (q < w)>());
    }
}; // class UniformBitsDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(UniformBits, UIntType)

} // namespace mckl

#endif // MCKL_RANDOM_UNIFORM_BITS_DISTRIBUTION_HPP
