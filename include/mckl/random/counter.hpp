//============================================================================
// MCKL/include/mckl/random/counter.hpp
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

#ifndef MCKL_RANDOM_COUNTER_HPP
#define MCKL_RANDOM_COUNTER_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/seed.hpp>

namespace mckl
{

namespace internal
{

template <std::size_t, typename T, std::size_t K>
inline void increment_single(std::array<T, K> &, std::false_type)
{
}

template <std::size_t N, typename T, std::size_t K>
inline void increment_single(std::array<T, K> &ctr, std::true_type)
{
    if (++std::get<N>(ctr) != 0)
        return;

    increment_single<N + 1>(ctr, std::integral_constant<bool, N + 1 < K>());
}

} // namespace mckl::internal

/// \brief Increment a counter by one
/// \ingroup Random
template <typename T, std::size_t K>
inline void increment(std::array<T, K> &ctr)
{
    internal::increment_single<0>(ctr, std::true_type());
}

/// \brief Increment a counter by given steps
/// \ingroup Random
template <typename T, std::size_t K, T NSkip>
inline void increment(std::array<T, K> &ctr, std::integral_constant<T, NSkip>)
{
    if (ctr.front() < std::numeric_limits<T>::max() - NSkip) {
        ctr.front() += NSkip;
    } else {
        ctr.front() += NSkip;
        internal::increment_single<1>(
            ctr, std::integral_constant<bool, 1 < K>());
    }
}

/// \brief Increment a counter by given steps
/// \ingroup Random
template <typename T, std::size_t K>
inline void increment(std::array<T, K> &ctr, T nskip)
{
    if (ctr.front() < std::numeric_limits<T>::max() - nskip) {
        ctr.front() += nskip;
    } else {
        ctr.front() += nskip;
        internal::increment_single<1>(
            ctr, std::integral_constant<bool, 1 < K>());
    }
}

namespace internal
{

template <std::size_t, typename T, std::size_t K, std::size_t Blocks>
inline void increment_block_set(const std::array<T, K> &,
    std::array<std::array<T, K>, Blocks> &, std::false_type)
{
}

template <std::size_t B, typename T, std::size_t K, std::size_t Blocks>
inline void increment_block_set(const std::array<T, K> &ctr,
    std::array<std::array<T, K>, Blocks> &ctr_block, std::true_type)
{
    std::get<B>(ctr_block) = ctr;
    increment_block_set<B + 1>(
        ctr, ctr_block, std::integral_constant<bool, B + 1 < Blocks>());
}

template <std::size_t, typename T, std::size_t K, std::size_t Blocks>
inline void increment_block(std::array<T, K> &,
    std::array<std::array<T, K>, Blocks> &, std::false_type)
{
}

template <std::size_t B, typename T, std::size_t K, std::size_t Blocks>
inline void increment_block(std::array<T, K> &ctr,
    std::array<std::array<T, K>, Blocks> &ctr_block, std::true_type)
{
    increment(std::get<B>(ctr_block), std::integral_constant<T, B + 1>());
    increment_block<B + 1>(
        ctr, ctr_block, std::integral_constant<bool, B + 1 < Blocks>());
}

template <std::size_t, typename T, std::size_t K, std::size_t Blocks>
inline void increment_block_safe(std::array<T, K> &,
    std::array<std::array<T, K>, Blocks> &, std::false_type)
{
}

template <std::size_t B, typename T, std::size_t K, std::size_t Blocks>
inline void increment_block_safe(std::array<T, K> &ctr,
    std::array<std::array<T, K>, Blocks> &ctr_block, std::true_type)
{
    std::get<B>(ctr_block).front() += B + 1;
    increment_block_safe<B + 1>(
        ctr, ctr_block, std::integral_constant<bool, B + 1 < Blocks>());
}

} // namespace mckl::internal

/// \brief Increment a counter by a given steps, and store each step in an
/// array of counters
/// \ingroup Random
template <typename T, std::size_t K, std::size_t Blocks>
inline void increment(
    std::array<T, K> &ctr, std::array<std::array<T, K>, Blocks> &ctr_block)
{
    internal::increment_block_set<0>(
        ctr, ctr_block, std::integral_constant<bool, 0 < Blocks>());
    if (ctr.front() < std::numeric_limits<T>::max() - static_cast<T>(Blocks)) {
        internal::increment_block_safe<0>(
            ctr, ctr_block, std::integral_constant<bool, 0 < Blocks>());
    } else {
        internal::increment_block<0>(
            ctr, ctr_block, std::integral_constant<bool, 0 < Blocks>());
    }
    ctr = ctr_block.back();
}

/// \brief Counter based RNG engine
/// \ingroup Random
///
/// \tparam ResultType The ouptut integer type of the counter-based RNG engine
/// \tparam Generator The generator that transfer counter and key to random
/// integer buffer.
/// - Requirement
/// ~~~{.cpp}
/// ctr_type; // counter type
/// key_type; // key type
/// static constexpr std::size_t size(); // Size of buffer in bytes
/// void reset(const key_type &key);     // reset generator key
///
/// // Increment counter and generate one random buffer
/// void operator()(ctr_type &ctr,
///     std::array<ResultType, size() / sizeof(ResultType)> &buffer);
///
/// // Increment counter and generate n random buffers
/// void operator()(ctr_type &ctr, std::size_t n,
///     std::array<ResultType, size() / sizeof(ResultType)> *buffer);
/// ~~~
/// - Restrictions: `size() % sizeof(ResultType) == 0`
template <typename ResultType, typename Generator>
class CounterEngine
{
    static_assert(std::is_unsigned<ResultType>::value,
        "**CounterEngine** used with ResultType other than unsigned intger "
        "types");

    static_assert(Generator::size() % sizeof(ResultType) == 0,
        "**CounterEngine** used with Generator::size() not divisible by "
        "sizeof(ResultType)");

    template <typename T>
    using is_seed_seq =
        internal::is_seed_seq<T, CounterEngine<ResultType, Generator>>;

    public:
    using result_type = ResultType;
    using generator_type = Generator;
    using ctr_type = typename generator_type::ctr_type;
    using key_type = typename generator_type::key_type;
    using skip_type = typename ctr_type::value_type;

    explicit CounterEngine(result_type s = 1) : index_(M_) { seed(s); }

    template <typename SeedSeq>
    explicit CounterEngine(SeedSeq &seq,
        typename std::enable_if<is_seed_seq<SeedSeq>::value>::type * = nullptr)
        : index_(M_)
    {
        seed(seq);
    }

    explicit CounterEngine(const key_type &k) : index_(M_) { seed(k); }

    void seed(result_type s)
    {
        key_type key;
        std::memset(key.data(), 0, sizeof(key_type));
        std::memcpy(key.data(), &s, std::min(sizeof(s), sizeof(key)));
        reset(key);
    }

    template <typename SeedSeq>
    void seed(SeedSeq &seq,
        typename std::enable_if<is_seed_seq<SeedSeq>::value>::type * = nullptr)
    {
        key_type key;
        std::array<unsigned, sizeof(key) / sizeof(unsigned) + 1> s;
        seq.generator(s.begin(), s.end());
        std::memcpy(key.data(), s.data(), std::min(sizeof(s), sizeof(key)));
        reset(key);
    }

    void seed(const key_type &key) { reset(key); }

    key_type key() const { return generator_.key(); }

    ctr_type ctr() const { return ctr_; }

    void key(const key_type &k) { reset(k); }

    void ctr(const ctr_type &c)
    {
        ctr_ = c;
        index_ = M_;
    }

    result_type operator()()
    {
        if (index_ == M_) {
            generator_(ctr_, buffer_);
            index_ = 0;
        }

        return buffer_[static_cast<std::size_t>(index_++)];
    }

    void operator()(std::size_t n, result_type *r)
    {
        const std::size_t remain = static_cast<std::size_t>(M_ - index_);

        if (n < remain) {
            std::copy_n(buffer_.data() + index_, n, r);
            index_ += static_cast<unsigned>(n);
            return;
        }

        std::copy_n(buffer_.data() + index_, remain, r);
        r += remain;
        n -= remain;
        index_ = M_;

        const std::size_t m = n / M_;
        generator_(ctr_, m, reinterpret_cast<buffer_type *>(r));
        r += m * M_;
        n -= m * M_;

        generator_(ctr_, buffer_);
        std::copy_n(buffer_.data(), n, r);
        index_ = static_cast<unsigned>(n);
    }

    /// \brief Discard the buffer
    std::size_t discard()
    {
        const std::size_t remain = static_cast<std::size_t>(M_ - index_);
        index_ = M_;

        return remain;
    }

    void discard(skip_type nskip)
    {
        if (nskip == 0)
            return;

        const skip_type remain = static_cast<skip_type>(M_ - index_);
        if (nskip <= remain) {
            index_ += static_cast<unsigned>(nskip);
            return;
        }
        nskip -= remain;
        index_ = M_;

        skip_type M = static_cast<skip_type>(M_);
        std::size_t buf_size = sizeof(buffer_type);
        std::size_t ctr_size = sizeof(ctr_type);
        skip_type rate = static_cast<skip_type>(buf_size / ctr_size);
        increment(ctr_, nskip / M * rate);
        generator_(ctr_, buffer_);
        index_ = static_cast<unsigned>(nskip % M);
    }

    static constexpr result_type min()
    {
        return std::numeric_limits<result_type>::min();
    }

    static constexpr result_type max()
    {
        return std::numeric_limits<result_type>::max();
    }

    /// \brief `eng1 == eng2` is a sufficent condition for subsequent call of
    /// `operator()` output the same results. But it is not a necessary
    /// condition.
    friend bool operator==(const CounterEngine<ResultType, Generator> &eng1,
        const CounterEngine<ResultType, Generator> &eng2)
    {
        if (eng1.buffer_ != eng2.buffer_)
            return false;
        if (eng1.ctr_ != eng2.ctr_)
            return false;
        if (eng1.generator_ != eng2.generator_)
            return false;
        if (eng1.index_ != eng2.index_)
            return false;
        return true;
    }

    /// \brief `eng1 != eng2` is a necessary condition for subsequent call of
    /// `operator()` output different results. But it is not a sufficient
    /// condition.
    friend bool operator!=(const CounterEngine<ResultType, Generator> &eng1,
        const CounterEngine<ResultType, Generator> &eng2)
    {
        return !(eng1 == eng2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const CounterEngine<ResultType, Generator> &eng)
    {
        if (!os)
            return os;

        os << eng.buffer_ << ' ';
        os << eng.ctr_ << ' ';
        os << eng.generator_ << ' ';
        os << eng.index_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        CounterEngine<ResultType, Generator> &eng)
    {
        if (!is)
            return is;

        CounterEngine<ResultType, Generator> eng_tmp;
        is >> std::ws >> eng_tmp.buffer_;
        is >> std::ws >> eng_tmp.ctr_;
        is >> std::ws >> eng_tmp.generator_;
        is >> std::ws >> eng_tmp.index_;

        if (is)
            eng = std::move(eng_tmp);

        return is;
    }

    private:
    static constexpr unsigned M_ = Generator::size() / sizeof(ResultType);

    using buffer_type = std::array<ResultType, M_>;

    buffer_type buffer_;
    ctr_type ctr_;
    generator_type generator_;
    unsigned index_;

    void reset(const key_type key)
    {
        ctr_.fill(0);
        generator_.reset(key);
        index_ = M_;
    }
}; // class CounterEngine

/// \brief Seeding counter RNG engines
/// \ingroup Random
///
/// \details
/// This differs from the generic `Seed` class in that, it directly create the
/// key in the counter based RNG. If the key length is one, then it works the
/// same as the generic version. Otherwise, the divisor is ignored, and the
/// last element of the key is set to the remainder. The first element is set
/// to the seed, which increase by one each time a new seed is generated.
template <typename ResultType, typename Generator>
class Seed<CounterEngine<ResultType, Generator>>
{
    public:
    using rng_type = CounterEngine<ResultType, Generator>;
    using result_type = typename rng_type::key_type::value_type;

    Seed(const Seed<CounterEngine<ResultType, Generator>> &) = delete;
    Seed<CounterEngine<ResultType, Generator>> &operator=(
        const Seed<CounterEngine<ResultType, Generator>> &) = delete;

    static Seed<CounterEngine<ResultType, Generator>> &instance()
    {
        static Seed<CounterEngine<ResultType, Generator>> seed;

        return seed;
    }

    /// \brief Create a new RNG using a new seed
    rng_type operator()() { return rng_type(get(dispatch_type())); }

    /// \brief Seed a single RNG
    void operator()(rng_type &rng) { rng.seed(get(dispatch_type())); }

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
        const Seed<CounterEngine<ResultType, Generator>> &sg)
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
        Seed<CounterEngine<ResultType, Generator>> &sg)
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
    using key_type = typename rng_type::key_type;
    using dispatch_type =
        std::integral_constant<bool, (std::tuple_size<key_type>::value > 1)>;

    std::atomic<result_type> seed_;
    result_type max_;
    result_type divisor_;
    result_type remainder_;

    Seed() : seed_(0), max_(0), divisor_(1), remainder_(0)
    {
        modulo(divisor_, remainder_);
    }

    void modulo(result_type, result_type remainder, std::true_type)
    {
        max_ = std::numeric_limits<result_type>::max();
        remainder_ = remainder;
    }

    void modulo(result_type divisor, result_type remainder, std::false_type)
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

    key_type get(std::true_type)
    {
        result_type s = seed_.fetch_add(1);
        key_type k;
        k.front() = s + 1;
        k.back() = remainder_;

        return k;
    }

    key_type get(std::false_type)
    {
        result_type s = seed_.fetch_add(1);
        key_type k;
        k.front() = (s % max_ + 1) * divisor_ + remainder_;

        return k;
    }
}; // class Seed

template <typename ResultType, typename Generator>
inline void rand(
    CounterEngine<ResultType, Generator> &rng, std::size_t n, ResultType *r)
{
    rng(n, r);
}

} // namespace mckl

#endif // MCKL_RANDOM_COUNTER_HPP
