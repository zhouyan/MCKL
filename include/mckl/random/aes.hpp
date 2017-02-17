//============================================================================
// MCKL/include/mckl/random/aes.hpp
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

#ifndef MCKL_RANDOM_AES_HPP
#define MCKL_RANDOM_AES_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/aes_constants.hpp>
#include <mckl/random/internal/aes_generic.hpp>
#include <mckl/random/internal/aes_key_seq.hpp>
#include <mckl/random/counter.hpp>
#include <mckl/random/increment.hpp>

#if MCKL_HAS_AESNI
#include <mckl/random/internal/aes_aesni.hpp>
#else
#endif

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

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

#if MCKL_USE_AESNI

using AES128KeySeqGenerator = AES128KeySeqGeneratorAESNIImpl;
using AES192KeySeqGenerator = AES192KeySeqGeneratorAESNIImpl;
using AES256KeySeqGenerator = AES256KeySeqGeneratorAESNIImpl;

template <typename Constants>
using ARSKeySeqGenerator = ARSKeySeqGeneratorAESNIImpl<Constants>;

template <typename KeySeqType>
using AESGeneratorImpl = AESGeneratorAESNIImpl<KeySeqType>;

#else // MCKL_USE_AESNI

using AES128KeySeqGenerator = AES128KeySeqGeneratorGenericImpl;
using AES192KeySeqGenerator = AES192KeySeqGeneratorGenericImpl;
using AES256KeySeqGenerator = AES256KeySeqGeneratorGenericImpl;

template <typename Constants>
using ARSKeySeqGenerator = ARSKeySeqGeneratorGenericImpl<Constants>;

template <typename KeySeqType>
using AESGeneratorImpl = AESGeneratorGenericImpl<KeySeqType>;

#endif // MCKL_USE_AESNI

} // namespace mck::internal

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
/// \tparam Rounds The number of rounds
/// \tparam Constants A trait class that defines algorithm constants
template <std::size_t Rounds = MCKL_ARS_ROUNDS,
    typename Constants = ARSConstants>
using ARSKeySeq =
    internal::ARSKeySeqImpl<Rounds, internal::ARSKeySeqGenerator<Constants>>;

/// \brief RNG generator using AES round functions
/// \ingroup AES
template <typename KeySeqType>
class AESGenerator
{
    static_assert(KeySeqType::rounds() != 0,
        "**AESGenerate** used with KeySeqType::rounds() equal to zero");

    public:
    using ctr_type = Counter<std::uint32_t, 4>;
    using key_type = typename KeySeqType::key_type;

    static constexpr std::size_t size() { return sizeof(std::uint32_t) * 4; }

    key_type key() const { return key_seq_.key(); }

    void reset(const key_type &key) { key_seq_.set(key); }

    void operator()(const void *plain, void *cipher) const
    {
        internal::AESGeneratorImpl<KeySeqType>::eval(plain, cipher, key_seq_);
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, ResultType *r) const
    {
        internal::AESGeneratorImpl<KeySeqType>::eval(ctr, r, key_seq_);
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, std::size_t n, ResultType *r) const
    {
        internal::AESGeneratorImpl<KeySeqType>::eval(ctr, n, r, key_seq_);
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
    KeySeqType key_seq_;
}; // class AESGenerator

/// \brief RNG engine using AES round function
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

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_AES_HPP
