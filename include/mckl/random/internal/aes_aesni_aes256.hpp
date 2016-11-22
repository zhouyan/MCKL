//============================================================================
// MCKL/include/mckl/random/internal/aes_aesni_aes256.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_AES_AESNI_AES256_HPP
#define MCKL_RANDOM_INTERNAL_AES_AESNI_AES256_HPP

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

namespace mckl
{

namespace internal
{

class AES256KeySeqGeneratorAESNIImpl
{
    public:
    using key_type = std::array<std::uint32_t, 8>;
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
        xmm1_ = _mm_set_epi32(static_cast<int>(std::get<3>(key)),
            static_cast<int>(std::get<2>(key)),
            static_cast<int>(std::get<1>(key)),
            static_cast<int>(std::get<0>(key)));
        xmm3_ = _mm_set_epi32(static_cast<int>(std::get<7>(key)),
            static_cast<int>(std::get<6>(key)),
            static_cast<int>(std::get<5>(key)),
            static_cast<int>(std::get<4>(key)));
        std::get<0>(rk) = xmm1_;
        std::get<1>(rk) = xmm3_;
        generate<2>(rk, std::integral_constant<bool, 2 < Rp1>());
    }

    private:
    __m128i xmm1_;
    __m128i xmm2_;
    __m128i xmm3_;
    __m128i xmm4_;

    template <std::size_t, std::size_t Rp1>
    void generate(std::array<__m128i, Rp1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate(std::array<__m128i, Rp1> &rk, std::true_type)
    {
        generate_key<N>(rk, std::integral_constant<bool, N % 2 == 0>());
        generate<N + 1>(rk, std::integral_constant<bool, N + 1 < Rp1>());
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_key(std::array<__m128i, Rp1> &rk, std::true_type)
    {
        xmm2_ = aeskeygenassist_si128<(N / 2) % 256>(xmm3_);
        expand_key(std::true_type());
        std::get<N>(rk) = xmm1_;
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_key(std::array<__m128i, Rp1> &rk, std::false_type)
    {
        xmm4_ = aeskeygenassist_si128<0>(xmm1_);
        expand_key(std::false_type());
        std::get<N>(rk) = xmm3_;
    }

    void expand_key(std::true_type)
    {
        xmm2_ = _mm_shuffle_epi32(xmm2_, 0xFF);
        xmm4_ = _mm_slli_si128(xmm1_, 4);
        xmm1_ = _mm_xor_si128(xmm1_, xmm4_);
        xmm4_ = _mm_slli_si128(xmm4_, 4);
        xmm1_ = _mm_xor_si128(xmm1_, xmm4_);
        xmm4_ = _mm_slli_si128(xmm4_, 4);
        xmm1_ = _mm_xor_si128(xmm1_, xmm4_);
        xmm1_ = _mm_xor_si128(xmm1_, xmm2_);
    }

    void expand_key(std::false_type)
    {
        xmm2_ = _mm_shuffle_epi32(xmm4_, 0xAA);
        xmm4_ = _mm_slli_si128(xmm3_, 4);
        xmm3_ = _mm_xor_si128(xmm3_, xmm4_);
        xmm4_ = _mm_slli_si128(xmm4_, 4);
        xmm3_ = _mm_xor_si128(xmm3_, xmm4_);
        xmm4_ = _mm_slli_si128(xmm4_, 4);
        xmm3_ = _mm_xor_si128(xmm3_, xmm4_);
        xmm3_ = _mm_xor_si128(xmm3_, xmm2_);
    }
}; // class AES256KeySeqGeneratorAESNIImpl

class AES256GeneratorAESNIImpl
{
    using KeySeqType = AESKeySeqImpl<14, AES256KeySeqGeneratorAESNIImpl>;

    public:
    static void eval(const void *plain, void *cipher, const KeySeqType &ks)
    {
        __m128i xmm0 =
            _mm_loadu_si128(reinterpret_cast<const __m128i *>(plain));

        xmm0 = _mm_xor_si128(xmm0, ks.get<0x0>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x1>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x2>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x3>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x4>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x5>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x6>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x7>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x8>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x9>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xA>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xB>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xC>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xD>());
        xmm0 = _mm_aesenclast_si128(xmm0, ks.get<0xE>());

        _mm_storeu_si128(reinterpret_cast<__m128i *>(cipher), xmm0);
    }

    template <typename ResultType>
    static void eval(
        std::array<std::uint64_t, 2> &ctr, ResultType *r, const KeySeqType &ks)
    {
        MCKL_INLINE_CALL increment(ctr);

        __m128i xmm0 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));

        xmm0 = _mm_xor_si128(xmm0, ks.get<0x0>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x1>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x2>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x3>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x4>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x5>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x6>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x7>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x8>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x9>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xA>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xB>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xC>());
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xD>());
        xmm0 = _mm_aesenclast_si128(xmm0, ks.get<0xE>());

        _mm_storeu_si128(reinterpret_cast<__m128i *>(r), xmm0);
    }

    MCKL_RANDOM_INTERNAL_AES_AESNI_EVAL

    private:
    template <typename ResultType>
    static void eval_kernel(std::array<std::uint64_t, 2> &ctr, std::size_t n,
        ResultType *r, const KeySeqType &ks)
    {
#if MCKL_USE_EXTERN_LIBRARY
#if MCKL_USE_AVX2
        mckl_aes256_aesni_avx2_kernel(ctr.data(), n, r, ks.get().data());
#else
        mckl_aes256_aesni_sse2_kernel(ctr.data(), n, r, ks.get().data());
#endif
#else  // MCKL_USE_EXTERN_LIBRARY
        constexpr std::size_t S = 8;
        constexpr std::size_t N = S;

        const std::array<__m128i, KeySeqType::rounds() + 1> rk(ks.get());

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

            MCKL_RANDOM_INTERNAL_AES_AESNI_ENCFIRST(std::get<0x0>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0x1>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0x2>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0x3>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0x4>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0x5>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0x6>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0x7>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0x8>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0x9>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0xA>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0xB>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0xC>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(std::get<0xD>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_ENCLAST(std::get<0xE>(rk))
            MCKL_RANDOM_INTERNAL_AES_AESNI_STORE(n, N, rptr);
        }
#endif // MCKL_USE_EXTERN_LIBRARY
    }
}; // class AES256GeneratorAESNIImpl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_AES_AESNI_AES256_HPP
