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

namespace mckl
{

/// \brief Seeding RNG engines
/// \ingroup Random
///
/// \details
/// The sequence of seeds are belongs to the equivalent class \f$s \mod D
/// \equiv R\f$ where \f$D > 0\f$, \f$R \ge 0\f$. The defaults are \f$D = 1\f$
/// and \f$R = 0\f$. Each time `get()` is called, a new seed is returned.
///
/// \note
/// If \f$D = 1\f$ (and \f$R = 0\f$), then there will be exactly two seeds
/// equal to `1` in any sequence of length \f$2^N\f$, where \f$N\f$ is the
/// number of digits in the unsigned integer type. They are either one at the
/// beginning and one at the end, or consecutive.
template <typename RNGType>
class Seed
{
    public:
    using rng_type = RNGType;
    using result_type = typename rng_type::result_type;

    Seed(const Seed<RNGType> &) = delete;
    Seed<RNGType> &operator=(const Seed<RNGType> &) = delete;

    static Seed<RNGType> &instance()
    {
        static Seed<RNGType> seed;

        return seed;
    }

    /// \brief Create a new RNG using a new seed
    rng_type create() { return rng_type(get()); }

    /// \brief Seed a single RNG
    void operator()(rng_type &rng) { rng.seed(get()); }

    /// \brief Seed a sequence of RNGs
    template <typename OutputIter>
    OutputIter operator()(std::size_t n, OutputIter first)
    {
        static_assert(
            std::is_same<rng_type,
                typename std::remove_cv<typename std::iterator_traits<
                    OutputIter>::value_type>::type>::value,
            "**Seed** used with an unsupported RNG type");

        for (std::size_t i = 0; i != n; ++i, ++first)
            first->seed(get());

        return first;
    }

    /// \brief Seed a sequence of RNGs
    template <typename OutputIter>
    OutputIter operator()(OutputIter first, OutputIter last)
    {
        static_assert(
            std::is_same<rng_type,
                typename std::remove_cv<typename std::iterator_traits<
                    OutputIter>::value_type>::type>::value,
            "**Seed** used with an unsupported RNG type");

        while (first != last) {
            first->seed(get());
            ++first;
        }

        return first;
    }

    /// \brief Set the seed to `s % max() * divisor() + remainder()`
    void set(result_type s) { seed_ = s % max_; }

    /// \brief Get a seed
    result_type get()
    {
        result_type s = seed_.fetch_add(1);

        return (s % max_ + 1) * divisor_ + remainder_;
    }

    /// \brief The maximum of the seed
    result_type max() const { return max_; }

    /// \brief The divisor of the output seed
    result_type divisor() const { return divisor_; }

    /// \brief The remainder of the output seed
    result_type remainder() const { return remainder_; }

    /// \brief Set the divisor and the remainder
    void modulo(result_type divisor, result_type remainder)
    {
        runtime_assert(divisor > remainder,
            "**Seed::modulo** the remainder is not smaller than the divisor");

        result_type maxs =
            (std::numeric_limits<result_type>::max() - remainder) / divisor;
        runtime_assert(maxs > 1,
            "**Seed::modulo** the maximum of the internal seed will be no "
            "larger than 1");

        divisor_ = divisor;
        remainder_ = remainder;
        max_ = maxs;

        set(seed_);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os, const Seed<RNGType> &sg)
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
        std::basic_istream<CharT, Traits> &is, Seed<RNGType> &sg)
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

    private:
    std::atomic<result_type> seed_;
    result_type max_;
    result_type divisor_;
    result_type remainder_;

    Seed() : seed_(0), max_(0), divisor_(1), remainder_(0)
    {
        modulo(divisor_, remainder_);
    }
}; // class Seed

} // namespace mckl

#endif // MCKL_RANDOM_SEED_HPP
