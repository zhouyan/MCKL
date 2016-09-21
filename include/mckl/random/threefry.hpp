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

template <typename T, std::size_t K, std::size_t N, typename,
    bool = (N % 4 == 0)>
class ThreefrySubKey
{
    public:
    static void eval(std::array<T, K> &, const std::array<T, K + 1> &) {}
}; // class ThreefrySubKey

template <typename T, std::size_t K, std::size_t N, typename Constants>
class ThreefrySubKey<T, K, N, Constants, true>
{
    public:
    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par)
    {
        eval<0>(state, par, std::integral_constant<bool, 0 < K>());
        state.back() += static_cast<T>(s_);
    }

    private:
    static constexpr std::size_t s_ = N / 4;

    template <std::size_t>
    static void eval(
        std::array<T, K> &, const std::array<T, K + 1> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par,
        std::true_type)
    {
        std::get<I>(state) += std::get<(s_ + I) % (K + 1)>(par);
        eval<I + 1>(state, par, std::integral_constant<bool, I + 1 < K>());
    }
}; // class ThreefrySubKey

template <typename T, std::size_t K, std::size_t N, typename, bool = (N > 0)>
class ThreefrySBox
{
    public:
    static void eval(std::array<T, K> &) {}
}; // class ThreefrySBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class ThreefrySBox<T, K, N, Constants, true>
{
    public:
    static void eval(std::array<T, K> &state)
    {
        eval<0>(state, std::integral_constant<bool, 1 < K>());
    }

    private:
    template <std::size_t>
    static void eval(std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(std::array<T, K> &state, std::true_type)
    {
        static constexpr int L = Constants::rotate[I / 2][(N - 1) % 8];
        static constexpr int R = std::numeric_limits<T>::digits - L;

        T x = std::get<I + 1>(state);
        std::get<I>(state) += x;
        std::get<I + 1>(state) = (x << L) | (x >> R);
        std::get<I + 1>(state) ^= std::get<I>(state);
        eval<I + 2>(state, std::integral_constant<bool, I + 3 < K>());
    }
}; // class ThreefrySBox

template <typename T, std::size_t K, std::size_t N, typename, bool = (N > 0)>
class ThreefryPBox
{
    public:
    static void eval(std::array<T, K> &) {}
}; // class ThreefryPBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class ThreefryPBox<T, K, N, Constants, true>
{
    public:
    static void eval(std::array<T, K> &state)
    {
        std::array<T, K> tmp;
        eval<0>(state, tmp, std::integral_constant<bool, 0 < N>());
        state = tmp;
    }

    private:
    template <std::size_t>
    static void eval(
        const std::array<T, K> &, std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(
        const std::array<T, K> &state, std::array<T, K> &tmp, std::true_type)
    {
        static constexpr std::size_t P = Constants::permute[I];

        std::get<I>(tmp) = std::get<P>(state);
        eval<I + 1>(state, tmp, std::integral_constant<bool, I + 1 < K>());
    }
}; // class ThreefryPBox

template <typename T, std::size_t N>
class ThreefryPBox<T, 2, N, ThreefryConstants<T, 2>, true>
{
    public:
    static void eval(std::array<T, 2> &) {}
}; // class ThreefryPBox

template <typename T, std::size_t N>
class ThreefryPBox<T, 4, N, ThreefryConstants<T, 4>, true>
{
    public:
    static void eval(std::array<T, 4> &state)
    {
        std::swap(std::get<1>(state), std::get<3>(state));
    }
}; // class ThreefryPBox

template <typename T, std::size_t N>
class ThreefryPBox<T, 8, N, ThreefryConstants<T, 8>, true>
{
    public:
    static void eval(std::array<T, 8> &state)
    {
        T x0 = std::get<0>(state);
        T x3 = std::get<3>(state);
        std::get<0>(state) = std::get<2>(state);
        std::get<2>(state) = std::get<4>(state);
        std::get<3>(state) = std::get<7>(state);
        std::get<4>(state) = std::get<6>(state);
        std::get<6>(state) = x0;
        std::get<7>(state) = x3;
    }
}; // class ThreefryPBox

template <typename T, std::size_t N>
class ThreefryPBox<T, 16, N, ThreefryConstants<T, 16>, true>
{
    public:
    static void eval(std::array<T, 16> &state)
    {
        T x1 = std::get<1>(state);
        T x3 = std::get<3>(state);
        T x4 = std::get<4>(state);
        T x5 = std::get<5>(state);
        T x7 = std::get<7>(state);
        T x8 = std::get<8>(state);
        std::get<1>(state) = std::get<9>(state);
        std::get<3>(state) = std::get<13>(state);
        std::get<4>(state) = std::get<6>(state);
        std::get<5>(state) = std::get<11>(state);
        std::get<6>(state) = x4;
        std::get<7>(state) = std::get<15>(state);
        std::get<8>(state) = std::get<10>(state);
        std::get<9>(state) = x7;
        std::get<10>(state) = std::get<12>(state);
        std::get<11>(state) = x3;
        std::get<12>(state) = std::get<14>(state);
        std::get<13>(state) = x5;
        std::get<14>(state) = x8;
        std::get<15>(state) = x1;
    }
}; // class ThreefryPBox

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
    using ctr_type = std::array<T, K>;
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
        generate<0>(buf.state, par_, std::true_type());
        buffer = buf.result;
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr,
        std::array<ResultType, size() / sizeof(ResultType)> &buffer) const
    {
        union {
            std::array<T, K> state;
            ctr_type ctr;
            std::array<ResultType, size() / sizeof(ResultType)> result;
        } buf;

        increment(ctr);
        buf.ctr = ctr;
        generate<0>(buf.state, par_, std::true_type());
        buffer = buf.result;
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, std::size_t n,
        std::array<ResultType, size() / sizeof(ResultType)> *buffer) const
    {
        union {
            std::array<T, K> state;
            ctr_type ctr;
            std::array<ResultType, size() / sizeof(ResultType)> result;
        } buf;

        for (std::size_t i = 0; i != n; ++i) {
            increment(ctr);
            buf.ctr = ctr;
            generate<0>(buf.state, par_, std::true_type());
            buffer[i] = buf.result;
        }
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
    template <std::size_t>
    void generate(std::array<T, K> &, const std::array<T, K + 1> &,
        std::false_type) const MCKL_ALWAYS_INLINE
    {
    }

    template <std::size_t N>
    void generate(std::array<T, K> &state, const std::array<T, K + 1> &par,
        std::true_type) const MCKL_ALWAYS_INLINE
    {
        internal::ThreefrySBox<T, K, N, Constants>::eval(state);
        internal::ThreefryPBox<T, K, N, Constants>::eval(state);
        internal::ThreefrySubKey<T, K, N, Constants>::eval(state, par);
        generate<N + 1>(
            state, par, std::integral_constant<bool, (N < Rounds)>());
    }

    private:
    std::array<T, K + 1> par_;
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
