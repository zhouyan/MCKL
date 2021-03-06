//============================================================================
// MCKL/include/mckl/random/counter.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
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
#include <mckl/random/increment.hpp>

namespace mckl {

MCKL_PUSH_CLANG_WARNING("-Wpadded")
/// \brief Counter based RNG engine
/// \ingroup Random
///
/// \tparam ResultType The ouptut integer type of the counter-based RNG engine
/// \tparam Generator The generator that transfer counter and key to a result
/// block
/// - Requirement
/// ~~~{.cpp}
/// ctr_type; /* counter type */
/// key_type; /* key type */
/// static constexpr std::size_t size(); /* Size of the result block in byte */
/// void reset(const key_type &key);     /* reset generator key */
///
/// /* Increment counter once and generate one result block */
/// void operator()(ctr_type &ctr, result_type *r);
///
/// /* Increment counter n times and generate n result blocks */
/// void operator()(ctr_type &ctr, std::size_t n, result_type *r);
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

  public:
    using result_type = ResultType;
    using generator_type = Generator;
    using ctr_type = typename generator_type::ctr_type;
    using key_type = typename generator_type::key_type;
    using skip_type = typename ctr_type::value_type;

  private:
    template <typename T>
    using is_seed_seq = internal::is_seed_seq<T,
        CounterEngine<ResultType, Generator>, key_type>;

  public:
    explicit CounterEngine(result_type s = 1) : index_(M_) { seed(s); }

    template <typename SeedSeq>
    explicit CounterEngine(SeedSeq &seq,
        std::enable_if_t<is_seed_seq<SeedSeq>::value> * = nullptr)
        : index_(M_)
    {
        seed(seq);
    }

    explicit CounterEngine(const key_type &k) : index_(M_) { seed(k); }

    void seed(result_type s)
    {
        key_type key;
        std::fill(key.begin(), key.end(), 0);
        key.front() = static_cast<typename key_type::value_type>(s);
        reset(key);
    }

    template <typename SeedSeq>
    void seed(SeedSeq &seq,
        std::enable_if_t<is_seed_seq<SeedSeq>::value> * = nullptr)
    {
        key_type key;
        seq.generator(key.begin(), key.end());
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

    generator_type &generator() { return generator_; }

    const generator_type &generator() const { return generator_; }

    result_type operator()()
    {
        if (index_ == M_) {
            generator_(ctr_, result_.data());
            index_ = 0;
        }

        return result_[index_++];
    }

    void operator()(std::size_t n, result_type *r)
    {
        const std::size_t remain = static_cast<std::size_t>(M_ - index_);

        if (n <= remain) {
            std::memcpy(r, result_.data() + index_, sizeof(result_type) * n);
            index_ += static_cast<unsigned>(n);
            return;
        }

        std::memcpy(r, result_.data() + index_, sizeof(result_type) * remain);
        r += remain;
        n -= remain;
        index_ = M_;

        const std::size_t m = n / M_;
        generator_(ctr_, m, r);
        r += m * M_;
        n -= m * M_;

        generator_(ctr_, result_.data());
        std::memcpy(r, result_.data(), sizeof(result_type) * n);
        index_ = static_cast<unsigned>(n);
    }

    /// \brief Discard the result
    ///
    /// \return The number of results discarded
    std::size_t discard()
    {
        const std::size_t remain = static_cast<std::size_t>(M_ - index_);
        index_ = M_;

        return remain;
    }

    void discard(skip_type nskip)
    {
        if (nskip == 0) {
            return;
        }

        const skip_type remain = static_cast<skip_type>(M_ - index_);
        if (nskip <= remain) {
            index_ += static_cast<unsigned>(nskip);
            return;
        }
        nskip -= remain;
        index_ = M_;

        skip_type M = static_cast<skip_type>(M_);
        std::size_t res_size = sizeof(result_type) * M_;
        std::size_t ctr_size = sizeof(ctr_type);
        skip_type rate = static_cast<skip_type>(res_size / ctr_size);
        increment(ctr_, nskip / M * rate);
        generator_(ctr_, result_.data());
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

    friend bool operator==(const CounterEngine<ResultType, Generator> &eng1,
        const CounterEngine<ResultType, Generator> &eng2)
    {
        if (eng1.result_ != eng2.result_) {
            return false;
        }
        if (eng1.ctr_ != eng2.ctr_) {
            return false;
        }
        if (eng1.generator_ != eng2.generator_) {
            return false;
        }
        if (eng1.index_ != eng2.index_) {
            return false;
        }
        return true;
    }

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
        if (!os) {
            return os;
        }

        os << eng.result_ << ' ';
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
        if (!is) {
            return is;
        }

        CounterEngine<ResultType, Generator> eng_tmp;
        is >> std::ws >> eng_tmp.result_;
        is >> std::ws >> eng_tmp.ctr_;
        is >> std::ws >> eng_tmp.generator_;
        is >> std::ws >> eng_tmp.index_;

        if (is) {
            eng = std::move(eng_tmp);
        }

        return is;
    }

  private:
    static constexpr unsigned M_ = Generator::size() / sizeof(ResultType);

    std::array<result_type, M_> result_;
    ctr_type ctr_;
    generator_type generator_;
    unsigned index_;

    void reset(const key_type key)
    {
        std::fill(ctr_.begin(), ctr_.end(), 0);
        generator_.reset(key);
        index_ = M_;
    }
}; // class CounterEngine
MCKL_POP_CLANG_WARNING

template <typename ResultType, typename Generator>
class SeedTrait<CounterEngine<ResultType, Generator>>
{
  public:
    using type = typename CounterEngine<ResultType, Generator>::key_type;
}; // class SeedTrait

template <typename ResultType, typename Generator>
inline void rand(
    CounterEngine<ResultType, Generator> &rng, std::size_t n, ResultType *r)
{
    rng(n, r);
}

} // namespace mckl

#endif // MCKL_RANDOM_COUNTER_HPP
