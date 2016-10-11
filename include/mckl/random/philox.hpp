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
#include <mckl/random/internal/threefry_constants.hpp>
#include <mckl/random/counter.hpp>

/// \brief PhiloxGenerator default rounds
/// \ingroup Config
#ifndef MCKL_PHILOX_ROUNDS
#define MCKL_PHILOX_ROUNDS 10
#endif

namespace mckl
{

namespace internal
{

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#include <mckl/random/internal/philox_generic.hpp>
#if MCKL_USE_AVX2
#include <mckl/random/internal/philox_avx2_32.hpp>
#elif MCKL_USE_SSE2
#include <mckl/random/internal/philox_sse2_32.hpp>
#endif

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

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
    using ctr_type = Counter<T, K>;
    using key_type = std::array<T, K / 2>;

    static constexpr std::size_t size() { return sizeof(T) * K; }

    key_type key() const { return key_; }

    void reset(const key_type &key) { key_ = key; }

    void enc(const ctr_type &ctr, ctr_type &buffer) const
    {
        alignas(32) union {
            std::array<T, K> state;
            ctr_type result;
        } buf;

        buf.result = ctr;
        internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, key_);
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
        internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, key_);
        std::copy(buf.result.begin(), buf.result.end(), buffer);
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, std::size_t n, ResultType *buffer) const
    {
        generate(ctr, n, buffer,
            std::integral_constant<bool, internal::PhiloxGeneratorImpl<T, K,
                                             Rounds, Constants>::batch()>());
    }

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
            internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::blocks();

        alignas(32) union {
            std::array<std::array<T, K>, blocks> state;
            std::array<ctr_type, blocks> ctr_block;
        } buf;

        const std::size_t m = n / blocks;
        const std::size_t l = n % blocks;
        for (std::size_t i = 0; i != m; ++i, buffer += stride * blocks) {
            MCKL_FLATTEN_CALL increment(ctr, buf.ctr_block);
            internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
                buf.state, key_);
            std::memcpy(buffer, buf.state.data(), size() * blocks);
        }
        for (std::size_t i = 0; i != l; ++i, buffer += stride)
            operator()(ctr, buffer);
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
