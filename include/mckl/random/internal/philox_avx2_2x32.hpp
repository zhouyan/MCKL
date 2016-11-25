//============================================================================
// MCKL/include/mckl/random/internal/philox_avx2_2x32.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_AVX2_2X32_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_AVX2_2X32_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/philox_avx2_32_common.hpp>
#include <mckl/random/internal/philox_common.hpp>
#include <mckl/random/internal/philox_constants.hpp>
#include <mckl/random/internal/philox_generic_2x.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

namespace mckl
{

namespace internal
{

template <typename T, typename Constants>
class Philox2x32GeneratorAVX2Impl
{
    static_assert(std::numeric_limits<T>::digits == 32,
        "**Philox2x32GeneratorAVX2Impl** used with T other than a 32-bit "
        "unsigned integers");

    static constexpr std::size_t K = 2;
    static constexpr std::size_t Rounds = 10;

    public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K / 2> &key)
    {
        Philox2xGeneratorGenericImpl<T, Constants>::eval(plain, cipher, key);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 1> &ctr, ResultType *r,
        const std::array<T, K / 2> &key)
    {
        Philox2xGeneratorGenericImpl<T, Constants>::eval(ctr, r, key);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 1> &ctr, std::size_t n,
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
    static void eval_kernel(std::array<std::uint64_t, 1> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K / 2> &key)
    {
#if MCKL_USE_ASM_LIBRARY
        constexpr T m0 = Constants::multiplier::value[0];
        constexpr T w0 = Constants::weyl::value[0];

        const T mwk[6] = {m0, 0, 0, w0, 0, std::get<0>(key)};
        mckl_philox2x32_avx2_kernel(ctr.data(), n, r, mwk);
#else  // MCKL_USE_ASM_LIBRARY
        constexpr std::size_t S = 8;
        constexpr std::size_t N = sizeof(__m256i) * S / (sizeof(T) * K);

        const int k0 = static_cast<int>(std::get<0>(key));
        const __m256i ymmk0 = _mm256_set_epi32(k0, 0, k0, 0, k0, 0, k0, 0);

        __m256i ymmc =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(std::get<0>(ctr)));
        ctr.front() += n;

        __m256i *rptr = reinterpret_cast<__m256i *>(r);
        while (n != 0) {
            __m256i ymm0 = _mm256_add_epi64(
                ymmc, _mm256_set_epi64x(0x04, 0x03, 0x02, 0x01));
            __m256i ymm1 = _mm256_add_epi64(
                ymmc, _mm256_set_epi64x(0x08, 0x07, 0x06, 0x05));
            __m256i ymm2 = _mm256_add_epi64(
                ymmc, _mm256_set_epi64x(0x0C, 0x0B, 0x0A, 0x09));
            __m256i ymm3 = _mm256_add_epi64(
                ymmc, _mm256_set_epi64x(0x10, 0x0F, 0x0E, 0x0D));
            __m256i ymm4 = _mm256_add_epi64(
                ymmc, _mm256_set_epi64x(0x14, 0x13, 0x12, 0x11));
            __m256i ymm5 = _mm256_add_epi64(
                ymmc, _mm256_set_epi64x(0x18, 0x17, 0x16, 0x15));
            __m256i ymm6 = _mm256_add_epi64(
                ymmc, _mm256_set_epi64x(0x1C, 0x1B, 0x1A, 0x19));
            __m256i ymm7 = _mm256_add_epi64(
                ymmc, _mm256_set_epi64x(0x20, 0x1F, 0x1E, 0x1D));
            ymmc = _mm256_add_epi64(ymmc, _mm256_set1_epi64x(0x20));

            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_RBOX(2, 0, 0xB1)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_RBOX(2, 1, 0xB1)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_RBOX(2, 2, 0xB1)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_RBOX(2, 3, 0xB1)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_RBOX(2, 4, 0xB1)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_RBOX(2, 5, 0xB1)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_RBOX(2, 6, 0xB1)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_RBOX(2, 7, 0xB1)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_RBOX(2, 8, 0xB1)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_RBOX(2, 9, 0xB1)

            if (n >= N) {
                n -= N;
                _mm256_storeu_si256(rptr++, ymm0);
                _mm256_storeu_si256(rptr++, ymm1);
                _mm256_storeu_si256(rptr++, ymm2);
                _mm256_storeu_si256(rptr++, ymm3);
                _mm256_storeu_si256(rptr++, ymm4);
                _mm256_storeu_si256(rptr++, ymm5);
                _mm256_storeu_si256(rptr++, ymm6);
                _mm256_storeu_si256(rptr++, ymm7);
            } else {
                std::array<__m256i, S> s;
                std::get<0>(s) = ymm0;
                std::get<1>(s) = ymm1;
                std::get<2>(s) = ymm2;
                std::get<3>(s) = ymm3;
                std::get<4>(s) = ymm4;
                std::get<5>(s) = ymm5;
                std::get<6>(s) = ymm6;
                std::get<7>(s) = ymm7;
                std::memcpy(rptr, s.data(), n * sizeof(T) * K);
                break;
            }
        }
#endif // MCKL_USE_ASM_LIBRARY
    }
}; // class Philox2x32GeneratorAVX2Impl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_PHILOX_AVX2_2X32_HPP
