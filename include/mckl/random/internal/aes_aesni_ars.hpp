//============================================================================
// MCKL/include/mckl/random/internal/aes_aesni_ars.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_AES_AESNI_ARS_HPP
#define MCKL_RANDOM_INTERNAL_AES_AESNI_ARS_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/aes_key_seq.hpp>

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

template <typename Constants>
class ARSKeySeqGeneratorAESNIImpl
{
    public:
    using key_type = std::array<std::uint32_t, 4>;
    using rk_type = __m128i;

    template <std::size_t Rp1>
    static key_type key(const std::array<__m128i, Rp1> &rk)
    {
        key_type key;
        std::memcpy(key.data(), rk.data(), sizeof(key_type));

        return key;
    }

    template <std::size_t Rp1>
    void operator()(const key_type &key, std::array<__m128i, Rp1> &rk)
    {
        key_ = _mm_set_epi32(static_cast<int>(std::get<3>(key)),
            static_cast<int>(std::get<2>(key)),
            static_cast<int>(std::get<1>(key)),
            static_cast<int>(std::get<0>(key)));
        generate<0>(rk, std::integral_constant<bool, 0 < Rp1>());
    }

    private:
    __m128i key_;

    template <std::size_t, std::size_t Rp1>
    void generate(std::array<__m128i, Rp1> &, std::false_type) const
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate(std::array<__m128i, Rp1> &rk, std::true_type) const
    {
        constexpr MCKL_INT64 w0 =
            static_cast<MCKL_INT64>(Constants::weyl::value[0] * N);
        constexpr MCKL_INT64 w1 =
            static_cast<MCKL_INT64>(Constants::weyl::value[1] * N);

        __m128i w = _mm_set_epi64x(w1, w0);
        std::get<N>(rk) = _mm_add_epi64(key_, w);
        generate<N + 1>(rk, std::integral_constant<bool, N + 1 < Rp1>());
    }
}; // class ARSKeySeqGeneratorAESNIImpl

template <typename Constants>
using ARSKeySeqAESNI =
    ARSKeySeqImpl<5, ARSKeySeqGeneratorAESNIImpl<Constants>>;

template <typename Constants>
class AESGeneratorAESNIImplARS
{
    using KeySeqType = ARSKeySeqAESNI<Constants>;

    public:
    static void eval(const void *plain, void *cipher, const KeySeqType &ks)
    {
        constexpr MCKL_INT64 w0 =
            static_cast<MCKL_INT64>(Constants::weyl::value[0]);
        constexpr MCKL_INT64 w1 =
            static_cast<MCKL_INT64>(Constants::weyl::value[1]);
        __m128i xmmw = _mm_set_epi64x(w1, w0);

        auto &&key = ks.key();
        __m128i xmmk =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(key)),
                static_cast<MCKL_INT64>(std::get<0>(key)));

        __m128i xmm0 =
            _mm_loadu_si128(reinterpret_cast<const __m128i *>(plain));

        xmm0 = _mm_xor_si128(xmm0, xmmk);        // Round 0
        xmmk = _mm_add_epi64(xmmk, xmmw);        // Round 1
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);     // Round 1
        xmmk = _mm_add_epi64(xmmk, xmmw);        // Round 2
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);     // Round 2
        xmmk = _mm_add_epi64(xmmk, xmmw);        // Round 3
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);     // Round 3
        xmmk = _mm_add_epi64(xmmk, xmmw);        // Round 4
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);     // Round 4
        xmmk = _mm_add_epi64(xmmk, xmmw);        // Round 5
        xmm0 = _mm_aesenclast_si128(xmm0, xmmk); // Round 5

        _mm_storeu_si128(reinterpret_cast<__m128i *>(cipher), xmm0);
    }

    template <typename ResultType>
    static void eval(
        std::array<std::uint64_t, 2> &ctr, ResultType *r, const KeySeqType &ks)
    {
        ++std::get<0>(ctr);
        if (std::get<0>(ctr) == 0)
            ++std::get<1>(ctr);

        constexpr MCKL_INT64 w0 =
            static_cast<MCKL_INT64>(Constants::weyl::value[0]);
        constexpr MCKL_INT64 w1 =
            static_cast<MCKL_INT64>(Constants::weyl::value[1]);
        __m128i xmmw = _mm_set_epi64x(w1, w0);

        auto &&key = ks.key();
        __m128i xmmk =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(key)),
                static_cast<MCKL_INT64>(std::get<0>(key)));

        __m128i xmm0 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));

        xmm0 = _mm_xor_si128(xmm0, xmmk);        // Round 0
        xmmk = _mm_add_epi64(xmmk, xmmw);        // Round 1
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);     // Round 1
        xmmk = _mm_add_epi64(xmmk, xmmw);        // Round 2
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);     // Round 2
        xmmk = _mm_add_epi64(xmmk, xmmw);        // Round 3
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);     // Round 3
        xmmk = _mm_add_epi64(xmmk, xmmw);        // Round 4
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);     // Round 4
        xmmk = _mm_add_epi64(xmmk, xmmw);        // Round 5
        xmm0 = _mm_aesenclast_si128(xmm0, xmmk); // Round 5

        _mm_storeu_si128(reinterpret_cast<__m128i *>(r), xmm0);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 2> &ctr, std::size_t n,
        ResultType *r, const KeySeqType &ks)
    {
        if (ctr.front() >= std::numeric_limits<std::uint64_t>::max() - n) {
            MCKL_NOINLINE_CALL eval_overflow(ctr, n, r, ks);
            return;
        }

        constexpr std::size_t N = 8;
        constexpr std::size_t R = sizeof(__m128i) / sizeof(ResultType);

        constexpr MCKL_INT64 w0 =
            static_cast<MCKL_INT64>(Constants::weyl::value[0]);
        constexpr MCKL_INT64 w1 =
            static_cast<MCKL_INT64>(Constants::weyl::value[1]);
        __m128i xmmw = _mm_set_epi64x(w1, w0);

        auto &&key = ks.key();
        __m128i xmmk0 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(key)),
                static_cast<MCKL_INT64>(std::get<0>(key)));
        __m128i xmmk1 = _mm_add_epi64(xmmk0, xmmw);
        __m128i xmmk2 = _mm_add_epi64(xmmk1, xmmw);
        __m128i xmmk3 = _mm_add_epi64(xmmk2, xmmw);
        __m128i xmmk4 = _mm_add_epi64(xmmk3, xmmw);
        __m128i xmmk5 = _mm_add_epi64(xmmk4, xmmw);

        __m128i xmmc =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));

        while (n >= N) {
            __m128i xmm0 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 1));
            __m128i xmm1 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 2));
            __m128i xmm2 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 3));
            __m128i xmm3 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 4));
            __m128i xmm4 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 5));
            __m128i xmm5 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 6));
            __m128i xmm6 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 7));
            __m128i xmm7 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 8));
            xmmc = xmm7;

            xmm0 = _mm_xor_si128(xmm0, xmmk0); // Round 0
            xmm1 = _mm_xor_si128(xmm1, xmmk0); // Round 0
            xmm2 = _mm_xor_si128(xmm2, xmmk0); // Round 0
            xmm3 = _mm_xor_si128(xmm3, xmmk0); // Round 0
            xmm4 = _mm_xor_si128(xmm4, xmmk0); // Round 0
            xmm5 = _mm_xor_si128(xmm5, xmmk0); // Round 0
            xmm6 = _mm_xor_si128(xmm6, xmmk0); // Round 0
            xmm7 = _mm_xor_si128(xmm7, xmmk0); // Round 0

            xmm0 = _mm_aesenc_si128(xmm0, xmmk1); // Round 1
            xmm1 = _mm_aesenc_si128(xmm1, xmmk1); // Round 1
            xmm2 = _mm_aesenc_si128(xmm2, xmmk1); // Round 1
            xmm3 = _mm_aesenc_si128(xmm3, xmmk1); // Round 1
            xmm4 = _mm_aesenc_si128(xmm4, xmmk1); // Round 1
            xmm5 = _mm_aesenc_si128(xmm5, xmmk1); // Round 1
            xmm6 = _mm_aesenc_si128(xmm6, xmmk1); // Round 1
            xmm7 = _mm_aesenc_si128(xmm7, xmmk1); // Round 1

            xmm0 = _mm_aesenc_si128(xmm0, xmmk2); // Round 2
            xmm1 = _mm_aesenc_si128(xmm1, xmmk2); // Round 2
            xmm2 = _mm_aesenc_si128(xmm2, xmmk2); // Round 2
            xmm3 = _mm_aesenc_si128(xmm3, xmmk2); // Round 2
            xmm4 = _mm_aesenc_si128(xmm4, xmmk2); // Round 2
            xmm5 = _mm_aesenc_si128(xmm5, xmmk2); // Round 2
            xmm6 = _mm_aesenc_si128(xmm6, xmmk2); // Round 2
            xmm7 = _mm_aesenc_si128(xmm7, xmmk2); // Round 2

            xmm0 = _mm_aesenc_si128(xmm0, xmmk3); // Round 3
            xmm1 = _mm_aesenc_si128(xmm1, xmmk3); // Round 3
            xmm2 = _mm_aesenc_si128(xmm2, xmmk3); // Round 3
            xmm3 = _mm_aesenc_si128(xmm3, xmmk3); // Round 3
            xmm4 = _mm_aesenc_si128(xmm4, xmmk3); // Round 3
            xmm5 = _mm_aesenc_si128(xmm5, xmmk3); // Round 3
            xmm6 = _mm_aesenc_si128(xmm6, xmmk3); // Round 3
            xmm7 = _mm_aesenc_si128(xmm7, xmmk3); // Round 3

            xmm0 = _mm_aesenc_si128(xmm0, xmmk4); // Round 4
            xmm1 = _mm_aesenc_si128(xmm1, xmmk4); // Round 4
            xmm2 = _mm_aesenc_si128(xmm2, xmmk4); // Round 4
            xmm3 = _mm_aesenc_si128(xmm3, xmmk4); // Round 4
            xmm4 = _mm_aesenc_si128(xmm4, xmmk4); // Round 4
            xmm5 = _mm_aesenc_si128(xmm5, xmmk4); // Round 4
            xmm6 = _mm_aesenc_si128(xmm6, xmmk4); // Round 4
            xmm7 = _mm_aesenc_si128(xmm7, xmmk4); // Round 4

            xmm0 = _mm_aesenclast_si128(xmm0, xmmk5); // Round 5
            xmm1 = _mm_aesenclast_si128(xmm1, xmmk5); // Round 5
            xmm2 = _mm_aesenclast_si128(xmm2, xmmk5); // Round 5
            xmm3 = _mm_aesenclast_si128(xmm3, xmmk5); // Round 5
            xmm4 = _mm_aesenclast_si128(xmm4, xmmk5); // Round 5
            xmm5 = _mm_aesenclast_si128(xmm5, xmmk5); // Round 5
            xmm6 = _mm_aesenclast_si128(xmm6, xmmk5); // Round 5
            xmm7 = _mm_aesenclast_si128(xmm7, xmmk5); // Round 5

            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + R * 0), xmm0);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + R * 1), xmm1);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + R * 2), xmm2);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + R * 3), xmm3);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + R * 4), xmm4);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + R * 5), xmm5);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + R * 6), xmm6);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + R * 7), xmm7);

            n -= N;
            r += N * R;
        }

        for (std::size_t i = 0; i != n; ++i, r += R) {
            __m128i xmm0 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 1));
            xmmc = xmm0;

            xmm0 = _mm_xor_si128(xmm0, xmmk0);        // Round 0
            xmm0 = _mm_aesenc_si128(xmm0, xmmk1);     // Round 1
            xmm0 = _mm_aesenc_si128(xmm0, xmmk2);     // Round 2
            xmm0 = _mm_aesenc_si128(xmm0, xmmk3);     // Round 3
            xmm0 = _mm_aesenc_si128(xmm0, xmmk4);     // Round 4
            xmm0 = _mm_aesenclast_si128(xmm0, xmmk5); // Round 5

            _mm_storeu_si128(reinterpret_cast<__m128i *>(r), xmm0);
        }

        _mm_storeu_si128(reinterpret_cast<__m128i *>(ctr.data()), xmmc);
    }

    private:
    template <typename ResultType>
    MCKL_NOINLINE static void eval_overflow(std::array<std::uint64_t, 2> &ctr,
        std::size_t n, ResultType *r, const KeySeqType &ks)
    {
        constexpr std::size_t R = sizeof(__m128i) / sizeof(ResultType);

        for (std::size_t i = 0; i != n; ++i, r += R)
            eval(ctr, r, ks);
    }
}; // class AESGeneratorAESNIImplARS

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_AES_AESNI_ARS_HPP
