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

    /// \brief Type of input paramters such as keys and messages
    class param_type
    {
        public:
        /// \brief Construct a parameter given a buffer
        ///
        /// \param N The length of the buffer in bits
        /// \param data The address of the buffer
        /// \param type The type of the parameter
        param_type(std::size_t N, const void *data, value_type type = 0)
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
    ///
    /// \param M The message
    /// \param N The lenght of the hash value in bits
    /// \param H The hash value
    ///
    /// \details
    /// `M.type()` is ignored
    static void hash(const param_type &M, std::size_t N, void *H)
    {
        key_type Kp = {{0}};
        std::array<std::uint32_t, 8> config = configure(N);
        param_type C(256, config.data(), t_cfg_);
        key_type G0 = ubi(Kp, C, 0, t_cfg_);
        key_type G1 = ubi(G0, M, 0, t_msg_);
        output(G1, N, H);
    }

    /// \brief UBI
    ///
    /// \param G The input key
    /// \param M The message
    /// \param t0 The lower half of the initial tweak value
    /// \param t1 The upper half of the initial tweak value
    ///
    /// \details
    /// `M.type()` is ignored
    static key_type ubi(
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

    /// \brief Output
    ///
    /// \param G The chaining value
    /// \param N The length of the output in bits
    /// \param H The output
    static void output(const key_type &G, std::size_t N, void *H)
    {
        char *C = static_cast<char *>(H);
        const std::size_t k = internal::BufferSize<key_type>::value;
        alignas(32) std::array<key_type, k> buffer;

        N = N / CHAR_BIT + (N % CHAR_BIT == 0 ? 0 : 1);
        const std::size_t n = N / bytes(); // Number of full blocks
        const std::size_t r = N % bytes(); // Number of remaining bytes
        const std::size_t m = n / k;
        const std::size_t l = n % k;
        // FIXME Big-endian need to swap bytes
        std::uint64_t ctr = 0;
        param_type M(64, &ctr, t_out_);
        for (std::size_t i = 0; i != m; ++i) {
            for (std::size_t j = 0; j != k; ++j, ++ctr)
                buffer[j] = ubi(G, M, 0, t_out_);
            std::memcpy(C, buffer.data(), bytes() * k);
            C += bytes() * k;
        }
        for (std::size_t j = 0; j != l; ++j, ++ctr)
            buffer[j] = ubi(G, M, 0, t_out_);
        std::memcpy(C, buffer.data(), bytes() * l);
        C += bytes() * l;
        if (r != 0) {
            buffer[0] = ubi(G, M, 0, t_out_);
            std::memcpy(C, buffer.data(), r);
        }
    }

    private:
    using ctr_type = typename Generator::ctr_type;

    static_assert(bits() % 8 == 0,
        "**Skein** used on a platform where one byte is a multiple of eight "
        "bits");

    static_assert(sizeof(key_type) == sizeof(ctr_type),
        "**Skein** used with a Generator with counter and key different in "
        "size");

    static constexpr int tbits_ = std::numeric_limits<value_type>::digits;

    static constexpr value_type t_key_ = static_cast<value_type>(0x00)
        << (tbits_ - 8);
    static constexpr value_type t_cfg_ = static_cast<value_type>(0x04)
        << (tbits_ - 8);
    static constexpr value_type t_prs_ = static_cast<value_type>(0x08)
        << (tbits_ - 8);
    static constexpr value_type t_pub_ = static_cast<value_type>(0x0C)
        << (tbits_ - 8);
    static constexpr value_type t_kdf_ = static_cast<value_type>(0x10)
        << (tbits_ - 8);
    static constexpr value_type t_non_ = static_cast<value_type>(0x14)
        << (tbits_ - 8);
    static constexpr value_type t_msg_ = static_cast<value_type>(0x30)
        << (tbits_ - 8);
    static constexpr value_type t_out_ = static_cast<value_type>(0x3F)
        << (tbits_ - 8);

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

    static void set_flags(value_type &t1, bool first, bool last, bool bpad)
    {
        static constexpr value_type mark_first = const_one<value_type>()
            << (tbits_ - 2);
        static constexpr value_type mark_last = const_one<value_type>()
            << (tbits_ - 1);
        static constexpr value_type mark_bpad = const_one<value_type>()
            << (tbits_ - 9);

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

    static std::array<std::uint32_t, 8> configure(std::uint64_t N,
        std::uint32_t Yl = 0, std::uint32_t Yf = 0, std::uint32_t Ym = 0)
    {
        return configure(N, Yl, Yf, Ym, internal::is_little_endian());
    }

    static std::array<std::uint32_t, 8> configure(std::uint64_t N,
        std::uint32_t Yl, std::uint32_t Yf, std::uint32_t Ym, std::true_type)
    {
        std::array<std::uint32_t, 8> C = {{0}};
        std::get<0>(C) = 0x33414853;
        std::get<1>(C) = 1;
        std::get<2>(C) = static_cast<std::uint32_t>(N);
        std::get<3>(C) = static_cast<std::uint32_t>(N >> 32);
        std::get<4>(C) += (Yl & 0xFF);
        std::get<4>(C) += (Yf & 0xFF) << 8;
        std::get<4>(C) += (Ym & 0xFF) << 16;

        return C;
    }

    static std::array<std::uint32_t, 8> configure(std::uint64_t N,
        std::uint32_t Yl, std::uint32_t Yf, std::uint32_t Ym, std::false_type)
    {
        union {
            std::array<std::uint8_t, 32> c;
            std::array<std::uint32_t, 8> u;
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
