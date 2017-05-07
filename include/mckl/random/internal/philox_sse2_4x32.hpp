//============================================================================
// MCKL/include/mckl/random/internal/philox_sse2_4x32.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/philox_common.hpp>
#include <mckl/random/internal/philox_constants.hpp>
#include <mckl/random/internal/philox_generic_4x.hpp>
#include <mckl/random/internal/philox_sse2_32_common.hpp>

MCKL_PUSH_GCC_WARNING("-Wignored-attributes")

namespace mckl {

namespace internal {

template <typename T, typename Constants>
class Philox4x32GeneratorSSE2Impl
{
    static_assert(std::numeric_limits<T>::digits == 32,
        "**Philox4x32GeneratorSSE2Impl** used with T other than a 32-bit "
        "unsigned integers");

    static constexpr std::size_t K = 4;
    static constexpr std::size_t Rounds = 10;

  public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K / 2> &key)
    {
        Philox4xGeneratorGenericImpl<T, Constants>::eval(plain, cipher, key);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 2> &ctr, ResultType *r,
        const std::array<T, K / 2> &key)
    {
        Philox4xGeneratorGenericImpl<T, Constants>::eval(ctr, r, key);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 2> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K / 2> &key)
    {
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        const std::size_t n0 =
            static_cast<std::size_t>(std::min(static_cast<std::uint64_t>(n),
                std::numeric_limits<std::uint64_t>::max() - ctr.front()));

        eval_kernel(ctr, n0, r, key);
        n -= n0;
        r += n0 * R;

        if (n != 0) {
            eval(ctr, r, key);
            n -= 1;
            r += R;
        }

        eval_kernel(ctr, n, r, key);
    }

  private:
    template <typename ResultType>
    static void eval_kernel(std::array<std::uint64_t, 2> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K / 2> &key)
    {
#if MCKL_USE_ASM_LIBRARY
        constexpr T m0 = Constants::multiplier::value[0];
        constexpr T m1 = Constants::multiplier::value[1];
        constexpr T w0 = Constants::weyl::value[0];
        constexpr T w1 = Constants::weyl::value[1];

        const T mwk[12] = {m0, 0, m1, 0, 0, w0, 0, w1, 0, std::get<0>(key), 0,
            std::get<1>(key)};
        mckl_philox4x32_sse2_kernel(ctr.data(), n, r, mwk);
#else  // MCKL_USE_ASM_LIBRARY
        constexpr std::size_t S = 8;
        constexpr std::size_t N = sizeof(__m128i) * S / (sizeof(T) * K);

        const int k0 = static_cast<int>(std::get<0>(key));
        const int k1 = static_cast<int>(std::get<1>(key));
        const __m128i xmmk0 = _mm_set_epi32(k1, 0, k0, 0);

        __m128i xmmc =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));
        ctr.front() += n;

        __m128i *rptr = reinterpret_cast<__m128i *>(r);
        while (n != 0) {
            __m128i xmm0 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 1));
            __m128i xmm1 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 2));
            __m128i xmm2 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 3));
            __m128i xmm3 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 4));
            __m128i xmm4 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 5));
            __m128i xmm5 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 6));
            __m128i xmm6 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 7));
            __m128i xmm7 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 8));
            xmmc = xmm7;

            xmm0 = _mm_shuffle_epi32(xmm0, 0xC6);
            xmm1 = _mm_shuffle_epi32(xmm1, 0xC6);
            xmm2 = _mm_shuffle_epi32(xmm2, 0xC6);
            xmm3 = _mm_shuffle_epi32(xmm3, 0xC6);
            xmm4 = _mm_shuffle_epi32(xmm4, 0xC6);
            xmm5 = _mm_shuffle_epi32(xmm5, 0xC6);
            xmm6 = _mm_shuffle_epi32(xmm6, 0xC6);
            xmm7 = _mm_shuffle_epi32(xmm7, 0xC6);

            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_RBOX(4, 0, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_RBOX(4, 1, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_RBOX(4, 2, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_RBOX(4, 3, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_RBOX(4, 4, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_RBOX(4, 5, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_RBOX(4, 6, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_RBOX(4, 7, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_RBOX(4, 8, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_RBOX(4, 9, 0xB1)

            if (n >= N) {
                n -= N;
                _mm_storeu_si128(rptr++, xmm0);
                _mm_storeu_si128(rptr++, xmm1);
                _mm_storeu_si128(rptr++, xmm2);
                _mm_storeu_si128(rptr++, xmm3);
                _mm_storeu_si128(rptr++, xmm4);
                _mm_storeu_si128(rptr++, xmm5);
                _mm_storeu_si128(rptr++, xmm6);
                _mm_storeu_si128(rptr++, xmm7);
            } else {
                std::array<__m128i, S> s;
                std::get<0>(s) = xmm0;
                std::get<1>(s) = xmm1;
                std::get<2>(s) = xmm2;
                std::get<3>(s) = xmm3;
                std::get<4>(s) = xmm4;
                std::get<5>(s) = xmm5;
                std::get<6>(s) = xmm6;
                std::get<7>(s) = xmm7;
                std::memcpy(rptr, s.data(), n * sizeof(T) * K);
                break;
            }
        }
#endif // MCKL_USE_ASM_LIBRARY
    }
}; // class Philox4x32GeneratorSSE2Impl

} // namespace mckl::internal

} // namespace mckl

MCKL_POP_GCC_WARNING

#endif // MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_HPP
