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

    static constexpr T tweaks[2] = {0, 0};

    static constexpr int rotate[1][8] = {{13, 15, 26, 6, 17, 29, 16, 24}};

    static constexpr std::size_t permute[2] = {0, 1};
}; // class ThreefryConstantsImpl

template <typename T>
class ThreefryConstantsImpl<T, 4, 32>
{
    public:
    static constexpr T parity = UINT32_C(0x1BD11BDA);

    static constexpr T tweaks[2] = {0, 0};

    static constexpr int rotate[2][8] = {
        {10, 11, 13, 23, 6, 17, 25, 18}, {26, 21, 27, 5, 20, 11, 10, 20}};

    static constexpr std::size_t permute[4] = {0, 3, 2, 1};
}; // class ThreefryConstantsImpl

template <typename T>
class ThreefryConstantsImpl<T, 2, 64>
{
    public:
    static constexpr T parity = UINT64_C(0x1BD11BDAA9FC1A22);

    static constexpr T tweaks[2] = {0, 0};

    static constexpr int rotate[1][8] = {{16, 42, 12, 31, 16, 32, 24, 21}};

    static constexpr std::size_t permute[2] = {0, 1};
}; // class ThreefryConstantsImpl

template <typename T>
class ThreefryConstantsImpl<T, 4, 64>
{
    public:
    static constexpr T parity = UINT64_C(0x1BD11BDAA9FC1A22);

    static constexpr T tweaks[2] = {0, 0};

    static constexpr int rotate[2][8] = {
        {14, 52, 23, 5, 25, 46, 58, 32}, {16, 57, 40, 37, 33, 12, 22, 32}};

    static constexpr std::size_t permute[4] = {0, 3, 2, 1};
}; // class ThreefryConstantsImpl

template <typename T>
class ThreefryConstantsImpl<T, 8, 64>
{
    public:
    static constexpr T parity = UINT64_C(0x1BD11BDAA9FC1A22);

    static constexpr T tweaks[2] = {0, 0};

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

    static constexpr T tweaks[2] = {0, 0};

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

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#include <mckl/random/internal/threefry_generic.hpp>
#if MCKL_USE_AVX2
#include <mckl/random/internal/threefry_avx2_32.hpp>
#include <mckl/random/internal/threefry_avx2_64.hpp>
#elif MCKL_USE_SSE2
#include <mckl/random/internal/threefry_sse2_32.hpp>
#include <mckl/random/internal/threefry_sse2_64.hpp>
#endif

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

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
        alignas(32) union {
            std::array<T, K> state;
            ctr_type result;
        } buf;

        buf.result = ctr;
        internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, par_);
        buffer = buf.result;
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, ResultType *buffer) const
    {
        alignas(32) union {
            std::array<T, K> state;
            ctr_type ctr;
            std::array<ResultType, size() / sizeof(ResultType)> result;
        } buf;

        MCKL_FLATTEN_CALL increment(ctr);
        buf.ctr = ctr;
        internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, par_);
        std::copy(buf.result.begin(), buf.result.end(), buffer);
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, std::size_t n, ResultType *buffer) const
    {
        generate(ctr, n, buffer,
            std::integral_constant<bool, internal::ThreefryGeneratorImpl<T, K,
                                             Rounds, Constants>::batch()>());
    }

    friend bool operator==(
        const ThreefryGenerator<T, K, Rounds, Constants> &gen1,
        const ThreefryGenerator<T, K, Rounds, Constants> &gen2)
    {
        return gen1.par_ == gen2.par_;
    }

    friend bool operator!=(
        const ThreefryGenerator<T, K, Rounds, Constants> &gen1,
        const ThreefryGenerator<T, K, Rounds, Constants> &gen2)
    {
        return !(gen1 == gen2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const ThreefryGenerator<T, K, Rounds, Constants> &gen)
    {
        if (!os)
            return os;

        os << gen.par_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        ThreefryGenerator<T, K, Rounds, Constants> &gen)
    {
        if (!is)
            return is;

        ThreefryGenerator<T, K, Rounds, Constants> gen_tmp;
        gen_tmp.par_.fill(0);
        is >> std::ws >> gen_tmp.par_;

        if (is)
            gen = std::move(gen_tmp);

        return is;
    }

    private:
    std::array<T, K + 1> par_;

    template <typename ResultType>
    void generate(ctr_type &ctr, std::size_t n, ResultType *buffer,
        std::false_type) const
    {
        static constexpr std::size_t stride = size() / sizeof(ResultType);

        for (std::size_t i = 0; i != n; ++i, buffer += stride)
            operator()(ctr, buffer);
    }

    template <typename ResultType>
    void generate(
        ctr_type &ctr, std::size_t n, ResultType *buffer, std::true_type) const
    {
        static constexpr std::size_t stride = size() / sizeof(ResultType);
        static constexpr std::size_t blocks =
            internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::blocks();

        alignas(32) union {
            std::array<std::array<T, K>, blocks> state;
            std::array<ctr_type, blocks> ctr_block;
        } buf;

        const std::size_t m = n / blocks;
        const std::size_t l = n % blocks;
        for (std::size_t i = 0; i != m; ++i, buffer += stride * blocks) {
            MCKL_FLATTEN_CALL increment(ctr, buf.ctr_block);
            internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::eval(
                buf.state, par_);
            std::memcpy(buffer, buf.state.data(), size() * blocks);
        }
        for (std::size_t i = 0; i != l; ++i, buffer += stride)
            operator()(ctr, buffer);
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
