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
#include <mckl/random/internal/aes_aesni_common.hpp>
#include <mckl/random/internal/aes_key_seq.hpp>
#include <mckl/random/increment.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

extern "C" {

void mckl_ars_aesni_sse2_kernel(void *, std::size_t, void *, const void *);
void mckl_ars_aesni_avx2_kernel(void *, std::size_t, void *, const void *);

} // extern "C"

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
class ARSGeneratorAESNIImpl
{
    using KeySeqType =
        ARSKeySeqImpl<5, ARSKeySeqGeneratorAESNIImpl<Constants>>;

    public:
    static void eval(const void *plain, void *cipher, const KeySeqType &ks)
    {
        constexpr MCKL_INT64 w0 =
            static_cast<MCKL_INT64>(Constants::weyl::value[0]);
        constexpr MCKL_INT64 w1 =
            static_cast<MCKL_INT64>(Constants::weyl::value[1]);
        const __m128i xmmw = _mm_set_epi64x(w1, w0);

        auto &&key = ks.key();
        __m128i xmmk =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(key)),
                static_cast<MCKL_INT64>(std::get<0>(key)));

        __m128i xmm0 =
            _mm_loadu_si128(reinterpret_cast<const __m128i *>(plain));

        xmm0 = _mm_xor_si128(xmm0, xmmk);
        xmmk = _mm_add_epi64(xmmk, xmmw);
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);
        xmmk = _mm_add_epi64(xmmk, xmmw);
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);
        xmmk = _mm_add_epi64(xmmk, xmmw);
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);
        xmmk = _mm_add_epi64(xmmk, xmmw);
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);
        xmmk = _mm_add_epi64(xmmk, xmmw);
        xmm0 = _mm_aesenclast_si128(xmm0, xmmk);

        _mm_storeu_si128(reinterpret_cast<__m128i *>(cipher), xmm0);
    }

    template <typename ResultType>
    static void eval(
        std::array<std::uint64_t, 2> &ctr, ResultType *r, const KeySeqType &ks)
    {
        MCKL_INLINE_CALL increment(ctr);

        constexpr MCKL_INT64 w0 =
            static_cast<MCKL_INT64>(Constants::weyl::value[0]);
        constexpr MCKL_INT64 w1 =
            static_cast<MCKL_INT64>(Constants::weyl::value[1]);
        const __m128i xmmw = _mm_set_epi64x(w1, w0);

        auto &&key = ks.key();
        __m128i xmmk =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(key)),
                static_cast<MCKL_INT64>(std::get<0>(key)));

        __m128i xmm0 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));

        xmm0 = _mm_xor_si128(xmm0, xmmk);
        xmmk = _mm_add_epi64(xmmk, xmmw);
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);
        xmmk = _mm_add_epi64(xmmk, xmmw);
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);
        xmmk = _mm_add_epi64(xmmk, xmmw);
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);
        xmmk = _mm_add_epi64(xmmk, xmmw);
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);
        xmmk = _mm_add_epi64(xmmk, xmmw);
        xmm0 = _mm_aesenclast_si128(xmm0, xmmk);

        _mm_storeu_si128(reinterpret_cast<__m128i *>(r), xmm0);
    }

    MCKL_RANDOM_INTERNAL_AES_AESNI_EVAL

    private:
    template <typename ResultType>
    static void eval_kernel(std::array<std::uint64_t, 2> &ctr, std::size_t n,
        ResultType *r, const KeySeqType &ks)
    {
#if MCKL_USE_EXTERN_LIBRARY
        constexpr std::uint64_t w0 = Constants::weyl::value[0];
        constexpr std::uint64_t w1 = Constants::weyl::value[1];

        auto &&key = ks.key();

        const std::uint64_t wk[4] = {w0, w1,
            static_cast<std::uint64_t>(std::get<0>(key)) +
                (static_cast<std::uint64_t>(std::get<1>(key)) << 32),
            static_cast<std::uint64_t>(std::get<2>(key)) +
                (static_cast<std::uint64_t>(std::get<3>(key)) << 32)};
#if MCKL_USE_AVX2
        mckl_ars_aesni_avx2_kernel(ctr.data(), n, r, wk);
#else
        mckl_ars_aesni_sse2_kernel(ctr.data(), n, r, wk);
#endif
#else  // MCKL_USE_EXTERN_LIBRARY
        constexpr std::size_t S = 8;
        constexpr std::size_t N = S;

        constexpr MCKL_INT64 w0 =
            static_cast<MCKL_INT64>(Constants::weyl::value[0]);
        constexpr MCKL_INT64 w1 =
            static_cast<MCKL_INT64>(Constants::weyl::value[1]);
        const __m128i xmmw = _mm_set_epi64x(w1, w0);

        auto &&key = ks.key();
        const __m128i xmmk0 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(key)),
                static_cast<MCKL_INT64>(std::get<0>(key)));
        const __m128i xmmk1 = _mm_add_epi64(xmmk0, xmmw);
        const __m128i xmmk2 = _mm_add_epi64(xmmk1, xmmw);
        const __m128i xmmk3 = _mm_add_epi64(xmmk2, xmmw);
        const __m128i xmmk4 = _mm_add_epi64(xmmk3, xmmw);
        const __m128i xmmk5 = _mm_add_epi64(xmmk4, xmmw);

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

            MCKL_RANDOM_INTERNAL_AES_AESNI_ENCFIRST(xmmk0)
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(xmmk1)
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(xmmk2)
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(xmmk3)
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(xmmk4)
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENCLAST(xmmk5)
            MCKL_RANDOM_INTERNAL_AES_AESNI_STORE(n, N, rptr)
        }
#endif // MCKL_USE_EXTERN_LIBRARY
    }
}; // class ARSGeneratorAESNIImpl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_AES_AESNI_ARS_HPP
