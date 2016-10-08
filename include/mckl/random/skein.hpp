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
    static_assert(CHAR_BIT == 8,
        "**Skein** only support platforms with 8 bits per byte");

    public:
    using generator_type = Generator;

    /// \brief Simple hashing
    void operator()(std::size_t Nm, const void *msg, std::size_t No, void *out)
    {
        key_type Kp = {{0}};
        std::array<std::uint32_t, 8> C = configure(No);
        key_type G0 = ubi(Kp, 256, C.data(), 0, t_cfg_);
        key_type G1 = ubi(G0, Nm, msg, 0, t_msg_);
        output(G1, No, out);
    }

    private:
    using key_type = typename generator_type::key_type;
    using ctr_type = typename generator_type::ctr_type;
    using T = typename key_type::value_type;

    static constexpr std::size_t Nb_ = sizeof(key_type);
    static constexpr int tbits_ = std::numeric_limits<T>::digits;

    static constexpr T mark_last_ = const_one<T>() << (tbits_ - 1);
    static constexpr T mark_first_ = const_one<T>() << (tbits_ - 2);
    static constexpr T mark_bpad_ = const_one<T>() << (tbits_ - 9);

    static constexpr T t_key_ = static_cast<T>(0x00) << (tbits_ - 8);
    static constexpr T t_cfg_ = static_cast<T>(0x04) << (tbits_ - 8);
    static constexpr T t_prs_ = static_cast<T>(0x08) << (tbits_ - 8);
    static constexpr T t_pub_ = static_cast<T>(0x0C) << (tbits_ - 8);
    static constexpr T t_kdf_ = static_cast<T>(0x10) << (tbits_ - 8);
    static constexpr T t_non_ = static_cast<T>(0x14) << (tbits_ - 8);
    static constexpr T t_msg_ = static_cast<T>(0x30) << (tbits_ - 8);
    static constexpr T t_out_ = static_cast<T>(0x3F) << (tbits_ - 8);

    generator_type generator_;

    key_type ubi(
        const key_type &G, std::size_t Nbit, const void *msg, T t0, T t1)
    {
        const char *cmsg = static_cast<const char *>(msg);

        union {
            key_type H;
            ctr_type result;
        } buf;
        buf.H = G;

        ctr_type M = {{0}};

        // Process only one block
        if (Nbit <= Nb_ * CHAR_BIT) {
            t0 += block(cmsg, M, Nbit);
            t1 |= mark_first_;
            t1 |= mark_last_;
            t1 |= Nbit % CHAR_BIT == 0 ? 0 : mark_bpad_;
            buf.result = chain(buf.H, M, t0, t1);

            return buf.H;
        }

        // Process first block
        t0 += block(cmsg, M);
        t1 |= mark_first_;
        t1 &= ~mark_last_;
        t1 &= ~mark_bpad_;
        buf.result = chain(buf.H, M, t0, t1);
        Nbit -= Nb_ * CHAR_BIT;
        cmsg += Nb_;

        // Process all intermediate blocks
        t1 &= ~mark_first_;
        while (Nbit > Nb_ * CHAR_BIT) {
            t0 += block(cmsg, M);
            buf.result = chain(buf.H, M, t0, t1);
            Nbit -= Nb_ * CHAR_BIT;
            cmsg += Nb_;
        }

        // Process last block
        t0 += block(cmsg, M, Nbit);
        t1 |= mark_last_;
        t1 |= Nbit % CHAR_BIT == 0 ? 0 : mark_bpad_;
        buf.result = chain(buf.H, M, t0, t1);

        return buf.H;
    }

    void output(const key_type &G, std::size_t No, void *out)
    {
        const std::size_t Nbyte = No / CHAR_BIT + (No % CHAR_BIT == 0 ? 0 : 1);
        const std::size_t n = Nbyte / sizeof(ctr_type);

        char *cout = static_cast<char *>(out);

        const std::size_t k = internal::BufferSize<ctr_type>::value;
        const std::size_t m = n / k;
        const std::size_t l = n % k;
        std::array<ctr_type, k> buffer;
        ctr_type ctr = {{0}};
        for (std::size_t i = 0; i != m; ++i) {
            for (std::size_t j = 0; j != k; ++j) {
                buffer[j] = ubi(G, 8 * CHAR_BIT, ctr.data(), 0, t_out_);
                increment(ctr);
            }
            std::memcpy(cout, buffer.data(), sizeof(ctr_type) * k);
            cout += sizeof(ctr_type) * k;
        }
        for (std::size_t j = 0; j != l; ++j) {
            buffer[j] = ubi(G, 8 * CHAR_BIT, ctr.data(), 0, t_out_);
            increment(ctr);
        }
        std::memcpy(cout, buffer.data(), sizeof(ctr_type) * l);
        cout += sizeof(ctr_type) * l;
        if (Nbyte % sizeof(ctr_type) != 0) {
            buffer[0] = ubi(G, 8 * CHAR_BIT, ctr.data(), 0, t_out_);
            std::memcpy(cout, buffer.data(), Nbyte % sizeof(ctr_type));
        }
    }

    ctr_type chain(const key_type &G, const ctr_type &M, T t0, T t1)
    {
        ctr_type result;
        generator_.reset(G);
        generator_.tweak(t0, t1);
        generator_.enc(M, result);
        for (std::size_t i = 0; i != M.size(); ++i)
            result[i] ^= M[i];

        return result;
    }

    std::size_t block(const char *cmsg, ctr_type &M) const
    {
        std::memcpy(M.data(), cmsg, sizeof(ctr_type));

        return sizeof(ctr_type);
    }

    std::size_t block(const char *cmsg, ctr_type &M, std::size_t Nbit) const
    {
        if (Nbit == 0) {
            std::fill(M.begin(), M.end(), 0);

            return 0;
        }

        std::array<std::uint8_t, sizeof(ctr_type)> tmp = {{0}};
        std::size_t n = Nbit / CHAR_BIT + (Nbit % CHAR_BIT == 0 ? 0 : 1);
        std::copy_n(cmsg, std::min(n, sizeof(ctr_type)), tmp.begin());

        Nbit %= CHAR_BIT;
        if (Nbit != 0) {
            int R = CHAR_BIT - static_cast<int>(Nbit) - 1;
            tmp[n - 1] >>= R;
            tmp[n - 1] |= 1;
            tmp[n - 1] <<= R;
        }
        std::memcpy(M.data(), tmp.data(), sizeof(ctr_type));

        return n;
    }

    std::array<std::uint32_t, 8> configure(std::uint64_t No,
        std::uint32_t Yl = 0, std::uint32_t Yf = 0, std::uint32_t Ym = 0) const
    {
        return configure(No, Yl, Yf, Ym, internal::is_little_endian());
    }

    std::array<std::uint32_t, 8> configure(std::uint64_t No, std::uint32_t Yl,
        std::uint32_t Yf, std::uint32_t Ym, std::true_type) const
    {
        std::array<std::uint32_t, 8> C = {{0}};
        std::get<0>(C) = UINT32_C(0x33414853);
        std::get<1>(C) = 1;
        std::get<2>(C) = static_cast<std::uint32_t>(No);
        std::get<3>(C) = static_cast<std::uint32_t>(No >> 32);
        std::get<4>(C) += (Yl & UINT32_C(0xFF)) << (CHAR_BIT * 0);
        std::get<4>(C) += (Yf & UINT32_C(0xFF)) << (CHAR_BIT * 1);
        std::get<4>(C) += (Ym & UINT32_C(0xFF)) << (CHAR_BIT * 2);

        return C;
    }

    std::array<std::uint32_t, 8> configure(std::uint64_t No, std::uint32_t Yl,
        std::uint32_t Yf, std::uint32_t Ym, std::false_type) const
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
        std::get<0x08>(buf.c) =
            static_cast<std::uint8_t>((No >> (CHAR_BIT * 0)) & 0xFF);
        std::get<0x09>(buf.c) =
            static_cast<std::uint8_t>((No >> (CHAR_BIT * 1)) & 0xFF);
        std::get<0x0A>(buf.c) =
            static_cast<std::uint8_t>((No >> (CHAR_BIT * 2)) & 0xFF);
        std::get<0x0B>(buf.c) =
            static_cast<std::uint8_t>((No >> (CHAR_BIT * 3)) & 0xFF);
        std::get<0x0C>(buf.c) =
            static_cast<std::uint8_t>((No >> (CHAR_BIT * 4)) & 0xFF);
        std::get<0x0D>(buf.c) =
            static_cast<std::uint8_t>((No >> (CHAR_BIT * 5)) & 0xFF);
        std::get<0x0E>(buf.c) =
            static_cast<std::uint8_t>((No >> (CHAR_BIT * 6)) & 0xFF);
        std::get<0x0F>(buf.c) =
            static_cast<std::uint8_t>((No >> (CHAR_BIT * 7)) & 0xFF);
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
