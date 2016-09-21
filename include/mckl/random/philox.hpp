//============================================================================
// MCKL/include/mckl/random/philox.hpp
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

#ifndef MCKL_RANDOM_PHILOX_HPP
#define MCKL_RANDOM_PHILOX_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/counter.hpp>
#include <mckl/random/threefry.hpp>

#ifdef MCKL_MSVC
#include <intrin.h>
#endif

/// \brief PhiloxGenerator default rounds
/// \ingroup Config
#ifndef MCKL_PHILOX_ROUNDS
#define MCKL_PHILOX_ROUNDS 10
#endif

namespace mckl
{

namespace internal
{

template <typename, std::size_t, int>
class PhiloxConstantsImpl;

template <typename T>
class PhiloxConstantsImpl<T, 2, 32>
{
    public:
    static constexpr T weyl[2] = {UINT32_C(0x9E3779B9), UINT32_C(0xBB67AE85)};

    static constexpr T multiplier[1] = {UINT32_C(0xD256D193)};
}; // class PhiloxConstantsImpl

template <typename T>
class PhiloxConstantsImpl<T, 4, 32>
{
    public:
    static constexpr T weyl[2] = {UINT32_C(0x9E3779B9), UINT32_C(0xBB67AE85)};

    static constexpr T multiplier[2] = {
        UINT32_C(0xCD9E8D57), UINT32_C(0xD2511F53)};
}; // class PhiloxConstantsImpl

template <typename T>
class PhiloxConstantsImpl<T, 2, 64>
{
    public:
    static constexpr T weyl[2] = {
        UINT64_C(0x9E3779B97F4A7C15), UINT64_C(0xBB67AE8584CAA73B)};

    static constexpr T multiplier[1] = {UINT64_C(0xD2B74407B1CE6E93)};
}; // class PhiloxConstantsImpl

template <typename T>
class PhiloxConstantsImpl<T, 4, 64>
{
    public:
    static constexpr T weyl[2] = {
        UINT64_C(0x9E3779B97F4A7C15), UINT64_C(0xBB67AE8584CAA73B)};

    static constexpr T multiplier[2] = {
        UINT64_C(0xCA5A826395121157), UINT64_C(0xD2E7470EE14C6C93)};
}; // class PhiloxConstantsImpl

} // namespace mckl::internal

/// \brief Default Philox constants
/// \ingroup Philox
template <typename T, std::size_t K>
using PhiloxConstants =
    internal::PhiloxConstantsImpl<T, K, std::numeric_limits<T>::digits>;

namespace internal
{

template <typename T, int = std::numeric_limits<T>::digits>
class PhiloxHiLo;

template <typename T>
class PhiloxHiLo<T, 32>
{
    public:
    static void eval(T a, T b, T &hi, T &lo)
    {
        union {
            std::uint64_t prod;
            std::array<T, 2> result;
        } buf;

        buf.prod =
            static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b);
#if MCKL_HAS_X86 // little endian
        hi = std::get<1>(buf.result);
        lo = std::get<0>(buf.result);
#else  // MCKL_HAS_X86
        hi = static_cast<T>(buf.prod >> 32);
        lo = static_cast<T>(buf.prod);
#endif // MCKL_HAS_X86
    }
}; // class PhiloxHiLo

template <typename T>
class PhiloxHiLo<T, 64>
{
    public:
    static void eval(T a, T b, T &hi, T &lo)
    {
#if MCKL_HAS_INT128

#ifdef MCKL_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif // MCKL_GCC
        union {
            unsigned MCKL_INT128 prod;
            std::array<T, 2> result;
        } buf;

        buf.prod = static_cast<unsigned MCKL_INT128>(a) *
            static_cast<unsigned MCKL_INT128>(b);
#ifdef MCKL_GCC
#pragma GCC diagnostic pop
#endif // MCKL_GCC

#if MCKL_HAS_X86 // littel endia
        hi = std::get<1>(buf.result);
        lo = std::get<0>(buf.result);
#else  // MCKL_HAS_X86
        hi = static_cast<T>(buf.prod >> 64);
        lo = static_cast<T>(buf.prod);
#endif // MCKL_HAS_X86

#elif defined(MCKL_MSVC)

        unsigned __int64 Multiplier =
            static_cast<unsigned __int64>(multiplier::value);
        unsigned __int64 Multiplicand = static_cast<unsigned __int64>(b);
        unsigned __int64 hi_tmp = 0;
        lo = static_cast<T>(_umul128(Multiplier, Multiplicand, &hi_tmp));
        hi = static_cast<T>(hi_tmp);

#else // MCKL_HAS_INT128

        const T a = multiplier::value;
        const T lomask = (const_one<T>() << 32) - 1;
        const T ahi = a >> 32;
        const T alo = a & lomask;
        const T bhi = b >> 32;
        const T blo = b & lomask;
        const T ahbl = ahi * blo;
        const T albh = alo * bhi;
        const T ahbl_albh = ((ahbl & lomask) + (albh & lomask));

        lo = a * b;
        hi = ahi * bhi + (ahbl >> 32) + (albh >> 32);
        hi += ahbl_albh >> 32;
        hi += ((lo >> 32) < (ahbl_albh & lomask));
        std::array<T, 2> hilo = {{lo, hi}};

        return hilo;

#endif // MCKL_HAS_INT128
    }
}; // class PhiloxHiLo

template <typename T, std::size_t K, std::size_t N, typename Constants,
    bool = (N > 1)>
class PhiloxBumpKey
{
    public:
    static void eval(std::array<T, K / 2> &) {}
}; // class PhiloxBumpKey

template <typename T, std::size_t N, typename Constants>
class PhiloxBumpKey<T, 2, N, Constants, true>
{
    public:
    static void eval(std::array<T, 1> &par)
    {
        static constexpr T w0 = Constants::weyl[0];

        std::get<0>(par) += w0;
    }
}; // class PhiloxBumpKey

template <typename T, std::size_t N, typename Constants>
class PhiloxBumpKey<T, 4, N, Constants, true>
{
    public:
    static void eval(std::array<T, 2> &par)
    {
        static constexpr T w0 = Constants::weyl[0];
        static constexpr T w1 = Constants::weyl[1];

        std::get<0>(par) += w0;
        std::get<1>(par) += w1;
    }
}; // class PhiloxBumpKey

template <typename T, std::size_t K, typename Constants>
class PhiloxInitPar
{
    public:
    template <std::size_t Rp1>
    static void eval(const std::array<T, K / 2> &key,
        std::array<std::array<T, K / 2>, Rp1> &par)
    {
        std::get<0>(par) = key;
        eval<1>(par, std::integral_constant<bool, 1 < Rp1>());
    }

    private:
    template <std::size_t, std::size_t Rp1>
    static void eval(std::array<std::array<T, K / 2>, Rp1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rp1>
    static void eval(
        std::array<std::array<T, K / 2>, Rp1> &par, std::true_type)
    {
        std::get<N>(par) = std::get<N - 1>(par);
        PhiloxBumpKey<T, K, N, Constants>::eval(std::get<N>(par));
        eval<N + 1>(par, std::integral_constant<bool, N + 1 < Rp1>());
    }
}; // class PhiloxInitPar

template <typename T, std::size_t K, std::size_t N, typename, bool = (N > 0)>
class PhiloxSBox
{
    public:
    template <std::size_t Rp1>
    static void eval(
        std::array<T, K> &, const std::array<std::array<T, K / 2>, Rp1> &)
    {
    }
}; // class PhiloxSBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class PhiloxSBox<T, K, N, Constants, true>
{
    public:
    template <std::size_t Rp1>
    static void eval(std::array<T, K> &state,
        const std::array<std::array<T, K / 2>, Rp1> &par)
    {
        eval<0>(state, par, std::integral_constant<bool, 1 < K>());
    }

    private:
    template <std::size_t, std::size_t Rp1>
    static void eval(std::array<T, K> &,
        const std::array<std::array<T, K / 2>, Rp1> &, std::false_type)
    {
    }

    template <std::size_t I, std::size_t Rp1>
    static void eval(std::array<T, K> &state,
        const std::array<std::array<T, K / 2>, Rp1> &par, std::true_type)
    {
        static constexpr T m = Constants::multiplier[I / 2];

        T x = std::get<I + 1>(state) ^ std::get<I / 2>(std::get<N>(par));
        PhiloxHiLo<T>::eval(
            std::get<I>(state), m, std::get<I>(state), std::get<I + 1>(state));
        std::get<I>(state) ^= x;
        eval<I + 2>(state, par, std::integral_constant<bool, I + 3 < K>());
    }
}; // class PhiloxSBox

template <typename T, std::size_t K, std::size_t N, bool = (N > 0)>
class PhiloxPBox
{
    public:
    static void eval(std::array<T, K> &) {}
}; // class PhiloxPBox

template <typename T, std::size_t K, std::size_t N>
class PhiloxPBox<T, K, N, true>
{
    public:
    static void eval(std::array<T, K> &state)
    {
        std::array<T, K> tmp;
        eval<0>(state, tmp, std::integral_constant<bool, 0 < K>());
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
        static constexpr std::size_t P =
            ThreefryConstants<T, K>::permute[K - I - 1];
        static constexpr std::size_t J = K - P - 1;

        std::get<I>(tmp) = std::get<J>(state);
        eval<I + 1>(state, tmp, std::integral_constant<bool, I + 1 < K>());
    }
}; // class PhiloxPBox

template <typename T, std::size_t N>
class PhiloxPBox<T, 2, N, true>
{
    public:
    static void eval(std::array<T, 2> &) {}
}; // class PhiloxPBox

template <typename T, std::size_t N>
class PhiloxPBox<T, 4, N, true>
{
    public:
    static void eval(std::array<T, 4> &state)
    {
        std::swap(std::get<0>(state), std::get<2>(state));
    }
}; // class PhiloxPBox

} // namespace mckl::internal

/// \brief Philox RNG generator
/// \ingroup Philox
///
/// \tparam T State type, must be 32- or 64-bit unsigned integers
/// \tparam K State vector length, must be 2 or 4 (for 32- or 64-bit states)
/// \tparam Rounds Number of SP rounds
/// \tparam Constants A trait class that defines algorithm constants, see
/// PhiloxConstants
///
/// \details
/// This generator implement the Philox algorithm in
/// [Random123](http://www.deshawresearch.com/resources_random123.html),
/// developed John K. Salmon, Mark A. Moraes, Ron O. Dror, and David E. Shaw.
template <typename T, std::size_t K, std::size_t Rounds = MCKL_PHILOX_ROUNDS,
    typename Constants = PhiloxConstants<T, K>>
class PhiloxGenerator
{
    static_assert(std::is_unsigned<T>::value,
        "**PhiloxGenerator** used with T other than unsigned integer types");

    static_assert(K != 0 && K % 2 == 0,
        "**PhiloxGenerator** used with K other than multiples of 2");

    static_assert(
        Rounds != 0, "**PhiloxGenerator** used with rounds equal to zero");

    public:
    using ctr_type = std::array<T, K>;
    using key_type = std::array<T, K / 2>;

    static constexpr std::size_t size() { return sizeof(T) * K; }

    key_type key() const { return key_; }

    void reset(const key_type &key) { key_ = key; }

    void enc(const ctr_type &ctr, ctr_type &buffer)
    {
        union {
            std::array<T, K> state;
            ctr_type result;
        } buf;

        std::array<key_type, Rounds + 1> par;
        internal::PhiloxInitPar<T, K, Constants>::eval(key_, par);

        buf.result = ctr;
        generate<0>(buf.state, par, std::true_type());
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

        std::array<key_type, Rounds + 1> par;
        internal::PhiloxInitPar<T, K, Constants>::eval(key_, par);

        increment(ctr);
        buf.ctr = ctr;
        generate<0>(buf.state, par, std::true_type());
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

        std::array<key_type, Rounds + 1> par;
        internal::PhiloxInitPar<T, K, Constants>::eval(key_, par);

        for (std::size_t i = 0; i != n; ++i) {
            increment(ctr);
            buf.ctr = ctr;
            generate<0>(buf.state, par, std::true_type());
            buffer[i] = buf.result;
        }
    }

    friend bool operator==(const PhiloxGenerator<T, K, Rounds> &gen1,
        const PhiloxGenerator<T, K, Rounds> &gen2)
    {
        return gen1.key_ == gen2.key_;
    }

    friend bool operator!=(const PhiloxGenerator<T, K, Rounds> &gen1,
        const PhiloxGenerator<T, K, Rounds> &gen2)
    {
        return !(gen1 == gen2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const PhiloxGenerator<T, K, Rounds> &gen)
    {
        if (!os)
            return os;

        os << gen.key_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        PhiloxGenerator<T, K, Rounds> &gen)
    {
        if (!is)
            return is;

        key_type key = {{0}};
        is >> std::ws >> key;

        if (is)
            gen.key_ = std::move(key);

        return is;
    }

    private:
    key_type key_;

    template <std::size_t>
    void generate(std::array<T, K> &, std::array<key_type, Rounds + 1> &,
        std::false_type) const MCKL_ALWAYS_INLINE
    {
    }

    template <std::size_t N>
    void generate(std::array<T, K> &state,
        std::array<key_type, Rounds + 1> &par,
        std::true_type) const MCKL_ALWAYS_INLINE
    {
        internal::PhiloxPBox<T, K, N>::eval(state);
        internal::PhiloxSBox<T, K, N, Constants>::eval(state, par);
        generate<N + 1>(
            state, par, std::integral_constant<bool, (N < Rounds)>());
    }
}; // class PhiloxGenerator

/// \brief Philox RNG engine
/// \ingroup Philox
template <typename ResultType, typename T, std::size_t K,
    std::size_t Rounds = MCKL_PHILOX_ROUNDS,
    typename Constants = PhiloxConstants<T, K>>
using PhiloxEngine =
    CounterEngine<ResultType, PhiloxGenerator<T, K, Rounds, Constants>>;

/// \brief Philox2x32 RNG engine
/// \ingroup Philox
template <typename ResultType>
using Philox2x32Engine = PhiloxEngine<ResultType, std::uint32_t, 2>;

/// \brief Philox4x32 RNG engine
/// \ingroup Philox
template <typename ResultType>
using Philox4x32Engine = PhiloxEngine<ResultType, std::uint32_t, 4>;

/// \brief Philox2x64 RNG engine
/// \ingroup Philox
template <typename ResultType>
using Philox2x64Engine = PhiloxEngine<ResultType, std::uint64_t, 2>;

/// \brief Philox4x64 RNG engine
/// \ingroup Philox
template <typename ResultType>
using Philox4x64Engine = PhiloxEngine<ResultType, std::uint64_t, 4>;

/// \brief Philox2x32 RNG engine with 32-bit integer output
/// \ingroup Philox
using Philox2x32 = Philox2x32Engine<std::uint32_t>;

/// \brief Philox4x32 RNG engine with 32-bit integer output
/// \ingroup Philox
using Philox4x32 = Philox4x32Engine<std::uint32_t>;

/// \brief Philox2x64 RNG engine with 32-bit integer output
/// \ingroup Philox
using Philox2x64 = Philox2x64Engine<std::uint32_t>;

/// \brief Philox4x64 RNG engine with 32-bit integer output
/// \ingroup Philox
using Philox4x64 = Philox4x64Engine<std::uint32_t>;

/// \brief Philox2x32 RNG engine with 64-bit integer output
/// \ingroup Philox
using Philox2x32_64 = Philox2x32Engine<std::uint64_t>;

/// \brief Philox4x32 RNG engine with 64-bit integer output
/// \ingroup Philox
using Philox4x32_64 = Philox4x32Engine<std::uint64_t>;

/// \brief Philox2x64 RNG engine with 64-bit integer output
/// \ingroup Philox
using Philox2x64_64 = Philox2x64Engine<std::uint64_t>;

/// \brief Philox4x64 RNG engine with 64-bit integer output
/// \ingroup Philox
using Philox4x64_64 = Philox4x64Engine<std::uint64_t>;

} // namespace mckl

#endif // MCKL_RANDOM_PHILOX_HPP
