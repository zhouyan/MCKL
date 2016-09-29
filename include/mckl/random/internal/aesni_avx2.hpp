//============================================================================
// MCKL/include/mckl/random/aesni_avx2.hpp
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

template <std::size_t Rounds, typename Constants>
class AESNIGeneratorImpl<ARSKeySeq<Rounds, Constants>>
{
    public:
    static constexpr std::size_t blocks()
    {
        return Rounds < 7 ?
            16 :
            AESNIGeneratorGenericImpl<ARSKeySeq<Rounds, Constants>>::blocks();
    }

    static void eval(__m128i &state, const std::array<__m128i, Rounds + 1> &rk)
    {
        AESNIGeneratorGenericImpl<ARSKeySeq<Rounds, Constants>>::eval(
            state, rk);
    }

    static void eval(std::array<__m128i, blocks()> &state,
        const std::array<__m128i, Rounds + 1> &rk)
    {
        eval(state, rk, std::integral_constant<bool, (Rounds < 7)>());
    }

    private:
    static void eval(std::array<__m128i, blocks()> &state,
        const std::array<__m128i, Rounds + 1> &rk, std::false_type)
    {
        AESNIGeneratorGenericImpl<ARSKeySeq<Rounds, Constants>>::eval(
            state, rk);
    }

    static void eval(std::array<__m128i, 16> &state,
        const std::array<__m128i, Rounds + 1> &rk, std::true_type)
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
        eval<0x10>(state, rk, std::integral_constant<bool, 0x10 < Rounds>());
        enclast<Rounds>(state, rk);
    }

    template <std::size_t>
    static void eval(std::array<__m128i, 16> &,
        const std::array<__m128i, Rounds + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<__m128i, 16> &s,
        const std::array<__m128i, Rounds + 1> &rk, std::true_type)
    {
        enc<N>(s, rk);
        eval<N + 1>(s, rk, std::integral_constant<bool, N + 1 < Rounds>());
    }

    template <std::size_t N>
    static void encfirst(
        std::array<__m128i, 16> &s, const std::array<__m128i, Rounds + 1> &rk)
    {
        encfirst<N>(s, rk, std::integral_constant<bool, N == 0>());
    }

    template <std::size_t>
    static void encfirst(std::array<__m128i, 16> &,
        const std::array<__m128i, Rounds + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void encfirst(std::array<__m128i, 16> &s,
        const std::array<__m128i, Rounds + 1> &rk, std::true_type)
    {
        const __m128i k = std::get<N>(rk);
        std::get<0x0>(s) = _mm_xor_si128(std::get<0x0>(s), k);
        std::get<0x1>(s) = _mm_xor_si128(std::get<0x1>(s), k);
        std::get<0x2>(s) = _mm_xor_si128(std::get<0x2>(s), k);
        std::get<0x3>(s) = _mm_xor_si128(std::get<0x3>(s), k);
        std::get<0x4>(s) = _mm_xor_si128(std::get<0x4>(s), k);
        std::get<0x5>(s) = _mm_xor_si128(std::get<0x5>(s), k);
        std::get<0x6>(s) = _mm_xor_si128(std::get<0x6>(s), k);
        std::get<0x7>(s) = _mm_xor_si128(std::get<0x7>(s), k);
        std::get<0x8>(s) = _mm_xor_si128(std::get<0x8>(s), k);
        std::get<0x9>(s) = _mm_xor_si128(std::get<0x9>(s), k);
        std::get<0xA>(s) = _mm_xor_si128(std::get<0xA>(s), k);
        std::get<0xB>(s) = _mm_xor_si128(std::get<0xB>(s), k);
        std::get<0xC>(s) = _mm_xor_si128(std::get<0xC>(s), k);
        std::get<0xD>(s) = _mm_xor_si128(std::get<0xD>(s), k);
        std::get<0xE>(s) = _mm_xor_si128(std::get<0xE>(s), k);
        std::get<0xF>(s) = _mm_xor_si128(std::get<0xF>(s), k);
    }

    template <std::size_t N>
    static void enc(
        std::array<__m128i, 16> &s, const std::array<__m128i, Rounds + 1> &rk)
    {
        enc<N>(s, rk, std::integral_constant<bool, (N > 0 && N < Rounds)>());
    }

    template <std::size_t>
    static void enc(std::array<__m128i, 16> &,
        const std::array<__m128i, Rounds + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void enc(std::array<__m128i, 16> &s,
        const std::array<__m128i, Rounds + 1> &rk, std::true_type)
    {
        const __m128i k = std::get<N>(rk);
        std::get<0x0>(s) = _mm_aesenc_si128(std::get<0x0>(s), k);
        std::get<0x1>(s) = _mm_aesenc_si128(std::get<0x1>(s), k);
        std::get<0x2>(s) = _mm_aesenc_si128(std::get<0x2>(s), k);
        std::get<0x3>(s) = _mm_aesenc_si128(std::get<0x3>(s), k);
        std::get<0x4>(s) = _mm_aesenc_si128(std::get<0x4>(s), k);
        std::get<0x5>(s) = _mm_aesenc_si128(std::get<0x5>(s), k);
        std::get<0x6>(s) = _mm_aesenc_si128(std::get<0x6>(s), k);
        std::get<0x7>(s) = _mm_aesenc_si128(std::get<0x7>(s), k);
        std::get<0x8>(s) = _mm_aesenc_si128(std::get<0x8>(s), k);
        std::get<0x9>(s) = _mm_aesenc_si128(std::get<0x9>(s), k);
        std::get<0xA>(s) = _mm_aesenc_si128(std::get<0xA>(s), k);
        std::get<0xB>(s) = _mm_aesenc_si128(std::get<0xB>(s), k);
        std::get<0xC>(s) = _mm_aesenc_si128(std::get<0xC>(s), k);
        std::get<0xD>(s) = _mm_aesenc_si128(std::get<0xD>(s), k);
        std::get<0xE>(s) = _mm_aesenc_si128(std::get<0xE>(s), k);
        std::get<0xF>(s) = _mm_aesenc_si128(std::get<0xF>(s), k);
    }

    template <std::size_t N>
    static void enclast(
        std::array<__m128i, 16> &s, const std::array<__m128i, Rounds + 1> &rk)
    {
        enclast<N>(s, rk, std::integral_constant<bool, N == Rounds>());
    }

    template <std::size_t>
    static void enclast(std::array<__m128i, 16> &,
        const std::array<__m128i, Rounds + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void enclast(std::array<__m128i, 16> &s,
        const std::array<__m128i, Rounds + 1> &rk, std::true_type)
    {
        const __m128i k = std::get<N>(rk);
        std::get<0x0>(s) = _mm_aesenclast_si128(std::get<0x0>(s), k);
        std::get<0x1>(s) = _mm_aesenclast_si128(std::get<0x1>(s), k);
        std::get<0x2>(s) = _mm_aesenclast_si128(std::get<0x2>(s), k);
        std::get<0x3>(s) = _mm_aesenclast_si128(std::get<0x3>(s), k);
        std::get<0x4>(s) = _mm_aesenclast_si128(std::get<0x4>(s), k);
        std::get<0x5>(s) = _mm_aesenclast_si128(std::get<0x5>(s), k);
        std::get<0x6>(s) = _mm_aesenclast_si128(std::get<0x6>(s), k);
        std::get<0x7>(s) = _mm_aesenclast_si128(std::get<0x7>(s), k);
        std::get<0x8>(s) = _mm_aesenclast_si128(std::get<0x8>(s), k);
        std::get<0x9>(s) = _mm_aesenclast_si128(std::get<0x9>(s), k);
        std::get<0xA>(s) = _mm_aesenclast_si128(std::get<0xA>(s), k);
        std::get<0xB>(s) = _mm_aesenclast_si128(std::get<0xB>(s), k);
        std::get<0xC>(s) = _mm_aesenclast_si128(std::get<0xC>(s), k);
        std::get<0xD>(s) = _mm_aesenclast_si128(std::get<0xD>(s), k);
        std::get<0xE>(s) = _mm_aesenclast_si128(std::get<0xE>(s), k);
        std::get<0xF>(s) = _mm_aesenclast_si128(std::get<0xF>(s), k);
    }
}; // class AESNIGeneratorImpl
