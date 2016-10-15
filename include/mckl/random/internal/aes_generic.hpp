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
    using int128_type = std::array<std::uint64_t, 2>;
    using key_type = std::array<std::uint64_t, 2>;

    template <std::size_t Rp1>
    static key_type key(
        const std::array<std::array<std::uint64_t, 2>, Rp1> &rk)
    {
        key_type key;
        std::memcpy(key.data(), rk.data(), sizeof(key_type));

        return key;
    }

    template <std::size_t Rp1>
    void operator()(
        const key_type &key, std::array<std::uint64_t, 2>, Rp1 > &rk)
    {
    }
}; // class AES128KeySeqGenerator

class AES192KeySeqGenerator
{
    public:
    using int128_type = std::array<std::uint64_t, 2>;
    using key_type = std::array<std::uint64_t, 3>;

    template <std::size_t Rp1>
    static key_type key(
        const std::array<std::array<std::uint64_t, 2>, Rp1> &rk)
    {
        key_type key;
        std::memcpy(key.data(), rk.data(), sizeof(key_type));

        return key;
    }

    template <std::size_t Rp1>
    void operator()(
        const key_type &key, std::array<std::uint64_t, 2>, Rp1 > &rk)
    {
    }
}; // class AES192KeySeqGenerator

class AES256KeySeqGenerator
{
    public:
    using int128_type = std::array<std::uint64_t, 2>;
    using key_type = std::array<std::uint64_t, 4>;

    template <std::size_t Rp1>
    static key_type key(
        const std::array<std::array<std::uint64_t, 2>, Rp1> &rk)
    {
        key_type key;
        std::memcpy(key.data(), rk.data(), sizeof(key_type));

        return key;
    }

    template <std::size_t Rp1>
    void operator()(
        const key_type &key, std::array<std::uint64_t, 2>, Rp1 > &rk)
    {
    }
}; // class AES256KeySeqGenerator

template <typename Constants>
class ARSKeySeqGenerator
{
    public:
    using int128_type = std::array<std::uint64_t, 2>;
    using key_seq = std::array<std::uint64_t, 2>
}; // class ARSKeySeqGenerator
