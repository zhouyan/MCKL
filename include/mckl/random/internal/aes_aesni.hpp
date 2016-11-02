//============================================================================
// MCKL/include/mckl/random/internal/aes_aesni.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_AES_AESNI_HPP
#define MCKL_RANDOM_INTERNAL_AES_AESNI_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/aes_unroll.hpp>
#include <mckl/random/increment.hpp>

#if MCKL_HAS_AVX2
#include <mckl/random/internal/u01_avx2.hpp>
#include <mckl/random/internal/uniform_real_avx2.hpp>
#endif

#define MCKL_DEFINE_RANDOM_INTERNAL_AES_AESNI_AVX2_U01(                       \
    lr, bits, Lower, Upper)                                                   \
    template <typename RealType>                                              \
    static void u01_##lr##_u##bits(Counter<std::uint32_t, 4> &ctr,            \
        const std::array<__m128i, KeySeqType::rounds() + 1> &rk,              \
        std::size_t n, RealType *r)                                           \
    {                                                                         \
        eval<U01AVX2Impl<std::uint##bits##_t, RealType, Lower, Upper>>(       \
            ctr, rk, n, r);                                                   \
    }

#define MCKL_DEFINE_RANDOM_INTERNAL_AES_AESNI_AVX2_UNIFORM_REAL(bits)         \
    template <typename RealType>                                              \
    static void uniform_real_u##bits(Counter<std::uint32_t, 4> &ctr,          \
        const std::array<__m128i, KeySeqType::rounds() + 1> &rk,              \
        std::size_t n, RealType *r, RealType a, RealType b)                   \
    {                                                                         \
        eval<UniformRealAVX2Impl<std::uint##bits##_t, RealType>>(             \
            ctr, rk, n, r, a, b);                                             \
    }

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

class AES128KeySeqGeneratorAESNIImpl
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
        xmm1_ = _mm_set_epi32(static_cast<int>(std::get<3>(key)),
            static_cast<int>(std::get<2>(key)),
            static_cast<int>(std::get<1>(key)),
            static_cast<int>(std::get<0>(key)));
        std::get<0>(rk) = xmm1_;
        generate<1>(rk, std::integral_constant<bool, 1 < Rp1>());
    }

    private:
    __m128i xmm1_;
    __m128i xmm2_;
    __m128i xmm3_;

    template <std::size_t, std::size_t Rp1>
    void generate(std::array<__m128i, Rp1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate(std::array<__m128i, Rp1> &rk, std::true_type)
    {
        xmm2_ = aeskeygenassist_si128<N % 256>(xmm1_);
        expand_key();
        std::get<N>(rk) = xmm1_;
        generate<N + 1>(rk, std::integral_constant<bool, N + 1 < Rp1>());
    }

    void expand_key()
    {
        xmm2_ = _mm_shuffle_epi32(xmm2_, 0xFF); // pshufd xmm2, xmm2, 0xFF
        xmm3_ = _mm_slli_si128(xmm1_, 0x04);    // pslldq xmm3, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_);    // pxor   xmm1, xmm3
        xmm3_ = _mm_slli_si128(xmm3_, 0x04);    // pslldq xmm3, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_);    // pxor   xmm1, xmm3
        xmm3_ = _mm_slli_si128(xmm3_, 0x04);    // pslldq xmm3, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_);    // pxor   xmm1, xmm3
        xmm1_ = _mm_xor_si128(xmm1_, xmm2_);    // pxor   xmm1, xmm2
    }
}; // class AES128KeySeqGeneratorAESNIImpl

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
        xmm2_ = _mm_shuffle_epi32(xmm2_, 0xFF); // pshufd xmm2, xmm2, 0xFF
        xmm4_ = _mm_slli_si128(xmm1_, 0x04);    // pslldq xmm4, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm4_);    // pxor   xmm1, xmm4
        xmm4_ = _mm_slli_si128(xmm4_, 0x04);    // pslldq xmm4, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm4_);    // pxor   xmm1, xmm4
        xmm4_ = _mm_slli_si128(xmm4_, 0x04);    // pslldq xmm4, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm4_);    // pxor   xmm1, xmm4
        xmm1_ = _mm_xor_si128(xmm1_, xmm2_);    // pxor   xmm1, xmm2
    }

    void expand_key(std::false_type)
    {
        xmm2_ = _mm_shuffle_epi32(xmm4_, 0xAA); // pshufd xmm2, xmm4, 0xAA
        xmm4_ = _mm_slli_si128(xmm3_, 0x04);    // pslldq xmm4, 0x04
        xmm3_ = _mm_xor_si128(xmm3_, xmm4_);    // pxor   xmm3, xmm4
        xmm4_ = _mm_slli_si128(xmm4_, 0x04);    // pslldq xmm4, 0x04
        xmm3_ = _mm_xor_si128(xmm3_, xmm4_);    // pxor   xmm3, xmm4
        xmm4_ = _mm_slli_si128(xmm4_, 0x04);    // pslldq xmm4, 0x04
        xmm3_ = _mm_xor_si128(xmm3_, xmm4_);    // pxor   xmm3, xmm4
        xmm3_ = _mm_xor_si128(xmm3_, xmm2_);    // pxor   xmm1, xmm2
    }
}; // class AES256KeySeqGeneratorAESNIImpl

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

template <typename KeySeqType>
class AESGeneratorAESNIImpl
{
    public:
    static constexpr bool batch() { return true; }

    static void eval(std::array<std::uint32_t, 4> &s,
        const std::array<__m128i, KeySeqType::rounds() + 1> &rk)
    {
        __m128i *const sptr = reinterpret_cast<__m128i *>(s.data());
        __m128i t = _mm_load_si128(sptr);
        eval(t, rk);
        _mm_store_si128(sptr, t);
    }

    template <typename ResultType>
    static void eval(Counter<std::uint32_t, 4> &ctr,
        const std::array<__m128i, KeySeqType::rounds() + 1> &rk, std::size_t n,
        ResultType *r)
    {
        eval<transform>(ctr, rk, n, r);
    }

#if MCKL_HAS_AVX2

    MCKL_DEFINE_RANDOM_INTERNAL_AES_AESNI_AVX2_U01(cc, 32, Closed, Closed)
    MCKL_DEFINE_RANDOM_INTERNAL_AES_AESNI_AVX2_U01(co, 32, Closed, Open)
    MCKL_DEFINE_RANDOM_INTERNAL_AES_AESNI_AVX2_U01(oc, 32, Open, Closed)
    MCKL_DEFINE_RANDOM_INTERNAL_AES_AESNI_AVX2_U01(oo, 32, Open, Open)
    MCKL_DEFINE_RANDOM_INTERNAL_AES_AESNI_AVX2_UNIFORM_REAL(32)

    MCKL_DEFINE_RANDOM_INTERNAL_AES_AESNI_AVX2_U01(cc, 64, Closed, Closed)
    MCKL_DEFINE_RANDOM_INTERNAL_AES_AESNI_AVX2_U01(co, 64, Closed, Open)
    MCKL_DEFINE_RANDOM_INTERNAL_AES_AESNI_AVX2_U01(oc, 64, Open, Closed)
    MCKL_DEFINE_RANDOM_INTERNAL_AES_AESNI_AVX2_U01(oo, 64, Open, Open)
    MCKL_DEFINE_RANDOM_INTERNAL_AES_AESNI_AVX2_UNIFORM_REAL(64)

#endif // MCKL_HAS_AVX2

    private:
    static constexpr std::size_t rounds_ = KeySeqType::rounds();

    class transform
    {
        public:
        using uint_type = std::uint32_t;

        template <std::size_t S, typename ResultType>
        MCKL_FLATTEN static ResultType *eval(
            const std::array<__m128i, S> &s, ResultType *r)
        {
            std::memcpy(r, s.data(), sizeof(s));

            return r + sizeof(s) / sizeof(ResultType);
        }

        template <typename ResultType>
        MCKL_FLATTEN static ResultType *eval(
            std::size_t n, const uint_type *s, ResultType *r)
        {
            std::memcpy(r, s, sizeof(uint_type) * n);

            return r + sizeof(uint_type) * n / sizeof(ResultType);
        }
    }; // class transform

    static void eval(
        __m128i &s, const std::array<__m128i, KeySeqType::rounds() + 1> &rk)
    {
        s = _mm_xor_si128(s, std::get<0>(rk));
        MCKL_RANDOM_INTERNAL_AES_UNROLL(0);
        s = _mm_aesenclast_si128(s, std::get<rounds_>(rk));
    }

    template <typename Transform, typename ResultType, typename... Args>
    static void eval(Counter<std::uint32_t, 4> &ctr,
        const std::array<__m128i, KeySeqType::rounds() + 1> &rk, std::size_t n,
        ResultType *r, Args &&... args)
    {
        using uint_type = typename Transform::uint_type;

        constexpr std::size_t S = rounds_ < 8 ? 16 : 8;
        constexpr std::size_t nstride = S;
        constexpr std::size_t ustride = sizeof(__m128i) / sizeof(uint_type);

        std::array<__m128i, S> s;
        while (n >= nstride) {
            MCKL_FLATTEN_CALL increment_si128(ctr, s);
            MCKL_FLATTEN_CALL xor_si128(s, std::get<0>(rk));
            MCKL_RANDOM_INTERNAL_AES_UNROLL_ROUND(0);
            MCKL_FLATTEN_CALL aesenclast_si128(s, std::get<rounds_>(rk));
            MCKL_FLATTEN_CALL r =
                Transform::eval(s, r, std::forward<Args>(args)...);
            n -= nstride;
        }

        alignas(32) union {
            std::array<std::array<std::uint32_t, 4>, nstride> s;
            std::array<Counter<std::uint32_t, 4>, nstride> c;
            std::array<uint_type, nstride * ustride> u;
        } buf;
        for (std::size_t i = 0; i != n; ++i) {
            MCKL_FLATTEN_CALL increment(ctr);
            buf.c[i] = ctr;
            eval(buf.s[i], rk);
        }
        MCKL_FLATTEN_CALL Transform::eval(
            n * ustride, buf.u.data(), r, std::forward<Args>(args)...);
    }

    template <std::size_t>
    static void round(
        __m128i &, const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void round(
        __m128i &s, const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        MCKL_RANDOM_INTERNAL_AES_UNROLL_ROUND(N);
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
        MCKL_RANDOM_INTERNAL_AES_UNROLL_ROUND(N);
    }

    template <std::size_t N>
    static void rbox(__m128i &s, const std::array<__m128i, rounds_ + 1> &rk)
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
    static void rbox(
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

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_AES_AESNI_HPP
