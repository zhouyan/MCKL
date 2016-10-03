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

template <typename RNGType, typename ResultType, typename StateType>
inline ResultType seed_enc(const StateType &state)
{
    static_assert(sizeof(StateType) == sizeof(ResultType),
        "**seed_enc** state and result have different sizes");

    static_assert(sizeof(StateType) == sizeof(typename RNGType::ctr_type),
        "**seed_enc** state and RNGType::ctr_type have different sizes");

    union {
        StateType state;
        ResultType result;
        typename RNGType::ctr_type ctr;
    } buf;

    buf.state = state;
    RNGType rng(0);
    rng.enc(buf.ctr, buf.ctr);

    return buf.result;
}

} // namespace mckl::internal

/// \brief Seed generator for use with RNG accepting scalar seeds
template <typename ResultType, typename ID = ResultType, bool Randomize = true>
class SeedGenerator
{
    static_assert(std::is_unsigned<ResultType>::value,
        "**SeedGenerator** ResultType is not an unsigned integer type");

    public:
    using result_type = ResultType;

    SeedGenerator(const SeedGenerator<ResultType, ID, Randomize> &) = delete;

    SeedGenerator<ResultType, ID, Randomize> &operator=(
        const SeedGenerator<ResultType, ID, Randomize> &) = delete;

    static SeedGenerator<ResultType, ID, Randomize> &instance()
    {
        static SeedGenerator<ResultType, ID, Randomize> seed;

        return seed;
    }

    /// \brief Set the internal seed
    void set(result_type s) { seed_ = s; }

    /// \brief Get the next seed
    result_type get()
    {
        result_type s = seed_.fetch_add(1);
        s %= maxs_;
        s *= np_;
        s += rank_;

        return enc(s, std::integral_constant<bool, Randomize>());
    }

    /// \brief The number of partitions
    result_type np() const { return np_; }

    /// \brief The rank of this partition
    result_type rank() const { return rank_; }

    /// \brief Set the number of partitions and the rank of this partition
    void partition(result_type np, result_type rank)
    {
        runtime_assert(np > 0,
            "**SeedGenerator::partition** the number of the partitions is "
            "zero");
        if (np < 1)
            np = 1;

        runtime_assert(np > rank,
            "**SeedGenerator::partition** the rank is not smaller than the "
            "number of partitions");
        if (rank >= np)
            rank = 0;

        result_type maxs = std::numeric_limits<result_type>::max();
        if (np > 1)
            maxs = (maxs - rank) / np + 1;

        np_ = np;
        rank_ = rank;
        maxs_ = maxs;
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const SeedGenerator<ResultType, ID, Randomize> &sg)
    {
        if (!os)
            return os;

        os << sg.np_ << ' ';
        os << sg.rank_ << ' ';
        os << sg.maxs_ << ' ';
        os << sg.seed_ << ' ';

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        SeedGenerator<ResultType, ID, Randomize> &sg)
    {
        if (!is)
            return is;

        result_type np;
        result_type rank;
        result_type maxs;
        result_type seed;
        is >> std::ws >> np;
        is >> std::ws >> rank;
        is >> std::ws >> maxs;
        is >> std::ws >> seed;

        if (is) {
            sg.np_ = np;
            sg.rank_ = rank;
            sg.maxs_ = maxs;
            sg.seed_ = seed;
        }

        return is;
    }

    protected:
    SeedGenerator() : seed_(1) { partition(1, 0); }

    private:
    result_type np_;
    result_type rank_;
    result_type maxs_;
    std::atomic<result_type> seed_;

    result_type enc(result_type s, std::false_type) { return s; }

    result_type enc(result_type s, std::true_type)
    {
        return enc(s, std::integral_constant<int,
                          std::numeric_limits<result_type>::digits>());
    }

    template <std::size_t D>
    result_type enc(result_type s, std::integral_constant<int, D>)
    {
        return enc(s, std::false_type());
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

/// \brief Seed generator for RNG with fixed width keys as seeds
template <typename T, std::size_t K, typename ID, bool Randomize>
class SeedGenerator<std::array<T, K>, ID, Randomize>
{
    public:
    using seed_type = typename std::conditional<K != 0 &&
            sizeof(T) * K % sizeof(std::uint64_t) == 0,
        std::uint64_t, T>::type;

    using result_type = std::array<T, K>;

    SeedGenerator(
        const SeedGenerator<std::array<T, K>, ID, Randomize> &) = delete;

    SeedGenerator<std::array<T, K>, ID, Randomize> &operator=(
        const SeedGenerator<std::array<T, K>, ID, Randomize> &) = delete;

    static SeedGenerator<std::array<T, K>, ID, Randomize> &instance()
    {
        static SeedGenerator<std::array<T, K>, ID, Randomize> seed;

        return seed;
    }

    /// \brief Set the internal seed
    void set(seed_type s) { seed_ = s; }

    /// \brief Get the next key
    result_type get() { return get(std::integral_constant<bool, (M_ > 1)>()); }

    /// \brief The number of partitions
    seed_type np() const { return np_; }

    /// \brief The rank of this partition
    seed_type rank() const { return rank_; }

    /// \brief Set the number of partitions and the rank of this partition
    void partition(seed_type np, seed_type rank)
    {
        partition(np, rank, std::integral_constant<bool, (M_ > 1)>());
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const SeedGenerator<std::array<T, K>, ID, Randomize> &sg)
    {
        if (!os)
            return os;

        os << sg.np_ << ' ';
        os << sg.rank_ << ' ';
        os << sg.maxs_ << ' ';
        os << sg.seed_ << ' ';

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        SeedGenerator<std::array<T, K>, ID, Randomize> &sg)
    {
        if (!is)
            return is;

        seed_type np;
        seed_type rank;
        seed_type maxs;
        seed_type seed;
        is >> std::ws >> np;
        is >> std::ws >> rank;
        is >> std::ws >> maxs;
        is >> std::ws >> seed;

        if (is) {
            sg.np_ = np;
            sg.rank_ = rank;
            sg.maxs_ = maxs;
            sg.seed_ = seed;
        }

        return is;
    }

    protected:
    SeedGenerator() : seed_(1) { partition(1, 0); }

    private:
    static constexpr std::size_t M_ =
        K != 0 && sizeof(T) * K % sizeof(std::uint64_t) == 0 ?
        sizeof(T) * K / sizeof(std::uint64_t) :
        K;

    seed_type np_;
    seed_type rank_;
    seed_type maxs_;
    std::atomic<seed_type> seed_;

    void partition(seed_type np, seed_type rank, std::true_type)
    {
        np_ = np;
        rank_ = rank;
        maxs_ = std::numeric_limits<seed_type>::max();
    }

    void partition(seed_type np, seed_type rank, std::false_type)
    {
        runtime_assert(np > 0,
            "**SeedGenerator::partition** the number of the partitions is "
            "zero");
        if (np < 1)
            np = 1;

        runtime_assert(np > rank,
            "**SeedGenerator::partition** the rank is not smaller than the "
            "number of partitions");
        if (rank >= np)
            rank = 0;

        seed_type maxs = std::numeric_limits<seed_type>::max();
        if (np > 1)
            maxs = (maxs - rank) / np + 1;

        np_ = np;
        rank_ = rank;
        maxs_ = maxs;
    }

    result_type get(std::true_type)
    {
        seed_type s = seed_.fetch_add(1);
        std::array<seed_type, M_> k = {{0}};
        k.front() = s;
        k.back() = rank_;

        return enc(k, std::integral_constant<bool, Randomize>());
    }

    result_type get(std::false_type)
    {
        seed_type s = seed_.fetch_add(1);
        s %= maxs_;
        s *= np_;
        s += rank_;
        std::array<seed_type, M_> k = {{s}};

        return enc(k, std::integral_constant<bool, Randomize>());
    }

    result_type enc(const std::array<seed_type, M_> &k, std::false_type)
    {
        union {
            result_type result;
            std::array<seed_type, M_> k;
        } buf;

        buf.k = k;

        return buf.result;
    }

    result_type enc(const std::array<seed_type, M_> &k, std::true_type)
    {
        return enc(k,
            std::integral_constant<int, std::numeric_limits<T>::digits * K>());
    }

    template <int D>
    result_type enc(
        const std::array<seed_type, M_> &k, std::integral_constant<int, D>)
    {
        return enc(k, std::false_type());
    }

    result_type enc(
        const std::array<seed_type, M_> &k, std::integral_constant<int, 64>)
    {
        return internal::seed_enc<Threefry2x32, result_type>(k);
    }

    result_type enc(
        const std::array<seed_type, M_> &k, std::integral_constant<int, 128>)
    {
        return internal::seed_enc<Threefry2x64, result_type>(k);
    }

    result_type enc(
        const std::array<seed_type, M_> &k, std::integral_constant<int, 256>)
    {
        return internal::seed_enc<Threefry4x64, result_type>(k);
    }

    result_type enc(
        const std::array<seed_type, M_> &k, std::integral_constant<int, 512>)
    {
        return internal::seed_enc<Threefry8x64, result_type>(k);
    }

    result_type enc(
        const std::array<seed_type, M_> &k, std::integral_constant<int, 1024>)
    {
        return internal::seed_enc<Threefry16x64, result_type>(k);
    }
}; // class Seed

/// \brief Seeding RNG engines
/// \ingroup Random
template <typename RNGType, typename ID = typename SeedType<RNGType>::type,
    bool Randomize = true>
using Seed = SeedGenerator<typename SeedType<RNGType>::type, ID, Randomize>;

} // namespace mckl

#endif // MCKL_RANDOM_SEED_HPP
