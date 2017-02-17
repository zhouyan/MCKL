//============================================================================
// MCKL/include/mckl/random/internal/aes_aesni.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_AES_AESNI_HPP
#define MCKL_RANDOM_INTERNAL_AES_AESNI_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/aes_aesni_aes128.hpp>
#include <mckl/random/internal/aes_aesni_aes192.hpp>
#include <mckl/random/internal/aes_aesni_aes256.hpp>
#include <mckl/random/internal/aes_aesni_ars.hpp>
#include <mckl/random/internal/aes_key_seq.hpp>
#include <mckl/random/internal/aes_unroll.hpp>
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

template <typename KeySeqType>
class AESGeneratorAESNIImpl
{
    public:
    static void eval(const void *plain, void *cipher, const KeySeqType &ks)
    {
        const std::array<__m128i, rounds_ + 1> rk(ks.get());

        __m128i s = _mm_loadu_si128(reinterpret_cast<const __m128i *>(plain));
        s = _mm_xor_si128(s, std::get<0>(rk));
        MCKL_RANDOM_INTERNAL_AES_UNROLL(0, s, rk);
        s = _mm_aesenclast_si128(s, std::get<rounds_>(rk));
        _mm_storeu_si128(reinterpret_cast<__m128i *>(cipher), s);
    }

    template <typename ResultType>
    static void eval(
        Counter<std::uint32_t, 4> &ctr, ResultType *r, const KeySeqType &ks)
    {
        const std::array<__m128i, rounds_ + 1> rk(ks.get());

        MCKL_INLINE_CALL increment(ctr);
        __m128i s =
            _mm_loadu_si128(reinterpret_cast<const __m128i *>(ctr.data()));
        s = _mm_xor_si128(s, std::get<0>(rk));
        MCKL_RANDOM_INTERNAL_AES_UNROLL(0, s, rk);
        s = _mm_aesenclast_si128(s, std::get<rounds_>(rk));
        _mm_storeu_si128(reinterpret_cast<__m128i *>(r), s);
    }

    template <typename ResultType>
    static void eval(Counter<std::uint32_t, 4> &ctr, std::size_t n,
        ResultType *r, const KeySeqType &ks)
    {
        constexpr std::size_t S = 8;
        constexpr std::size_t N = S;
        constexpr std::size_t R = sizeof(__m128i) / sizeof(ResultType);

        const std::array<__m128i, rounds_ + 1> rk(ks.get());
        while (n >= N) {
            std::array<__m128i, S> s;
            MCKL_INLINE_CALL increment_si128(ctr, s);
            MCKL_INLINE_CALL xor_si128(s, std::get<0>(rk));
            MCKL_RANDOM_INTERNAL_AES_UNROLL_ROUND(0, s, rk);
            MCKL_INLINE_CALL aesenclast_si128(s, std::get<rounds_>(rk));
            std::memcpy(r, s.data(), sizeof(__m128i) * N);
            n -= N;
            r += N * R;
        }

        alignas(32) std::array<ResultType, N * R> t;
        for (std::size_t i = 0; i != n; ++i) {
            MCKL_INLINE_CALL increment(ctr);
            __m128i s =
                _mm_loadu_si128(reinterpret_cast<const __m128i *>(ctr.data()));
            s = _mm_xor_si128(s, std::get<0>(rk));
            MCKL_RANDOM_INTERNAL_AES_UNROLL(0, s, rk);
            s = _mm_aesenclast_si128(s, std::get<rounds_>(rk));
            _mm_store_si128(reinterpret_cast<__m128i *>(t.data() + i * R), s);
        }
        std::memcpy(r, t.data(), sizeof(__m128i) * n);
    }

    private:
    static constexpr std::size_t rounds_ = KeySeqType::rounds();

    template <std::size_t>
    static void round(
        __m128i &, const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void round(
        __m128i &s, const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        MCKL_RANDOM_INTERNAL_AES_UNROLL_ROUND(N, s, rk);
    }

    template <std::size_t, std::size_t S>
    static void round(std::array<__m128i, S> &,
        const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t S>
    static void round(std::array<__m128i, S> &s,
        const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        MCKL_RANDOM_INTERNAL_AES_UNROLL_ROUND(N, s, rk);
    }

    template <std::size_t N>
    MCKL_INLINE static void rbox(
        __m128i &s, const std::array<__m128i, rounds_ + 1> &rk)
    {
        rbox<N>(s, rk, std::integral_constant<bool, (N > 0 && N < rounds_)>());
    }

    template <std::size_t>
    static void rbox(
        __m128i &, const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void rbox(
        __m128i &s, const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        s = _mm_aesenc_si128(s, std::get<N>(rk));
    }

    template <std::size_t N, std::size_t S>
    MCKL_INLINE static void rbox(
        std::array<__m128i, S> &s, const std::array<__m128i, rounds_ + 1> &rk)
    {
        rbox<N>(s, rk, std::integral_constant<bool, (N > 0 && N < rounds_)>());
    }

    template <std::size_t, std::size_t S>
    static void rbox(std::array<__m128i, S> &,
        const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t S>
    static void rbox(std::array<__m128i, S> &s,
        const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        aesenc_si128(s, std::get<N>(rk));
    }
}; // class AESGeneratorAESNIImpl

template <>
class AESGeneratorAESNIImpl<AESKeySeqImpl<10, AES128KeySeqGeneratorAESNIImpl>>
    : public AES128GeneratorAESNIImpl
{
}; // class AESGeneratorAESNIImpl

template <>
class AESGeneratorAESNIImpl<AESKeySeqImpl<12, AES192KeySeqGeneratorAESNIImpl>>
    : public AES192GeneratorAESNIImpl
{
}; // class AESGeneratorAESNIImpl

template <>
class AESGeneratorAESNIImpl<AESKeySeqImpl<14, AES256KeySeqGeneratorAESNIImpl>>
    : public AES256GeneratorAESNIImpl
{
}; // class AESGeneratorAESNIImpl

template <typename Constants>
class AESGeneratorAESNIImpl<
    ARSKeySeqImpl<5, ARSKeySeqGeneratorAESNIImpl<Constants>>>
    : public ARSGeneratorAESNIImpl<Constants>
{
}; // class AESGeneratorAESNIImpl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_AES_AESNI_HPP
