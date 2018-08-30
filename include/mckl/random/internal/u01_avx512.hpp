//============================================================================
// MCKL/include/mckl/random/internal/u01_avx512.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_U01_AVX512_HPP
#define MCKL_RANDOM_INTERNAL_U01_AVX512_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/u01_generic.hpp>

MCKL_PUSH_GCC_WARNING("-Wignored-attributes")

#define MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512(u)                        \
    const __m512i *uptr = reinterpret_cast<const __m512i *>(u);               \
    __m512i zmm0 = _mm512_loadu_si512(uptr++);                                \
    __m512i zmm1 = _mm512_loadu_si512(uptr++);                                \
    __m512i zmm2 = _mm512_loadu_si512(uptr++);                                \
    __m512i zmm3 = _mm512_loadu_si512(uptr++);

#define MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512_CVTEPU32_EPI64(u)         \
    const __m256i *uptr = reinterpret_cast<const __m256i *>(u);               \
    __m512i zmm0 = _mm512_cvtepu32_epi64(_mm256_loadu_si256(uptr++));         \
    __m512i zmm1 = _mm512_cvtepu32_epi64(_mm256_loadu_si256(uptr++));         \
    __m512i zmm2 = _mm512_cvtepu32_epi64(_mm256_loadu_si256(uptr++));         \
    __m512i zmm3 = _mm512_cvtepu32_epi64(_mm256_loadu_si256(uptr++));

#define MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PS(r)                          \
    _mm512_storeu_ps(r + 0x00, _mm512_castsi512_ps(zmm0));                    \
    _mm512_storeu_ps(r + 0x10, _mm512_castsi512_ps(zmm1));                    \
    _mm512_storeu_ps(r + 0x20, _mm512_castsi512_ps(zmm2));                    \
    _mm512_storeu_ps(r + 0x30, _mm512_castsi512_ps(zmm3));

#define MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PD(r)                          \
    _mm512_storeu_pd(r + 0x00, _mm512_castsi512_pd(zmm0));                    \
    _mm512_storeu_pd(r + 0x08, _mm512_castsi512_pd(zmm1));                    \
    _mm512_storeu_pd(r + 0x10, _mm512_castsi512_pd(zmm2));                    \
    _mm512_storeu_pd(r + 0x18, _mm512_castsi512_pd(zmm3));

#define MCKL_RANDOM_INTERNAL_U01_AVX512_AND1ADD_EPI32                         \
    const __m512i mask = _mm512_set1_epi32(1);                                \
    zmm0 = _mm512_add_epi32(zmm0, _mm512_and_si512(zmm0, mask));              \
    zmm1 = _mm512_add_epi32(zmm1, _mm512_and_si512(zmm1, mask));              \
    zmm2 = _mm512_add_epi32(zmm2, _mm512_and_si512(zmm2, mask));              \
    zmm3 = _mm512_add_epi32(zmm3, _mm512_and_si512(zmm3, mask));

#define MCKL_RANDOM_INTERNAL_U01_AVX512_AND1ADD_EPI64                         \
    const __m512i mask = _mm512_set1_epi64(1);                                \
    zmm0 = _mm512_add_epi64(zmm0, _mm512_and_si512(zmm0, mask));              \
    zmm1 = _mm512_add_epi64(zmm1, _mm512_and_si512(zmm1, mask));              \
    zmm2 = _mm512_add_epi64(zmm2, _mm512_and_si512(zmm2, mask));              \
    zmm3 = _mm512_add_epi64(zmm3, _mm512_and_si512(zmm3, mask));

#define MCKL_RANDOM_INTERNAL_U01_AVX512_SLLI_EPI32(imm8)                      \
    zmm0 = _mm512_slli_epi32(zmm0, imm8);                                     \
    zmm1 = _mm512_slli_epi32(zmm1, imm8);                                     \
    zmm2 = _mm512_slli_epi32(zmm2, imm8);                                     \
    zmm3 = _mm512_slli_epi32(zmm3, imm8);

#define MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI32(imm8)                      \
    zmm0 = _mm512_srli_epi32(zmm0, imm8);                                     \
    zmm1 = _mm512_srli_epi32(zmm1, imm8);                                     \
    zmm2 = _mm512_srli_epi32(zmm2, imm8);                                     \
    zmm3 = _mm512_srli_epi32(zmm3, imm8);

#define MCKL_RANDOM_INTERNAL_U01_AVX512_SLLI_EPI64(imm8)                      \
    zmm0 = _mm512_slli_epi64(zmm0, imm8);                                     \
    zmm1 = _mm512_slli_epi64(zmm1, imm8);                                     \
    zmm2 = _mm512_slli_epi64(zmm2, imm8);                                     \
    zmm3 = _mm512_slli_epi64(zmm3, imm8);

#define MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI64(imm8)                      \
    zmm0 = _mm512_srli_epi64(zmm0, imm8);                                     \
    zmm1 = _mm512_srli_epi64(zmm1, imm8);                                     \
    zmm2 = _mm512_srli_epi64(zmm2, imm8);                                     \
    zmm3 = _mm512_srli_epi64(zmm3, imm8);

#define MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU32_PS_31                        \
    zmm0 = _mm512_castps_si512(_mm512_cvtepi32_ps(zmm0));                     \
    zmm1 = _mm512_castps_si512(_mm512_cvtepi32_ps(zmm1));                     \
    zmm2 = _mm512_castps_si512(_mm512_cvtepi32_ps(zmm2));                     \
    zmm3 = _mm512_castps_si512(_mm512_cvtepi32_ps(zmm3));

#define MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU32_PS                           \
    const __m512 c = _mm512_set1_ps(Pow2<float, 23>::value);                  \
    const __m512 d = _mm512_set1_ps(Pow2<float, 16>::value);                  \
    const __m512i l = _mm512_set1_epi32(static_cast<int>(0xFFFF));            \
    __m512i xmm0 = _mm512_and_si512(zmm0, l);                                 \
    __m512i xmm1 = _mm512_and_si512(zmm1, l);                                 \
    __m512i xmm2 = _mm512_and_si512(zmm2, l);                                 \
    __m512i xmm3 = _mm512_and_si512(zmm3, l);                                 \
    MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI32(16);                           \
    zmm0 = _mm512_add_epi32(zmm0, _mm512_castps_si512(c));                    \
    zmm1 = _mm512_add_epi32(zmm1, _mm512_castps_si512(c));                    \
    zmm2 = _mm512_add_epi32(zmm2, _mm512_castps_si512(c));                    \
    zmm3 = _mm512_add_epi32(zmm3, _mm512_castps_si512(c));                    \
    xmm0 = _mm512_add_epi32(xmm0, _mm512_castps_si512(c));                    \
    xmm1 = _mm512_add_epi32(xmm1, _mm512_castps_si512(c));                    \
    xmm2 = _mm512_add_epi32(xmm2, _mm512_castps_si512(c));                    \
    xmm3 = _mm512_add_epi32(xmm3, _mm512_castps_si512(c));                    \
    zmm0 = _mm512_castps_si512(_mm512_sub_ps(_mm512_castsi512_ps(zmm0), c));  \
    zmm1 = _mm512_castps_si512(_mm512_sub_ps(_mm512_castsi512_ps(zmm1), c));  \
    zmm2 = _mm512_castps_si512(_mm512_sub_ps(_mm512_castsi512_ps(zmm2), c));  \
    zmm3 = _mm512_castps_si512(_mm512_sub_ps(_mm512_castsi512_ps(zmm3), c));  \
    xmm0 = _mm512_castps_si512(_mm512_sub_ps(_mm512_castsi512_ps(xmm0), c));  \
    xmm1 = _mm512_castps_si512(_mm512_sub_ps(_mm512_castsi512_ps(xmm1), c));  \
    xmm2 = _mm512_castps_si512(_mm512_sub_ps(_mm512_castsi512_ps(xmm2), c));  \
    xmm3 = _mm512_castps_si512(_mm512_sub_ps(_mm512_castsi512_ps(xmm3), c));  \
    zmm0 = _mm512_castps_si512(_mm512_fmadd_ps(                               \
        _mm512_castsi512_ps(zmm0), d, _mm512_castsi512_ps(xmm0)));            \
    zmm1 = _mm512_castps_si512(_mm512_fmadd_ps(                               \
        _mm512_castsi512_ps(zmm1), d, _mm512_castsi512_ps(xmm1)));            \
    zmm2 = _mm512_castps_si512(_mm512_fmadd_ps(                               \
        _mm512_castsi512_ps(zmm2), d, _mm512_castsi512_ps(xmm2)));            \
    zmm3 = _mm512_castps_si512(_mm512_fmadd_ps(                               \
        _mm512_castsi512_ps(zmm3), d, _mm512_castsi512_ps(xmm3)));

#define MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU64_PD_52                        \
    const __m512d c = _mm512_set1_pd(Pow2<double, 52>::value);                \
    zmm0 = _mm512_add_epi64(zmm0, _mm512_castpd_si512(c));                    \
    zmm1 = _mm512_add_epi64(zmm1, _mm512_castpd_si512(c));                    \
    zmm2 = _mm512_add_epi64(zmm2, _mm512_castpd_si512(c));                    \
    zmm3 = _mm512_add_epi64(zmm3, _mm512_castpd_si512(c));                    \
    zmm0 = _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(zmm0), c));  \
    zmm1 = _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(zmm1), c));  \
    zmm2 = _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(zmm2), c));  \
    zmm3 = _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(zmm3), c));

#define MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU64_PD                           \
    const __m512d c = _mm512_set1_pd(Pow2<double, 52>::value);                \
    const __m512d d = _mm512_set1_pd(Pow2<double, 32>::value);                \
    const __m512i l = _mm512_set1_epi64(static_cast<MCKL_INT64>(0xFFFFFFFF)); \
    __m512i xmm0 = _mm512_and_si512(zmm0, l);                                 \
    __m512i xmm1 = _mm512_and_si512(zmm1, l);                                 \
    __m512i xmm2 = _mm512_and_si512(zmm2, l);                                 \
    __m512i xmm3 = _mm512_and_si512(zmm3, l);                                 \
    MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI64(32);                           \
    zmm0 = _mm512_add_epi64(zmm0, _mm512_castpd_si512(c));                    \
    zmm1 = _mm512_add_epi64(zmm1, _mm512_castpd_si512(c));                    \
    zmm2 = _mm512_add_epi64(zmm2, _mm512_castpd_si512(c));                    \
    zmm3 = _mm512_add_epi64(zmm3, _mm512_castpd_si512(c));                    \
    zmm0 = _mm512_add_epi64(zmm0, _mm512_castpd_si512(c));                    \
    xmm1 = _mm512_add_epi64(xmm1, _mm512_castpd_si512(c));                    \
    xmm2 = _mm512_add_epi64(xmm2, _mm512_castpd_si512(c));                    \
    xmm3 = _mm512_add_epi64(xmm3, _mm512_castpd_si512(c));                    \
    zmm0 = _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(zmm0), c));  \
    zmm1 = _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(zmm1), c));  \
    zmm2 = _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(zmm2), c));  \
    zmm3 = _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(zmm3), c));  \
    xmm0 = _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(xmm0), c));  \
    xmm1 = _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(xmm1), c));  \
    xmm2 = _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(xmm2), c));  \
    xmm3 = _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(xmm3), c));  \
    zmm0 = _mm512_castpd_si512(_mm512_fmadd_pd(                               \
        _mm512_castsi512_pd(zmm0), d, _mm512_castsi512_pd(xmm0)));            \
    zmm1 = _mm512_castpd_si512(_mm512_fmadd_pd(                               \
        _mm512_castsi512_pd(zmm1), d, _mm512_castsi512_pd(xmm1)));            \
    zmm2 = _mm512_castpd_si512(_mm512_fmadd_pd(                               \
        _mm512_castsi512_pd(zmm2), d, _mm512_castsi512_pd(xmm2)));            \
    zmm3 = _mm512_castpd_si512(_mm512_fmadd_pd(                               \
        _mm512_castsi512_pd(zmm3), d, _mm512_castsi512_pd(xmm3)));

#define MCKL_RANDOM_INTERNAL_U01_AVX512_MUL_PS(a)                             \
    zmm0 = _mm512_castps_si512(_mm512_mul_ps(_mm512_castsi512_ps(zmm0), a));  \
    zmm1 = _mm512_castps_si512(_mm512_mul_ps(_mm512_castsi512_ps(zmm1), a));  \
    zmm2 = _mm512_castps_si512(_mm512_mul_ps(_mm512_castsi512_ps(zmm2), a));  \
    zmm3 = _mm512_castps_si512(_mm512_mul_ps(_mm512_castsi512_ps(zmm3), a));

#define MCKL_RANDOM_INTERNAL_U01_AVX512_MUL_PD(a)                             \
    zmm0 = _mm512_castpd_si512(_mm512_mul_pd(_mm512_castsi512_pd(zmm0), a));  \
    zmm1 = _mm512_castpd_si512(_mm512_mul_pd(_mm512_castsi512_pd(zmm1), a));  \
    zmm2 = _mm512_castpd_si512(_mm512_mul_pd(_mm512_castsi512_pd(zmm2), a));  \
    zmm3 = _mm512_castpd_si512(_mm512_mul_pd(_mm512_castsi512_pd(zmm3), a));

#define MCKL_RANDOM_INTERNAL_U01_AVX512_FMADD_PS(a, b)                        \
    zmm0 = _mm512_castps_si512(                                               \
        _mm512_fmadd_ps(_mm512_castsi512_ps(zmm0), a, b));                    \
    zmm1 = _mm512_castps_si512(                                               \
        _mm512_fmadd_ps(_mm512_castsi512_ps(zmm1), a, b));                    \
    zmm2 = _mm512_castps_si512(                                               \
        _mm512_fmadd_ps(_mm512_castsi512_ps(zmm2), a, b));                    \
    zmm3 = _mm512_castps_si512(                                               \
        _mm512_fmadd_ps(_mm512_castsi512_ps(zmm3), a, b));

#define MCKL_RANDOM_INTERNAL_U01_AVX512_FMADD_PD(a, b)                        \
    zmm0 = _mm512_castpd_si512(                                               \
        _mm512_fmadd_pd(_mm512_castsi512_pd(zmm0), a, b));                    \
    zmm1 = _mm512_castpd_si512(                                               \
        _mm512_fmadd_pd(_mm512_castsi512_pd(zmm1), a, b));                    \
    zmm2 = _mm512_castpd_si512(                                               \
        _mm512_fmadd_pd(_mm512_castsi512_pd(zmm2), a, b));                    \
    zmm3 = _mm512_castpd_si512(                                               \
        _mm512_fmadd_pd(_mm512_castsi512_pd(zmm3), a, b));

namespace mckl {

namespace internal {

template <typename UIntType, typename RealType, typename Lower, typename Upper,
    int = std::numeric_limits<UIntType>::digits>
class U01AVX512Impl : public U01GenericImpl<UIntType, RealType, Lower, Upper>
{
}; // class U01AVX512Impl

template <typename UIntType, typename RealType, typename Lower, typename Upper>
class U01AVX512ImplBase
{
  public:
    MCKL_INLINE static RealType eval(UIntType u)
    {
        return U01GenericImpl<UIntType, RealType, Lower, Upper>::eval(u);
    }

    MCKL_INLINE static void eval(std::size_t n, const UIntType *u, RealType *r)
    {
        constexpr std::size_t S = 4;
        constexpr std::size_t N = sizeof(__m512i) * S / sizeof(RealType);

        while (n >= N) {
            U01AVX512Impl<UIntType, RealType, Lower, Upper>::eval(u, r);
            n -= N;
            u += N;
            r += N;
        }

        for (std::size_t i = 0; i != n; ++i) {
            r[i] = eval(u[i]);
        }
    }
}; // class U01AVX512ImplBase

template <typename UIntType, typename RealType, int Q,
    int = std::numeric_limits<UIntType>::digits>
class U01CanonicalAVX512Impl
    : public U01CanonicalGenericImpl<UIntType, RealType, Q>
{
}; // class U01CanonicalAVX512Impl

template <typename UIntType, typename RealType, int Q>
class U01CanonicalAVX512ImplBase
{
  public:
    MCKL_INLINE static RealType eval(const UIntType *u)
    {
        return U01CanonicalGenericImpl<UIntType, RealType, Q>::eval(u);
    }

    MCKL_INLINE static void eval(std::size_t n, const UIntType *u, RealType *r)
    {
        constexpr std::size_t S = 4;
        constexpr std::size_t N = sizeof(__m512i) * S / sizeof(RealType);

        while (n >= N) {
            U01CanonicalAVX512Impl<UIntType, RealType, Q>::eval(u, r);
            n -= N;
            u += N * Q;
            r += N;
        }

        for (std::size_t i = 0; i != n; ++i, u += Q) {
            r[i] = eval(u);
        }
    }
}; // class U01CanonicalCanonicalAVX512ImplBase

template <typename UIntType>
class U01AVX512Impl<UIntType, float, Closed, Closed, 32>
    : public U01AVX512ImplBase<UIntType, float, Closed, Closed>
{
  public:
    using U01AVX512ImplBase<UIntType, float, Closed, Closed>::eval;

  private:
    friend U01AVX512ImplBase<UIntType, float, Closed, Closed>;

    MCKL_INLINE static void eval(const UIntType *u, float *r)
    {
        const __m512 d25 = _mm512_set1_ps(Pow2<float, -25>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_SLLI_EPI32(1)
        MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI32(7)
        MCKL_RANDOM_INTERNAL_U01_AVX512_AND1ADD_EPI32
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU32_PS_31
        MCKL_RANDOM_INTERNAL_U01_AVX512_MUL_PS(d25)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PS(r)
    }
}; // class U01AVX512Impl

template <typename UIntType>
class U01AVX512Impl<UIntType, float, Closed, Open, 32>
    : public U01AVX512ImplBase<UIntType, float, Closed, Open>
{
  public:
    using U01AVX512ImplBase<UIntType, float, Closed, Open>::eval;

  private:
    friend U01AVX512ImplBase<UIntType, float, Closed, Open>;

    MCKL_INLINE static void eval(const UIntType *u, float *r)
    {
        const __m512 d24 = _mm512_set1_ps(Pow2<float, -24>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI32(8)
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU32_PS_31
        MCKL_RANDOM_INTERNAL_U01_AVX512_MUL_PS(d24)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PS(r)
    }
}; // class U01AVX512Impl

template <typename UIntType>
class U01AVX512Impl<UIntType, float, Open, Closed, 32>
    : public U01AVX512ImplBase<UIntType, float, Open, Closed>
{
  public:
    using U01AVX512ImplBase<UIntType, float, Open, Closed>::eval;

  private:
    friend U01AVX512ImplBase<UIntType, float, Open, Closed>;

    MCKL_INLINE static void eval(const UIntType *u, float *r)
    {
        const __m512 d24 = _mm512_set1_ps(Pow2<float, -24>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI32(8)
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU32_PS_31
        MCKL_RANDOM_INTERNAL_U01_AVX512_FMADD_PS(d24, d24)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PS(r)
    }
}; // class U01AVX512Impl

template <typename UIntType>
class U01AVX512Impl<UIntType, float, Open, Open, 32>
    : public U01AVX512ImplBase<UIntType, float, Open, Open>
{
  public:
    using U01AVX512ImplBase<UIntType, float, Open, Open>::eval;

  private:
    friend U01AVX512ImplBase<UIntType, float, Open, Open>;

    MCKL_INLINE static void eval(const UIntType *u, float *r)
    {
        const __m512 d23 = _mm512_set1_ps(Pow2<float, -23>::value);
        const __m512 d24 = _mm512_set1_ps(Pow2<float, -24>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI32(9)
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU32_PS_31
        MCKL_RANDOM_INTERNAL_U01_AVX512_FMADD_PS(d23, d24)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PS(r)
    }
}; // class U01AVX512Impl

template <typename UIntType>
class U01AVX512Impl<UIntType, double, Closed, Closed, 32>
    : public U01AVX512ImplBase<UIntType, double, Closed, Closed>
{
  public:
    using U01AVX512ImplBase<UIntType, double, Closed, Closed>::eval;

  private:
    friend U01AVX512ImplBase<UIntType, double, Closed, Closed>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m512d d32 = _mm512_set1_pd(Pow2<double, -32>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512_CVTEPU32_EPI64(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_AND1ADD_EPI64
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU64_PD_52
        MCKL_RANDOM_INTERNAL_U01_AVX512_MUL_PD(d32)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PD(r)
    }
}; // class U01AVX512Impl

template <typename UIntType>
class U01AVX512Impl<UIntType, double, Closed, Open, 32>
    : public U01AVX512ImplBase<UIntType, double, Closed, Open>
{
  public:
    using U01AVX512ImplBase<UIntType, double, Closed, Open>::eval;

  private:
    friend U01AVX512ImplBase<UIntType, double, Closed, Open>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m512d d32 = _mm512_set1_pd(Pow2<double, -32>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512_CVTEPU32_EPI64(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU64_PD_52
        MCKL_RANDOM_INTERNAL_U01_AVX512_MUL_PD(d32)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PD(r)
    }
}; // class U01AVX512Impl

template <typename UIntType>
class U01AVX512Impl<UIntType, double, Open, Closed, 32>
    : public U01AVX512ImplBase<UIntType, double, Open, Closed>
{
  public:
    using U01AVX512ImplBase<UIntType, double, Open, Closed>::eval;

  private:
    friend U01AVX512ImplBase<UIntType, double, Open, Closed>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m512d d32 = _mm512_set1_pd(Pow2<double, -32>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512_CVTEPU32_EPI64(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU64_PD_52
        MCKL_RANDOM_INTERNAL_U01_AVX512_FMADD_PD(d32, d32)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PD(r)
    }
}; // class U01AVX512Impl

template <typename UIntType>
class U01AVX512Impl<UIntType, double, Open, Open, 32>
    : public U01AVX512ImplBase<UIntType, double, Open, Open>
{
  public:
    using U01AVX512ImplBase<UIntType, double, Open, Open>::eval;

  private:
    friend U01AVX512ImplBase<UIntType, double, Open, Open>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m512d d32 = _mm512_set1_pd(Pow2<double, -32>::value);
        const __m512d d33 = _mm512_set1_pd(Pow2<double, -33>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512_CVTEPU32_EPI64(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU64_PD_52
        MCKL_RANDOM_INTERNAL_U01_AVX512_FMADD_PD(d32, d33)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PD(r)
    }
}; // class U01AVX512Impl

template <typename UIntType>
class U01AVX512Impl<UIntType, double, Closed, Closed, 64>
    : public U01AVX512ImplBase<UIntType, double, Closed, Closed>
{
  public:
    using U01AVX512ImplBase<UIntType, double, Closed, Closed>::eval;

  private:
    friend U01AVX512ImplBase<UIntType, double, Closed, Closed>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m512d d54 = _mm512_set1_pd(Pow2<double, -54>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_SLLI_EPI64(1)
        MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI64(10)
        MCKL_RANDOM_INTERNAL_U01_AVX512_AND1ADD_EPI64
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU64_PD
        MCKL_RANDOM_INTERNAL_U01_AVX512_MUL_PD(d54)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PD(r)
    }
}; // class U01AVX512Impl

template <typename UIntType>
class U01AVX512Impl<UIntType, double, Closed, Open, 64>
    : public U01AVX512ImplBase<UIntType, double, Closed, Open>
{
  public:
    using U01AVX512ImplBase<UIntType, double, Closed, Open>::eval;

  private:
    friend U01AVX512ImplBase<UIntType, double, Closed, Open>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m512d d53 = _mm512_set1_pd(Pow2<double, -53>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI64(11)
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU64_PD
        MCKL_RANDOM_INTERNAL_U01_AVX512_MUL_PD(d53)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PD(r)
    }
}; // class U01AVX512Impl

template <typename UIntType>
class U01AVX512Impl<UIntType, double, Open, Closed, 64>
    : public U01AVX512ImplBase<UIntType, double, Open, Closed>
{
  public:
    using U01AVX512ImplBase<UIntType, double, Open, Closed>::eval;

  private:
    friend U01AVX512ImplBase<UIntType, double, Open, Closed>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m512d d53 = _mm512_set1_pd(Pow2<double, -53>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI64(11)
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU64_PD
        MCKL_RANDOM_INTERNAL_U01_AVX512_FMADD_PD(d53, d53)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PD(r)
    }
}; // class U01AVX512Impl

template <typename UIntType>
class U01AVX512Impl<UIntType, double, Open, Open, 64>
    : public U01AVX512ImplBase<UIntType, double, Open, Open>
{
  public:
    using U01AVX512ImplBase<UIntType, double, Open, Open>::eval;

  private:
    friend U01AVX512ImplBase<UIntType, double, Open, Open>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m512d d52 = _mm512_set1_pd(Pow2<double, -52>::value);
        const __m512d d53 = _mm512_set1_pd(Pow2<double, -53>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI64(12)
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU64_PD_52
        MCKL_RANDOM_INTERNAL_U01_AVX512_FMADD_PD(d52, d53)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PD(r)
    }
}; // class U01AVX512Impl

template <typename UIntType>
class U01CanonicalAVX512Impl<UIntType, float, 1, 32>
    : public U01CanonicalAVX512ImplBase<UIntType, float, 1>
{
  public:
    using U01CanonicalAVX512ImplBase<UIntType, float, 1>::eval;

  private:
    friend U01CanonicalAVX512ImplBase<UIntType, float, 1>;

    MCKL_INLINE static void eval(const UIntType *u, float *r)
    {
        const __m512 d32 = _mm512_set1_ps(Pow2<float, -32>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512(u);
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU32_PS
        MCKL_RANDOM_INTERNAL_U01_AVX512_MUL_PS(d32)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PS(r)
    }
}; // class U01CanonicalAVX512Impl

template <typename UIntType>
class U01CanonicalAVX512Impl<UIntType, double, 2, 32>
    : public U01CanonicalAVX512ImplBase<UIntType, double, 2>
{
  public:
    using U01CanonicalAVX512ImplBase<UIntType, double, 2>::eval;

  private:
    friend U01CanonicalAVX512ImplBase<UIntType, double, 2>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m512d d32 = _mm512_set1_pd(Pow2<double, -32>::value);
        const __m512d d64 = _mm512_set1_pd(Pow2<double, -64>::value);
        const __m512d c = _mm512_set1_pd(Pow2<double, 52>::value);
        const __m512i l =
            _mm512_set1_epi64(static_cast<MCKL_INT64>(0xFFFFFFFF));

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512(u)

        __m512i xmm0 = _mm512_and_si512(zmm0, l);
        __m512i xmm1 = _mm512_and_si512(zmm1, l);
        __m512i xmm2 = _mm512_and_si512(zmm2, l);
        __m512i xmm3 = _mm512_and_si512(zmm3, l);

        MCKL_RANDOM_INTERNAL_U01_AVX512_SRLI_EPI64(32);

        zmm0 = _mm512_add_epi64(zmm0, _mm512_castpd_si512(c));
        zmm1 = _mm512_add_epi64(zmm1, _mm512_castpd_si512(c));
        zmm2 = _mm512_add_epi64(zmm2, _mm512_castpd_si512(c));
        zmm3 = _mm512_add_epi64(zmm3, _mm512_castpd_si512(c));

        xmm0 = _mm512_add_epi64(xmm0, _mm512_castpd_si512(c));
        xmm1 = _mm512_add_epi64(xmm1, _mm512_castpd_si512(c));
        xmm2 = _mm512_add_epi64(xmm2, _mm512_castpd_si512(c));
        xmm3 = _mm512_add_epi64(xmm3, _mm512_castpd_si512(c));

        zmm0 =
            _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(zmm0), c));
        zmm1 =
            _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(zmm1), c));
        zmm2 =
            _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(zmm2), c));
        zmm3 =
            _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(zmm3), c));

        xmm0 =
            _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(xmm0), c));
        xmm1 =
            _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(xmm1), c));
        xmm2 =
            _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(xmm2), c));
        xmm3 =
            _mm512_castpd_si512(_mm512_sub_pd(_mm512_castsi512_pd(xmm3), c));

        MCKL_RANDOM_INTERNAL_U01_AVX512_MUL_PD(d32);

        zmm0 = _mm512_castpd_si512(_mm512_fmadd_pd(
            _mm512_castsi512_pd(xmm0), d64, _mm512_castsi512_pd(zmm0)));
        zmm1 = _mm512_castpd_si512(_mm512_fmadd_pd(
            _mm512_castsi512_pd(xmm1), d64, _mm512_castsi512_pd(zmm1)));
        zmm2 = _mm512_castpd_si512(_mm512_fmadd_pd(
            _mm512_castsi512_pd(xmm2), d64, _mm512_castsi512_pd(zmm2)));
        zmm3 = _mm512_castpd_si512(_mm512_fmadd_pd(
            _mm512_castsi512_pd(xmm3), d64, _mm512_castsi512_pd(zmm3)));

        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PD(r)
    }
}; // class U01CanonicalAVX512Impl

template <typename UIntType>
class U01CanonicalAVX512Impl<UIntType, double, 1, 64>
    : public U01CanonicalAVX512ImplBase<UIntType, double, 1>
{
  public:
    using U01CanonicalAVX512ImplBase<UIntType, double, 1>::eval;

  private:
    friend U01CanonicalAVX512ImplBase<UIntType, double, 1>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m512d d64 = _mm512_set1_pd(Pow2<double, -64>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX512_LOADU_SI512(u)
        MCKL_RANDOM_INTERNAL_U01_AVX512_CVTEPU64_PD
        MCKL_RANDOM_INTERNAL_U01_AVX512_MUL_PD(d64)
        MCKL_RANDOM_INTERNAL_U01_AVX512_STOREU_PD(r)
    }
}; // class U01CanonicalAVX512Impl

} // namespace internal

} // namespace mckl

MCKL_POP_GCC_WARNING

#endif // MCKL_RANDOM_INTERNAL_U01_AVX512_HPP
