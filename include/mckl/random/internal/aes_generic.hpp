//============================================================================
// MCKL/include/mckl/random/internal/aes_generic.hpp
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

class AES128KeySeqGenerator
{
    public:
    using key_type = std::array<std::uint32_t, 4>;
    using rk_type = std::array<std::uint32_t, 4>;

    template <std::size_t Rp1>
    static key_type key(
        const std::array<std::array<std::uint32_t, 4>, Rp1> &rk)
    {
        key_type key;
        std::memcpy(key.data(), rk.data(), sizeof(key_type));

        return key;
    }

    template <std::size_t Rp1>
    void operator()(
        const key_type &key, std::array<std::array<std::uint32_t, 4>, Rp1> &rk)
    {
        tmp0_ = key;
        std::get<0>(rk) = tmp0_;
        generate<1>(rk, std::integral_constant<bool, 1 < Rp1>());
    }

    private:
    std::array<std::uint32_t, 4> tmp0_;

    template <std::size_t, std::size_t Rp1>
    void generate(
        std::array<std::array<std::uint32_t, 4>, Rp1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate(
        std::array<std::array<std::uint32_t, 4>, Rp1> &rk, std::true_type)
    {
        static constexpr std::uint32_t rcon = AESRCon::value[N % 256];

        const std::uint32_t *const table = aes_table() + 1024;
        const std::uint32_t t = std::get<3>(tmp0_);

        std::get<0>(tmp0_) ^= rcon;
        std::get<0>(tmp0_) ^= table[(t >> 0x08) & 0xFF] & 0x000000FF;
        std::get<0>(tmp0_) ^= table[(t >> 0x10) & 0xFF] & 0x0000FF00;
        std::get<0>(tmp0_) ^= table[(t >> 0x18) & 0xFF] & 0x00FF0000;
        std::get<0>(tmp0_) ^= table[(t >> 0x00) & 0xFF] & 0xFF000000;
        std::get<1>(tmp0_) ^= std::get<0>(tmp0_);
        std::get<2>(tmp0_) ^= std::get<1>(tmp0_);
        std::get<3>(tmp0_) ^= std::get<2>(tmp0_);

        std::get<N>(rk) = tmp0_;
        generate<N + 1>(rk, std::integral_constant<bool, N + 1 < Rp1>());
    }
}; // class AES128KeySeqGenerator

class AES192KeySeqGenerator
{
    public:
    using key_type = std::array<std::uint32_t, 6>;
    using rk_type = std::array<std::uint32_t, 4>;

    template <std::size_t Rp1>
    static key_type key(
        const std::array<std::array<std::uint32_t, 4>, Rp1> &rk)
    {
        key_type key;
        std::memcpy(key.data(), rk.data(), sizeof(key_type));

        return key;
    }

    template <std::size_t Rp1>
    void operator()(
        const key_type &key, std::array<std::array<std::uint32_t, 4>, Rp1> &rk)
    {
        static constexpr std::size_t Rs1 =
            Rp1 * 4 / 6 + ((Rp1 * 4) % 6 == 0 ? 0 : 1);

        std::array<std::array<std::uint32_t, 6>, Rs1> rs;
        tmp0_ = key;
        std::get<0>(rs) = tmp0_;
        generate<1>(rs, std::integral_constant<bool, 1 < Rs1>());
        std::memcpy(rk.data(), rs.data(), sizeof(std::uint32_t) * Rp1 * 4);
    }

    private:
    std::array<std::uint32_t, 6> tmp0_;

    template <std::size_t, std::size_t Rs1>
    void generate(
        std::array<std::array<std::uint32_t, 6>, Rs1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rs1>
    void generate(
        std::array<std::array<std::uint32_t, 6>, Rs1> &rk, std::true_type)
    {
        static constexpr std::uint32_t rcon = AESRCon::value[N % 256];

        const std::uint32_t *const table = aes_table() + 1024;
        const std::uint32_t t = std::get<5>(tmp0_);

        std::get<0>(tmp0_) ^= rcon;
        std::get<0>(tmp0_) ^= table[(t >> 0x08) & 0xFF] & 0x000000FF;
        std::get<0>(tmp0_) ^= table[(t >> 0x10) & 0xFF] & 0x0000FF00;
        std::get<0>(tmp0_) ^= table[(t >> 0x18) & 0xFF] & 0x00FF0000;
        std::get<0>(tmp0_) ^= table[(t >> 0x00) & 0xFF] & 0xFF000000;
        std::get<1>(tmp0_) ^= std::get<0>(tmp0_);
        std::get<2>(tmp0_) ^= std::get<1>(tmp0_);
        std::get<3>(tmp0_) ^= std::get<2>(tmp0_);
        std::get<4>(tmp0_) ^= std::get<3>(tmp0_);
        std::get<5>(tmp0_) ^= std::get<4>(tmp0_);

        std::get<N>(rk) = tmp0_;
        generate<N + 1>(rk, std::integral_constant<bool, N + 1 < Rs1>());
    }
}; // class AES192KeySeqGenerator

class AES256KeySeqGenerator
{
    public:
    using key_type = std::array<std::uint32_t, 8>;
    using rk_type = std::array<std::uint32_t, 4>;

    template <std::size_t Rp1>
    static key_type key(
        const std::array<std::array<std::uint32_t, 4>, Rp1> &rk)
    {
        key_type key;
        std::memcpy(key.data(), rk.data(), sizeof(key_type));

        return key;
    }

    template <std::size_t Rp1>
    void operator()(
        const key_type &key, std::array<std::array<std::uint32_t, 4>, Rp1> &rk)
    {
        std::get<0>(tmp0_) = std::get<0>(key);
        std::get<1>(tmp0_) = std::get<1>(key);
        std::get<2>(tmp0_) = std::get<2>(key);
        std::get<3>(tmp0_) = std::get<3>(key);
        std::get<0>(tmp1_) = std::get<4>(key);
        std::get<1>(tmp1_) = std::get<5>(key);
        std::get<2>(tmp1_) = std::get<6>(key);
        std::get<3>(tmp1_) = std::get<7>(key);
        std::get<0>(rk) = tmp0_;
        std::get<1>(rk) = tmp1_;
        generate<2>(rk, std::integral_constant<bool, 2 < Rp1>());
    }

    private:
    std::array<std::uint32_t, 4> tmp0_;
    std::array<std::uint32_t, 4> tmp1_;

    template <std::size_t, std::size_t Rp1>
    void generate(
        std::array<std::array<std::uint32_t, 4>, Rp1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate(
        std::array<std::array<std::uint32_t, 4>, Rp1> &rk, std::true_type)
    {
        generate_key<N>(rk, std::integral_constant<bool, N % 2 == 0>());
        generate<N + 1>(rk, std::integral_constant<bool, N + 1 < Rp1>());
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_key(
        std::array<std::array<std::uint32_t, 4>, Rp1> &rk, std::true_type)
    {
        static constexpr std::uint32_t rcon = AESRCon::value[(N / 2) % 256];

        const std::uint32_t *const table = aes_table() + 1024;
        const std::uint32_t t = std::get<3>(tmp1_);

        std::get<0>(tmp0_) ^= rcon;
        std::get<0>(tmp0_) ^= table[(t >> 0x08) & 0xFF] & 0x000000FF;
        std::get<0>(tmp0_) ^= table[(t >> 0x10) & 0xFF] & 0x0000FF00;
        std::get<0>(tmp0_) ^= table[(t >> 0x18) & 0xFF] & 0x00FF0000;
        std::get<0>(tmp0_) ^= table[(t >> 0x00) & 0xFF] & 0xFF000000;
        std::get<1>(tmp0_) ^= std::get<0>(tmp0_);
        std::get<2>(tmp0_) ^= std::get<1>(tmp0_);
        std::get<3>(tmp0_) ^= std::get<2>(tmp0_);

        std::get<N>(rk) = tmp0_;
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_key(
        std::array<std::array<std::uint32_t, 4>, Rp1> &rk, std::false_type)
    {
        const std::uint32_t *const table = aes_table() + 1024;
        const std::uint32_t t = std::get<3>(tmp0_);

        std::get<0>(tmp1_) ^= table[(t >> 0x00) & 0xFF] & 0x000000FF;
        std::get<0>(tmp1_) ^= table[(t >> 0x08) & 0xFF] & 0x0000FF00;
        std::get<0>(tmp1_) ^= table[(t >> 0x10) & 0xFF] & 0x00FF0000;
        std::get<0>(tmp1_) ^= table[(t >> 0x18) & 0xFF] & 0xFF000000;
        std::get<1>(tmp1_) ^= std::get<0>(tmp1_);
        std::get<2>(tmp1_) ^= std::get<1>(tmp1_);
        std::get<3>(tmp1_) ^= std::get<2>(tmp1_);

        std::get<N>(rk) = tmp1_;
    }
}; // class AES256KeySeqGenerator

template <typename Constants>
class ARSKeySeqGenerator
{
    public:
    using key_type = std::array<std::uint32_t, 4>;
    using rk_type = std::array<std::uint32_t, 4>;

    template <std::size_t Rp1>
    static key_type key(
        const std::array<std::array<std::uint32_t, 4>, Rp1> &rk)
    {
        key_type key;
        std::memcpy(key.data(), rk.data(), sizeof(key_type));

        return key;
    }

    template <std::size_t Rp1>
    void operator()(
        const key_type &key, std::array<std::array<std::uint32_t, 4>, Rp1> &rk)
    {
        std::get<0>(key_) = static_cast<std::uint64_t>(std::get<0>(key)) +
            (static_cast<std::uint64_t>(std::get<1>(key)) << 32);
        std::get<1>(key_) = static_cast<std::uint64_t>(std::get<2>(key)) +
            (static_cast<std::uint64_t>(std::get<3>(key)) << 32);
        generate<0>(rk, std::integral_constant<bool, 0 < Rp1>());
    }

    private:
    std::array<std::uint64_t, 2> key_;

    template <std::size_t, std::size_t Rp1>
    void generate(
        std::array<std::array<std::uint32_t, 4>, Rp1> &, std::false_type) const
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate(std::array<std::array<std::uint32_t, 4>, Rp1> &rk,
        std::true_type) const
    {
        static constexpr std::uint64_t w0 = Constants::weyl::value[0] * N;
        static constexpr std::uint64_t w1 = Constants::weyl::value[1] * N;

        std::uint64_t k0 = std::get<0>(key_) + w0;
        std::uint64_t k1 = std::get<1>(key_) + w1;
        std::get<0>(std::get<N>(rk)) = static_cast<std::uint32_t>(k0);
        std::get<1>(std::get<N>(rk)) = static_cast<std::uint32_t>(k0 >> 32);
        std::get<2>(std::get<N>(rk)) = static_cast<std::uint32_t>(k1);
        std::get<3>(std::get<N>(rk)) = static_cast<std::uint32_t>(k1 >> 32);
        generate<N + 1>(rk, std::integral_constant<bool, N + 1 < Rp1>());
    }
}; // class ARSKeySeqGenerator

template <typename KeySeqType>
class AESGeneratorImpl
{
    public:
    static constexpr bool batch() { return false; }

    static void eval(std::array<std::uint32_t, 4> &state,
        const std::array<std::array<std::uint32_t, 4>,
            KeySeqType::rounds() + 1> &rk)
    {
        encfirst<0x0>(state, rk);
        enc<0x1>(state, rk);
        enc<0x2>(state, rk);
        enc<0x3>(state, rk);
        enc<0x4>(state, rk);
        enc<0x5>(state, rk);
        enc<0x6>(state, rk);
        enc<0x7>(state, rk);
        enc<0x8>(state, rk);
        enc<0x9>(state, rk);
        enc<0xA>(state, rk);
        enc<0xB>(state, rk);
        enc<0xC>(state, rk);
        enc<0xD>(state, rk);
        enc<0xE>(state, rk);
        enc<0xF>(state, rk);
        round<0x10>(state, rk, std::integral_constant<bool, 0x10 < rounds_>());
        enclast<rounds_>(state, rk);
    }

    private:
    static constexpr std::size_t rounds_ = KeySeqType::rounds();

    template <std::size_t>
    static void round(std::array<std::uint32_t, 4> &,
        const std::array<std::array<std::uint32_t, 4>, rounds_ + 1> &,
        std::false_type)
    {
    }

    template <std::size_t N>
    static void round(std::array<std::uint32_t, 4> &s,
        const std::array<std::array<std::uint32_t, 4>, rounds_ + 1> &rk,
        std::true_type)
    {
        enc<N>(s, rk);
        round<N + 1>(s, rk, std::integral_constant<bool, N + 1 < rounds_>());
    }

    template <std::size_t N>
    static void encfirst(std::array<std::uint32_t, 4> &s,
        const std::array<std::array<std::uint32_t, 4>, rounds_ + 1> &rk)
    {
        encfirst<N>(s, rk, std::integral_constant<bool, N == 0>());
    }

    template <std::size_t>
    static void encfirst(std::array<std::uint32_t, 4> &,
        const std::array<std::array<std::uint32_t, 4>, rounds_ + 1> &,
        std::false_type)
    {
    }

    template <std::size_t N>
    static void encfirst(std::array<std::uint32_t, 4> &s,
        const std::array<std::array<std::uint32_t, 4>, rounds_ + 1> &rk,
        std::true_type)
    {
        std::get<0>(s) ^= std::get<0>(std::get<N>(rk));
        std::get<1>(s) ^= std::get<1>(std::get<N>(rk));
        std::get<2>(s) ^= std::get<2>(std::get<N>(rk));
        std::get<3>(s) ^= std::get<3>(std::get<N>(rk));
    }

    template <std::size_t N>
    static void enc(std::array<std::uint32_t, 4> &s,
        const std::array<std::array<std::uint32_t, 4>, rounds_ + 1> &rk)
    {
        enc<N>(s, rk, std::integral_constant<bool, (N > 0 && N < rounds_)>());
    }

    template <std::size_t>
    static void enc(std::array<std::uint32_t, 4> &,
        const std::array<std::array<std::uint32_t, 4>, rounds_ + 1> &,
        std::false_type)
    {
    }

    template <std::size_t N>
    static void enc(std::array<std::uint32_t, 4> &s,
        const std::array<std::array<std::uint32_t, 4>, rounds_ + 1> &rk,
        std::true_type)
    {
        const std::uint32_t *table = aes_table();
        std::array<std::uint32_t, 4> t(std::get<N>(rk));
        std::get<0>(t) ^= table[std::get<0>(s) & 0xFF];
        std::get<1>(t) ^= table[std::get<1>(s) & 0xFF];
        std::get<2>(t) ^= table[std::get<2>(s) & 0xFF];
        std::get<3>(t) ^= table[std::get<3>(s) & 0xFF];

        table += 256;
        std::get<0>(s) >>= 8;
        std::get<1>(s) >>= 8;
        std::get<2>(s) >>= 8;
        std::get<3>(s) >>= 8;
        std::get<0>(t) ^= table[std::get<1>(s) & 0xFF];
        std::get<1>(t) ^= table[std::get<2>(s) & 0xFF];
        std::get<2>(t) ^= table[std::get<3>(s) & 0xFF];
        std::get<3>(t) ^= table[std::get<0>(s) & 0xFF];

        table += 256;
        std::get<0>(s) >>= 8;
        std::get<1>(s) >>= 8;
        std::get<2>(s) >>= 8;
        std::get<3>(s) >>= 8;
        std::get<0>(t) ^= table[std::get<2>(s) & 0xFF];
        std::get<1>(t) ^= table[std::get<3>(s) & 0xFF];
        std::get<2>(t) ^= table[std::get<0>(s) & 0xFF];
        std::get<3>(t) ^= table[std::get<1>(s) & 0xFF];

        table += 256;
        std::get<0>(s) >>= 8;
        std::get<1>(s) >>= 8;
        std::get<2>(s) >>= 8;
        std::get<3>(s) >>= 8;
        std::get<0>(t) ^= table[std::get<3>(s) & 0xFF];
        std::get<1>(t) ^= table[std::get<0>(s) & 0xFF];
        std::get<2>(t) ^= table[std::get<1>(s) & 0xFF];
        std::get<3>(t) ^= table[std::get<2>(s) & 0xFF];

        s = t;
    }

    template <std::size_t N>
    static void enclast(std::array<std::uint32_t, 4> &s,
        const std::array<std::array<std::uint32_t, 4>, rounds_ + 1> &rk)
    {
        enclast<N>(s, rk, std::integral_constant<bool, N == rounds_>());
    }

    template <std::size_t>
    static void enclast(std::array<std::uint32_t, 4> &,
        const std::array<std::array<std::uint32_t, 4>, rounds_ + 1> &,
        std::false_type)
    {
    }

    template <std::size_t N>
    static void enclast(std::array<std::uint32_t, 4> &s,
        const std::array<std::array<std::uint32_t, 4>, rounds_ + 1> &rk,
        std::true_type)
    {
        const std::uint32_t *const table = aes_table() + 1024;
        std::array<std::uint32_t, 4> t(std::get<N>(rk));
        std::get<0>(t) ^= table[std::get<0>(s) & 0xFF] & 0x000000FF;
        std::get<1>(t) ^= table[std::get<1>(s) & 0xFF] & 0x000000FF;
        std::get<2>(t) ^= table[std::get<2>(s) & 0xFF] & 0x000000FF;
        std::get<3>(t) ^= table[std::get<3>(s) & 0xFF] & 0x000000FF;

        std::get<0>(s) >>= 8;
        std::get<1>(s) >>= 8;
        std::get<2>(s) >>= 8;
        std::get<3>(s) >>= 8;
        std::get<0>(t) ^= table[std::get<1>(s) & 0xFF] & 0x0000FF00;
        std::get<1>(t) ^= table[std::get<2>(s) & 0xFF] & 0x0000FF00;
        std::get<2>(t) ^= table[std::get<3>(s) & 0xFF] & 0x0000FF00;
        std::get<3>(t) ^= table[std::get<0>(s) & 0xFF] & 0x0000FF00;

        std::get<0>(s) >>= 8;
        std::get<1>(s) >>= 8;
        std::get<2>(s) >>= 8;
        std::get<3>(s) >>= 8;
        std::get<0>(t) ^= table[std::get<2>(s) & 0xFF] & 0x00FF0000;
        std::get<1>(t) ^= table[std::get<3>(s) & 0xFF] & 0x00FF0000;
        std::get<2>(t) ^= table[std::get<0>(s) & 0xFF] & 0x00FF0000;
        std::get<3>(t) ^= table[std::get<1>(s) & 0xFF] & 0x00FF0000;

        std::get<0>(s) >>= 8;
        std::get<1>(s) >>= 8;
        std::get<2>(s) >>= 8;
        std::get<3>(s) >>= 8;
        std::get<0>(t) ^= table[std::get<3>(s) & 0xFF] & 0xFF000000;
        std::get<1>(t) ^= table[std::get<0>(s) & 0xFF] & 0xFF000000;
        std::get<2>(t) ^= table[std::get<1>(s) & 0xFF] & 0xFF000000;
        std::get<3>(t) ^= table[std::get<2>(s) & 0xFF] & 0xFF000000;

        s = t;
    }
}; // class AESGeneratorImpl
