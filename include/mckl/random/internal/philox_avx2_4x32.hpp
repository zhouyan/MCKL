//============================================================================
// MCKL/include/mckl/random/internal/philox_avx2_4x32.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/philox_common.hpp>
#include <mckl/random/internal/philox_constants.hpp>
#include <mckl/random/internal/philox_generic_4x.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#define MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_RBOX(ymmk, imm8)                \
    ymmt0 = _mm256_mul_epu32(ymms0, ymmm);                                    \
    ymmt1 = _mm256_mul_epu32(ymms1, ymmm);                                    \
    ymmt2 = _mm256_mul_epu32(ymms2, ymmm);                                    \
    ymmt3 = _mm256_mul_epu32(ymms3, ymmm);                                    \
    ymmt4 = _mm256_mul_epu32(ymms4, ymmm);                                    \
    ymmt5 = _mm256_mul_epu32(ymms5, ymmm);                                    \
    ymmt6 = _mm256_mul_epu32(ymms6, ymmm);                                    \
    ymmt7 = _mm256_mul_epu32(ymms7, ymmm);                                    \
    ymms0 = _mm256_and_si256(ymms0, ymma);                                    \
    ymms1 = _mm256_and_si256(ymms1, ymma);                                    \
    ymms2 = _mm256_and_si256(ymms2, ymma);                                    \
    ymms3 = _mm256_and_si256(ymms3, ymma);                                    \
    ymms4 = _mm256_and_si256(ymms4, ymma);                                    \
    ymms5 = _mm256_and_si256(ymms5, ymma);                                    \
    ymms6 = _mm256_and_si256(ymms6, ymma);                                    \
    ymms7 = _mm256_and_si256(ymms7, ymma);                                    \
    ymms0 = _mm256_xor_si256(ymms0, ymmk);                                    \
    ymms1 = _mm256_xor_si256(ymms1, ymmk);                                    \
    ymms2 = _mm256_xor_si256(ymms2, ymmk);                                    \
    ymms3 = _mm256_xor_si256(ymms3, ymmk);                                    \
    ymms4 = _mm256_xor_si256(ymms4, ymmk);                                    \
    ymms5 = _mm256_xor_si256(ymms5, ymmk);                                    \
    ymms6 = _mm256_xor_si256(ymms6, ymmk);                                    \
    ymms7 = _mm256_xor_si256(ymms7, ymmk);                                    \
    ymms0 = _mm256_xor_si256(ymms0, ymmt0);                                   \
    ymms1 = _mm256_xor_si256(ymms1, ymmt1);                                   \
    ymms2 = _mm256_xor_si256(ymms2, ymmt2);                                   \
    ymms3 = _mm256_xor_si256(ymms3, ymmt3);                                   \
    ymms4 = _mm256_xor_si256(ymms4, ymmt4);                                   \
    ymms5 = _mm256_xor_si256(ymms5, ymmt5);                                   \
    ymms6 = _mm256_xor_si256(ymms6, ymmt6);                                   \
    ymms7 = _mm256_xor_si256(ymms7, ymmt7);                                   \
    ymms0 = _mm256_shuffle_epi32(ymms0, imm8);                                \
    ymms1 = _mm256_shuffle_epi32(ymms1, imm8);                                \
    ymms2 = _mm256_shuffle_epi32(ymms2, imm8);                                \
    ymms3 = _mm256_shuffle_epi32(ymms3, imm8);                                \
    ymms4 = _mm256_shuffle_epi32(ymms4, imm8);                                \
    ymms5 = _mm256_shuffle_epi32(ymms5, imm8);                                \
    ymms6 = _mm256_shuffle_epi32(ymms6, imm8);                                \
    ymms7 = _mm256_shuffle_epi32(ymms7, imm8);

namespace mckl
{

namespace internal
{

template <typename T, typename Constants>
class Philox4x32GeneratorAVX2Impl
{
    static_assert(std::numeric_limits<T>::digits == 32,
        "**Philox4x32GeneratorAVX2Impl** used with T other than a 32-bit "
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
        if (ctr.front() >= std::numeric_limits<std::uint64_t>::max() - n) {
            MCKL_NOINLINE_CALL Philox4xGeneratorGenericImpl<T,
                Constants>::eval(ctr, n, r, key);
            return;
        }

        constexpr std::size_t S = 8;
        constexpr std::size_t N = sizeof(__m256i) * S / (sizeof(T) * K);
        constexpr std::size_t M = sizeof(__m256i) / sizeof(ResultType);
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        constexpr int m0 = static_cast<int>(Constants::multiplier::value[0]);
        constexpr int m1 = static_cast<int>(Constants::multiplier::value[1]);
        __m256i ymmm = _mm256_set_epi32(0, m1, 0, m0, 0, m1, 0, m0);

        constexpr int ma = static_cast<int>(0xFFFFFFFF);
        __m256i ymma = _mm256_set_epi32(ma, 0, ma, 0, ma, 0, ma, 0);

        constexpr int w0 = static_cast<int>(Constants::weyl::value[0]);
        constexpr int w1 = static_cast<int>(Constants::weyl::value[1]);
        __m256i ymmw = _mm256_set_epi32(w1, 0, w0, 0, w1, 0, w0, 0);

        const int k0 = static_cast<int>(std::get<0>(key));
        const int k1 = static_cast<int>(std::get<1>(key));
        __m256i ymmk0 = _mm256_set_epi32(k1, 0, k0, 0, k1, 0, k0, 0);
        __m256i ymmk1 = _mm256_add_epi32(ymmk0, ymmw);
        __m256i ymmk2 = _mm256_add_epi32(ymmk1, ymmw);
        __m256i ymmk3 = _mm256_add_epi32(ymmk2, ymmw);
        __m256i ymmk4 = _mm256_add_epi32(ymmk3, ymmw);
        __m256i ymmk5 = _mm256_add_epi32(ymmk4, ymmw);
        __m256i ymmk6 = _mm256_add_epi32(ymmk5, ymmw);
        __m256i ymmk7 = _mm256_add_epi32(ymmk6, ymmw);
        __m256i ymmk8 = _mm256_add_epi32(ymmk7, ymmw);
        __m256i ymmk9 = _mm256_add_epi32(ymmk8, ymmw);

        __m256i ymmc =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));
        ctr.front() += n / N * N;

        while (n >= N) {
            __m256i ymms0 =
                _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x02, 0, 0x01));
            __m256i ymms1 =
                _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x04, 0, 0x03));
            __m256i ymms2 =
                _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x06, 0, 0x05));
            __m256i ymms3 =
                _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x08, 0, 0x07));
            __m256i ymms4 =
                _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x0A, 0, 0x09));
            __m256i ymms5 =
                _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x0C, 0, 0x0B));
            __m256i ymms6 =
                _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x0E, 0, 0x0D));
            __m256i ymms7 =
                _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x10, 0, 0x0F));

            ymmc = _mm256_add_epi64(
                ymmc, _mm256_set_epi64x(0, static_cast<MCKL_INT64>(N), 0,
                          static_cast<MCKL_INT64>(N)));

            __m256i ymmt0;
            __m256i ymmt1;
            __m256i ymmt2;
            __m256i ymmt3;
            __m256i ymmt4;
            __m256i ymmt5;
            __m256i ymmt6;
            __m256i ymmt7;

            ymms0 = _mm256_shuffle_epi32(ymms0, 0xC6); // 3 0 1 2
            ymms1 = _mm256_shuffle_epi32(ymms1, 0xC6); // 3 0 1 2
            ymms2 = _mm256_shuffle_epi32(ymms2, 0xC6); // 3 0 1 2
            ymms3 = _mm256_shuffle_epi32(ymms3, 0xC6); // 3 0 1 2
            ymms4 = _mm256_shuffle_epi32(ymms4, 0xC6); // 3 0 1 2
            ymms5 = _mm256_shuffle_epi32(ymms5, 0xC6); // 3 0 1 2
            ymms6 = _mm256_shuffle_epi32(ymms6, 0xC6); // 3 0 1 2
            ymms7 = _mm256_shuffle_epi32(ymms7, 0xC6); // 3 0 1 2

            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_RBOX(ymmk0, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_RBOX(ymmk1, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_RBOX(ymmk2, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_RBOX(ymmk3, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_RBOX(ymmk4, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_RBOX(ymmk5, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_RBOX(ymmk6, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_RBOX(ymmk7, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_RBOX(ymmk8, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_RBOX(ymmk9, 0xB1)

            _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 0), ymms0);
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 1), ymms1);
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 2), ymms2);
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 3), ymms3);
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 4), ymms4);
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 5), ymms5);
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 6), ymms6);
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 7), ymms7);

            n -= N;
            r += N * R;
        }

        MCKL_NOINLINE_CALL Philox4xGeneratorGenericImpl<T, Constants>::eval(
            ctr, n, r, key);
    }
}; // class Philox4x32GeneratorAVX2Impl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_PHILOX_AVX2_4X32_HPP
