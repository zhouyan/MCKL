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
    using key_type = typename Generator::key_type;
    using value_type = typename key_type::value_type;

    /// \brief Number of bytes of internal state
    static constexpr std::size_t bytes() { return sizeof(key_type); }

    /// \brief Number of bits of internal state
    static constexpr std::size_t bits()
    {
        return std::numeric_limits<value_type>::digits *
            std::tuple_size<key_type>::value;
    }

    /// \brief Simple hashing
    static void hash(
        std::size_t Nm, const void *msg, std::size_t No, void *out)
    {
        key_type Kp = {{0}};
        std::array<std::uint32_t, 8> C = configure(No);
        key_type G0 = ubi(Kp, 256, C.data(), 0, t_cfg_);
        key_type G1 = ubi(G0, Nm, msg, 0, t_msg_);
        output(G1, No, out);
    }

    /// \brief UBI chaining
    static key_type ubi(const key_type &G, std::size_t Nbit, const void *msg,
        value_type t0, value_type t1)
    {
        const std::uint8_t *msg8 = static_cast<const std::uint8_t *>(msg);

        union {
            key_type H;
            ctr_type result;
        } buf;
        buf.H = G;

        ctr_type M = {{0}};

        // Process only one block
        if (Nbit <= bits()) {
            t0 += block(msg8, M, Nbit);
            t1 |= mark_first_;
            t1 |= mark_last_;
            t1 |= Nbit % 8 == 0 ? 0 : mark_bpad_;
            buf.result = chain(buf.H, M, t0, t1);

            return buf.H;
        }

        // Process first block
        t0 += block(msg8, M);
        t1 |= mark_first_;
        t1 &= ~mark_last_;
        t1 &= ~mark_bpad_;
        buf.result = chain(buf.H, M, t0, t1);
        Nbit -= bits();
        msg8 += bytes();

        // Process all intermediate blocks
        t1 &= ~mark_first_;
        while (Nbit > bits()) {
            t0 += block(msg8, M);
            buf.result = chain(buf.H, M, t0, t1);
            Nbit -= bits();
            msg8 += bytes();
        }

        // Process last block
        t0 += block(msg8, M, Nbit);
        t1 |= mark_last_;
        t1 |= Nbit % 8 == 0 ? 0 : mark_bpad_;
        buf.result = chain(buf.H, M, t0, t1);

        return buf.H;
    }

    private:
    static_assert(bytes() * 8 == bits(),
        "**Skein** used on a platform where one byte is not eight bits");

    using ctr_type = typename Generator::ctr_type;

    static constexpr int tbits_ = std::numeric_limits<value_type>::digits;

    static constexpr value_type mark_last_ = const_one<value_type>()
        << (tbits_ - 1);
    static constexpr value_type mark_first_ = const_one<value_type>()
        << (tbits_ - 2);
    static constexpr value_type mark_bpad_ = const_one<value_type>()
        << (tbits_ - 9);
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

    static void output(const key_type &G, std::size_t No, void *out)
    {
        const std::size_t Nbyte = No / 8 + (No % 8 == 0 ? 0 : 1);
        const std::size_t n = Nbyte / bytes();

        std::uint8_t *out8 = static_cast<std::uint8_t *>(out);

        const std::size_t k = internal::BufferSize<key_type>::value;
        const std::size_t m = n / k;
        const std::size_t l = n % k;
        std::array<key_type, k> buffer;
        std::uint64_t c = 0;
        for (std::size_t i = 0; i != m; ++i) {
            for (std::size_t j = 0; j != k; ++j, ++c)
                buffer[j] = ubi(G, 64, &c, 0, t_out_);
            std::memcpy(out8, buffer.data(), bytes() * k);
            out8 += bytes() * k;
        }
        for (std::size_t j = 0; j != l; ++j, ++c)
            buffer[j] = ubi(G, 64, &c, 0, t_out_);
        std::memcpy(out8, buffer.data(), bytes() * l);
        out8 += bytes() * l;
        if (Nbyte % bytes() != 0) {
            buffer[0] = ubi(G, 64, &c, 0, t_out_);
            std::memcpy(out8, buffer.data(), Nbyte % bytes());
        }
    }

    static ctr_type chain(
        const key_type &G, const ctr_type &M, value_type t0, value_type t1)
    {
        ctr_type result;
        Generator generator;
        generator.reset(G);
        generator.tweak(t0, t1);
        generator.enc(M, result);
        for (std::size_t i = 0; i != M.size(); ++i)
            result[i] ^= M[i];

        return result;
    }

    static std::size_t block(const std::uint8_t *msg8, ctr_type &M)
    {
        std::memcpy(M.data(), msg8, bytes());

        return bytes();
    }

    static std::size_t block(
        const std::uint8_t *msg8, ctr_type &M, std::size_t Nbit)
    {
        if (Nbit == 0) {
            std::fill(M.begin(), M.end(), 0);
            return 0;
        }

        if (Nbit >= bits())
            return block(msg8, M);

        std::array<std::uint8_t, bytes()> tmp = {{0}};
        std::size_t n = Nbit / 8 + (Nbit % 8 == 0 ? 0 : 1);
        std::copy_n(msg8, n < bytes() ? n : bytes(), tmp.begin());

        Nbit %= 8;
        if (Nbit != 0) {
            int R = 7 - static_cast<int>(Nbit);
            tmp[n - 1] >>= R;
            tmp[n - 1] |= 1;
            tmp[n - 1] <<= R;
        }
        std::memcpy(M.data(), tmp.data(), bytes());

        return n;
    }

    static std::array<std::uint32_t, 8> configure(std::uint64_t No,
        std::uint32_t Yl = 0, std::uint32_t Yf = 0, std::uint32_t Ym = 0)
    {
        return configure(No, Yl, Yf, Ym, internal::is_little_endian());
    }

    static std::array<std::uint32_t, 8> configure(std::uint64_t No,
        std::uint32_t Yl, std::uint32_t Yf, std::uint32_t Ym, std::true_type)
    {
        std::array<std::uint32_t, 8> C = {{0}};
        std::get<0>(C) = UINT32_C(0x33414853);
        std::get<1>(C) = 1;
        std::get<2>(C) = static_cast<std::uint32_t>(No);
        std::get<3>(C) = static_cast<std::uint32_t>(No >> 32);
        std::get<4>(C) += (Yl & UINT32_C(0xFF));
        std::get<4>(C) += (Yf & UINT32_C(0xFF)) << 8;
        std::get<4>(C) += (Ym & UINT32_C(0xFF)) << 16;

        return C;
    }

    static std::array<std::uint32_t, 8> configure(std::uint64_t No,
        std::uint32_t Yl, std::uint32_t Yf, std::uint32_t Ym, std::false_type)
    {
        union {
            std::array<std::uint8_t, 32> c;
            std::array<std::uint32_t, 8> u;
        } buf;
        std::fill(buf.u.begin(), buf.u.end(), 0);
        std::get<0x00>(buf.c) = UINT8_C(0x53);
        std::get<0x01>(buf.c) = UINT8_C(0x48);
        std::get<0x02>(buf.c) = UINT8_C(0x41);
        std::get<0x03>(buf.c) = UINT8_C(0x33);
        std::get<0x04>(buf.c) = UINT8_C(0x01);
        std::get<0x08>(buf.c) = static_cast<std::uint8_t>(No & 0xFF);
        std::get<0x09>(buf.c) = static_cast<std::uint8_t>((No >> 8) & 0xFF);
        std::get<0x0A>(buf.c) = static_cast<std::uint8_t>((No >> 16) & 0xFF);
        std::get<0x0B>(buf.c) = static_cast<std::uint8_t>((No >> 24) & 0xFF);
        std::get<0x0C>(buf.c) = static_cast<std::uint8_t>((No >> 32) & 0xFF);
        std::get<0x0D>(buf.c) = static_cast<std::uint8_t>((No >> 40) & 0xFF);
        std::get<0x0E>(buf.c) = static_cast<std::uint8_t>((No >> 48) & 0xFF);
        std::get<0x0F>(buf.c) = static_cast<std::uint8_t>((No >> 56) & 0xFF);
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
