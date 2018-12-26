//============================================================================
// MCKL/include/mckl/random/skein.hpp
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

#ifndef MCKL_RANDOM_SKEIN_HPP
#define MCKL_RANDOM_SKEIN_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/threefry.hpp>

MCKL_PUSH_CLANG_WARNING("-Wpadded")

namespace mckl {

/// \brief Skein hash function
/// \ingroup Random
template <typename Generator>
class Skein
{
  public:
    /// \brief Type of the key
    using key_type = typename Generator::key_type;

    /// \brief Type of the key and tweak words
    using value_type = typename key_type::value_type;

    /// \brief Values of the type field
    class type_field
    {
      public:
        /// \brief Key (for MAC or KDF)
        static constexpr int key() { return 0; }

        /// \brief Configuration block
        static constexpr int cfg() { return 4; }

        /// \brief Personalization string
        static constexpr int prs() { return 8; }

        /// \brief Public key (for digital signature hashing)
        static constexpr int pub() { return 12; }

        /// \brief Key identifier (KDF)
        static constexpr int kdf() { return 16; }

        /// \brief Nonce (for stream cipher or randomized hashing)
        static constexpr int non() { return 20; }

        /// \brief Message
        static constexpr int msg() { return 48; }

        /// \brief Output
        static constexpr int out() { return 63; }
    }; // class type

    /// \brief Type of input paramters such as keys and messages
    class param_type
    {
      public:
        /// \brief Construct a parameter given bit strings
        ///
        /// \param N The length of the bit string
        /// \param data The address of the bit string
        /// \param type The type of the parameter
        explicit param_type(std::size_t N = 0, const void *data = nullptr,
            int type = type_field::msg())
            : N_(N), data_(static_cast<const char *>(data)), type_(type & 0x3F)
        {
        }

        /// \brief The length of the string in bits
        std::size_t bits() const { return N_; }

        /// \brief The length of the string in bytes
        std::size_t bytes() const
        {
            return N_ / CHAR_BIT + (N_ % CHAR_BIT == 0 ? 0 : 1);
        }

        /// \brief The address of the string
        const char *data() const { return data_; }

        /// \brief The type of the string
        int type() const { return type_; }

      private:
        std::size_t N_;
        const char *data_;
        int type_;
    }; // class param_type

    /// \brief Configure type for stream hasher
    class stream_hasher
    {
      public:
        /// \brief State type for stream hasher
        class state_type
        {
          private:
            key_type G_;
            value_type t0_;
            value_type t1_;
            int type_;

            friend stream_hasher;
        }; // class state_type

        explicit stream_hasher(std::size_t N,
            const param_type &K = param_type(), int Yl = 0, int Yf = 0,
            int Ym = 0)
            : N_(N)
            , K_(K)
            , Yl_(Yl & 0xFF)
            , Yf_(Yf & 0xFF)
            , Ym_(Ym & 0xFF)
            , C_(configure(N_, Yl_, Yf_, Ym_))
            , simple_((Yl_ | Yf_ | Ym_) == 0)
        {
            internal::union_le<char>(C_);
        }

        state_type init() const
        {
            state_type ret;
            std::memset(&ret, 0, sizeof(ret));

            if (K_.bits() != 0) {
                set_type(ret.t1_, type_field::key());
                ret.G_ = ubi(ret.G_, K_, ret.t0_, ret.t1_);
            }

            set_type(ret.t1_, type_field::cfg());
            ret.G_ = ubi(ret.G_, param_type(256, C_.data()), ret.t0_, ret.t1_);

            return ret;
        }

        void update(state_type &state, const param_type &M) const
        {
            if (state.type_ == 0) {
                runtime_assert(M.type() > type_field::cfg(),
                    "**Skein::hash** M[0].type() <= type_field::cfg()");
            } else {
                runtime_assert(M.type() > state.type_,
                    "**Skein::hash** M[i].type() <= M[i - 1].type()");
            }

            if (simple_) {
                set_type(state.t1_, M.type());
                state.G_ = ubi(state.G_, M, state.t0_, state.t1_);
            } else {
                set_type(state.t1_, M.type());
                state.G_ = M.type() == type_field::msg() ?
                    ubi_tree(state.G_, M, Yl_, Yf_, Ym_, 0) :
                    ubi(state.G_, M, state.t0_, state.t1_);
            }

            state.type_ = M.type();
        }

        void output(const state_type &state, void *H) const
        {
            runtime_assert(state.type_ < type_field::out(),
                "**Skein::hash** M[n - 1].type() >= type_field::out()");

            Skein::output(state.G_, N_, H);
        }

      private:
        std::size_t N_;
        param_type K_;
        int Yl_;
        int Yf_;
        int Ym_;
        std::array<std::uint64_t, 4> C_;
        bool simple_;
    }; // class stream_hasher

    /// \brief The number of bytes of internal state
    static constexpr std::size_t bytes() { return sizeof(key_type); }

    /// \brief The number of bits of internal state
    static constexpr std::size_t bits() { return sizeof(key_type) * CHAR_BIT; }

    /// \brief Simple hashing
    /// \param M The messages
    /// \param N The lenght of the hash value in bits
    /// \param H The hash value
    static void hash(const param_type &M, std::size_t N, void *H)
    {
        hash(1, &M, N, H);
    }

    /// \brief Full Skein interface
    ///
    /// \param n The number of messages
    /// \param M The messages
    /// \param N The lenght of the hash value in bits
    /// \param H The hash value
    /// \param K The optional key
    /// \param Yl Tree leaf size
    /// \param Yf Tree fan-out
    /// \param Ym Tree maximum height
    ///
    /// \details
    /// `K.type()` is ignored. No operation will be done if the types of
    /// messages are not valid. And runtime assertions will be rasied if
    /// assertion is not disabled.
    ///
    /// Tree hashing is ignored for now
    static void hash(std::size_t n, const param_type *M, std::size_t N,
        void *H, const param_type &K = param_type(), int Yl = 0, int Yf = 0,
        int Ym = 0)
    {
        stream_hasher hasher(N, K, Yl, Yf, Ym);
        auto state = hasher.init();
        for (std::size_t i = 0; i != n; ++i) {
            hasher.update(state, M[i]);
        }
        hasher.output(state, H);
    }

    /// \brief UBI
    ///
    /// \param G The input key
    /// \param M The message
    /// \param t0 The lower half of the initial tweak value
    /// \param t1 The upper half of the initial tweak value
    static key_type ubi(
        const key_type &G, const param_type &M, value_type t0, value_type t1)
    {
        std::size_t N = M.bits();
        const char *C = M.data();
        const std::size_t k = internal::BufferSize<key_type>::value;
        alignas(MCKL_ALIGNMENT) std::array<key_type, k> message;

        const bool B = N % CHAR_BIT != 0;
        key_type H = G;

        // Process the only block
        if (N <= bits()) {
            get_block(t0, C, message[0], N);
            set_flags(t1, true, true, B);
            enc_block(H, message[0], t0, t1);

            return H;
        }

        // Process the first block
        set_flags(t1, true, false, B);
        get_block(t0, C, message[0]);
        enc_block(H, message[0], t0, t1);
        N -= bits();
        C += bytes();

        // Process the second and the last block
        if (N <= bits()) {
            get_block(t0, C, message[0], N);
            set_flags(t1, false, true, B);
            enc_block(H, message[0], t0, t1);

            return H;
        }

        // Process the intermediate blocks
        set_flags(t1, false, false, B);
        const std::size_t n = N / bits() - (N % bits() == 0 ? 1 : 0);
        const std::size_t m = n / k;
        const std::size_t l = n % k;
        for (std::size_t i = 0; i != m; ++i) {
            std::memcpy(message.data(), C, bytes() * k);
            for (std::size_t j = 0; j != k; ++j) {
                t0 += bytes();
                enc_block(H, message[j], t0, t1);
            }
            N -= bits() * k;
            C += bytes() * k;
        }
        std::memcpy(message.data(), C, bytes() * l);
        for (std::size_t j = 0; j != l; ++j) {
            t0 += bytes();
            enc_block(H, message[j], t0, t1);
        }
        N -= bits() * l;
        C += bytes() * l;

        // Process the last block
        set_flags(t1, false, true, B);
        get_block(t0, C, message[0], N);
        enc_block(H, message[0], t0, t1);

        return H;
    }

    /// \brief Output
    ///
    /// \param G The chaining value
    /// \param N The length of the output in bits
    /// \param H The output
    static void output(const key_type &G, std::size_t N, void *H)
    {
        N = N / CHAR_BIT + (N % CHAR_BIT == 0 ? 0 : 1);
        char *C = static_cast<char *>(H);

        value_type t0 = sizeof(std::uint64_t);
        value_type t1 = 0;
        set_type(t1, type_field::out());
        set_flags(t1, true, true, false);
        Generator generator;
        generator.reset(G);
        generator.tweak(t0, t1);

        key_type ctr = {{0}};
        key_type buf = {{0}};
        const std::size_t n = N / bytes();
        const std::size_t m = N % bytes();
        if (n < std::numeric_limits<typename key_type::value_type>::max()) {
            for (std::size_t i = 0; i != n; ++i) {
                key_type M = ctr;
                internal::union_le<char>(M);
                generator(M.data(), buf.data());
                buf.front() ^= M.front();
                std::memcpy(C, buf.data(), bytes());
                ++ctr.front();
                C += bytes();
            }
            if (m != 0) {
                key_type M = ctr;
                internal::union_le<char>(M);
                generator(M.data(), buf.data());
                buf.front() ^= M.front();
                std::memcpy(C, buf.data(), m);
            }
        } else {
            for (std::size_t i = 0; i != n; ++i) {
                key_type M = ctr;
                generator(M.data(), buf.data());
                buf.front() ^= M.front();
                for (std::size_t j = 0; j != M.size(); ++j) {
                    buf[j] ^= M[j];
                }
                std::memcpy(C, buf.data(), bytes());
                increment(ctr);
                C += bytes();
            }
            if (m != 0) {
                key_type M = ctr;
                internal::union_le<char>(M);
                generator(M.data(), buf.data());
                for (std::size_t j = 0; j != M.size(); ++j) {
                    buf[j] ^= M[j];
                }
                std::memcpy(C, buf.data(), m);
            }
        }
    }

  private:
    static_assert(bits() >= 64,
        "**Skein** used with a Generator with less than 64 bits");

    static_assert(std::is_unsigned<value_type>::value,
        "**Skein** used with a Generator with key_type::value_type not an "
        "unsigned integer type");

    static_assert(std::numeric_limits<value_type>::digits >= 32,
        "**Skein** used with a Generator with key_type::value_type less than "
        "32 bits");

    static_assert(sizeof(key_type) == Generator::size(),
        "**Skein** used with a Generator with block and key different in "
        "size");

    static key_type ubi_tree(
        const key_type &G, const param_type &M, int Yl, int Yf, int Ym, int l)
    {
        value_type t0 = 0;
        value_type t1 = 0;

        // Process the first level
        if (l == 0) {
            if (M.bits() == 0) {
                t0 = 0;
                set_level(t1, 1);
                set_type(t1, type_field::msg());
                return ubi(G, M, t0, t1);
            }
            std::size_t Nl = bytes() * (const_one<std::size_t>() << Yl);
            std::size_t k1 = M.bytes() / Nl + (M.bytes() % Nl == 0 ? 0 : 1);
            Vector<key_type> M1(k1);
            set_level(t1, 1);
            set_type(t1, type_field::msg());
            for (std::size_t i = 0; i != k1; ++i) {
                t0 = static_cast<value_type>(i * Nl);
                std::size_t N =
                    i + 1 == k1 ? M.bits() - i * Nl * CHAR_BIT : Nl * CHAR_BIT;
                M1[i] = ubi(G, param_type(N, M.data() + i * Nl), t0, t1);
                internal::union_le<char>(M1[i]);
            }
            return ubi_tree(
                G, param_type(bits() * k1, M1.data()), Yl, Yf, Ym, 1);
        }

        // Process the only block
        if (M.bits() == bits()) {
            key_type H = {{0}};
            std::memcpy(H.data(), M.data(), bytes());
            internal::union_le<char>(H);
            return H;
        }

        // Process the maximum level
        if (l + 1 == Ym) {
            t0 = 0;
            set_level(t1, Ym);
            set_type(t1, type_field::msg());
            return ubi(G, M, t0, t1);
        }

        std::size_t Nn = bytes() * (const_one<std::size_t>() << Yf);
        std::size_t kl = M.bytes() / Nn + (M.bytes() % Nn == 0 ? 0 : 1);
        Vector<key_type> Ml(kl);
        set_level(t1, l + 1);
        set_type(t1, type_field::msg());
        for (std::size_t i = 0; i != kl; ++i) {
            t0 = static_cast<value_type>(i * Nn);
            std::size_t N =
                i + 1 == kl ? M.bits() - i * Nn * CHAR_BIT : Nn * CHAR_BIT;
            Ml[i] = ubi(G, param_type(N, M.data() + i * Nn), t0, t1);
            internal::union_le<char>(Ml[i]);
        }

        return ubi_tree(
            G, param_type(bits() * kl, Ml.data()), Yl, Yf, Ym, l + 1);
    }

    static void enc_block(
        key_type &H, const key_type &M, value_type t0, value_type t1)
    {
        Generator generator;
        generator.reset(H);
        generator.tweak(t0, t1);
        generator(M.data(), H.data());
        for (std::size_t i = 0; i != M.size(); ++i) {
            H[i] ^= M[i];
        }
        internal::union_le<char>(H);
    }

    static void set_flags(value_type &t1, bool first, bool last, bool bpad)
    {
        constexpr int N = std::numeric_limits<value_type>::digits;
        constexpr value_type mask_first = const_one<value_type>() << (N - 2);
        constexpr value_type mask_last = const_one<value_type>() << (N - 1);
        constexpr value_type mask_bpad = const_one<value_type>() << (N - 9);

        if (first) {
            t1 |= mask_first;
        } else {
            t1 &= ~mask_first;
        }

        if (last) {
            t1 |= mask_last;
            if (bpad) {
                t1 |= mask_bpad;
            } else {
                t1 &= ~mask_bpad;
            }
        } else {
            t1 &= ~mask_last;
            t1 &= ~mask_bpad;
        }
    }

    static void set_type(value_type &t1, int type)
    {
        constexpr int N = std::numeric_limits<value_type>::digits;
        constexpr value_type mask = static_cast<value_type>(0x3F) << (N - 8);

        t1 &= ~mask;
        t1 ^= (static_cast<value_type>(type) << (N - 8)) & mask;
    }

    static void set_level(value_type &t1, int level)
    {
        constexpr int N = std::numeric_limits<value_type>::digits;
        constexpr value_type mask = static_cast<value_type>(0x7F) << (N - 16);

        t1 &= ~mask;
        t1 ^= (static_cast<value_type>(level) << (N - 16)) & mask;
    }

    static void get_block(value_type &t0, const char *C, key_type &M)
    {
        std::memcpy(M.data(), C, bytes());
        t0 += bytes();
    }

    static void get_block(
        value_type &t0, const char *C, key_type &M, std::size_t N)
    {
        if (N == 0) {
            std::fill(M.begin(), M.end(), 0);
            return;
        }

        if (N >= bits()) {
            get_block(t0, C, M);
            return;
        }

        std::array<char, bytes()> tmp = {{0}};
        std::size_t n = N / CHAR_BIT + (N % CHAR_BIT == 0 ? 0 : 1);
        std::memcpy(tmp.data(), C, n);

        N %= CHAR_BIT;
        if (N != 0) {
            int R = 7 - static_cast<int>(N);
            tmp[n - 1] >>= R;
            tmp[n - 1] |= 1;
            tmp[n - 1] <<= R;
        }
        std::memcpy(M.data(), tmp.data(), bytes());
        t0 += n;
    }

    static std::array<std::uint64_t, 4> configure(
        std::size_t N, int Yl, int Yf, int Ym)
    {
        std::array<std::uint64_t, 4> C = {{0}};
        std::get<0>(C) = 0x133414853;
        std::get<1>(C) = static_cast<std::uint64_t>(N);
        std::get<2>(C) += static_cast<std::uint64_t>(Yl);
        std::get<2>(C) += static_cast<std::uint64_t>(Yf << 8);
        std::get<2>(C) += static_cast<std::uint64_t>(Ym << 16);

        return C;
    }
}; // class Skein

/// \brief Skein-256 hash function
/// \ingroup Random
using Skein256 = Skein<typename Threefish256::generator_type>;

/// \brief Skein-512 hash function
/// \ingroup Random
using Skein512 = Skein<typename Threefish512::generator_type>;

/// \brief Skein-1024 hash function
/// \ingroup Random
using Skein1024 = Skein<typename Threefish1024::generator_type>;

} // namespace mckl

MCKL_POP_CLANG_WARNING

#endif // MCKL_RANDOM_SKEIN_HPP
