//============================================================================
// MCKL/include/mckl/random/threefry.hpp
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

#ifndef MCKL_RANDOM_THREEFRY_HPP
#define MCKL_RANDOM_THREEFRY_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/counter.hpp>

/// \brief ThreefryGenerator default rounds
/// \ingroup Config
#ifndef MCKL_THREEFRY_ROUNDS
#define MCKL_THREEFRY_ROUNDS 20
#endif

namespace mckl
{

namespace internal
{

template <typename, std::size_t, int>
class ThreefryConstantsImpl;

template <typename T>
class ThreefryConstantsImpl<T, 2, 32>
{
    public:
    static constexpr T parity = UINT32_C(0x1BD11BDA);

    static constexpr int rotate[1][8] = {{13, 15, 26, 6, 17, 29, 16, 24}};

    static constexpr std::size_t permute[2] = {0, 1};
}; // class ThreefryConstantsImpl

template <typename T>
class ThreefryConstantsImpl<T, 4, 32>
{
    public:
    static constexpr T parity = UINT32_C(0x1BD11BDA);

    static constexpr int rotate[2][8] = {
        {10, 11, 13, 23, 6, 17, 25, 18}, {26, 21, 27, 5, 20, 11, 10, 20}};

    static constexpr std::size_t permute[4] = {0, 3, 2, 1};
}; // class ThreefryConstantsImpl

template <typename T>
class ThreefryConstantsImpl<T, 2, 64>
{
    public:
    static constexpr T parity = UINT64_C(0x1BD11BDAA9FC1A22);

    static constexpr int rotate[1][8] = {{16, 42, 12, 31, 16, 32, 24, 21}};

    static constexpr std::size_t permute[2] = {0, 1};
}; // class ThreefryConstantsImpl

template <typename T>
class ThreefryConstantsImpl<T, 4, 64>
{
    public:
    static constexpr T parity = UINT64_C(0x1BD11BDAA9FC1A22);

    static constexpr int rotate[2][8] = {
        {14, 52, 23, 5, 25, 46, 58, 32}, {16, 57, 40, 37, 33, 12, 22, 32}};

    static constexpr std::size_t permute[4] = {0, 3, 2, 1};
}; // class ThreefryConstantsImpl

template <typename T>
class ThreefryConstantsImpl<T, 8, 64>
{
    public:
    static constexpr T parity = UINT64_C(0x1BD11BDAA9FC1A22);

    static constexpr int rotate[4][8] = {{46, 33, 17, 44, 39, 13, 25, 8},
        {36, 27, 49, 9, 30, 50, 29, 35}, {19, 14, 36, 54, 34, 10, 39, 56},
        {37, 42, 39, 56, 24, 17, 43, 22}};

    static constexpr std::size_t permute[8] = {2, 1, 4, 7, 6, 5, 0, 3};
}; // class ThreefryConstantsImpl

template <typename T>
class ThreefryConstantsImpl<T, 16, 64>
{
    public:
    static constexpr T parity = UINT64_C(0x1BD11BDAA9FC1A22);

    static constexpr int rotate[8][8] = {{24, 38, 33, 5, 41, 16, 31, 9},
        {13, 19, 4, 20, 9, 34, 44, 48}, {8, 10, 51, 48, 37, 56, 47, 35},
        {47, 55, 13, 41, 31, 51, 46, 52}, {8, 49, 34, 47, 12, 4, 19, 23},
        {17, 18, 41, 28, 47, 53, 42, 31}, {22, 23, 59, 16, 44, 42, 44, 37},
        {37, 52, 17, 25, 30, 41, 25, 20}};

    static constexpr std::size_t permute[16] = {
        0, 9, 2, 13, 6, 11, 4, 15, 10, 7, 12, 3, 14, 5, 8, 1};
}; // class ThreefryConstantsImpl

} // namespace mckl::internal

/// \brief Default Threefry constants
/// \ingroup Threefry
template <typename T, std::size_t K>
using ThreefryConstants =
    internal::ThreefryConstantsImpl<T, K, std::numeric_limits<T>::digits>;

namespace internal
{

#include <mckl/random/internal/threefry_generic.hpp>

#if MCKL_HAS_AVX2
#include <mckl/random/internal/threefry_avx2.hpp>
#endif

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class ThreefryGeneratorImpl
    : public ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>
{
}; // class PhiloxGeneratorImpl

template <typename T, std::size_t Rounds>
class ThreefryGeneratorImpl<T, 2, Rounds, ThreefryConstants<T, 2>>
#if MCKL_HAS_AVX2
    : public ThreefryGeneratorAVX2Impl<T, 2, Rounds, ThreefryConstants<T, 2>,
          ThreefryGeneratorImpl<T, 2, Rounds, ThreefryConstants<T, 2>>>
#else
    : public ThreefryGeneratorGenericImpl<T, 2, Rounds,
          ThreefryConstants<T, 2>>
#endif
{
    public:
#if MCKL_HAS_AVX2
    static void pbox(std::array<__m256i, 8> &, std::array<__m256i, 8> &) {}
#endif
}; // class ThreefryGeneratorImpl

template <typename T, std::size_t Rounds>
class ThreefryGeneratorImpl<T, 4, Rounds, ThreefryConstants<T, 4>>
#if MCKL_HAS_AVX2
    : public ThreefryGeneratorAVX2Impl<T, 4, Rounds, ThreefryConstants<T, 4>,
          ThreefryGeneratorImpl<T, 4, Rounds, ThreefryConstants<T, 4>>>
#else
    : public ThreefryGeneratorGenericImpl<T, 4, Rounds,
          ThreefryConstants<T, 4>>
#endif
{
#if MCKL_HAS_AVX2
    public:
    static void pbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t)
    {
        pbox(s, t,
            std::integral_constant<int, std::numeric_limits<T>::digits>());
    }

    private:
    static void pbox(std::array<__m256i, 8> &, std::array<__m256i, 8> &t,
        std::integral_constant<int, 64>)
    {
        // s: 2' 2 0' 0
        // t: 3' 3 1' 1

        // 1 0 3 2
        std::get<0>(t) = _mm256_permute4x64_epi64(std::get<0>(t), 0x4E);
        std::get<1>(t) = _mm256_permute4x64_epi64(std::get<1>(t), 0x4E);
        std::get<2>(t) = _mm256_permute4x64_epi64(std::get<2>(t), 0x4E);
        std::get<3>(t) = _mm256_permute4x64_epi64(std::get<3>(t), 0x4E);
        std::get<4>(t) = _mm256_permute4x64_epi64(std::get<4>(t), 0x4E);
        std::get<5>(t) = _mm256_permute4x64_epi64(std::get<5>(t), 0x4E);
        std::get<6>(t) = _mm256_permute4x64_epi64(std::get<6>(t), 0x4E);
        std::get<7>(t) = _mm256_permute4x64_epi64(std::get<7>(t), 0x4E);
    }
#endif
}; // class ThreefryGeneratorImpl

template <typename T, std::size_t Rounds>
class ThreefryGeneratorImpl<T, 8, Rounds, ThreefryConstants<T, 8>>
#if MCKL_HAS_AVX2
    : public ThreefryGeneratorAVX2Impl<T, 8, Rounds, ThreefryConstants<T, 8>,
          ThreefryGeneratorImpl<T, 8, Rounds, ThreefryConstants<T, 8>>>
#else
    : public ThreefryGeneratorGenericImpl<T, 8, Rounds,
          ThreefryConstants<T, 8>>
#endif
{
#if MCKL_HAS_AVX2
    public:
    static void pbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t)
    {
        pbox(s, t,
            std::integral_constant<int, std::numeric_limits<T>::digits>());
    }

    private:
    static void pbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t,
        std::integral_constant<int, 64>)
    {
        // s: 6 2 4 0
        // t; 7 3 5 1

        // 0 1 3 2
        std::get<0>(s) = _mm256_permute4x64_epi64(std::get<0>(s), 0x1E);
        std::get<1>(s) = _mm256_permute4x64_epi64(std::get<1>(s), 0x1E);
        std::get<2>(s) = _mm256_permute4x64_epi64(std::get<2>(s), 0x1E);
        std::get<3>(s) = _mm256_permute4x64_epi64(std::get<3>(s), 0x1E);
        std::get<4>(s) = _mm256_permute4x64_epi64(std::get<4>(s), 0x1E);
        std::get<5>(s) = _mm256_permute4x64_epi64(std::get<5>(s), 0x1E);
        std::get<6>(s) = _mm256_permute4x64_epi64(std::get<6>(s), 0x1E);
        std::get<7>(s) = _mm256_permute4x64_epi64(std::get<7>(s), 0x1E);

        // 2 3 1 0
        std::get<0>(t) = _mm256_permute4x64_epi64(std::get<0>(t), 0xB4);
        std::get<1>(t) = _mm256_permute4x64_epi64(std::get<1>(t), 0xB4);
        std::get<2>(t) = _mm256_permute4x64_epi64(std::get<2>(t), 0xB4);
        std::get<3>(t) = _mm256_permute4x64_epi64(std::get<3>(t), 0xB4);
        std::get<4>(t) = _mm256_permute4x64_epi64(std::get<4>(t), 0xB4);
        std::get<5>(t) = _mm256_permute4x64_epi64(std::get<5>(t), 0xB4);
        std::get<6>(t) = _mm256_permute4x64_epi64(std::get<6>(t), 0xB4);
        std::get<7>(t) = _mm256_permute4x64_epi64(std::get<7>(t), 0xB4);
    }
#endif
}; // class ThreefryGeneratorImpl

template <typename T, std::size_t Rounds>
class ThreefryGeneratorImpl<T, 16, Rounds, ThreefryConstants<T, 16>>
#if MCKL_HAS_AVX2
    : public ThreefryGeneratorAVX2Impl<T, 16, Rounds, ThreefryConstants<T, 16>,
          ThreefryGeneratorImpl<T, 16, Rounds, ThreefryConstants<T, 16>>>
#else
    : public ThreefryGeneratorGenericImpl<T, 16, Rounds,
          ThreefryConstants<T, 16>>
#endif
{
#if MCKL_HAS_AVX2
    public:
    static void pbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t)
    {
        pbox(s, t,
            std::integral_constant<int, std::numeric_limits<T>::digits>());
    }

    private:
    static void pbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t,
        std::integral_constant<int, 64>)
    {
        // s: 6 2 4 0 | 8 12 10 14
        // t; 7 3 5 1 | 9 13 11 15

        // 1 2 3 0
        std::get<0>(s) = _mm256_permute4x64_epi64(std::get<0>(s), 0x6C);
        std::get<2>(s) = _mm256_permute4x64_epi64(std::get<2>(s), 0x6C);
        std::get<4>(s) = _mm256_permute4x64_epi64(std::get<4>(s), 0x6C);
        std::get<6>(s) = _mm256_permute4x64_epi64(std::get<6>(s), 0x6C);

        // 0 1 3 2
        std::get<1>(s) = _mm256_permute4x64_epi64(std::get<1>(s), 0x1E);
        std::get<3>(s) = _mm256_permute4x64_epi64(std::get<3>(s), 0x1E);
        std::get<5>(s) = _mm256_permute4x64_epi64(std::get<5>(s), 0x1E);
        std::get<7>(s) = _mm256_permute4x64_epi64(std::get<7>(s), 0x1E);

        // 3 1 2 0
        __m256i t0 = _mm256_permute4x64_epi64(std::get<1>(t), 0xD8);
        __m256i t2 = _mm256_permute4x64_epi64(std::get<3>(t), 0xD8);
        __m256i t4 = _mm256_permute4x64_epi64(std::get<5>(t), 0xD8);
        __m256i t6 = _mm256_permute4x64_epi64(std::get<7>(t), 0xD8);

        // 0 2 1 3
        std::get<1>(t) = _mm256_permute4x64_epi64(std::get<0>(t), 0x27);
        std::get<3>(t) = _mm256_permute4x64_epi64(std::get<2>(t), 0x27);
        std::get<5>(t) = _mm256_permute4x64_epi64(std::get<4>(t), 0x27);
        std::get<7>(t) = _mm256_permute4x64_epi64(std::get<6>(t), 0x27);

        std::get<0>(t) = t0;
        std::get<2>(t) = t2;
        std::get<4>(t) = t4;
        std::get<6>(t) = t6;
    }
#endif
}; // class ThreefryGeneratorImpl

} // namespace mckl::internal

/// \brief Threefry RNG generator
/// \ingroup Threefry
///
/// \tparam T State type, must be 32- or 64-bit unsigned integers
/// \tparam K State vector length, must be 2 or 4 (for 32- or 64-bit states) or
/// 8 or 16 (64-bit state)
/// \tparam Rounds Number of SP rounds
/// \tparam Constants A trait class that defines algorithm constants, see
/// ThreefryConstants
///
/// \details
/// This generator implement the Threefry algorithm in
/// [Random123](http://www.deshawresearch.com/resources_random123.html),
/// developed John K. Salmon, Mark A. Moraes, Ron O. Dror, and David E. Shaw.
template <typename T, std::size_t K, std::size_t Rounds = MCKL_THREEFRY_ROUNDS,
    typename Constants = ThreefryConstants<T, K>>
class ThreefryGenerator
{
    static_assert(std::is_unsigned<T>::value,
        "**ThreefryGenerator** used with T other than unsigned integer types");

    static_assert(K != 0 && K % 2 == 0,
        "**ThreefryGenerator** used with K other than multiples of 2");

    static_assert(
        Rounds != 0, "**ThreefryGenerator** used with rounds equal to zero");

    public:
    using ctr_type =
        typename std::conditional<(sizeof(T) * K) % sizeof(std::uint64_t) == 0,
            std::array<std::uint64_t, (sizeof(T) * K) / sizeof(std::uint64_t)>,
            std::array<T, K>>::type;
    using key_type = std::array<T, K>;

    static constexpr std::size_t size() { return sizeof(T) * K; }

    key_type key() const
    {
        key_type key;
        std::copy_n(par_.begin(), K, key.begin());

        return key;
    }

    void reset(const key_type &key)
    {
        static constexpr T p = Constants::parity;

        std::copy(key.begin(), key.end(), par_.begin());
        par_.back() = p;
        for (std::size_t i = 0; i != key.size(); ++i)
            par_.back() ^= par_[i];
    }

    void enc(const ctr_type &ctr, ctr_type &buffer) const
    {
        union {
            std::array<T, K> state;
            ctr_type result;
        } buf;

        buf.result = ctr;
        internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, par_);
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
        generate(ctr, n, buffer);
    }

    friend bool operator==(const ThreefryGenerator<T, K, Rounds> &gen1,
        const ThreefryGenerator<T, K, Rounds> &gen2)
    {
        return gen1.par_ == gen2.par_;
    }

    friend bool operator!=(const ThreefryGenerator<T, K, Rounds> &gen1,
        const ThreefryGenerator<T, K, Rounds> &gen2)
    {
        return !(gen1 == gen2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const ThreefryGenerator<T, K, Rounds> &gen)
    {
        if (!os)
            return os;

        os << gen.par_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        ThreefryGenerator<T, K, Rounds> &gen)
    {
        if (!is)
            return is;

        ThreefryGenerator<T, K, Rounds> gen_tmp;
        is >> std::ws >> gen_tmp.par_;

        if (is)
            gen = std::move(gen_tmp);

        return is;
    }

    private:
    std::array<T, K + 1> par_;

    void generate(std::array<T, K> &ctr, std::array<T, K> &buffer) const
    {
        increment(ctr);
        buffer = ctr;
        internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::eval(
            buffer, par_);
    }

    template <typename ResultType>
    void generate(ctr_type &ctr,
        std::array<ResultType, size() / sizeof(ResultType)> &buffer) const
    {
        union {
            std::array<T, K> state;
            ctr_type ctr;
            std::array<ResultType, size() / sizeof(ResultType)> result;
        } buf;

        increment(ctr);
        buf.ctr = ctr;
        internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, par_);
        buffer = buf.result;
    }

    void generate(
        std::array<T, K> &ctr, std::size_t n, std::array<T, K> *buffer) const
    {
        static constexpr std::size_t blocks =
            internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::blocks;

        using state_type = std::array<std::array<T, K>, blocks>;

        const std::size_t m = n / blocks;
        const std::size_t l = n % blocks;
        for (std::size_t i = 0; i != m; ++i, buffer += blocks) {
            state_type &state = *reinterpret_cast<state_type *>(buffer);
            increment(ctr, state);
            internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::eval(
                state, par_);
        }
        for (std::size_t i = 0; i != l; ++i)
            generate(ctr, buffer[i]);
    }

    template <typename ResultType>
    void generate(ctr_type &ctr, std::size_t n,
        std::array<ResultType, size() / sizeof(ResultType)> *buffer) const
    {
        static constexpr std::size_t blocks =
            internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::blocks;

        union {
            std::array<std::array<T, K>, blocks> state;
            std::array<ctr_type, blocks> ctr_block;
        } buf;

        const std::size_t m = n / blocks;
        const std::size_t l = n % blocks;
        for (std::size_t i = 0; i != m; ++i, buffer += blocks) {
            increment(ctr, buf.ctr_block);
            internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::eval(
                buf.state, par_);
            std::memcpy(buffer, buf.state.data(), size() * blocks);
        }
        for (std::size_t i = 0; i != l; ++i)
            generate(ctr, buffer[i]);
    }
}; // class ThreefryGenerator

/// \brief Threefry RNG engine
/// \ingroup Threefry
template <typename ResultType, typename T, std::size_t K,
    std::size_t Rounds = MCKL_THREEFRY_ROUNDS,
    typename Constants = ThreefryConstants<T, K>>
using ThreefryEngine =
    CounterEngine<ResultType, ThreefryGenerator<T, K, Rounds, Constants>>;

/// \brief Threefry2x32 RNG engine
/// \ingroup Threefry
template <typename ResultType>
using Threefry2x32Engine = ThreefryEngine<ResultType, std::uint32_t, 2>;

/// \brief Threefry4x32 RNG engine
/// \ingroup Threefry
template <typename ResultType>
using Threefry4x32Engine = ThreefryEngine<ResultType, std::uint32_t, 4>;

/// \brief Threefry2x64 RNG engine
/// \ingroup Threefry
template <typename ResultType>
using Threefry2x64Engine = ThreefryEngine<ResultType, std::uint64_t, 2>;

/// \brief Threefry4x64 RNG engine
/// \ingroup Threefry
template <typename ResultType>
using Threefry4x64Engine = ThreefryEngine<ResultType, std::uint64_t, 4>;

/// \brief Threefry8x64 RNG engine
/// \ingroup Threefry
template <typename ResultType>
using Threefry8x64Engine = ThreefryEngine<ResultType, std::uint64_t, 8>;

/// \brief Threefry16x64 RNG engine
/// \ingroup Threefry
template <typename ResultType>
using Threefry16x64Engine = ThreefryEngine<ResultType, std::uint64_t, 16>;

/// \brief Threefish-256 RNG engine
template <typename ResultType>
using Threefish256Engine = ThreefryEngine<ResultType, std::uint64_t, 4, 72>;

/// \brief Threefish-512 RNG engine
template <typename ResultType>
using Threefish512Engine = ThreefryEngine<ResultType, std::uint64_t, 8, 72>;

/// \brief Threefish-1024 RNG engine
template <typename ResultType>
using Threefish1024Engine = ThreefryEngine<ResultType, std::uint64_t, 16, 80>;

/// \brief Threefry2x32 RNG engine with 32-bit integer output
/// \ingroup Threefry
using Threefry2x32 = Threefry2x32Engine<std::uint32_t>;

/// \brief Threefry4x32 RNG engine with 32-bit integer output
/// \ingroup Threefry
using Threefry4x32 = Threefry4x32Engine<std::uint32_t>;

/// \brief Threefry2x64 RNG engine with 32-bit integer output
/// \ingroup Threefry
using Threefry2x64 = Threefry2x64Engine<std::uint32_t>;

/// \brief Threefry4x64 RNG engine with 32-bit integer output
/// \ingroup Threefry
using Threefry4x64 = Threefry4x64Engine<std::uint32_t>;

/// \brief Threefry8x64 RNG engine with 32-bit integer output
/// \ingroup Threefry
using Threefry8x64 = Threefry8x64Engine<std::uint32_t>;

/// \brief Threefry16x64 RNG engine with 32-bit integer output
/// \ingroup Threefry
using Threefry16x64 = Threefry16x64Engine<std::uint32_t>;

/// \brief Threefish-256 RNG engine with 32-bit integer output
/// \ingroup Threefry
using Threefish256 = Threefish256Engine<std::uint32_t>;

/// \brief Threefish-512 RNG engine with 32-bit integer output
/// \ingroup Threefry
using Threefish512 = Threefish512Engine<std::uint32_t>;

/// \brief Threefish-1024 RNG engine with 32-bit integer output
/// \ingroup Threefry
using Threefish1024 = Threefish1024Engine<std::uint32_t>;

/// \brief Threefry2x32 RNG engine with 64-bit integer output
/// \ingroup Threefry
using Threefry2x32_64 = Threefry2x32Engine<std::uint64_t>;

/// \brief Threefry4x32 RNG engine with 64-bit integer output
/// \ingroup Threefry
using Threefry4x32_64 = Threefry4x32Engine<std::uint64_t>;

/// \brief Threefry2x64 RNG engine with 64-bit integer output
/// \ingroup Threefry
using Threefry2x64_64 = Threefry2x64Engine<std::uint64_t>;

/// \brief Threefry4x64 RNG engine with 64-bit integer output
/// \ingroup Threefry
using Threefry4x64_64 = Threefry4x64Engine<std::uint64_t>;

/// \brief Threefry8x64 RNG engine with 64-bit integer output
/// \ingroup Threefry
using Threefry8x64_64 = Threefry8x64Engine<std::uint64_t>;

/// \brief Threefry16x64 RNG engine with 64-bit integer output
/// \ingroup Threefry
using Threefry16x64_64 = Threefry16x64Engine<std::uint64_t>;

/// \brief Threefish-256 RNG engine with 64-bit integer output
/// \ingroup Threefry
using Threefish256_64 = Threefish256Engine<std::uint64_t>;

/// \brief Threefish-512 RNG engine with 64-bit integer output
/// \ingroup Threefry
using Threefish512_64 = Threefish512Engine<std::uint64_t>;

/// \brief Threefish-1024 RNG engine with 64-bit integer output
/// \ingroup Threefry
using Threefish1024_64 = Threefish1024Engine<std::uint64_t>;

} // namespace mckl

#endif // MCKL_RANDOM_THREEFRY_HPP
