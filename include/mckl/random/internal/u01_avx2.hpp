//============================================================================
// MCKL/include/mckl/random/internal/u01_avx2.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_U01_AVX2_HPP
#define MCKL_RANDOM_INTERNAL_U01_AVX2_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/u01_generic.hpp>

namespace mckl
{

namespace internal
{

template <typename UIntType, typename RealType, typename Lower, typename Upper,
    int = std::numeric_limits<UIntType>::digits>
class U01AVX2Impl : public U01GenericImpl<UIntType, RealType, Lower, Upper>
{
}; // U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Closed, Closed, 32>
{
    public:
    static double eval(UIntType u)
    {
        return U01GenericImpl<UIntType, double, Closed, Closed>::eval(u);
    }

    static void eval(
        const std::array<__m128i, 8> &s, std::array<__m256d, 8> &t)
    {
        const __m256d p32 = _mm256_castsi256_pd( // 2^{-32}
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DF0000000000000)));
        const __m128i mask = _mm_set1_epi32(1);

        std::array<__m128i, 8> p;
        std::array<__m256d, 8> q;

        std::get<0>(p) = _mm_and_si128(std::get<0>(s), mask);
        std::get<1>(p) = _mm_and_si128(std::get<1>(s), mask);
        std::get<2>(p) = _mm_and_si128(std::get<2>(s), mask);
        std::get<3>(p) = _mm_and_si128(std::get<3>(s), mask);
        std::get<4>(p) = _mm_and_si128(std::get<4>(s), mask);
        std::get<5>(p) = _mm_and_si128(std::get<5>(s), mask);
        std::get<6>(p) = _mm_and_si128(std::get<6>(s), mask);
        std::get<7>(p) = _mm_and_si128(std::get<7>(s), mask);

        convert_u32_256d(p, q);
        convert_u32_256d(s, t);
        add_256d(t, q);
        mul_256d(t, p32);
    }

    static void eval(
        const std::array<__m128i, 16> &s, std::array<__m256d, 16> &t)
    {
        const __m256d p32 = _mm256_castsi256_pd( // 2^{-32}
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DF0000000000000)));
        const __m128i mask = _mm_set1_epi32(1);

        std::array<__m128i, 16> p;
        std::array<__m256d, 16> q;

        std::get<0x0>(p) = _mm_and_si128(std::get<0x0>(s), mask);
        std::get<0x1>(p) = _mm_and_si128(std::get<0x1>(s), mask);
        std::get<0x2>(p) = _mm_and_si128(std::get<0x2>(s), mask);
        std::get<0x3>(p) = _mm_and_si128(std::get<0x3>(s), mask);
        std::get<0x4>(p) = _mm_and_si128(std::get<0x4>(s), mask);
        std::get<0x5>(p) = _mm_and_si128(std::get<0x5>(s), mask);
        std::get<0x6>(p) = _mm_and_si128(std::get<0x6>(s), mask);
        std::get<0x7>(p) = _mm_and_si128(std::get<0x7>(s), mask);
        std::get<0x8>(p) = _mm_and_si128(std::get<0x8>(s), mask);
        std::get<0x9>(p) = _mm_and_si128(std::get<0x9>(s), mask);
        std::get<0xA>(p) = _mm_and_si128(std::get<0xA>(s), mask);
        std::get<0xB>(p) = _mm_and_si128(std::get<0xB>(s), mask);
        std::get<0xC>(p) = _mm_and_si128(std::get<0xC>(s), mask);
        std::get<0xD>(p) = _mm_and_si128(std::get<0xD>(s), mask);
        std::get<0xE>(p) = _mm_and_si128(std::get<0xE>(s), mask);
        std::get<0xF>(p) = _mm_and_si128(std::get<0xF>(s), mask);

        convert_u32_256d(p, q);
        convert_u32_256d(s, t);
        add_256d(t, q);
        mul_256d(t, p32);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Closed, Open, 32>
{
    public:
    static double eval(UIntType u)
    {
        return U01GenericImpl<UIntType, double, Closed, Open>::eval(u);
    }

    template <std::size_t S>
    static void eval(
        const std::array<__m128i, S> &s, std::array<__m256d, S> &t)
    {
        const __m256d p32 = _mm256_castsi256_pd( // 2^{-32}
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DF0000000000000)));

        convert_u32_256d(s, t);
        mul_256d(t, p32);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Open, Closed, 32>
{
    public:
    static double eval(UIntType u)
    {
        return U01GenericImpl<UIntType, double, Open, Closed>::eval(u);
    }

    template <std::size_t S>
    static void eval(
        const std::array<__m128i, S> &s, std::array<__m256d, S> &t)
    {
        const __m256d p32 = _mm256_castsi256_pd( // 2^{-32}
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DF0000000000000)));

        convert_u32_256d(s, t);
        fma_256d(t, p32, p32);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Open, Open, 32>
{
    public:
    static double eval(UIntType u)
    {
        return U01GenericImpl<UIntType, double, Open, Open>::eval(u);
    }

    template <std::size_t S>
    static void eval(
        const std::array<__m128i, S> &s, std::array<__m256d, S> &t)
    {
        const __m256d p32 = _mm256_castsi256_pd( // 2^{-32}
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DF0000000000000)));
        const __m256d p33 = _mm256_castsi256_pd( // 2^{-33}
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DE0000000000000)));

        convert_u32_256d(s, t);
        fma_256d(t, p32, p33);
    }
}; // class U01AVX2Impl

template <typename UIntType, typename RealType,
    int = std::numeric_limits<UIntType>::digits>
class UniformRealAVX2Impl;

template <typename UIntType>
class UniformRealAVX2Impl<UIntType, double, 32>
{
    public:
    static double eval(UIntType u, double a, double b)
    {
        return U01GenericImpl<UIntType, double, Closed, Open>::eval(u) *
            (b - a) +
            a;
    }

    template <std::size_t S>
    static void eval(const std::array<__m128i, S> &s,
        std::array<__m256d, S> &t, double a, double b)
    {
        const __m256d p32 = _mm256_castsi256_pd( // 2^{-32}
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DF0000000000000)));

        convert_u32_256d(s, t);
        mul_256d(t, p32);
        fma_256d(t, a, b);
    }
}; // class UniformRealAVX2Impl

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_U01_AVX2_HPP
