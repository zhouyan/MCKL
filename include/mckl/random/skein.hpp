//============================================================================
// MCKL/include/mckl/random/skein.hpp
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

#ifndef MCKL_RANDOM_SKEIN_HPP
#define MCKL_RANDOM_SKEIN_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/threefry.hpp>

namespace mckl
{

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
        static constexpr value_type key() { return 0; }

        /// \brief Configuration block
        static constexpr value_type cfg() { return 4; }

        /// \brief Personalization string
        static constexpr value_type prs() { return 8; }

        /// \brief Public key (for digital signature hashing)
        static constexpr value_type pub() { return 12; }

        /// \brief Key identifier (KDF)
        static constexpr value_type kdf() { return 16; }

        /// \brief Nonce (for stream cipher or randomized hashing)
        static constexpr value_type non() { return 20; }

        /// \brief Message
        static constexpr value_type msg() { return 48; }

        /// \brief Output
        static constexpr value_type out() { return 63; }
    }; // class type

    /// \brief Type of input paramters such as keys and messages
    class param_type
    {
        public:
        /// \brief Construct a parameter given a buffer
        ///
        /// \param N The length of the buffer in bits
        /// \param data The address of the buffer
        /// \param type The type of the parameter
        param_type(std::size_t N = 0, const void *data = nullptr,
            value_type type = type_field::msg())
            : N_(N), data_(data), type_(type)
        {
        }

        /// \brief The length of the buffer in bits
        std::size_t bits() const { return N_; }

        /// \brief The length of the buffer in bytes
        std::size_t bytes() const
        {
            return N_ / CHAR_BIT + (N_ % CHAR_BIT == 0 ? 0 : 1);
        }

        /// \brief The address of the buffer
        const void *data() const { return data_; }

        /// \brief The type of the buffer
        value_type type() const { return type_; }

        private:
        std::size_t N_;
        const void *data_;
        value_type type_;
    }; // class param_type

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
        if (!check_type(n, M))
            return;

        key_type G = {{0}};
        std::array<std::uint64_t, 4> C = configure(N, Yl, Yf, Ym);
        value_type t0 = 0;
        value_type t1 = 0;

        if (K.bits() != 0) {
            set_type(t1, type_field::key());
            G = ubi(G, K, t0, t1);
        }

        set_type(t1, type_field::cfg());
        G = ubi(G, param_type(256, C.data()), t0, t1);

        for (std::size_t i = 0; i != n; ++i) {
            set_type(t1, M[i].type());
            G = ubi(G, M[i], t0, t1);
        }

        output(G, N, H);
    }

    /// \brief UBI
    ///
    /// \param G The input key
    /// \param M The message
    /// \param t0 The lower half of the initial tweak value
    /// \param t1 The upper half of the initial tweak value
    /// \param Yl Tree leaf size
    /// \param Yf Tree fan-out
    /// \param Ym Tree maximum height
    static key_type ubi(const key_type &G, const param_type &M, value_type t0,
        value_type t1, int Yl = 0, int Yf = 0, int Ym = 0)
    {
        return (Yl | Yf | Ym) == 0 && M.type() == type_field::msg() ?
            ubi_block(G, M, t0, t1) :
            ubi_tree(G, M, t0, t1, Yl, Yf, Ym);
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

        // FIXME Big-endian

        ctr_type ctr = {{0}};
        ctr_type buf = {{0}};
        const std::size_t n = N / bytes();
        const std::size_t m = N % bytes();
        if (n < std::numeric_limits<typename ctr_type::value_type>::max()) {
            for (std::size_t i = 0; i != n; ++i) {
                generator.enc(ctr, buf);
                buf.front() ^= ctr.front();
                std::memcpy(C, buf.data(), bytes());
                ++ctr.front();
                C += bytes();
            }
            if (m != 0) {
                generator.enc(ctr, buf);
                buf.front() ^= ctr.front();
                std::memcpy(C, buf.data(), m);
            }
        } else {
            for (std::size_t i = 0; i != n; ++i) {
                generator.enc(ctr, buf);
                buf.front() ^= ctr.front();
                for (std::size_t j = 0; j != ctr.size(); ++j)
                    buf[j] ^= ctr[j];
                std::memcpy(C, buf.data(), bytes());
                increment(ctr);
                C += bytes();
            }
            if (m != 0) {
                generator.enc(ctr, buf);
                for (std::size_t j = 0; j != ctr.size(); ++j)
                    buf[j] ^= ctr[j];
                std::memcpy(C, buf.data(), m);
            }
        }
    }

    private:
    using ctr_type = typename Generator::ctr_type;

    static_assert(bits() >= 64,
        "**Skein** used with a Generator with less than 64 bits");

    static_assert(std::numeric_limits<value_type>::digits >= 32,
        "**Skein** used with a Generator with key_type::value_type less than "
        "32 bits");

    static_assert(sizeof(key_type) == sizeof(ctr_type),
        "**Skein** used with a Generator with counter and key different in "
        "size");

    static key_type ubi_block(
        const key_type &G, const param_type &M, value_type t0, value_type t1)
    {
        std::size_t N = M.bits();
        const char *C = static_cast<const char *>(M.data());
        const std::size_t k = internal::BufferSize<ctr_type>::value;
        alignas(32) std::array<ctr_type, k> buffer;

        const bool B = N % CHAR_BIT != 0;

        union {
            key_type G;
            ctr_type H;
        } buf;
        buf.G = G;

        // Process the only block
        if (N <= bits()) {
            get_block(t0, C, buffer[0], N);
            set_flags(t1, true, true, B);
            enc_block(buf.G, buffer[0], t0, t1, buf.H);

            return buf.G;
        }

        // Process the first block
        set_flags(t1, true, false, B);
        get_block(t0, C, buffer[0]);
        enc_block(buf.G, buffer[0], t0, t1, buf.H);
        N -= bits();
        C += bytes();

        // Process the second and the last block
        if (N <= bits()) {
            get_block(t0, C, buffer[0], N);
            set_flags(t1, false, true, B);
            enc_block(buf.G, buffer[0], t0, t1, buf.H);

            return buf.G;
        }

        // Process the intermediate blocks
        set_flags(t1, false, false, B);
        const std::size_t n = N / bits() - (N % bits() == 0 ? 1 : 0);
        const std::size_t m = n / k;
        const std::size_t l = n % k;
        for (std::size_t i = 0; i != m; ++i) {
            std::memcpy(buffer.data(), C, bytes() * k);
            for (std::size_t j = 0; j != k; ++j) {
                t0 += bytes();
                enc_block(buf.G, buffer[j], t0, t1, buf.H);
            }
            N -= bits() * k;
            C += bytes() * k;
        }
        std::memcpy(buffer.data(), C, bytes() * l);
        for (std::size_t j = 0; j != l; ++j) {
            t0 += bytes();
            enc_block(buf.G, buffer[j], t0, t1, buf.H);
        }
        N -= bits() * l;
        C += bytes() * l;

        // Process the last block
        set_flags(t1, false, true, B);
        get_block(t0, C, buffer[0], N);
        enc_block(buf.G, buffer[0], t0, t1, buf.H);

        return buf.G;
    }

    static key_type ubi_tree(const key_type &G, const param_type &M,
        value_type t0, value_type t1, int, int, int)
    {
        return ubi_block(G, M, t0, t1);
    }

    static void enc_block(const key_type &G, const ctr_type &M, value_type t0,
        value_type t1, ctr_type &H)
    {
        Generator generator;
        generator.reset(G);
        generator.tweak(t0, t1);
        generator.enc(M, H);
        for (std::size_t i = 0; i != M.size(); ++i)
            H[i] ^= M[i];
    }

    static void set_type(value_type &t1, value_type type)
    {
        static constexpr int N = std::numeric_limits<value_type>::digits;

        const value_type mask = 0x3F;
        t1 &= ~(mask << (N - 8));
        t1 ^= (type & mask) << (N - 8);
    }

    static void set_flags(value_type &t1, bool first, bool last, bool bpad)
    {
        static constexpr int N = std::numeric_limits<value_type>::digits;

        static constexpr value_type mark_first = const_one<value_type>()
            << (N - 2);
        static constexpr value_type mark_last = const_one<value_type>()
            << (N - 1);
        static constexpr value_type mark_bpad = const_one<value_type>()
            << (N - 9);

        if (first)
            t1 |= mark_first;
        else
            t1 &= ~mark_first;

        if (last) {
            t1 |= mark_last;
            if (bpad)
                t1 |= mark_bpad;
            else
                t1 &= ~mark_bpad;
        } else {
            t1 &= ~mark_last;
            t1 &= ~mark_bpad;
        }
    }

    static void get_block(value_type &t0, const char *C, ctr_type &M)
    {
        std::memcpy(M.data(), C, bytes());
        t0 += bytes();
    }

    static void get_block(
        value_type &t0, const char *C, ctr_type &M, std::size_t N)
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
        std::copy_n(C, n, tmp.begin());

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
        std::size_t N, int Yl = 0, int Yf = 0, int Ym = 0)
    {
        return configure(N, Yl, Yf, Ym, internal::is_little_endian());
    }

    static std::array<std::uint64_t, 4> configure(
        std::size_t N, int Yl, int Yf, int Ym, std::true_type)
    {
        std::array<std::uint64_t, 4> C = {{0}};
        std::get<0>(C) = 0x133414853;
        std::get<1>(C) = static_cast<std::uint64_t>(N);
        std::get<2>(C) += static_cast<std::uint64_t>(Yl & 0xFF);
        std::get<2>(C) += static_cast<std::uint64_t>(Yf & 0xFF) << 8;
        std::get<2>(C) += static_cast<std::uint64_t>(Ym & 0xFF) << 16;

        return C;
    }

    static std::array<std::uint64_t, 4> configure(
        std::size_t N, unsigned Yl, unsigned Yf, unsigned Ym, std::false_type)
    {
        union {
            std::array<std::uint8_t, 32> c;
            std::array<std::uint64_t, 4> u;
        } buf;
        std::fill(buf.u.begin(), buf.u.end(), 0);

        std::get<0x00>(buf.c) = 0x53;
        std::get<0x01>(buf.c) = 0x48;
        std::get<0x02>(buf.c) = 0x41;
        std::get<0x03>(buf.c) = 0x33;
        std::get<0x04>(buf.c) = 0x01;
        std::get<0x08>(buf.c) = static_cast<std::uint8_t>(N & 0xFF);
        std::get<0x09>(buf.c) = static_cast<std::uint8_t>((N >> 8) & 0xFF);
        std::get<0x0A>(buf.c) = static_cast<std::uint8_t>((N >> 16) & 0xFF);
        std::get<0x0B>(buf.c) = static_cast<std::uint8_t>((N >> 24) & 0xFF);
        std::get<0x0C>(buf.c) = static_cast<std::uint8_t>((N >> 32) & 0xFF);
        std::get<0x0D>(buf.c) = static_cast<std::uint8_t>((N >> 40) & 0xFF);
        std::get<0x0E>(buf.c) = static_cast<std::uint8_t>((N >> 48) & 0xFF);
        std::get<0x0F>(buf.c) = static_cast<std::uint8_t>((N >> 56) & 0xFF);
        std::get<0x10>(buf.c) = static_cast<std::uint8_t>(Yl);
        std::get<0x11>(buf.c) = static_cast<std::uint8_t>(Yf);
        std::get<0x12>(buf.c) = static_cast<std::uint8_t>(Ym);

        return buf.u;
    }

    static bool check_type(std::size_t n, const param_type *M)
    {
        if (n == 0)
            return false;

        if (M[0].type() <= type_field::cfg()) {
            runtime_assert(
                false, "**Skein::hash** M[0].type() <= type_field::cfg()");
            return false;
        }

        for (std::size_t i = 1; i < n; ++i) {
            if (M[i].type() <= M[i - 1].type()) {
                runtime_assert(
                    false, "**Skein::hash** M[i].type() <= M[i - 1].type()");
                return false;
            }
        }

        if (M[n - 1].type() >= type_field::out()) {
            runtime_assert(
                false, "**Skein::hash** M[n - 1].type() >= type_field::out()");
            return false;
        }

        return true;
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

#endif // MCKL_RANDOM_SKEIN_HPP
