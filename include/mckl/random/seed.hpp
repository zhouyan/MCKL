//============================================================================
// MCKL/include/mckl/random/seed.hpp
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

#ifndef MCKL_RANDOM_SEED_HPP
#define MCKL_RANDOM_SEED_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/counter.hpp>
#include <mckl/random/threefry.hpp>

namespace mckl
{

namespace internal
{

template <typename RNGType, typename OutputType, typename InputType>
inline OutputType seed_enc(const InputType &input)
{
    static_assert(sizeof(InputType) == sizeof(OutputType),
        "**seed_enc** input and output have different sizes");

    static_assert(sizeof(InputType) == sizeof(typename RNGType::ctr_type),
        "**seed_enc** input and RNGType::ctr_type have different sizes");

    union {
        InputType k;
        OutputType key;
        typename RNGType::ctr_type ctr;
    } buf;

    buf.k = input;
    RNGType rng(0);
    rng.enc(buf.ctr, buf.ctr);

    return buf.key;
}

} // namespace mckl::internal

/// \brief Seed generator for use with RNG accepting scalar seeds
///
/// \details
/// The sequence of seeds are belongs to the equivalent class \f$s \mod D
/// \equiv R\f$ where \f$D > 0\f$, \f$R \ge 0\f$. The defaults are \f$D = 1\f$
/// and \f$R = 0\f$. Each time `operator()` is called, a new seed is used.
///
/// \note
/// If \f$D = 1\f$ (and \f$R = 0\f$), then there will be exactly two seeds
/// equal to `1` in any sequence of length \f$2^N\f$, where \f$N\f$ is the
/// number of digits in the unsigned integer type. They are either one at the
/// beginning and one at the end, or consecutive.
template <typename ResultType, typename ID = ResultType>
class SeedGenerator
{
    public:
    using result_type = ResultType;

    SeedGenerator(const SeedGenerator<ResultType, ID> &) = delete;

    SeedGenerator<ResultType, ID> &operator=(
        const SeedGenerator<ResultType, ID> &) = delete;

    static SeedGenerator<ResultType, ID> &instance()
    {
        static SeedGenerator<ResultType, ID> seed;

        return seed;
    }

    /// \brief Get a new seed
    result_type operator()()
    {
        result_type s = seed_.fetch_add(1);

        return (s % max_ + 1) * divisor_ + remainder_;
    }

    /// \brief Seed a single RNG
    template <typename RNGType>
    void operator()(RNGType &rng)
    {
        rng.seed(operator()());
    }

    /// \brief Seed a sequence of RNGs
    template <typename OutputIter>
    OutputIter operator()(std::size_t n, OutputIter first)
    {
        for (std::size_t i = 0; i != n; ++i, ++first)
            operator()(*first);

        return first;
    }

    /// \brief Seed a sequence of RNGs
    template <typename OutputIter>
    OutputIter operator()(OutputIter first, OutputIter last)
    {
        while (first != last) {
            operator()(*first);
            ++first;
        }

        return first;
    }

    /// \brief Set the seed to `s % max() * divisor() + remainder()`
    void set(result_type s) { seed_ = s % max_; }

    /// \brief The maximum of the seed
    result_type max() const { return (max_ + 1) * divisor_ + remainder_; }

    /// \brief The divisor of the output seed
    result_type divisor() const { return divisor_; }

    /// \brief The remainder of the output seed
    result_type remainder() const { return remainder_; }

    /// \brief Set the divisor and the remainder
    void modulo(result_type divisor, result_type remainder)
    {
        runtime_assert(divisor > remainder,
            "**SeedGenerator::modula** the remainder is not smaller than the "
            "divisor");

        result_type maxs =
            (std::numeric_limits<result_type>::max() - remainder) / divisor;
        runtime_assert(maxs > 1,
            "**SeedGenerator::modulo** the maximum of the internal seed will "
            "be no larger than 1");

        divisor_ = divisor;
        remainder_ = remainder;
        max_ = maxs;

        set(seed_);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const SeedGenerator<ResultType, ID> &sg)
    {
        if (!os)
            return os;

        os << sg.seed_ << ' ';
        os << sg.max_ << ' ';
        os << sg.divisor_ << ' ';
        os << sg.remainder_ << ' ';

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        SeedGenerator<ResultType, ID> &sg)
    {
        if (!is)
            return is;

        result_type seed;
        result_type max;
        result_type divisor;
        result_type remainder;
        is >> std::ws >> seed;
        is >> std::ws >> max;
        is >> std::ws >> divisor;
        is >> std::ws >> remainder;

        if (is) {
            sg.seed_ = seed;
            sg.max_ = max;
            sg.divisor_ = divisor;
            sg.remainder_ = remainder;
        }

        return is;
    }

    protected:
    SeedGenerator() : seed_(0), max_(0), divisor_(1), remainder_(0)
    {
        modulo(divisor_, remainder_);
    }

    private:
    std::atomic<result_type> seed_;
    result_type max_;
    result_type divisor_;
    result_type remainder_;

    result_type get()
    {
        result_type s = seed_.fetch_add(1);

        return enc((s % max_ + 1) * divisor_ + remainder_,
            std::integral_constant<int,
                std::numeric_limits<result_type>::digits>());
    }

    template <std::size_t D>
    result_type enc(result_type s, std::integral_constant<int, D>)
    {
        return s;
    }

    result_type enc(result_type s, std::integral_constant<int, 32>)
    {
        return s;
    }

    result_type enc(result_type s, std::integral_constant<int, 64>)
    {
        return internal::seed_enc<Threefry2x32, result_type>(s);
    }
}; // class SeedGenerator

/// \brief Seed generator for use with RNG accepting fixed width keys as seeds
///
/// \details
/// If `K` is less than two, then it operates as the generic `SeedGenerator`.
/// Otherwise, The sequence of keys are such that, the first element is
/// \f$1,2,\dots\f$ while the last element is \f$R\f$.
template <typename ResultType, std::size_t K, typename ID>
class SeedGenerator<std::array<ResultType, K>, ID>
{
    public:
    using result_type = typename std::conditional<K != 0 &&
            sizeof(ResultType) * K % sizeof(std::uint64_t) == 0,
        std::uint64_t, ResultType>::type;
    using key_type = std::array<ResultType, K>;

    SeedGenerator(
        const SeedGenerator<std::array<ResultType, K>, ID> &) = delete;

    SeedGenerator<std::array<ResultType, K>, ID> &operator=(
        const SeedGenerator<std::array<ResultType, K>, ID> &) = delete;

    static SeedGenerator<std::array<ResultType, K>, ID> &instance()
    {
        static SeedGenerator<std::array<ResultType, K>, ID> seed;

        return seed;
    }

    /// \brief Create a new RNG using a new seed
    key_type operator()() { return get(dispatch_type()); }

    /// \brief Seed a single RNG
    template <typename RNGType>
    void operator()(RNGType &rng)
    {
        rng.seed(operator()());
    }

    /// \brief Seed a sequence of RNGs
    template <typename OutputIter>
    OutputIter operator()(std::size_t n, OutputIter first)
    {
        for (std::size_t i = 0; i != n; ++i, ++first)
            operator()(*first);

        return first;
    }

    /// \brief Seed a sequence of RNGs
    template <typename OutputIter>
    OutputIter operator()(OutputIter first, OutputIter last)
    {
        while (first != last) {
            operator()(*first);
            ++first;
        }

        return first;
    }

    /// \brief Set the seed to `s % max() * divisor() + remainder()`
    void set(result_type s) { seed_ = s % max_; }

    /// \brief The maximum of the seed
    result_type max() const { return (max_ + 1) * divisor_ + remainder_; }

    /// \brief The divisor of the output seed
    result_type divisor() const { return divisor_; }

    /// \brief The remainder of the output seed
    result_type remainder() const { return remainder_; }

    /// \brief Set the divisor and the remainder
    void modulo(result_type divisor, result_type remainder)
    {
        modulo(divisor, remainder, dispatch_type());
        set(seed_);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const SeedGenerator<std::array<ResultType, K>, ID> &sg)
    {
        if (!os)
            return os;

        os << sg.seed_ << ' ';
        os << sg.max_ << ' ';
        os << sg.divisor_ << ' ';
        os << sg.remainder_ << ' ';

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        SeedGenerator<std::array<ResultType, K>, ID> &sg)
    {
        if (!is)
            return is;

        result_type seed;
        result_type max;
        result_type divisor;
        result_type remainder;
        is >> std::ws >> seed;
        is >> std::ws >> max;
        is >> std::ws >> divisor;
        is >> std::ws >> remainder;

        if (is) {
            sg.seed_ = seed;
            sg.max_ = max;
            sg.divisor_ = divisor;
            sg.remainder_ = remainder;
        }

        return is;
    }

    protected:
    SeedGenerator() : seed_(0), max_(0), divisor_(1), remainder_(0)
    {
        modulo(divisor_, remainder_);
    }

    private:
    static constexpr std::size_t M_ =
        K != 0 && sizeof(ResultType) * K % sizeof(std::uint64_t) == 0 ?
        sizeof(ResultType) * K / sizeof(std::uint64_t) :
        K;

    using dispatch_type = std::integral_constant<bool, (M_ > 1)>;

    std::atomic<result_type> seed_;
    result_type max_;
    result_type divisor_;
    result_type remainder_;

    void modulo(result_type, result_type remainder, std::true_type)
    {
        max_ = std::numeric_limits<result_type>::max();
        remainder_ = remainder;
    }

    void modulo(result_type divisor, result_type remainder, std::false_type)
    {
        runtime_assert(divisor > remainder,
            "**SeedGenerator::modulo** the remainder is not smaller than the "
            "divisor");

        result_type maxs =
            (std::numeric_limits<result_type>::max() - remainder) / divisor;
        runtime_assert(maxs > 1,
            "**SeedGenerator::modulo** the maximum of the internal seed will "
            "be no larger than 1");

        divisor_ = divisor;
        remainder_ = remainder;
        max_ = maxs;

        set(seed_);
    }

    key_type get(std::true_type)
    {
        result_type s = seed_.fetch_add(1);
        std::array<result_type, M_> k = {{0}};
        k.front() = s + 1;
        k.back() = remainder_;

        return enc(k, std::integral_constant<int,
                          std::numeric_limits<result_type>::digits * M_>());
    }

    key_type get(std::false_type)
    {
        result_type s = seed_.fetch_add(1);
        std::array<result_type, M_> k = {{0}};
        std::fill(k.begin(), k.end(), 0);
        k.front() = (s % max_ + 1) * divisor_ + remainder_;

        return enc(k, std::integral_constant<int,
                          std::numeric_limits<result_type>::digits * M_>());
    }

    template <int D>
    key_type enc(
        const std::array<result_type, M_> &k, std::integral_constant<int, D>)
    {
        union {
            key_type key;
            std::array<result_type, M_> k;
        } buf;

        buf.k = k;

        return buf.key;
    }

    key_type enc(
        const std::array<result_type, M_> &k, std::integral_constant<int, 64>)
    {
        return internal::seed_enc<Threefry2x32, key_type>(k);
    }

    key_type enc(
        const std::array<result_type, M_> &k, std::integral_constant<int, 128>)
    {
        return internal::seed_enc<Threefry2x64, key_type>(k);
    }

    key_type enc(
        const std::array<result_type, M_> &k, std::integral_constant<int, 256>)
    {
        return internal::seed_enc<Threefry4x64, key_type>(k);
    }

    key_type enc(
        const std::array<result_type, M_> &k, std::integral_constant<int, 512>)
    {
        return internal::seed_enc<Threefry8x64, key_type>(k);
    }

    key_type enc(const std::array<result_type, M_> &k,
        std::integral_constant<int, 1024>)
    {
        return internal::seed_enc<Threefry16x64, key_type>(k);
    }
}; // class Seed

/// \brief Seeding RNG engines
/// \ingroup Random
template <typename RNGType>
using Seed = SeedGenerator<typename SeedType<RNGType>::type>;

} // namespace mckl

#endif // MCKL_RANDOM_SEED_HPP
