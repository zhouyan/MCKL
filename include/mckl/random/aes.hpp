//============================================================================
// MCKL/include/mckl/random/aes.hpp
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

#ifndef MCKL_RANDOM_AES_HPP
#define MCKL_RANDOM_AES_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/aes_constants.hpp>
#include <mckl/random/counter.hpp>

/// \brief AES128Engine default rounds
/// \ingroup Config
#ifndef MCKL_AES128_ROUNDS
#define MCKL_AES128_ROUNDS 10
#endif

/// \brief AES192Engine default rounds
/// \ingroup Config
#ifndef MCKL_AES192_ROUNDS
#define MCKL_AES192_ROUNDS 12
#endif

/// \brief AES256Engine default rounds
/// \ingroup Config
#ifndef MCKL_AES256_ROUNDS
#define MCKL_AES256_ROUNDS 14
#endif

/// \brief ARSEngine default rounds
/// \ingroup Config
#ifndef MCKL_ARS_ROUNDS
#define MCKL_ARS_ROUNDS 5
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

#include <mckl/random/internal/aes_aesni.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

template <std::size_t Rounds, typename KeySeqGenerator>
class AESKeySeqImpl
{
    public:
    using int128_type = typename KeySeqGenerator::int128_type;
    using key_type = typename KeySeqGenerator::key_type;

    static constexpr std::size_t rounds() { return Rounds; }

    key_type key() const { return KeySeqGenerator::key(rk_); }

    void reset(const key_type &key)
    {
        KeySeqGenerator generator;
        generator(key, rk_);
    }

    const std::array<int128_type, rounds() + 1> get() const { return rk_; }

    friend bool operator==(const AESKeySeqImpl<Rounds, KeySeqGenerator> &seq1,
        const AESKeySeqImpl<Rounds, KeySeqGenerator> &seq2)
    {
        std::array<std::uint64_t, 2 * (rounds() + 1)> ks1;
        std::array<std::uint64_t, 2 * (rounds() + 1)> ks2;
        std::memcpy(ks1.data(), seq1.rk_.data(), 16 * (rounds() + 1));
        std::memcpy(ks2.data(), seq2.rk_.data(), 16 * (rounds() + 1));

        return ks1 == ks2;
    }

    friend bool operator!=(const AESKeySeqImpl<Rounds, KeySeqGenerator> &seq1,
        const AESKeySeqImpl<Rounds, KeySeqGenerator> &seq2)
    {
        return !(seq1 == seq2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const AESKeySeqImpl<Rounds, KeySeqGenerator> &seq)
    {
        if (!os)
            return os;

        std::array<std::uint64_t, 2 * (rounds() + 1)> ks;
        std::memcpy(ks.data(), seq.rk_.data(), 16 * (rounds() + 1));
        ostream(os, ks);

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        AESKeySeqImpl<Rounds, KeySeqGenerator> &seq)
    {
        if (!is)
            return is;

        std::array<std::uint64_t, 2 * (rounds() + 1)> ks;
        istream(is, ks);
        if (is)
            std::memcpy(seq.rk_.data(), ks.data(), 16 * (rounds() + 1));

        return is;
    }

    private:
    std::array<int128_type, rounds() + 1> rk_;
}; // class AESKeySeqImpl

template <std::size_t Rounds, typename Constants>
class ARSKeySeqImpl
{
    public:
    using int128_type = typename ARSKeySeqGenerator<Constants>::int128_type;
    using key_type = typename ARSKeySeqGenerator<Constants>::key_type;

    static constexpr std::size_t rounds() { return Rounds; }

    key_type key() const { return key_; }

    void reset(const key_type &key) { key_ = key; }

    std::array<int128_type, rounds() + 1> get() const
    {
        ARSKeySeqGenerator<Constants> generator;
        std::array<int128_type, rounds() + 1> rk;
        generator(key_, rk);

        return rk;
    }

    friend bool operator==(const ARSKeySeqImpl<Rounds, Constants> &seq1,
        const ARSKeySeqImpl<Rounds, Constants> &seq2)
    {
        return seq1.key_ == seq2.key_;
    }

    friend bool operator!=(const ARSKeySeqImpl<Rounds, Constants> &seq1,
        const ARSKeySeqImpl<Rounds, Constants> &seq2)
    {
        return !(seq1 == seq2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const ARSKeySeqImpl<Rounds, Constants> &seq)
    {
        if (!os)
            return os;

        ostream(os, seq.key_);

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        ARSKeySeqImpl<Rounds, Constants> &seq)
    {
        if (!is)
            return is;

        key_type k = {{0}};
        istream(is, k);
        if (is)
            seq.key_ = k;

        return is;
    }

    private:
    key_type key_;
}; // class ARSKeySeqImpl

} // namespace mck::/internal

/// \brief AES128Engine key sequence generator
/// \ingroup AES
template <std::size_t Rounds = MCKL_AES128_ROUNDS>
using AES128KeySeq =
    internal::AESKeySeqImpl<Rounds, internal::AES128KeySeqGenerator>;

/// \brief AES192Engine key sequence generator
/// \ingroup AES
template <std::size_t Rounds = MCKL_AES192_ROUNDS>
using AES192KeySeq =
    internal::AESKeySeqImpl<Rounds, internal::AES192KeySeqGenerator>;

/// \brief AES256Engine key sequence generator
/// \ingroup AES
template <std::size_t Rounds = MCKL_AES256_ROUNDS>
using AES256KeySeq =
    internal::AESKeySeqImpl<Rounds, internal::AES256KeySeqGenerator>;

/// \brief Default ARSEngine key sequence generator
/// \ingroup AES
///
/// \tparam Rounds Number of rounds of AES encryption
/// \tparam Constants A trait class that defines algorithm constants, see
/// ARSConstants
///
/// \details
/// This generator implement the ARS algorithm in
/// [Random123](http://www.deshawresearch.com/resources_random123.html),
/// developed John K. Salmon, Mark A. Moraes, Ron O. Dror, and David E. Shaw.
template <std::size_t Rounds = MCKL_ARS_ROUNDS,
    typename Constants = ARSConstants>
using ARSKeySeq = internal::ARSKeySeqImpl<Rounds, Constants>;

/// \brief RNG generator using AES round functions
/// \ingroup AES
template <typename KeySeqType>
class AESGenerator
{
    static_assert(KeySeqType::rounds() != 0,
        "**AESGenerate** used with KeySeqType::rounds() equal to zero");

    public:
    using ctr_type = Counter<std::uint64_t, 2>;
    using key_type = typename KeySeqType::key_type;

    static constexpr std::size_t size() { return 128 / CHAR_BIT; }

    key_type key() const { return key_seq_.key(); }

    void reset(const key_type &key) { key_seq_.reset(key); }

    void enc(const void *plain, void *cipher) const
    {
        alignas(32) union {
            std::array<int128_type, 1> state;
            std::array<char, size()> result;
        } buf;

        std::memcpy(buf.result.data(), plain, size());
        internal::union_le<char>(buf.state);
        std::array<int128_type, rounds_ + 1> rk(key_seq_.get());
        internal::AESGeneratorImpl<KeySeqType>::eval(
            std::get<0>(buf.state), rk);
        internal::union_le<int128_type>(buf.result);
        std::memcpy(cipher, buf.result.data(), size());
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, ResultType *result) const
    {
        std::array<int128_type, rounds_ + 1> rk(key_seq_.get());
        generate(ctr, result, rk);
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, std::size_t n, ResultType *result) const
    {
        generate(ctr, n, result,
            std::integral_constant<bool,
                internal::AESGeneratorImpl<KeySeqType>::batch()>());
    }

    friend bool operator==(const AESGenerator<KeySeqType> &gen1,
        const AESGenerator<KeySeqType> &gen2)
    {
        return gen1.key_seq_ == gen2.key_seq_;
    }

    friend bool operator!=(const AESGenerator<KeySeqType> &gen1,
        const AESGenerator<KeySeqType> &gen2)
    {
        return !(gen1 == gen2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const AESGenerator<KeySeqType> &gen)
    {
        if (!os)
            return os;

        os << gen.key_seq_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is, AESGenerator<KeySeqType> &gen)
    {
        if (!is)
            return is;

        AESGenerator<KeySeqType> gen_tmp;
        is >> std::ws >> gen_tmp.key_seq_;

        if (is)
            gen = std::move(gen_tmp);

        return is;
    }

    private:
    using int128_type = typename KeySeqType::int128_type;

    static constexpr std::size_t rounds_ = KeySeqType::rounds();

    KeySeqType key_seq_;

    template <typename ResultType>
    void generate(ctr_type &ctr, ResultType *result,
        const std::array<int128_type, rounds_ + 1> &rk) const
    {
        alignas(32) union {
            std::array<int128_type, 1> state;
            ctr_type ctr;
            std::array<ResultType, size() / sizeof(ResultType)> result;
        } buf;

        MCKL_FLATTEN_CALL increment(ctr);
        buf.ctr = ctr;
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        internal::union_le<typename ctr_type::value_type>(buf.state);
#endif
        internal::AESGeneratorImpl<KeySeqType>::eval(
            std::get<0>(buf.state), rk);
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        internal::union_le<int128_type>(buf.result);
#endif
        std::memcpy(result, buf.result.data(), size());
    }

    template <typename ResultType>
    void generate(ctr_type &ctr, std::size_t n, ResultType *result,
        const std::array<int128_type, rounds_ + 1> &rk, std::false_type) const
    {
        static constexpr std::size_t stride = size() / sizeof(ResultType);

        for (std::size_t i = 0; i != n; ++i, result += stride)
            generate(ctr, result, rk);
    }

    template <typename ResultType>
    void generate(
        ctr_type &ctr, std::size_t n, ResultType *result, std::true_type) const
    {
        static constexpr std::size_t stride = size() / sizeof(ResultType);
        static constexpr std::size_t blocks =
            internal::AESGeneratorImpl<KeySeqType>::blocks();

        alignas(32) union {
            std::array<int128_type, blocks> state;
            std::array<ctr_type, blocks> ctr_block;
            std::array<ResultType, size() / sizeof(ResultType) * blocks>
                result;
        } buf;

        std::array<int128_type, rounds_ + 1> rk(key_seq_.get());

        const std::size_t m = n / blocks;
        const std::size_t l = n % blocks;
        for (std::size_t i = 0; i != m; ++i, result += stride * blocks) {
            MCKL_FLATTEN_CALL increment(ctr, buf.ctr_block);
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
            internal::union_le<typename ctr_type::value_type>(buf.state);
#endif
            internal::AESGeneratorImpl<KeySeqType>::eval(buf.state, rk);
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
            internal::union_le<int128_type>(buf.result);
#endif
            std::memcpy(result, buf.result.data(), size() * blocks);
        }
        for (std::size_t i = 0; i != l; ++i, result += stride)
            generate(ctr, result, rk);
    }
}; // class AESGenerator

/// \brief RNG engine using AES-NI instructions
/// \ingroup AES
template <typename ResultType, typename KeySeqType>
using AESEngine = CounterEngine<ResultType, AESGenerator<KeySeqType>>;

/// \brief AES-128 RNG engine
/// \ingroup AES
template <typename ResultType, std::size_t Rounds = MCKL_AES128_ROUNDS>
using AES128Engine = AESEngine<ResultType, AES128KeySeq<Rounds>>;

/// \brief AES-192 RNG engine
/// \ingroup AES
template <typename ResultType, std::size_t Rounds = MCKL_AES192_ROUNDS>
using AES192Engine = AESEngine<ResultType, AES192KeySeq<Rounds>>;

/// \brief AES-256 RNG engine
/// \ingroup AES
template <typename ResultType, std::size_t Rounds = MCKL_AES256_ROUNDS>
using AES256Engine = AESEngine<ResultType, AES256KeySeq<Rounds>>;

/// \brief ARS RNG engine
/// \ingroup AES
template <typename ResultType, std::size_t Rounds = MCKL_ARS_ROUNDS,
    typename Constants = ARSConstants>
using ARSEngine = AESEngine<ResultType, ARSKeySeq<Rounds, Constants>>;

/// \brief AES-128 RNG engine with 32-bit integers output
/// \ingroup AES
using AES128 = AES128Engine<std::uint32_t>;

/// \brief AES-192 RNG engine with 32-bit integers output
/// \ingroup AES
using AES192 = AES192Engine<std::uint32_t>;

/// \brief AES-256 RNG engine with 32-bit integers output
/// \ingroup AES
using AES256 = AES256Engine<std::uint32_t>;

/// \brief ARS RNG engine with 32-bit integers output
/// \ingroup AES
using ARS = ARSEngine<std::uint32_t>;

/// \brief AES-128 RNG engine with 64-bit integers output
/// \ingroup AES
using AES128_64 = AES128Engine<std::uint64_t>;

/// \brief AES-192 RNG engine with 64-bit integers output
/// \ingroup AES
using AES192_64 = AES192Engine<std::uint64_t>;

/// \brief AES-256 RNG engine with 64-bit integers output
/// \ingroup AES
using AES256_64 = AES256Engine<std::uint64_t>;

/// \brief ARS RNG engine with 64-bit integers output
/// \ingroup AES
using ARS_64 = ARSEngine<std::uint64_t>;

} // namespace internal

#endif // MCKL_RANDOM_AES_HPP
