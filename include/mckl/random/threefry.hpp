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
#include <mckl/random/internal/threefry_constants.hpp>
#include <mckl/random/internal/threefry_generic.hpp>
#include <mckl/random/counter.hpp>
#include <mckl/random/increment.hpp>

#if MCKL_HAS_SSE2
#include <mckl/random/internal/threefry_sse2_32.hpp>
#include <mckl/random/internal/threefry_sse2_64.hpp>
#endif

#if MCKL_HAS_AVX2
#include <mckl/random/internal/threefry_avx2_32.hpp>
#include <mckl/random/internal/threefry_avx2_64.hpp>
#endif

/// \brief ThreefryGenerator default rounds
/// \ingroup Config
#ifndef MCKL_THREEFRY_ROUNDS
#define MCKL_THREEFRY_ROUNDS 20
#endif

namespace mckl
{

namespace internal
{

template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    int = std::numeric_limits<T>::digits>
class ThreefryGeneratorImpl
    : public ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>
{
}; // class ThreefryGeneratorImpl

#if MCKL_USE_AVX2

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class ThreefryGeneratorImpl<T, K, Rounds, Constants, 32>
    : public ThreefryGeneratorAVX2Impl32<T, K, Rounds, Constants>
{
}; // class ThreefryGeneratorImpl

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class ThreefryGeneratorImpl<T, K, Rounds, Constants, 64>
    : public ThreefryGeneratorAVX2Impl64<T, K, Rounds, Constants>
{
}; // class ThreefryGeneratorImpl

#elif MCKL_USE_SSE2

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class ThreefryGeneratorImpl<T, K, Rounds, Constants, 32>
    : public ThreefryGeneratorSSE2Impl32<T, K, Rounds, Constants>
{
}; // class ThreefryGeneratorImpl

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class ThreefryGeneratorImpl<T, K, Rounds, Constants, 64>
    : public ThreefryGeneratorSSE2Impl64<T, K, Rounds, Constants>
{
}; // class ThreefryGeneratorImpl

#endif // MCKL_USE_AVX2

} // namespace mckl::internal

/// \brief Threefry RNG generator
/// \ingroup Threefry
///
/// \tparam T State vector value type
/// \tparam K State vector length
/// \tparam Rounds The number of rounds
/// \tparam Constants A trait class that defines algorithm constants
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
    using ctr_type = Counter<T, K>;
    using key_type = std::array<T, K>;

    static constexpr std::size_t size() { return sizeof(T) * K; }

    key_type key() const
    {
        key_type key;
        std::memcpy(key.data(), par_.data(), sizeof(T) * K);

        return key;
    }

    void reset(const key_type &key)
    {
        constexpr T p = Constants::parity::value;

        std::memcpy(par_.data(), key.data(), sizeof(T) * K);
        std::get<K>(par_) = p;
        for (std::size_t i = 0; i != key.size(); ++i)
            std::get<K>(par_) ^= par_[i];
        std::get<K + 1>(par_) = 0;
        std::get<K + 2>(par_) = 0;
        std::get<K + 3>(par_) = 0;
    }

    std::pair<T, T> tweak() const
    {
        return std::make_pair(std::get<K + 1>(par_), std::get<K + 2>(par_));
    }

    void tweak(T t0, T t1)
    {
        std::get<K + 1>(par_) = t0;
        std::get<K + 2>(par_) = t1;
        std::get<K + 3>(par_) = t0 ^ t1;
    }

    void operator()(const void *plain, void *cipher) const
    {
        alignas(32) union {
            std::array<T, K> state;
            std::array<char, size()> result;
        } buf;

        std::memcpy(buf.state.data(), plain, size());
        internal::union_le<char>(buf.state);
        internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, par_);
        internal::union_le<T>(buf.result);
        std::memcpy(cipher, buf.state.data(), size());
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, ResultType *result) const
    {
        alignas(32) union {
            std::array<T, K> state;
            ctr_type ctr;
            std::array<ResultType, size() / sizeof(ResultType)> result;
        } buf;

        increment(ctr);
        buf.ctr = ctr;
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        internal::union_le<typename ctr_type::value_type>(buf.state);
#endif
        internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, par_);
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        internal::union_le<T>(buf.result);
#endif
        std::memcpy(result, buf.result.data(), size());
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, std::size_t n, ResultType *result) const
    {
        generate(ctr, n, result,
            std::integral_constant<bool, internal::ThreefryGeneratorImpl<T, K,
                                             Rounds, Constants>::batch()>());
    }

    template <typename RealType>
    void u01_cc_u32(ctr_type &ctr, std::size_t n, RealType *result) const
    {
        internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::u01_cc_u32(
            ctr, par_, n, result);
    }

    template <typename RealType>
    void u01_co_u32(ctr_type &ctr, std::size_t n, RealType *result) const
    {
        internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::u01_co_u32(
            ctr, par_, n, result);
    }

    template <typename RealType>
    void u01_oc_u32(ctr_type &ctr, std::size_t n, RealType *result) const
    {
        internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::u01_oc_u32(
            ctr, par_, n, result);
    }

    template <typename RealType>
    void u01_oo_u32(ctr_type &ctr, std::size_t n, RealType *result) const
    {
        internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::u01_oo_u32(
            ctr, par_, n, result);
    }

    template <typename RealType>
    void uniform_real_u32(ctr_type &ctr, std::size_t n, RealType *result,
        RealType a, RealType b) const
    {
        internal::ThreefryGeneratorImpl<T, K, Rounds,
            Constants>::uniform_real_u32(ctr, par_, n, result, a, b);
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
    std::array<T, K + 4> par_;

    template <typename ResultType>
    void generate(ctr_type &ctr, std::size_t n, ResultType *result,
        std::false_type) const
    {
        constexpr std::size_t stride = size() / sizeof(ResultType);

        for (std::size_t i = 0; i != n; ++i, result += stride)
            operator()(ctr, result);
    }

    template <typename ResultType>
    void generate(
        ctr_type &ctr, std::size_t n, ResultType *result, std::true_type) const
    {
        internal::ThreefryGeneratorImpl<T, K, Rounds, Constants>::eval(
            ctr, par_, n, result);
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

#if MCKL_USE_AVX2

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
inline void u01_cc_distribution(
    ThreefryEngine<std::uint32_t, T, K, Rounds, Constants> &rng, std::size_t n,
    float *r, typename std::enable_if<K <= 16>::type * = nullptr)
{
    rng.u01_cc_u32(n, r);
}

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
inline void u01_co_distribution(
    ThreefryEngine<std::uint32_t, T, K, Rounds, Constants> &rng, std::size_t n,
    float *r, typename std::enable_if<K <= 16>::type * = nullptr)
{
    rng.u01_co_u32(n, r);
}

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
inline void u01_oc_distribution(
    ThreefryEngine<std::uint32_t, T, K, Rounds, Constants> &rng, std::size_t n,
    float *r, typename std::enable_if<K <= 16>::type * = nullptr)
{
    rng.u01_oc_u32(n, r);
}

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
inline void u01_oo_distribution(
    ThreefryEngine<std::uint32_t, T, K, Rounds, Constants> &rng, std::size_t n,
    float *r, typename std::enable_if<K <= 16>::type * = nullptr)
{
    rng.u01_oo_u32(n, r);
}

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
inline void uniform_real_distribution(
    ThreefryEngine<std::uint32_t, T, K, Rounds, Constants> &rng, std::size_t n,
    float *r, float a, float b,
    typename std::enable_if<K <= 16>::type * = nullptr)
{
    rng.uniform_real_u32(n, r, a, b);
}

#if !MCKL_U01_USE_64BITS_DOUBLE

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
inline void u01_cc_distribution(
    ThreefryEngine<std::uint32_t, T, K, Rounds, Constants> &rng, std::size_t n,
    double *r, typename std::enable_if<K <= 8>::type * = nullptr)
{
    rng.u01_cc_u32(n, r);
}

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
inline void u01_co_distribution(
    ThreefryEngine<std::uint32_t, T, K, Rounds, Constants> &rng, std::size_t n,
    double *r, typename std::enable_if<K <= 8>::type * = nullptr)
{
    rng.u01_co_u32(n, r);
}

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
inline void u01_oc_distribution(
    ThreefryEngine<std::uint32_t, T, K, Rounds, Constants> &rng, std::size_t n,
    double *r, typename std::enable_if<K <= 8>::type * = nullptr)
{
    rng.u01_oc_u32(n, r);
}

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
inline void u01_oo_distribution(
    ThreefryEngine<std::uint32_t, T, K, Rounds, Constants> &rng, std::size_t n,
    double *r, typename std::enable_if<K <= 8>::type * = nullptr)
{
    rng.u01_oo_u32(n, r);
}

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
inline void uniform_real_distribution(
    ThreefryEngine<std::uint32_t, T, K, Rounds, Constants> &rng, std::size_t n,
    double *r, double a, double b,
    typename std::enable_if<K <= 8>::type * = nullptr)
{
    rng.uniform_real_u32(n, r, a, b);
}

#endif // !MCKL_U01_USE_64BITS_DOUBLE

#endif // MCKL_USE_AVX2

} // namespace mckl

#endif // MCKL_RANDOM_THREEFRY_HPP
