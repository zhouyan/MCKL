//============================================================================
// MCKL/include/mckl/random/aesni_generic.hpp
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

template <typename KeySeqType>
class AESNIGeneratorGenericImpl
{
    public:
    static constexpr std::size_t blocks() { return 8; }

    static void eval(__m128i &state,
        const std::array<__m128i, KeySeqType::rounds() + 1> &rk)
    {
        encfirst<0>(state, rk);
        enc<0x1>(state, rk);
        enc<0x2>(state, rk);
        enc<0x3>(state, rk);
        enc<0x4>(state, rk);
        enc<0x5>(state, rk);
        enc<0x6>(state, rk);
        enc<0x7>(state, rk);
        enc<0x8>(state, rk);
        enc<0x9>(state, rk);
        enc<0xA>(state, rk);
        enc<0xB>(state, rk);
        enc<0xC>(state, rk);
        enc<0xD>(state, rk);
        enc<0xE>(state, rk);
        enc<0xF>(state, rk);
        eval<0x10>(state, rk, std::integral_constant<bool, 0x10 < rounds_>());
        enclast<rounds_>(state, rk);
    }

    static void eval(std::array<__m128i, blocks()> &state,
        const std::array<__m128i, KeySeqType::rounds() + 1> &rk)
    {
        encfirst<0>(state, rk);
        enc<0x01>(state, rk);
        enc<0x02>(state, rk);
        enc<0x03>(state, rk);
        enc<0x04>(state, rk);
        enc<0x05>(state, rk);
        enc<0x06>(state, rk);
        enc<0x07>(state, rk);
        enc<0x08>(state, rk);
        enc<0x09>(state, rk);
        enc<0x0A>(state, rk);
        enc<0x0B>(state, rk);
        enc<0x0C>(state, rk);
        enc<0x0D>(state, rk);
        enc<0x0E>(state, rk);
        enc<0x0F>(state, rk);
        eval<0x10>(state, rk, std::integral_constant<bool, 0x10 < rounds_>());
        enclast<rounds_>(state, rk);
    }

    private:
    static constexpr std::size_t rounds_ = KeySeqType::rounds();

    template <std::size_t>
    static void eval(
        __m128i &, const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(
        __m128i &s, const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        enc<N>(s, rk);
        eval<N + 1>(s, rk, std::integral_constant<bool, N + 1 < rounds_>());
    }

    template <std::size_t>
    static void eval(std::array<__m128i, blocks()> &,
        const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<__m128i, blocks()> &s,
        const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        enc<N>(s, rk);
        eval<N + 1>(s, rk, std::integral_constant<bool, N + 1 < rounds_>());
    }

    template <std::size_t N>
    static void encfirst(
        __m128i &s, const std::array<__m128i, rounds_ + 1> &rk)
    {
        encfirst<N>(s, rk, std::integral_constant<bool, N == 0>());
    }

    template <std::size_t>
    static void encfirst(
        __m128i &, const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void encfirst(
        __m128i &s, const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        s = _mm_xor_si128(s, std::get<N>(rk));
    }

    template <std::size_t N>
    static void enc(__m128i &s, const std::array<__m128i, rounds_ + 1> &rk)
    {
        enc<N>(s, rk, std::integral_constant<bool, (N > 0 && N < rounds_)>());
    }

    template <std::size_t>
    static void enc(
        __m128i &, const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void enc(
        __m128i &s, const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        s = _mm_aesenc_si128(s, std::get<N>(rk));
    }

    template <std::size_t N>
    static void enclast(__m128i &s, const std::array<__m128i, rounds_ + 1> &rk)
    {
        enclast<N>(s, rk, std::integral_constant<bool, N == rounds_>());
    }

    template <std::size_t>
    static void enclast(
        __m128i &, const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void enclast(
        __m128i &s, const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        s = _mm_aesenclast_si128(s, std::get<N>(rk));
    }

    template <std::size_t N>
    static void encfirst(std::array<__m128i, blocks()> &s,
        const std::array<__m128i, rounds_ + 1> &rk)
    {
        encfirst<N>(s, rk, std::integral_constant<bool, N == 0>());
    }

    template <std::size_t>
    static void encfirst(std::array<__m128i, blocks()> &,
        const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void encfirst(std::array<__m128i, blocks()> &s,
        const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        const __m128i k = std::get<N>(rk);
        std::get<0>(s) = _mm_xor_si128(std::get<0>(s), k);
        std::get<1>(s) = _mm_xor_si128(std::get<1>(s), k);
        std::get<2>(s) = _mm_xor_si128(std::get<2>(s), k);
        std::get<3>(s) = _mm_xor_si128(std::get<3>(s), k);
        std::get<4>(s) = _mm_xor_si128(std::get<4>(s), k);
        std::get<5>(s) = _mm_xor_si128(std::get<5>(s), k);
        std::get<6>(s) = _mm_xor_si128(std::get<6>(s), k);
        std::get<7>(s) = _mm_xor_si128(std::get<7>(s), k);
    }

    template <std::size_t N>
    static void enc(std::array<__m128i, blocks()> &s,
        const std::array<__m128i, rounds_ + 1> &rk)
    {
        enc<N>(s, rk, std::integral_constant<bool, (N > 0 && N < rounds_)>());
    }

    template <std::size_t>
    static void enc(std::array<__m128i, blocks()> &,
        const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void enc(std::array<__m128i, blocks()> &s,
        const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        const __m128i k = std::get<N>(rk);
        std::get<0>(s) = _mm_aesenc_si128(std::get<0>(s), k);
        std::get<1>(s) = _mm_aesenc_si128(std::get<1>(s), k);
        std::get<2>(s) = _mm_aesenc_si128(std::get<2>(s), k);
        std::get<3>(s) = _mm_aesenc_si128(std::get<3>(s), k);
        std::get<4>(s) = _mm_aesenc_si128(std::get<4>(s), k);
        std::get<5>(s) = _mm_aesenc_si128(std::get<5>(s), k);
        std::get<6>(s) = _mm_aesenc_si128(std::get<6>(s), k);
        std::get<7>(s) = _mm_aesenc_si128(std::get<7>(s), k);
    }

    template <std::size_t N>
    static void enclast(std::array<__m128i, blocks()> &s,
        const std::array<__m128i, rounds_ + 1> &rk)
    {
        enclast<N>(s, rk, std::integral_constant<bool, N == rounds_>());
    }

    template <std::size_t>
    static void enclast(std::array<__m128i, blocks()> &,
        const std::array<__m128i, rounds_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void enclast(std::array<__m128i, blocks()> &s,
        const std::array<__m128i, rounds_ + 1> &rk, std::true_type)
    {
        const __m128i k = std::get<N>(rk);
        std::get<0>(s) = _mm_aesenclast_si128(std::get<0>(s), k);
        std::get<1>(s) = _mm_aesenclast_si128(std::get<1>(s), k);
        std::get<2>(s) = _mm_aesenclast_si128(std::get<2>(s), k);
        std::get<3>(s) = _mm_aesenclast_si128(std::get<3>(s), k);
        std::get<4>(s) = _mm_aesenclast_si128(std::get<4>(s), k);
        std::get<5>(s) = _mm_aesenclast_si128(std::get<5>(s), k);
        std::get<6>(s) = _mm_aesenclast_si128(std::get<6>(s), k);
        std::get<7>(s) = _mm_aesenclast_si128(std::get<7>(s), k);
    }
}; // class AESNIGeneratorImpl

template <typename KeySeqType>
class AESNIGeneratorImpl : public AESNIGeneratorGenericImpl<KeySeqType>
{
}; // class AESNIGeneratorImpl
