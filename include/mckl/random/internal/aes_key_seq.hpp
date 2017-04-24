//============================================================================
// MCKL/include/mckl/random/internal/aes_key_seq.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_AES_KEY_SEQ_HPP
#define MCKL_RANDOM_INTERNAL_AES_KEY_SEQ_HPP

#include <mckl/random/internal/common.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

namespace mckl {

namespace internal {

template <std::size_t Rounds, typename KeySeqGenerator>
class AESKeySeqImpl
{
  public:
    using key_type = typename KeySeqGenerator::key_type;
    using rk_type = typename KeySeqGenerator::rk_type;

    static constexpr std::size_t rounds() { return Rounds; }

    key_type key() const { return KeySeqGenerator::key(rk_); }

    void set(const key_type &key)
    {
        KeySeqGenerator generator;
        generator(key, rk_);
    }

    const std::array<rk_type, rounds() + 1> &get() const { return rk_; }

    template <std::size_t N>
    const rk_type &get() const
    {
        return std::get<N>(rk_);
    }

    friend bool operator==(const AESKeySeqImpl<Rounds, KeySeqGenerator> &seq1,
        const AESKeySeqImpl<Rounds, KeySeqGenerator> &seq2)
    {
        std::array<std::uint64_t, 2 * (rounds() + 1)> ks1;
        std::array<std::uint64_t, 2 * (rounds() + 1)> ks2;
        std::memcpy(ks1.data(), seq1.rk_.data(),
            sizeof(std::uint64_t) * 2 * (rounds() + 1));
        std::memcpy(ks2.data(), seq2.rk_.data(),
            sizeof(std::uint64_t) * 2 * (rounds() + 1));

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
        std::memcpy(ks.data(), seq.rk_.data(),
            sizeof(std::uint64_t) * 2 * (rounds() + 1));
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
        if (is) {
            std::memcpy(seq.rk_.data(), ks.data(),
                sizeof(std::uint64_t) * 2 * (rounds() + 1));
        }

        return is;
    }

  private:
    std::array<rk_type, rounds() + 1> rk_;
}; // class AESKeySeqImpl

template <std::size_t Rounds, typename KeySeqGenerator>
class ARSKeySeqImpl
{
  public:
    using key_type = typename KeySeqGenerator::key_type;
    using rk_type = typename KeySeqGenerator::rk_type;

    static constexpr std::size_t rounds() { return Rounds; }

    const key_type &key() const { return key_; }

    void set(const key_type &key) { key_ = key; }

    std::array<rk_type, rounds() + 1> get() const
    {
        KeySeqGenerator generator;
        std::array<rk_type, rounds() + 1> rk;
        generator(key_, rk);

        return rk;
    }

    friend bool operator==(const ARSKeySeqImpl<Rounds, KeySeqGenerator> &seq1,
        const ARSKeySeqImpl<Rounds, KeySeqGenerator> &seq2)
    {
        return seq1.key_ == seq2.key_;
    }

    friend bool operator!=(const ARSKeySeqImpl<Rounds, KeySeqGenerator> &seq1,
        const ARSKeySeqImpl<Rounds, KeySeqGenerator> &seq2)
    {
        return !(seq1 == seq2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const ARSKeySeqImpl<Rounds, KeySeqGenerator> &seq)
    {
        if (!os)
            return os;

        ostream(os, seq.key_);

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        ARSKeySeqImpl<Rounds, KeySeqGenerator> &seq)
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

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_AES_KEY_SEQ_HPP
