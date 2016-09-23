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

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
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
    static constexpr T weyl[1] = {UINT32_C(0x9E3779B9)};

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
    static constexpr T weyl[1] = {UINT64_C(0x9E3779B97F4A7C15)};

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

#include <mckl/random/internal/philox_generic.hpp>

#if MCKL_HAS_SSE2
#include <mckl/random/internal/philox_sse2.hpp>
#endif

#if MCKL_HAS_AVX2
#include <mckl/random/internal/philox_avx2.hpp>
#endif

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl
    : public PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>
{
}; // class PhiloxGeneratorImpl

template <typename T, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, 2, Rounds, Constants>
#if MCKL_HAS_AVX2
    : public PhiloxGeneratorAVX2Impl<T, 2, Rounds, Constants,
          PhiloxGeneratorImpl<T, 2, Rounds, Constants>>
#elif MCKL_HAS_SSE2
    : public PhiloxGeneratorSSE2Impl<T, 2, Rounds, Constants,
          PhiloxGeneratorImpl<T, 2, Rounds, Constants>>
#else
    : public PhiloxGeneratorGenericImpl<T, 2, Rounds, Constants>
#endif
{
    public:
#if MCKL_HAS_SSE2
    static void pbox(std::array<__m128i, 8> &, std::array<__m128i, 8> &) {}
#endif

#if MCKL_HAS_AVX2
    static void pbox(std::array<__m256i, 8> &, std::array<__m256i, 8> &) {}
#endif
}; // class PhiloxGeneratorImpl

template <typename T, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, 4, Rounds, Constants>
#if MCKL_HAS_AVX2
    : public PhiloxGeneratorAVX2Impl<T, 4, Rounds, Constants,
          PhiloxGeneratorImpl<T, 4, Rounds, Constants>>
#elif MCKL_HAS_SSE2
    : public PhiloxGeneratorSSE2Impl<T, 4, Rounds, Constants,
          PhiloxGeneratorImpl<T, 4, Rounds, Constants>>
#else  // MCKL_HAS_AVX2
    : public PhiloxGeneratorGenericImpl<T, 4, Rounds, Constants>
#endif // MCKL_HAS_AVX2
{
#if MCKL_HAS_SSE2
    public:
    static void pbox(std::array<__m128i, 8> &s, std::array<__m128i, 8> &t)
    {
        pbox(s, t,
            std::integral_constant<int, std::numeric_limits<T>::digits>());
    }

    private:
    static void pbox(std::array<__m128i, 8> &s, std::array<__m128i, 8> &,
        std::integral_constant<int, 32>)
    {
        // 3 0 1 2
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xC6);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xC6);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xC6);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xC6);
        std::get<4>(s) = _mm_shuffle_epi32(std::get<4>(s), 0xC6);
        std::get<5>(s) = _mm_shuffle_epi32(std::get<5>(s), 0xC6);
        std::get<6>(s) = _mm_shuffle_epi32(std::get<6>(s), 0xC6);
        std::get<7>(s) = _mm_shuffle_epi32(std::get<7>(s), 0xC6);
    }
#endif // MCKL_HAS_SSE2

#if MCKL_HAS_AVX2
    public:
    static void pbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t)
    {
        pbox(s, t,
            std::integral_constant<int, std::numeric_limits<T>::digits>());
    }

    private:
    static void pbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &,
        std::integral_constant<int, 32>)
    {
        // 3 0 1 2
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xC6);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xC6);
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0xC6);
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0xC6);
        std::get<4>(s) = _mm256_shuffle_epi32(std::get<4>(s), 0xC6);
        std::get<5>(s) = _mm256_shuffle_epi32(std::get<5>(s), 0xC6);
        std::get<6>(s) = _mm256_shuffle_epi32(std::get<6>(s), 0xC6);
        std::get<7>(s) = _mm256_shuffle_epi32(std::get<7>(s), 0xC6);
    }
#endif // MCKL_HAS_AVX2
};     // class PhiloxGeneratorImpl

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
    using ctr_type =
        typename std::conditional<(sizeof(T) * K) % sizeof(std::uint64_t) == 0,
            std::array<std::uint64_t, (sizeof(T) * K) / sizeof(std::uint64_t)>,
            std::array<T, K>>::type;
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

        buf.result = ctr;
        internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, key_);
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

    void generate(std::array<T, K> &ctr, std::array<T, K> &buffer) const
    {
        increment(ctr);
        buffer = ctr;
        internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
            buffer, key_);
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
        internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, key_);
        buffer = buf.result;
    }

    void generate(
        std::array<T, K> &ctr, std::size_t n, std::array<T, K> *buffer) const
    {
        static constexpr std::size_t blocks =
            internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::blocks();

        using state_type = std::array<std::array<T, K>, blocks>;

        const std::size_t m = n / blocks;
        const std::size_t l = n % blocks;
        for (std::size_t i = 0; i != m; ++i, buffer += blocks) {
            state_type &state = *reinterpret_cast<state_type *>(buffer);
            increment(ctr, state);
            internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
                state, key_);
        }
        for (std::size_t i = 0; i != l; ++i)
            generate(ctr, buffer[i]);
    }

    template <typename ResultType>
    void generate(ctr_type &ctr, std::size_t n,
        std::array<ResultType, size() / sizeof(ResultType)> *buffer) const
    {
        static constexpr std::size_t blocks =
            internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::blocks();

        union {
            std::array<std::array<T, K>, blocks> state;
            std::array<ctr_type, blocks> ctr_block;
        } buf;

        const std::size_t m = n / blocks;
        const std::size_t l = n % blocks;
        for (std::size_t i = 0; i != m; ++i, buffer += blocks) {
            increment(ctr, buf.ctr_block);
            internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
                buf.state, key_);
            std::memcpy(buffer, buf.state.data(), size() * blocks);
        }
        for (std::size_t i = 0; i != l; ++i)
            generate(ctr, buffer[i]);
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

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_PHILOX_HPP
