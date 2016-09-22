//============================================================================
// MCKL/include/mckl/random/aesni.hpp
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

#ifndef MCKL_RANDOM_AESNI_HPP
#define MCKL_RANDOM_AESNI_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/counter.hpp>
#include <wmmintrin.h>

#ifdef MCKL_GCC
#if __GNUC__ >= 6
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#define MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(N, val)                         \
    template <>                                                               \
    inline __m128i AESKeyGenAssist<N>(const __m128i &xmm)                     \
    {                                                                         \
        return _mm_aeskeygenassist_si128(xmm, val);                           \
    }

/// \brief ARSEngine default rounds
/// \ingroup Config
#ifndef MCKL_ARS_ROUNDS
#define MCKL_ARS_ROUNDS 5
#endif

/// \brief AESEngine default blocks
/// \ingroup Config
#ifndef MCKL_AESNI_BLOCKS
#define MCKL_AESNI_BLOCKS 8
#endif

namespace mckl
{

namespace internal
{

template <std::size_t K>
class AESNIEncFirst
{
    public:
    static void eval(std::array<__m128i, K> &state, const __m128i &rk)
    {
        eval<0>(state, rk, std::integral_constant<bool, 0 < K>());
    }

    private:
    template <std::size_t>
    static void eval(
        std::array<__m128i, K> &, const __m128i &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(
        std::array<__m128i, K> &state, const __m128i &rk, std::true_type)
    {
        std::get<I>(state) = _mm_xor_si128(std::get<I>(state), rk);
        eval<I + 1>(state, rk, std::integral_constant<bool, I + 1 < K>());
    }
}; // class AESNIEncFirst

template <std::size_t K>
class AESNIEncRound
{
    public:
    static void eval(std::array<__m128i, K> &state, const __m128i &rk)
    {
        eval<0>(state, rk, std::integral_constant<bool, 0 < K>());
    }

    private:
    template <std::size_t>
    static void eval(
        std::array<__m128i, K> &, const __m128i &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(
        std::array<__m128i, K> &state, const __m128i &rk, std::true_type)
    {
        std::get<I>(state) = _mm_aesenc_si128(std::get<I>(state), rk);
        eval<I + 1>(state, rk, std::integral_constant<bool, I + 1 < K>());
    }
}; // class AESNIEncRound

template <std::size_t K>
class AESNIEncLast
{
    public:
    static void eval(std::array<__m128i, K> &state, const __m128i &rk)
    {
        eval<0>(state, rk, std::integral_constant<bool, 0 < K>());
    }

    private:
    template <std::size_t>
    static void eval(
        std::array<__m128i, K> &, const __m128i &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(
        std::array<__m128i, K> &state, const __m128i &rk, std::true_type)
    {
        std::get<I>(state) = _mm_aesenclast_si128(std::get<I>(state), rk);
        eval<I + 1>(state, rk, std::integral_constant<bool, I + 1 < K>());
    }
}; // class AESNIEncLast

template <typename KeySeqType, std::size_t K, std::size_t N,
    bool = N <= KeySeqType::rounds>
class AESNIRound
{
    static constexpr std::size_t rounds = KeySeqType::rounds;

    public:
    static void eval(
        std::array<__m128i, K> &, const std::array<__m128i, rounds + 1> &)
    {
    }
}; // class AESNIRound

template <typename KeySeqType, std::size_t K, std::size_t N>
class AESNIRound<KeySeqType, K, N, true>
{
    public:
    static void eval(std::array<__m128i, K> &state,
        const std::array<__m128i, KeySeqType::rounds + 1> &par)
    {
        eval(state, par, std::integral_constant<bool, N == 0>(),
            std::integral_constant<bool, N == KeySeqType::rounds>());
    }

    private:
    static void eval(std::array<__m128i, K> &state,
        const std::array<__m128i, KeySeqType::rounds + 1> &par, std::true_type,
        std::false_type)
    {
        AESNIEncFirst<K>::eval(state, std::get<N>(par));
    }

    static void eval(std::array<__m128i, K> &state,
        const std::array<__m128i, KeySeqType::rounds + 1> &par,
        std::false_type, std::false_type)
    {
        AESNIEncRound<K>::eval(state, std::get<N>(par));
    }

    static void eval(std::array<__m128i, K> &state,
        const std::array<__m128i, KeySeqType::rounds + 1> &par,
        std::false_type, std::true_type)
    {
        AESNIEncLast<K>::eval(state, std::get<N>(par));
    }
}; // class AESNIRound

} // namespace mckl::internal

/// \brief RNG generator using AES-NI instructions
/// \ingroup AESNI
template <typename KeySeqType, std::size_t Blocks = MCKL_AESNI_BLOCKS>
class AESNIGenerator
{
    static_assert(
        Blocks != 0, "**AESNIGenerator** used with blocks equal to zero");

    static_assert(KeySeqType::rounds != 0,
        "**AESNIGenerate** used with KeySeqType::rounds equal to runds");

    public:
    using ctr_type = std::array<std::uint64_t, 2>;
    using key_type = typename KeySeqType::key_type;

    static constexpr std::size_t size() { return Blocks * sizeof(__m128i); }

    key_type key() const { return key_seq_.key(); }

    void reset(const key_type &key) { key_seq_.reset(key); }

    void enc(const ctr_type &ctr, ctr_type &buffer) const
    {
        union {
            std::array<__m128i, 1> state;
            ctr_type result;
        } buf;

        std::array<__m128i, KeySeqType::rounds + 1> tmp;
        const std::array<__m128i, KeySeqType::rounds + 1> &par = key_seq_(tmp);

        buf.result = ctr;
        generate<0>(buf.state, par, std::true_type());
        buffer = buf.result;
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr,
        std::array<ResultType, size() / sizeof(ResultType)> &buffer) const
    {
        generate(ctr, buffer);
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, std::size_t n,
        std::array<ResultType, size() / sizeof(ResultType)> *buffer) const
    {
        for (std::size_t i = 0; i != n; ++i)
            generate(ctr, buffer[i]);
    }

    friend bool operator==(const AESNIGenerator<KeySeqType, Blocks> &gen1,
        const AESNIGenerator<KeySeqType, Blocks> &gen2)
    {
        return gen1.key_seq_ == gen2.key_seq_;
    }

    friend bool operator!=(const AESNIGenerator<KeySeqType, Blocks> &gen1,
        const AESNIGenerator<KeySeqType, Blocks> &gen2)
    {
        return !(gen1 == gen2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const AESNIGenerator<KeySeqType, Blocks> &gen)
    {
        if (!os)
            return os;

        os << gen.key_seq_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        AESNIGenerator<KeySeqType, Blocks> &gen)
    {
        if (!is)
            return is;

        AESNIGenerator<KeySeqType, Blocks> gen_tmp;
        is >> std::ws >> gen_tmp.key_seq_;

        if (is)
            gen = std::move(gen_tmp);

        return is;
    }

    private:
    KeySeqType key_seq_;

    template <typename ResultType>
    void generate(ctr_type &ctr,
        std::array<ResultType, size() / sizeof(ResultType)> &buffer) const
    {
        union {
            std::array<__m128i, Blocks> state;
            std::array<ctr_type, Blocks> ctr_block;
            std::array<ResultType, size() / sizeof(ResultType)> result;
        } buf;

        std::array<__m128i, KeySeqType::rounds + 1> tmp;
        const std::array<__m128i, KeySeqType::rounds + 1> &par = key_seq_(tmp);

        increment(ctr, buf.ctr_block);
        generate<0>(buf.state, par, std::true_type());
        buffer = buf.result;
    }

    template <std::size_t, std::size_t K>
    void generate(std::array<__m128i, K> &,
        const std::array<__m128i, KeySeqType::rounds + 1> &,
        std::false_type) const
    {
    }

    template <std::size_t N, std::size_t K>
    void generate(std::array<__m128i, K> &state,
        const std::array<__m128i, KeySeqType::rounds + 1> &par,
        std::true_type) const
    {
        internal::AESNIRound<KeySeqType, K, N>::eval(state, par);
        generate<N + 1>(state, par,
            std::integral_constant<bool, N + 1 <= KeySeqType::rounds>());
    }
}; // class AESNIGenerator

/// \brief RNG engine using AES-NI instructions
/// \ingroup AESNI
template <typename ResultType, typename KeySeqType,
    std::size_t Blocks = MCKL_AESNI_BLOCKS>
using AESNIEngine =
    CounterEngine<ResultType, AESNIGenerator<KeySeqType, Blocks>>;

namespace internal
{

template <std::size_t>
inline __m128i AESKeyGenAssist(const __m128i &);

MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x00, 0x8D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x01, 0x01)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x02, 0x02)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x03, 0x04)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x04, 0x08)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x05, 0x10)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x06, 0x20)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x07, 0x40)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x08, 0x80)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x09, 0x1B)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x0A, 0x36)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x0B, 0x6C)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x0C, 0xD8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x0D, 0xAB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x0E, 0x4D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x0F, 0x9A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x10, 0x2F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x11, 0x5E)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x12, 0xBC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x13, 0x63)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x14, 0xC6)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x15, 0x97)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x16, 0x35)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x17, 0x6A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x18, 0xD4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x19, 0xB3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x1A, 0x7D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x1B, 0xFA)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x1C, 0xEF)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x1D, 0xC5)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x1E, 0x91)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x1F, 0x39)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x20, 0x72)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x21, 0xE4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x22, 0xD3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x23, 0xBD)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x24, 0x61)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x25, 0xC2)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x26, 0x9F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x27, 0x25)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x28, 0x4A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x29, 0x94)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x2A, 0x33)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x2B, 0x66)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x2C, 0xCC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x2D, 0x83)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x2E, 0x1D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x2F, 0x3A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x30, 0x74)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x31, 0xE8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x32, 0xCB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x33, 0x8D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x34, 0x01)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x35, 0x02)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x36, 0x04)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x37, 0x08)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x38, 0x10)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x39, 0x20)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x3A, 0x40)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x3B, 0x80)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x3C, 0x1B)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x3D, 0x36)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x3E, 0x6C)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x3F, 0xD8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x40, 0xAB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x41, 0x4D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x42, 0x9A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x43, 0x2F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x44, 0x5E)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x45, 0xBC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x46, 0x63)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x47, 0xC6)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x48, 0x97)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x49, 0x35)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x4A, 0x6A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x4B, 0xD4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x4C, 0xB3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x4D, 0x7D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x4E, 0xFA)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x4F, 0xEF)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x50, 0xC5)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x51, 0x91)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x52, 0x39)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x53, 0x72)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x54, 0xE4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x55, 0xD3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x56, 0xBD)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x57, 0x61)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x58, 0xC2)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x59, 0x9F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x5A, 0x25)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x5B, 0x4A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x5C, 0x94)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x5D, 0x33)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x5E, 0x66)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x5F, 0xCC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x60, 0x83)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x61, 0x1D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x62, 0x3A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x63, 0x74)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x64, 0xE8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x65, 0xCB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x66, 0x8D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x67, 0x01)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x68, 0x02)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x69, 0x04)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x6A, 0x08)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x6B, 0x10)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x6C, 0x20)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x6D, 0x40)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x6E, 0x80)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x6F, 0x1B)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x70, 0x36)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x71, 0x6C)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x72, 0xD8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x73, 0xAB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x74, 0x4D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x75, 0x9A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x76, 0x2F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x77, 0x5E)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x78, 0xBC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x79, 0x63)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x7A, 0xC6)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x7B, 0x97)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x7C, 0x35)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x7D, 0x6A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x7E, 0xD4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x7F, 0xB3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x80, 0x7D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x81, 0xFA)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x82, 0xEF)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x83, 0xC5)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x84, 0x91)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x85, 0x39)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x86, 0x72)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x87, 0xE4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x88, 0xD3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x89, 0xBD)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x8A, 0x61)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x8B, 0xC2)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x8C, 0x9F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x8D, 0x25)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x8E, 0x4A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x8F, 0x94)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x90, 0x33)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x91, 0x66)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x92, 0xCC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x93, 0x83)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x94, 0x1D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x95, 0x3A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x96, 0x74)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x97, 0xE8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x98, 0xCB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x99, 0x8D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x9A, 0x01)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x9B, 0x02)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x9C, 0x04)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x9D, 0x08)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x9E, 0x10)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x9F, 0x20)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA0, 0x40)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA1, 0x80)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA2, 0x1B)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA3, 0x36)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA4, 0x6C)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA5, 0xD8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA6, 0xAB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA7, 0x4D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA8, 0x9A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA9, 0x2F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xAA, 0x5E)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xAB, 0xBC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xAC, 0x63)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xAD, 0xC6)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xAE, 0x97)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xAF, 0x35)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB0, 0x6A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB1, 0xD4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB2, 0xB3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB3, 0x7D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB4, 0xFA)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB5, 0xEF)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB6, 0xC5)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB7, 0x91)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB8, 0x39)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB9, 0x72)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xBA, 0xE4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xBB, 0xD3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xBC, 0xBD)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xBD, 0x61)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xBE, 0xC2)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xBF, 0x9F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC0, 0x25)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC1, 0x4A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC2, 0x94)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC3, 0x33)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC4, 0x66)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC5, 0xCC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC6, 0x83)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC7, 0x1D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC8, 0x3A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC9, 0x74)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xCA, 0xE8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xCB, 0xCB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xCC, 0x8D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xCD, 0x01)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xCE, 0x02)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xCF, 0x04)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD0, 0x08)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD1, 0x10)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD2, 0x20)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD3, 0x40)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD4, 0x80)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD5, 0x1B)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD6, 0x36)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD7, 0x6C)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD8, 0xD8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD9, 0xAB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xDA, 0x4D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xDB, 0x9A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xDC, 0x2F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xDD, 0x5E)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xDE, 0xBC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xDF, 0x63)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE0, 0xC6)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE1, 0x97)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE2, 0x35)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE3, 0x6A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE4, 0xD4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE5, 0xB3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE6, 0x7D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE7, 0xFA)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE8, 0xEF)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE9, 0xC5)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xEA, 0x91)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xEB, 0x39)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xEC, 0x72)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xED, 0xE4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xEE, 0xD3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xEF, 0xBD)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF0, 0x61)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF1, 0xC2)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF2, 0x9F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF3, 0x25)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF4, 0x4A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF5, 0x94)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF6, 0x33)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF7, 0x66)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF8, 0xCC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF9, 0x83)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xFA, 0x1D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xFB, 0x3A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xFC, 0x74)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xFD, 0xE8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xFE, 0xCB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xFF, 0x8D)

} // namespace mckl::internal

/// \brief Default ARS constants
/// \ingroup AESNI
class ARSConstants
{
    public:
    static constexpr std::uint64_t weyl[2] = {
        UINT64_C(0x9E3779B97F4A7C15), UINT64_C(0xBB67AE8584CAA73B)};
}; // class ARSConstants

namespace internal
{

template <std::size_t Rounds, typename KeySeqGenerator>
class AESKeySeq
{
    public:
    static constexpr std::size_t rounds = Rounds;

    using key_type = typename KeySeqGenerator::key_type;

    key_type key() const { return KeySeqGenerator::key(key_seq_); }

    void reset(const key_type &key)
    {
        KeySeqGenerator generator;
        generator(key, key_seq_);
    }

    const std::array<__m128i, rounds + 1> &operator()(
        std::array<__m128i, rounds + 1> &) const
    {
        return key_seq_;
    }

    friend bool operator==(const AESKeySeq<rounds, KeySeqGenerator> &seq1,
        const AESKeySeq<rounds, KeySeqGenerator> &seq2)
    {
        alignas(16) std::array<std::uint64_t, 2 * (rounds + 1)> ks1;
        alignas(16) std::array<std::uint64_t, 2 * (rounds + 1)> ks2;
        std::memcpy(ks1.data(), seq1.key_seq_.data(), 16 * (rounds + 1));
        std::memcpy(ks2.data(), seq2.key_seq_.data(), 16 * (rounds + 1));

        return ks1 == ks2;
    }

    friend bool operator!=(const AESKeySeq<rounds, KeySeqGenerator> &seq1,
        const AESKeySeq<rounds, KeySeqGenerator> &seq2)
    {
        return !(seq1 == seq2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const AESKeySeq<rounds, KeySeqGenerator> &seq)
    {
        if (!os)
            return os;

        alignas(16) std::array<std::uint64_t, 2 * (rounds + 1)> ks;
        std::memcpy(ks.data(), seq.key_seq_.data(), 16 * (rounds + 1));
        ostream(os, ks);

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        AESKeySeq<rounds, KeySeqGenerator> &seq)
    {
        if (!is)
            return is;

        alignas(16) std::array<std::uint64_t, 2 * (rounds + 1)> ks;
        istream(is, ks);
        if (is)
            std::memcpy(seq.key_seq_.data(), ks.data(), 16 * (rounds + 1));

        return is;
    }

    private:
    std::array<__m128i, rounds + 1> key_seq_;
}; // class AESKeySeq

class AES128KeySeqGenerator
{
    public:
    using key_type = std::array<std::uint64_t, 2>;

    template <std::size_t Rp1>
    static key_type key(const std::array<__m128i, Rp1> &rk)
    {
        key_type key;
        _mm_storeu_si128(
            reinterpret_cast<__m128i *>(key.data()), std::get<0>(rk));

        return key;
    }

    template <std::size_t Rp1>
    void operator()(const key_type &key, std::array<__m128i, Rp1> &rk)
    {
        xmm1_ = _mm_loadu_si128(reinterpret_cast<const __m128i *>(key.data()));
        std::get<0>(rk) = xmm1_;
        generate_seq<1>(rk, std::integral_constant<bool, 1 < Rp1>());
    }

    private:
    __m128i xmm1_;
    __m128i xmm2_;
    __m128i xmm3_;

    template <std::size_t, std::size_t Rp1>
    void generate_seq(std::array<__m128i, Rp1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_seq(std::array<__m128i, Rp1> &rk, std::true_type)
    {
        xmm2_ = AESKeyGenAssist<N % 256>(xmm1_);
        expand_key();
        std::get<N>(rk) = xmm1_;
        generate_seq<N + 1>(rk, std::integral_constant<bool, N + 1 < Rp1>());
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
}; // class AES128KeySeq

class AES192KeySeqGenerator
{
    public:
    using key_type = std::array<std::uint64_t, 3>;

    template <std::size_t Rp1>
    static key_type key(const std::array<__m128i, Rp1> &rk)
    {
        key_type key;
        std::array<std::uint64_t, 2> tmp;
        _mm_storeu_si128(
            reinterpret_cast<__m128i *>(key.data()), std::get<0>(rk));
        _mm_storeu_si128(
            reinterpret_cast<__m128i *>(tmp.data()), std::get<1>(rk));
        key.back() = tmp.front();

        return key;
    }

    template <std::size_t Rp1>
    void operator()(const key_type &key, std::array<__m128i, Rp1> &rk)
    {
        std::array<std::uint64_t, 2> tmp = {{std::get<2>(key), 0}};
        xmm1_ = _mm_loadu_si128(reinterpret_cast<const __m128i *>(key.data()));
        xmm7_ = _mm_loadu_si128(reinterpret_cast<const __m128i *>(tmp.data()));
        std::get<0>(rk) = xmm1_;
        std::get<1>(rk) = xmm7_;

        xmm3_ = _mm_setzero_si128();
        xmm6_ = _mm_setzero_si128();
        xmm4_ = _mm_shuffle_epi32(xmm7_, 0x4F); // pshufd xmm4, xmm7, 0x4F

        std::array<unsigned char, Rp1 * 16 + 16> rk_tmp;
        generate_seq<1, Rp1>(
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
    void generate_seq(unsigned char *, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_seq(unsigned char *rk_ptr, std::true_type)
    {
        generate_key<N>(rk_ptr);
        complete_key<N>(
            rk_ptr, std::integral_constant<bool, N * 24 + 16 < Rp1 * 16>());
        generate_seq<N + 1, Rp1>(
            rk_ptr, std::integral_constant<bool, N * 24 + 24 < Rp1 * 16>());
    }

    template <std::size_t N>
    void generate_key(unsigned char *rk_ptr)
    {
        // In entry, N * 24 < Rp1 * 16
        // Required Storage: N * 24 + 16;

        xmm2_ = AESKeyGenAssist<N % 256>(xmm4_);
        generate_key_expansion();
        _mm_storeu_si128(reinterpret_cast<__m128i *>(rk_ptr + N * 24), xmm1_);
    }

    template <std::size_t>
    void complete_key(unsigned char *, std::false_type)
    {
    }

    template <std::size_t N>
    void complete_key(unsigned char *rk_ptr, std::true_type)
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
    void copy_key(
        std::array<__m128i, Rp1> &, const unsigned char *, std::false_type)
    {
    }

    template <std::size_t Rp1>
    void copy_key(std::array<__m128i, Rp1> &rk, const unsigned char *rk_ptr,
        std::true_type)
    {
        unsigned char *dst = reinterpret_cast<unsigned char *>(rk.data());
        std::memcpy(dst + 24, rk_ptr + 24, Rp1 * 16 - 24);
    }
}; // class AES192KeySeq

class AES256KeySeqGenerator
{
    public:
    using key_type = std::array<std::uint64_t, 4>;

    template <std::size_t Rp1>
    static key_type key(const std::array<__m128i, Rp1> &rk)
    {
        key_type key;
        _mm_storeu_si128(
            reinterpret_cast<__m128i *>(key.data()), std::get<0>(rk));
        _mm_storeu_si128(
            reinterpret_cast<__m128i *>(key.data() + 2), std::get<1>(rk));

        return key;
    }

    template <std::size_t Rp1>
    void operator()(const key_type &key, std::array<__m128i, Rp1> &rk)
    {
        xmm1_ = _mm_loadu_si128(reinterpret_cast<const __m128i *>(key.data()));
        xmm3_ =
            _mm_loadu_si128(reinterpret_cast<const __m128i *>(key.data() + 2));
        std::get<0>(rk) = xmm1_;
        std::get<1>(rk) = xmm3_;
        generate_seq<2>(rk, std::integral_constant<bool, 2 < Rp1>());
    }

    private:
    __m128i xmm1_;
    __m128i xmm2_;
    __m128i xmm3_;
    __m128i xmm4_;

    template <std::size_t, std::size_t Rp1>
    void generate_seq(std::array<__m128i, Rp1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_seq(std::array<__m128i, Rp1> &rk, std::true_type)
    {
        generate_key<N>(rk, std::integral_constant<bool, N % 2 == 0>());
        generate_seq<N + 1>(rk, std::integral_constant<bool, N + 1 < Rp1>());
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_key(std::array<__m128i, Rp1> &rk, std::true_type)
    {
        xmm2_ = AESKeyGenAssist<(N / 2) % 256>(xmm3_);
        expand_key(std::true_type());
        std::get<N>(rk) = xmm1_;
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_key(std::array<__m128i, Rp1> &rk, std::false_type)
    {
        xmm4_ = _mm_aeskeygenassist_si128(xmm1_, 0);
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
}; // class AESKey256

template <std::size_t Rounds, typename Constants>
class ARSKeySeqImpl
{
    public:
    static constexpr std::size_t rounds = Rounds;

    using key_type = std::array<std::uint64_t, 2>;

    ARSKeySeqImpl() : key_(_mm_setzero_si128()) {}

    key_type key() const
    {
        key_type key;
        _mm_storeu_si128(reinterpret_cast<__m128i *>(key.data()), key_);

        return key;
    }

    void reset(const key_type &key)
    {
        key_ = _mm_loadu_si128(reinterpret_cast<const __m128i *>(key.data()));
    }

    const std::array<__m128i, rounds + 1> &operator()(
        std::array<__m128i, rounds + 1> &rk) const
    {
        static constexpr std::uint64_t w0 = Constants::weyl[0];
        static constexpr std::uint64_t w1 = Constants::weyl[1];

        std::array<std::uint64_t, 2> tmp = {{w0, w1}};
        __m128i w =
            _mm_loadu_si128(reinterpret_cast<const __m128i *>(tmp.data()));
        std::get<0>(rk) = key_;
        generate<1>(rk, w, std::integral_constant<bool, 0 < rounds>());

        return rk;
    }

    friend bool operator==(
        const ARSKeySeqImpl &seq1, const ARSKeySeqImpl &seq2)
    {
        alignas(16) key_type k1;
        alignas(16) key_type k2;
        _mm_store_si128(reinterpret_cast<__m128i *>(k1.data()), seq1.key_);
        _mm_store_si128(reinterpret_cast<__m128i *>(k2.data()), seq2.key_);

        return k1 == k2;
    }

    friend bool operator!=(
        const ARSKeySeqImpl &seq1, const ARSKeySeqImpl &seq2)
    {
        return !(seq1 == seq2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os, const ARSKeySeqImpl &seq)
    {
        if (!os)
            return os;

        alignas(16) key_type k;
        _mm_store_si128(reinterpret_cast<__m128i *>(k.data()), seq.key_);
        ostream(os, k);

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is, ARSKeySeqImpl &seq)
    {
        if (!is)
            return is;

        alignas(16) key_type k = {{0}};
        istream(is, k);
        if (is) {
            seq.key_ =
                _mm_load_si128(reinterpret_cast<const __m128i *>(k.data()));
        }

        return is;
    }

    private:
    template <std::size_t>
    void generate(std::array<__m128i, rounds + 1> &, const __m128i &,
        std::false_type) const
    {
    }

    template <std::size_t N>
    void generate(std::array<__m128i, rounds + 1> &rk, const __m128i &w,
        std::true_type) const
    {
        std::get<N>(rk) = _mm_add_epi64(std::get<N - 1>(rk), w);
        generate<N + 1>(rk, w, std::integral_constant < bool, N<rounds>());
    }

    private:
    __m128i key_;
}; // class ARSKeySeqImpl

} // namespace mckl::internal

/// \brief AES128Engine key sequence generator
/// \ingroup AESNI
using AES128KeySeq = internal::AESKeySeq<10, internal::AES128KeySeqGenerator>;

/// \brief AES192Engine key sequence generator
/// \ingroup AESNI
using AES192KeySeq = internal::AESKeySeq<12, internal::AES192KeySeqGenerator>;

/// \brief AES256Engine key sequence generator
/// \ingroup AESNI
using AES256KeySeq = internal::AESKeySeq<14, internal::AES256KeySeqGenerator>;

/// \brief Default ARSEngine key sequence generator
/// \ingroup AESNI
///
/// \tparam Rounds Number of rounds of AES encryption
/// \tparam Constants A trait class that defines algorithm constants, see
/// ARSConstants
///
/// \details
/// This generator implement the ARS algorithm in
/// [Random123](http://www.deshawresearch.com/resources_random123.html),
/// developed John K. Salmon, Mark A. Moraes, Ron O. Dror, and David E. Shaw.
template <std::size_t Rounds, typename Constants = ARSConstants>
using ARSKeySeq = internal::ARSKeySeqImpl<Rounds, Constants>;

/// \brief AES-128 RNG engine
/// \ingroup AESNI
template <typename ResultType, std::size_t Blocks = MCKL_AESNI_BLOCKS>
using AES128Engine = AESNIEngine<ResultType, AES128KeySeq, Blocks>;

/// \brief AES-192 RNG engine
/// \ingroup AESNI
template <typename ResultType, std::size_t Blocks = MCKL_AESNI_BLOCKS>
using AES192Engine = AESNIEngine<ResultType, AES192KeySeq, Blocks>;

/// \brief AES-256 RNG engine
/// \ingroup AESNI
template <typename ResultType, std::size_t Blocks = MCKL_AESNI_BLOCKS>
using AES256Engine = AESNIEngine<ResultType, AES256KeySeq, Blocks>;

/// \brief ARS RNG engine
/// \ingroup AESNI
template <typename ResultType, std::size_t Rounds = MCKL_ARS_ROUNDS,
    std::size_t Blocks = MCKL_AESNI_BLOCKS, typename Constants = ARSConstants>
using ARSEngine =
    AESNIEngine<ResultType, ARSKeySeq<Rounds, Constants>, Blocks>;

/// \brief AES-128 RNG engine with 32-bit integers output, 1 block
/// \ingroup AESNI
using AES128x1 = AES128Engine<std::uint32_t, 1>;

/// \brief AES-128 RNG engine with 32-bit integers output, 2 blocks
/// \ingroup AESNI
using AES128x2 = AES128Engine<std::uint32_t, 2>;

/// \brief AES-128 RNG engine with 32-bit integers output, 4 blocks
/// \ingroup AESNI
using AES128x4 = AES128Engine<std::uint32_t, 4>;

/// \brief AES-128 RNG engine with 32-bit integers output, 8 blocks
/// \ingroup AESNI
using AES128x8 = AES128Engine<std::uint32_t, 8>;

/// \brief AES-128 RNG engine with 64-bit integers output, 1 block
/// \ingroup AESNI
using AES128x1_64 = AES128Engine<std::uint64_t, 1>;

/// \brief AES-128 RNG engine with 64-bit integers output, 2 blocks
/// \ingroup AESNI
using AES128x2_64 = AES128Engine<std::uint64_t, 2>;

/// \brief AES-128 RNG engine with 64-bit integers output, 4 blocks
/// \ingroup AESNI
using AES128x4_64 = AES128Engine<std::uint64_t, 4>;

/// \brief AES-128 RNG engine with 64-bit integers output, 8 blocks
/// \ingroup AESNI
using AES128x8_64 = AES128Engine<std::uint64_t, 8>;

/// \brief AES-128 RNG engine with 32-bit integers output
using AES128 = AES128x8;

/// \brief AES-128 RNG engine with 64-bit integers output
/// \ingroup AESNI
using AES128_64 = AES128x8_64;

/// \brief AES-192 RNG engine with 32-bit integers output, 1 block
/// \ingroup AESNI
using AES192x1 = AES192Engine<std::uint32_t, 1>;

/// \brief AES-192 RNG engine with 32-bit integers output, 2 blocks
/// \ingroup AESNI
using AES192x2 = AES192Engine<std::uint32_t, 2>;

/// \brief AES-192 RNG engine with 32-bit integers output, 4 blocks
/// \ingroup AESNI
using AES192x4 = AES192Engine<std::uint32_t, 4>;

/// \brief AES-192 RNG engine with 32-bit integers output, 8 blocks
/// \ingroup AESNI
using AES192x8 = AES192Engine<std::uint32_t, 8>;

/// \brief AES-192 RNG engine with 64-bit integers output, 1 block
/// \ingroup AESNI
using AES192x1_64 = AES192Engine<std::uint64_t, 1>;

/// \brief AES-192 RNG engine with 64-bit integers output, 2 blocks
/// \ingroup AESNI
using AES192x2_64 = AES192Engine<std::uint64_t, 2>;

/// \brief AES-192 RNG engine with 64-bit integers output, 4 blocks
/// \ingroup AESNI
using AES192x4_64 = AES192Engine<std::uint64_t, 4>;

/// \brief AES-192 RNG engine with 64-bit integers output, 8 blocks
/// \ingroup AESNI
using AES192x8_64 = AES192Engine<std::uint64_t, 8>;

/// \brief AES-192 RNG engine with 32-bit integers output
/// \ingroup AESNI
using AES192 = AES192x8;

/// \brief AES-192 RNG engine with 64-bit integers output
/// \ingroup AESNI
using AES192_64 = AES192x8_64;

/// \brief AES-256 RNG engine with 32-bit integers output, 1 block
/// \ingroup AESNI
using AES256x1 = AES256Engine<std::uint32_t, 1>;

/// \brief AES-256 RNG engine with 32-bit integers output, 2 blocks
/// \ingroup AESNI
using AES256x2 = AES256Engine<std::uint32_t, 2>;

/// \brief AES-256 RNG engine with 32-bit integers output, 4 blocks
/// \ingroup AESNI
using AES256x4 = AES256Engine<std::uint32_t, 4>;

/// \brief AES-256 RNG engine with 32-bit integers output, 8 blocks
/// \ingroup AESNI
using AES256x8 = AES256Engine<std::uint32_t, 8>;

/// \brief AES-256 RNG engine with 64-bit integers output, 1 block
/// \ingroup AESNI
using AES256x1_64 = AES256Engine<std::uint64_t, 1>;

/// \brief AES-256 RNG engine with 64-bit integers output, 2 blocks
/// \ingroup AESNI
using AES256x2_64 = AES256Engine<std::uint64_t, 2>;

/// \brief AES-256 RNG engine with 64-bit integers output, 4 blocks
/// \ingroup AESNI
using AES256x4_64 = AES256Engine<std::uint64_t, 4>;

/// \brief AES-256 RNG engine with 64-bit integers output, 8 blocks
/// \ingroup AESNI
using AES256x8_64 = AES256Engine<std::uint64_t, 8>;

/// \brief AES-256 RNG engine with 32-bit integers output
/// \ingroup AESNI
using AES256 = AES256x8;

/// \brief AES-256 RNG engine with 64-bit integers output
/// \ingroup AESNI
using AES256_64 = AES256x8_64;

/// \brief ARS RNG engine with 32-bit integers output, 1 block and default
/// rounds
/// \ingroup AESNI
using ARSx1 = ARSEngine<std::uint32_t, MCKL_ARS_ROUNDS, 1>;

/// \brief ARS RNG engine with 32-bit integers output, 2 blocks and default
/// rounds
/// \ingroup AESNI
using ARSx2 = ARSEngine<std::uint32_t, MCKL_ARS_ROUNDS, 2>;

/// \brief ARS RNG engine with 32-bit integers output, 4 blocks and default
/// rounds
/// \ingroup AESNI
using ARSx4 = ARSEngine<std::uint32_t, MCKL_ARS_ROUNDS, 4>;

/// \brief ARS RNG engine with 32-bit integers output, 8 blocks and default
/// rounds
/// \ingroup AESNI
using ARSx8 = ARSEngine<std::uint32_t, MCKL_ARS_ROUNDS, 8>;

/// \brief ARS RNG engine with 64-bit integers output, 1 block and default
/// rounds
/// \ingroup AESNI
using ARSx1_64 = ARSEngine<std::uint64_t, MCKL_ARS_ROUNDS, 1>;

/// \brief ARS RNG engine with 64-bit integers output, 2 blocks and default
/// rounds
/// \ingroup AESNI
using ARSx2_64 = ARSEngine<std::uint64_t, MCKL_ARS_ROUNDS, 2>;

/// \brief ARS RNG engine with 64-bit integers output, 4 blocks and default
/// rounds
/// \ingroup AESNI
using ARSx4_64 = ARSEngine<std::uint64_t, MCKL_ARS_ROUNDS, 4>;

/// \brief ARS RNG engine with 64-bit integers output, 8 blocks and default
/// rounds
/// \ingroup AESNI
using ARSx8_64 = ARSEngine<std::uint64_t, MCKL_ARS_ROUNDS, 8>;

/// \brief ARS RNG engine with 32-bit integers output, default blocks and
/// default rounds
/// \ingroup AESNI
using ARS = ARSx8;

/// \brief ARS RNG engine with 64-bit integers output, default blocks and
/// default rounds
/// \ingroup AESNI
using ARS_64 = ARSx8_64;

} // namespace mckl

#ifdef MCKL_GCC
#if __GNUC__ >= 6
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_AESNI_HPP
