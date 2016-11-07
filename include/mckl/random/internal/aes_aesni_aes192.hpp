//============================================================================
// MCKL/include/mckl/random/internal/aes_aesni_aes192.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_AES_AESNI_AES192_HPP
#define MCKL_RANDOM_INTERNAL_AES_AESNI_AES192_HPP

#include <mckl/random/internal/common.hpp>
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

class AES192KeySeqGeneratorAESNIImpl
{
    public:
    using key_type = std::array<std::uint32_t, 6>;
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
        xmm7_ = _mm_set_epi32(0, 0, static_cast<int>(std::get<5>(key)),
            static_cast<int>(std::get<4>(key)));
        std::get<0>(rk) = xmm1_;
        std::get<1>(rk) = xmm7_;

        xmm3_ = _mm_setzero_si128();
        xmm6_ = _mm_setzero_si128();
        xmm4_ = _mm_shuffle_epi32(xmm7_, 0x4F); // pshufd xmm4, xmm7, 0x4F

        std::array<char, Rp1 * 16 + 16> rk_tmp;
        generate<1, Rp1>(
            rk_tmp.data(), std::integral_constant<bool, 24 < Rp1 * 16>());
        copy_key(
            rk, rk_tmp.data(), std::integral_constant<bool, 24 < Rp1 * 16>());
    }

    private:
    __m128i xmm1_;
    __m128i xmm2_;
    __m128i xmm3_;
    __m128i xmm4_;
    __m128i xmm5_;
    __m128i xmm6_;
    __m128i xmm7_;

    template <std::size_t, std::size_t>
    void generate(char *, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate(char *rk_ptr, std::true_type)
    {
        generate_key<N>(rk_ptr);
        complete_key<N>(
            rk_ptr, std::integral_constant<bool, N * 24 + 16 < Rp1 * 16>());
        generate<N + 1, Rp1>(
            rk_ptr, std::integral_constant<bool, N * 24 + 24 < Rp1 * 16>());
    }

    template <std::size_t N>
    void generate_key(char *rk_ptr)
    {
        // In entry, N * 24 < Rp1 * 16
        // Required Storage: N * 24 + 16;

        xmm2_ = aeskeygenassist_si128<N % 256>(xmm4_);
        generate_key_expansion();
        _mm_storeu_si128(reinterpret_cast<__m128i *>(rk_ptr + N * 24), xmm1_);
    }

    template <std::size_t>
    void complete_key(char *, std::false_type)
    {
    }

    template <std::size_t N>
    void complete_key(char *rk_ptr, std::true_type)
    {
        // In entry, N * 24 + 16 < Rp1 * 16
        // Required storage: N * 24 + 32

        complete_key_expansion();
        _mm_storeu_si128(
            reinterpret_cast<__m128i *>(rk_ptr + N * 24 + 16), xmm7_);
    }

    void generate_key_expansion()
    {
        xmm2_ = _mm_shuffle_epi32(xmm2_, 0xFF); // pshufd xmm2, xmm2, 0xFF
        xmm3_ = _mm_castps_si128(               // shufps xmm3, xmm1, 0x10
            _mm_shuffle_ps(
                _mm_castsi128_ps(xmm3_), _mm_castsi128_ps(xmm1_), 0x10));
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_);     // pxor   xmm1, xmm3
        xmm3_ = _mm_castps_si128(_mm_shuffle_ps( // shufps xmm3, xmm1, 0x10
            _mm_castsi128_ps(xmm3_), _mm_castsi128_ps(xmm1_), 0x8C));
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_); // pxor   xmm1, xmm3
        xmm1_ = _mm_xor_si128(xmm1_, xmm2_); // pxor   xmm1, xmm2
    }

    void complete_key_expansion()
    {
        xmm5_ = xmm4_;                           // movdqa xmm5, xmm4
        xmm5_ = _mm_slli_si128(xmm5_, 0x04);     // pslldq xmm5, 0x04
        xmm6_ = _mm_castps_si128(_mm_shuffle_ps( // shufps xmm6, xmm1, 0x10
            _mm_castsi128_ps(xmm6_), _mm_castsi128_ps(xmm1_), 0xF0));
        xmm6_ = _mm_xor_si128(xmm6_, xmm5_);    // pxor   xmm6, xmm5
        xmm4_ = _mm_xor_si128(xmm4_, xmm6_);    // pxor   xmm4, xmm6
        xmm7_ = _mm_shuffle_epi32(xmm4_, 0x0E); // pshufd xmm7, xmm4, 0x0E
    }

    template <std::size_t Rp1>
    void copy_key(std::array<__m128i, Rp1> &, const char *, std::false_type)
    {
    }

    template <std::size_t Rp1>
    void copy_key(
        std::array<__m128i, Rp1> &rk, const char *rk_ptr, std::true_type)
    {
        char *dst = reinterpret_cast<char *>(rk.data());
        std::memcpy(dst + 24, rk_ptr + 24, Rp1 * 16 - 24);
    }
}; // class AES192KeySeqGeneratorAESNIImpl

using AES192KeySeqAESNI = AESKeySeqImpl<12, AES192KeySeqGeneratorAESNIImpl>;

class AESGeneratorAESNIImplAES192
{
    using KeySeqType = AES192KeySeqAESNI;

    public:
    static void eval(const void *plain, void *cipher, const KeySeqType &ks)
    {
        __m128i xmm0 =
            _mm_loadu_si128(reinterpret_cast<const __m128i *>(plain));

        xmm0 = _mm_xor_si128(xmm0, ks.get<0x0>());        // Round 0x0
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x1>());     // Round 0x1
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x2>());     // Round 0x2
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x3>());     // Round 0x3
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x4>());     // Round 0x4
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x5>());     // Round 0x5
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x6>());     // Round 0x6
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x7>());     // Round 0x7
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x8>());     // Round 0x8
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x9>());     // Round 0x9
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xA>());     // Round 0xA
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xB>());     // Round 0xB
        xmm0 = _mm_aesenclast_si128(xmm0, ks.get<0xC>()); // Round 0xC

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

        xmm0 = _mm_xor_si128(xmm0, ks.get<0x0>());        // Round 0x0
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x1>());     // Round 0x1
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x2>());     // Round 0x2
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x3>());     // Round 0x3
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x4>());     // Round 0x4
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x5>());     // Round 0x5
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x6>());     // Round 0x6
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x7>());     // Round 0x7
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x8>());     // Round 0x8
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x9>());     // Round 0x9
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xA>());     // Round 0xA
        xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xB>());     // Round 0xB
        xmm0 = _mm_aesenclast_si128(xmm0, ks.get<0xC>()); // Round 0xC

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

        __m128i xmmk0 = ks.get<0x0>();
        __m128i xmmk1 = ks.get<0x1>();
        __m128i xmmk2 = ks.get<0x2>();
        __m128i xmmk3 = ks.get<0x3>();
        __m128i xmmk4 = ks.get<0x4>();
        __m128i xmmk5 = ks.get<0x5>();
        __m128i xmmk;

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

            xmm0 = _mm_xor_si128(xmm0, xmmk0); // Round 0x0
            xmm1 = _mm_xor_si128(xmm1, xmmk0); // Round 0x0
            xmm2 = _mm_xor_si128(xmm2, xmmk0); // Round 0x0
            xmm3 = _mm_xor_si128(xmm3, xmmk0); // Round 0x0
            xmm4 = _mm_xor_si128(xmm4, xmmk0); // Round 0x0
            xmm5 = _mm_xor_si128(xmm5, xmmk0); // Round 0x0
            xmm6 = _mm_xor_si128(xmm6, xmmk0); // Round 0x0
            xmm7 = _mm_xor_si128(xmm7, xmmk0); // Round 0x0

            xmm0 = _mm_aesenc_si128(xmm0, xmmk1); // Round 0x1
            xmm1 = _mm_aesenc_si128(xmm1, xmmk1); // Round 0x1
            xmm2 = _mm_aesenc_si128(xmm2, xmmk1); // Round 0x1
            xmm3 = _mm_aesenc_si128(xmm3, xmmk1); // Round 0x1
            xmm4 = _mm_aesenc_si128(xmm4, xmmk1); // Round 0x1
            xmm5 = _mm_aesenc_si128(xmm5, xmmk1); // Round 0x1
            xmm6 = _mm_aesenc_si128(xmm6, xmmk1); // Round 0x1
            xmm7 = _mm_aesenc_si128(xmm7, xmmk1); // Round 0x1

            xmm0 = _mm_aesenc_si128(xmm0, xmmk2); // Round 0x2
            xmm1 = _mm_aesenc_si128(xmm1, xmmk2); // Round 0x2
            xmm2 = _mm_aesenc_si128(xmm2, xmmk2); // Round 0x2
            xmm3 = _mm_aesenc_si128(xmm3, xmmk2); // Round 0x2
            xmm4 = _mm_aesenc_si128(xmm4, xmmk2); // Round 0x2
            xmm5 = _mm_aesenc_si128(xmm5, xmmk2); // Round 0x2
            xmm6 = _mm_aesenc_si128(xmm6, xmmk2); // Round 0x2
            xmm7 = _mm_aesenc_si128(xmm7, xmmk2); // Round 0x2

            xmm0 = _mm_aesenc_si128(xmm0, xmmk3); // Round 0x3
            xmm1 = _mm_aesenc_si128(xmm1, xmmk3); // Round 0x3
            xmm2 = _mm_aesenc_si128(xmm2, xmmk3); // Round 0x3
            xmm3 = _mm_aesenc_si128(xmm3, xmmk3); // Round 0x3
            xmm4 = _mm_aesenc_si128(xmm4, xmmk3); // Round 0x3
            xmm5 = _mm_aesenc_si128(xmm5, xmmk3); // Round 0x3
            xmm6 = _mm_aesenc_si128(xmm6, xmmk3); // Round 0x3
            xmm7 = _mm_aesenc_si128(xmm7, xmmk3); // Round 0x3

            xmm0 = _mm_aesenc_si128(xmm0, xmmk4); // Round 0x4
            xmm1 = _mm_aesenc_si128(xmm1, xmmk4); // Round 0x4
            xmm2 = _mm_aesenc_si128(xmm2, xmmk4); // Round 0x4
            xmm3 = _mm_aesenc_si128(xmm3, xmmk4); // Round 0x4
            xmm4 = _mm_aesenc_si128(xmm4, xmmk4); // Round 0x4
            xmm5 = _mm_aesenc_si128(xmm5, xmmk4); // Round 0x4
            xmm6 = _mm_aesenc_si128(xmm6, xmmk4); // Round 0x4
            xmm7 = _mm_aesenc_si128(xmm7, xmmk4); // Round 0x4

            xmm0 = _mm_aesenc_si128(xmm0, xmmk5); // Round 0x5
            xmm1 = _mm_aesenc_si128(xmm1, xmmk5); // Round 0x5
            xmm2 = _mm_aesenc_si128(xmm2, xmmk5); // Round 0x5
            xmm3 = _mm_aesenc_si128(xmm3, xmmk5); // Round 0x5
            xmm4 = _mm_aesenc_si128(xmm4, xmmk5); // Round 0x5
            xmm5 = _mm_aesenc_si128(xmm5, xmmk5); // Round 0x5
            xmm6 = _mm_aesenc_si128(xmm6, xmmk5); // Round 0x5
            xmm7 = _mm_aesenc_si128(xmm7, xmmk5); // Round 0x5

            xmmk = ks.get<0x6>();
            xmm0 = _mm_aesenc_si128(xmm0, xmmk); // Round 0x6
            xmm1 = _mm_aesenc_si128(xmm1, xmmk); // Round 0x6
            xmm2 = _mm_aesenc_si128(xmm2, xmmk); // Round 0x6
            xmm3 = _mm_aesenc_si128(xmm3, xmmk); // Round 0x6
            xmm4 = _mm_aesenc_si128(xmm4, xmmk); // Round 0x6
            xmm5 = _mm_aesenc_si128(xmm5, xmmk); // Round 0x6
            xmm6 = _mm_aesenc_si128(xmm6, xmmk); // Round 0x6
            xmm7 = _mm_aesenc_si128(xmm7, xmmk); // Round 0x6

            xmmk = ks.get<0x7>();
            xmm0 = _mm_aesenc_si128(xmm0, xmmk); // Round 0x7
            xmm1 = _mm_aesenc_si128(xmm1, xmmk); // Round 0x7
            xmm2 = _mm_aesenc_si128(xmm2, xmmk); // Round 0x7
            xmm3 = _mm_aesenc_si128(xmm3, xmmk); // Round 0x7
            xmm4 = _mm_aesenc_si128(xmm4, xmmk); // Round 0x7
            xmm5 = _mm_aesenc_si128(xmm5, xmmk); // Round 0x7
            xmm6 = _mm_aesenc_si128(xmm6, xmmk); // Round 0x7
            xmm7 = _mm_aesenc_si128(xmm7, xmmk); // Round 0x7

            xmmk = ks.get<0x8>();
            xmm0 = _mm_aesenc_si128(xmm0, xmmk); // Round 0x8
            xmm1 = _mm_aesenc_si128(xmm1, xmmk); // Round 0x8
            xmm2 = _mm_aesenc_si128(xmm2, xmmk); // Round 0x8
            xmm3 = _mm_aesenc_si128(xmm3, xmmk); // Round 0x8
            xmm4 = _mm_aesenc_si128(xmm4, xmmk); // Round 0x8
            xmm5 = _mm_aesenc_si128(xmm5, xmmk); // Round 0x8
            xmm6 = _mm_aesenc_si128(xmm6, xmmk); // Round 0x8
            xmm7 = _mm_aesenc_si128(xmm7, xmmk); // Round 0x8

            xmmk = ks.get<0x9>();
            xmm0 = _mm_aesenc_si128(xmm0, xmmk); // Round 0x9
            xmm1 = _mm_aesenc_si128(xmm1, xmmk); // Round 0x9
            xmm2 = _mm_aesenc_si128(xmm2, xmmk); // Round 0x9
            xmm3 = _mm_aesenc_si128(xmm3, xmmk); // Round 0x9
            xmm4 = _mm_aesenc_si128(xmm4, xmmk); // Round 0x9
            xmm5 = _mm_aesenc_si128(xmm5, xmmk); // Round 0x9
            xmm6 = _mm_aesenc_si128(xmm6, xmmk); // Round 0x9
            xmm7 = _mm_aesenc_si128(xmm7, xmmk); // Round 0x9

            xmmk = ks.get<0xA>();
            xmm0 = _mm_aesenc_si128(xmm0, xmmk); // Round 0xA
            xmm1 = _mm_aesenc_si128(xmm1, xmmk); // Round 0xA
            xmm2 = _mm_aesenc_si128(xmm2, xmmk); // Round 0xA
            xmm3 = _mm_aesenc_si128(xmm3, xmmk); // Round 0xA
            xmm4 = _mm_aesenc_si128(xmm4, xmmk); // Round 0xA
            xmm5 = _mm_aesenc_si128(xmm5, xmmk); // Round 0xA
            xmm6 = _mm_aesenc_si128(xmm6, xmmk); // Round 0xA
            xmm7 = _mm_aesenc_si128(xmm7, xmmk); // Round 0xA

            xmmk = ks.get<0xB>();
            xmm0 = _mm_aesenc_si128(xmm0, xmmk); // Round 0xB
            xmm1 = _mm_aesenc_si128(xmm1, xmmk); // Round 0xB
            xmm2 = _mm_aesenc_si128(xmm2, xmmk); // Round 0xB
            xmm3 = _mm_aesenc_si128(xmm3, xmmk); // Round 0xB
            xmm4 = _mm_aesenc_si128(xmm4, xmmk); // Round 0xB
            xmm5 = _mm_aesenc_si128(xmm5, xmmk); // Round 0xB
            xmm6 = _mm_aesenc_si128(xmm6, xmmk); // Round 0xB
            xmm7 = _mm_aesenc_si128(xmm7, xmmk); // Round 0xB

            xmmk = ks.get<0xC>();
            xmm0 = _mm_aesenclast_si128(xmm0, xmmk); // Round 0xC
            xmm1 = _mm_aesenclast_si128(xmm1, xmmk); // Round 0xC
            xmm2 = _mm_aesenclast_si128(xmm2, xmmk); // Round 0xC
            xmm3 = _mm_aesenclast_si128(xmm3, xmmk); // Round 0xC
            xmm4 = _mm_aesenclast_si128(xmm4, xmmk); // Round 0xC
            xmm5 = _mm_aesenclast_si128(xmm5, xmmk); // Round 0xC
            xmm6 = _mm_aesenclast_si128(xmm6, xmmk); // Round 0xC
            xmm7 = _mm_aesenclast_si128(xmm7, xmmk); // Round 0xC

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

            xmm0 = _mm_xor_si128(xmm0, xmmk0);                // Round 0x0
            xmm0 = _mm_aesenc_si128(xmm0, xmmk1);             // Round 0x1
            xmm0 = _mm_aesenc_si128(xmm0, xmmk2);             // Round 0x2
            xmm0 = _mm_aesenc_si128(xmm0, xmmk3);             // Round 0x3
            xmm0 = _mm_aesenc_si128(xmm0, xmmk4);             // Round 0x4
            xmm0 = _mm_aesenc_si128(xmm0, xmmk5);             // Round 0x5
            xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x6>());     // Round 0x6
            xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x7>());     // Round 0x7
            xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x8>());     // Round 0x8
            xmm0 = _mm_aesenc_si128(xmm0, ks.get<0x9>());     // Round 0x9
            xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xA>());     // Round 0xA
            xmm0 = _mm_aesenc_si128(xmm0, ks.get<0xB>());     // Round 0xB
            xmm0 = _mm_aesenclast_si128(xmm0, ks.get<0xC>()); // Round 0xC

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
}; // class AESGeneratorAESNIImpl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_AES_AESNI_AES192_HPP
