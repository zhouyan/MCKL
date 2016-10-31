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
#include <mckl/random/internal/philox_constants.hpp>
#include <mckl/random/internal/philox_generic.hpp>
#include <mckl/random/counter.hpp>
#include <mckl/random/increment.hpp>

#if MCKL_HAS_SSE2
#include <mckl/random/internal/philox_sse2_32.hpp>
#endif

#if MCKL_HAS_AVX2
#include <mckl/random/internal/philox_avx2_32.hpp>
#endif

/// \brief PhiloxGenerator default rounds
/// \ingroup Config
#ifndef MCKL_PHILOX_ROUNDS
#define MCKL_PHILOX_ROUNDS 10
#endif

#define MCKL_DEFINE_RANDOM_PHILOX_U01(lr, bits)                               \
    template <typename RealType>                                              \
    void u01_##lr##_u##bits(ctr_type &ctr, std::size_t n, RealType *result)   \
        const                                                                 \
    {                                                                         \
        internal::PhiloxGeneratorImpl<T, K, Rounds,                           \
            Constants>::u01_##lr##_u##bits(ctr, key_, n, result);             \
    }

#define MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(lr, rbits, tbits, ftype)   \
    template <typename ResultType, typename T, std::size_t K,                 \
        std::size_t Rounds, typename Constants>                               \
    inline void u01_##lr##_distribution(                                      \
        PhiloxEngine<ResultType, T, K, Rounds, Constants> &rng,               \
        std::size_t n, ftype *r,                                              \
        typename std::enable_if<std::numeric_limits<ResultType>::digits ==    \
                rbits &&                                                      \
            std::numeric_limits<T>::digits == tbits>::type * = nullptr)       \
    {                                                                         \
        rng.u01_##lr##_u##rbits(n, r);                                        \
    }

#define MCKL_DEFINE_RANDOM_PHILOX_UNIFORM_REAL(bits)                          \
    template <typename RealType>                                              \
    void uniform_real_u##bits(ctr_type &ctr, std::size_t n, RealType *result, \
        RealType a, RealType b) const                                         \
    {                                                                         \
        internal::PhiloxGeneratorImpl<T, K, Rounds,                           \
            Constants>::uniform_real_u##bits(ctr, key_, n, result, a, b);     \
    }

#define MCKL_DEFINE_RANDOM_PHILOX_UNIFORM_REAL_DISTRIBUTION(                  \
    rbits, tbits, ftype)                                                      \
    template <typename ResultType, typename T, std::size_t K,                 \
        std::size_t Rounds, typename Constants>                               \
    inline void uniform_real_distribution(                                    \
        PhiloxEngine<ResultType, T, K, Rounds, Constants> &rng,               \
        std::size_t n, ftype *r, ftype a, ftype b,                            \
        typename std::enable_if<std::numeric_limits<ResultType>::digits ==    \
                rbits &&                                                      \
            std::numeric_limits<T>::digits == tbits>::type * = nullptr)       \
    {                                                                         \
        rng.uniform_real_u##rbits(n, r, a, b);                                \
    }

namespace mckl
{

namespace internal
{

template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    int = std::numeric_limits<T>::digits>
class PhiloxGeneratorImpl
    : public PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>
{
}; // class PhiloxGeneratorImpl

#if MCKL_USE_AVX2

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, K, Rounds, Constants, 32>
    : public PhiloxGeneratorAVX2Impl32<T, K, Rounds, Constants>
{
}; // class PhiloxGeneratorImpl

#elif MCKL_USE_SSE2

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, K, Rounds, Constants, 32>
    : public PhiloxGeneratorSSE2Impl32<T, K, Rounds, Constants>
{
}; // class PhiloxGeneratorImpl

#endif // MCKL_USE_AVX2

} // namespace mckl::internal

/// \brief Philox RNG generator
/// \ingroup Philox
///
/// \tparam T State vector value type
/// \tparam K State vector length
/// \tparam Rounds The number of rounds
/// \tparam Constants A trait class that defines algorithm constants
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
    using ctr_type = Counter<T, K>;
    using key_type = std::array<T, K / 2>;

    static constexpr std::size_t size() { return sizeof(T) * K; }

    key_type key() const { return key_; }

    void reset(const key_type &key) { key_ = key; }

    void operator()(const void *plain, void *cipher) const
    {
        alignas(32) union {
            std::array<T, K> state;
            std::array<char, size()> result;
        } buf;

        std::memcpy(buf.state.data(), plain, size());
        internal::union_le<char>(buf.state);
        internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, key_);
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

        MCKL_FLATTEN_CALL increment(ctr);
        buf.ctr = ctr;
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        internal::union_le<typename ctr_type::value_type>(buf.state);
#endif
        internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, key_);
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        internal::union_le<T>(buf.result);
#endif
        std::memcpy(result, buf.result.data(), size());
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, std::size_t n, ResultType *result) const
    {
        generate(ctr, n, result,
            std::integral_constant<bool, internal::PhiloxGeneratorImpl<T, K,
                                             Rounds, Constants>::batch()>());
    }

    MCKL_DEFINE_RANDOM_PHILOX_U01(cc, 32)
    MCKL_DEFINE_RANDOM_PHILOX_U01(co, 32)
    MCKL_DEFINE_RANDOM_PHILOX_U01(oc, 32)
    MCKL_DEFINE_RANDOM_PHILOX_U01(oo, 32)
    MCKL_DEFINE_RANDOM_PHILOX_UNIFORM_REAL(32)

    MCKL_DEFINE_RANDOM_PHILOX_U01(cc, 64)
    MCKL_DEFINE_RANDOM_PHILOX_U01(co, 64)
    MCKL_DEFINE_RANDOM_PHILOX_U01(oc, 64)
    MCKL_DEFINE_RANDOM_PHILOX_U01(oo, 64)
    MCKL_DEFINE_RANDOM_PHILOX_UNIFORM_REAL(64)

    friend bool operator==(
        const PhiloxGenerator<T, K, Rounds, Constants> &gen1,
        const PhiloxGenerator<T, K, Rounds, Constants> &gen2)
    {
        return gen1.key_ == gen2.key_;
    }

    friend bool operator!=(
        const PhiloxGenerator<T, K, Rounds, Constants> &gen1,
        const PhiloxGenerator<T, K, Rounds, Constants> &gen2)
    {
        return !(gen1 == gen2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const PhiloxGenerator<T, K, Rounds, Constants> &gen)
    {
        if (!os)
            return os;

        os << gen.key_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        PhiloxGenerator<T, K, Rounds, Constants> &gen)
    {
        if (!is)
            return is;

        PhiloxGenerator<T, K, Rounds, Constants> gen_tmp;
        gen_tmp.key_.fill(0);
        is >> std::ws >> gen_tmp.key_;

        if (is)
            gen = std::move(gen_tmp);

        return is;
    }

    private:
    key_type key_;

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
        internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
            ctr, key_, n, result);
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

#if MCKL_USE_AVX2

MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(cc, 32, 32, float)
MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(co, 32, 32, float)
MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(oc, 32, 32, float)
MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(oo, 32, 32, float)
MCKL_DEFINE_RANDOM_PHILOX_UNIFORM_REAL_DISTRIBUTION(32, 32, float)

MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(cc, 64, 32, double)
MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(co, 64, 32, double)
MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(oc, 64, 32, double)
MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(oo, 64, 32, double)
MCKL_DEFINE_RANDOM_PHILOX_UNIFORM_REAL_DISTRIBUTION(64, 32, double)

#if !MCKL_U01_USE_64BITS_DOUBLE

MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(cc, 32, 32, double)
MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(co, 32, 32, double)
MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(oc, 32, 32, double)
MCKL_DEFINE_RANDOM_PHILOX_U01_DISTRIBUTION(oo, 32, 32, double)
MCKL_DEFINE_RANDOM_PHILOX_UNIFORM_REAL_DISTRIBUTION(32, 32, double)

#endif // !MCKL_U01_USE_64BITS_DOUBLE

#endif // MCKL_USE_AVX2

} // namespace mckl

#endif // MCKL_RANDOM_PHILOX_HPP
