//============================================================================
// MCKL/include/mckl/random/seed.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2017, Yan Zhou
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
#include <mckl/random/skein.hpp>
#include <mckl/random/threefry.hpp>
#include <atomic>

/// \brief SeedGeneartor by default use randomized output
/// \ingroup Config
#ifndef MCKL_SEED_RANDOMIZE
#define MCKL_SEED_RANDOMIZE 1
#endif

/// \brief SeedGeneartor by default use atomic operation
/// \ingroup Config
#ifndef MCKL_SEED_ATOMIC
#define MCKL_SEED_ATOMIC 1
#endif

namespace mckl
{

/// \brief Seed generator
/// \ingroup Random
template <typename ResultType,
    typename ID = std::integral_constant<std::size_t, sizeof(ResultType)>,
#if MCKL_SEED_RANDOMIZE
    bool Randomize = true,
#else
    bool Randomize = false,
#endif
#if MCKL_SEED_ATOMIC
    bool Atomic = true
#else
    bool Atomic = false
#endif
    >
class SeedGenerator
{
    static_assert(sizeof(ResultType) % sizeof(std::uint32_t) == 0,
        "**SeedGenerator** size of ResultType is not a multiple of uint32_t");

    public:
    /// \brief The type of the internal seed
    using seed_type =
        std::conditional_t<sizeof(ResultType) % sizeof(std::uint64_t) == 0,
            std::uint64_t, std::uint32_t>;

    using result_type = ResultType;

    SeedGenerator(
        const SeedGenerator<ResultType, ID, Randomize, Atomic> &) = delete;

    SeedGenerator<ResultType, ID, Randomize, Atomic> &operator=(
        const SeedGenerator<ResultType, ID, Randomize, Atomic> &) = delete;

    static SeedGenerator<ResultType, ID, Randomize, Atomic> &instance()
    {
        static SeedGenerator<ResultType, ID, Randomize, Atomic> seed;

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
        const SeedGenerator<ResultType, ID, Randomize, Atomic> &sg)
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
        SeedGenerator<ResultType, ID, Randomize, Atomic> &sg)
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

    private:
    static constexpr std::size_t M_ = sizeof(result_type) / sizeof(seed_type);

    seed_type np_;
    seed_type rank_;
    seed_type maxs_;
    std::conditional_t<Atomic, std::atomic<seed_type>, seed_type> seed_;

    SeedGenerator() : seed_(1) { partition(1, 0); }

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
        seed_type s = seed_++;
        std::array<seed_type, M_> k = {{0}};
        k.front() = s;
        k.back() = rank_;

        return randomize(k, std::integral_constant<bool, Randomize>());
    }

    result_type get(std::false_type)
    {
        seed_type s = seed_++;
        s %= maxs_;
        s *= np_;
        s += rank_;
        std::array<seed_type, M_> k = {{s}};

        return randomize(k, std::integral_constant<bool, Randomize>());
    }

    result_type randomize(const std::array<seed_type, M_> &k, std::false_type)
    {
        union {
            std::array<seed_type, M_> k;
            result_type result;
        } buf;

        buf.k = k;
        internal::union_le<seed_type>(buf.result);

        return buf.result;
    }

    result_type randomize(const std::array<seed_type, M_> &k, std::true_type)
    {
        return randomize(
            k, std::integral_constant<int,
                   std::numeric_limits<seed_type>::digits * M_>());
    }

    template <int D>
    result_type randomize(
        const std::array<seed_type, M_> &k, std::integral_constant<int, D>)
    {
        union {
            std::array<char, sizeof(seed_type) * M_> state;
            std::array<seed_type, M_> k;
            result_type result;
        } buf;

        buf.k = k;
        internal::union_le<seed_type>(buf.state);
        Skein512::hash(
            Skein512::param_type(D, buf.state.data()), D, buf.state.data());
        internal::union_le<char>(buf.result);

        return buf.result;
    }

    result_type randomize(
        const std::array<seed_type, M_> &k, std::integral_constant<int, 32>)
    {
        union {
            std::array<std::uint16_t, 2> state;
            std::array<seed_type, M_> k;
            result_type result;
        } buf;

        buf.k = k;
        internal::union_le<seed_type>(buf.state);
        std::uint16_t x = std::get<0>(buf.state);
        std::uint16_t y = std::get<1>(buf.state);
        for (int i = 0; i != 22; ++i) {
            x = static_cast<std::uint16_t>((x << 9) | (x >> 7));
            x += y;
            y = static_cast<std::uint16_t>((y << 2) | (y >> 14));
            y ^= x;
        }
        std::get<0>(buf.state) = x;
        std::get<1>(buf.state) = y;
        internal::union_le<std::uint16_t>(buf.result);

        return buf.result;
    }

    result_type randomize(
        const std::array<seed_type, M_> &k, std::integral_constant<int, 64>)
    {
        return randomize<Threefry2x32>(k);
    }

    result_type randomize(
        const std::array<seed_type, M_> &k, std::integral_constant<int, 128>)
    {
        return randomize<Threefry2x64>(k);
    }

    result_type randomize(
        const std::array<seed_type, M_> &k, std::integral_constant<int, 256>)
    {
        return randomize<Threefry4x64>(k);
    }

    result_type randomize(
        const std::array<seed_type, M_> &k, std::integral_constant<int, 512>)
    {
        return randomize<Threefry8x64>(k);
    }

    result_type randomize(
        const std::array<seed_type, M_> &k, std::integral_constant<int, 1024>)
    {
        return randomize<Threefry16x64>(k);
    }

    template <typename RNGType>
    result_type randomize(const std::array<seed_type, M_> &k)
    {
        union {
            std::array<char, sizeof(seed_type) * M_> state;
            std::array<seed_type, M_> k;
            result_type result;
        } buf;

        buf.k = k;
        internal::union_le<seed_type>(buf.state);
        RNGType rng(0);
        rng.generator()(buf.state.data(), buf.state.data());
        internal::union_le<char>(buf.result);

        return buf.result;
    }
}; // class Seed

/// \brief RNG default seed generator
/// \ingroup Random
template <typename RNGType>
class Seed : public SeedGenerator<SeedType<RNGType>>
{
}; // class Seed

} // namespace mckl

#endif // MCKL_RANDOM_SEED_HPP
