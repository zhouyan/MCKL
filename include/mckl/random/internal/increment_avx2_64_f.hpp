//============================================================================
// MCKL/include/mckl/random/internal/increment_avx2_64_f.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_INCREMENT_AVX2_64_F_HPP
#define MCKL_RANDOM_INTERNAL_INCREMENT_AVX2_64_F_HPP

#include <mckl/random/internal/increment_generic.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

namespace mckl
{

namespace internal
{

template <typename T>
class IncrementBlockSI256<T, 1, 64, 64>
{
    static constexpr std::size_t K_ = 1;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m256i, 16> &s)
    {
        __m256i c =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0x0>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0x04, 0x03, 0x02, 0x01));
        std::get<0x1>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0x08, 0x07, 0x06, 0x05));
        std::get<0x2>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0x0C, 0x0B, 0x0A, 0x09));
        std::get<0x3>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0x10, 0x0F, 0x0E, 0x0D));
        std::get<0x4>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0x14, 0x13, 0x12, 0x11));
        std::get<0x5>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0x18, 0x17, 0x16, 0x15));
        std::get<0x6>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0x1C, 0x1B, 0x1A, 0x19));
        std::get<0x7>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0x20, 0x1F, 0x1E, 0x1D));

        __m256i d = _mm256_set1_epi64x(0x20);
        std::get<0x8>(s) = _mm256_add_epi64(std::get<0x0>(s), d);
        std::get<0x9>(s) = _mm256_add_epi64(std::get<0x1>(s), d);
        std::get<0xA>(s) = _mm256_add_epi64(std::get<0x2>(s), d);
        std::get<0xB>(s) = _mm256_add_epi64(std::get<0x3>(s), d);
        std::get<0xC>(s) = _mm256_add_epi64(std::get<0x4>(s), d);
        std::get<0xD>(s) = _mm256_add_epi64(std::get<0x5>(s), d);
        std::get<0xE>(s) = _mm256_add_epi64(std::get<0x6>(s), d);
        std::get<0xF>(s) = _mm256_add_epi64(std::get<0x7>(s), d);
    }
}; // class IncrementBlockSI256

template <typename T>
class IncrementBlockSI256<T, 2, 32, 64>
{
    static constexpr std::size_t K_ = 2;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m256i, 16> &s)
    {
        __m256i c =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0x0>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0, 0x02, 0, 0x01));
        std::get<0x1>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0, 0x04, 0, 0x03));
        std::get<0x2>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0, 0x06, 0, 0x05));
        std::get<0x3>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0, 0x08, 0, 0x07));
        std::get<0x4>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0, 0x0A, 0, 0x09));
        std::get<0x5>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0, 0x0C, 0, 0x0B));
        std::get<0x6>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0, 0x0E, 0, 0x0D));
        std::get<0x7>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0, 0x10, 0, 0x0F));

        __m256i d = _mm256_set_epi64x(0, 0x10, 0, 0x10);
        std::get<0x8>(s) = _mm256_add_epi64(std::get<0x0>(s), d);
        std::get<0x9>(s) = _mm256_add_epi64(std::get<0x1>(s), d);
        std::get<0xA>(s) = _mm256_add_epi64(std::get<0x2>(s), d);
        std::get<0xB>(s) = _mm256_add_epi64(std::get<0x3>(s), d);
        std::get<0xC>(s) = _mm256_add_epi64(std::get<0x4>(s), d);
        std::get<0xD>(s) = _mm256_add_epi64(std::get<0x5>(s), d);
        std::get<0xE>(s) = _mm256_add_epi64(std::get<0x6>(s), d);
        std::get<0xF>(s) = _mm256_add_epi64(std::get<0x7>(s), d);
    }
}; // class IncrementBlockSI256

template <typename T>
class IncrementBlockSI256<T, 4, 16, 64>
{
    static constexpr std::size_t K_ = 4;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m256i, 16> &s)
    {
        __m256i c =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
                static_cast<MCKL_INT64>(std::get<2>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0x0>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 1));
        std::get<0x1>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 2));
        std::get<0x2>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 3));
        std::get<0x3>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 4));
        std::get<0x4>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 5));
        std::get<0x5>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 6));
        std::get<0x6>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 7));
        std::get<0x7>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 8));

        __m256i d = _mm256_set_epi64x(0, 0, 0, 8);
        std::get<0x8>(s) = _mm256_add_epi64(std::get<0x0>(s), d);
        std::get<0x9>(s) = _mm256_add_epi64(std::get<0x1>(s), d);
        std::get<0xA>(s) = _mm256_add_epi64(std::get<0x2>(s), d);
        std::get<0xB>(s) = _mm256_add_epi64(std::get<0x3>(s), d);
        std::get<0xC>(s) = _mm256_add_epi64(std::get<0x4>(s), d);
        std::get<0xD>(s) = _mm256_add_epi64(std::get<0x5>(s), d);
        std::get<0xE>(s) = _mm256_add_epi64(std::get<0x6>(s), d);
        std::get<0xF>(s) = _mm256_add_epi64(std::get<0x7>(s), d);
    }
}; // class IncrementBlockSI256

template <typename T>
class IncrementBlockSI256<T, 8, 8, 64>
{
    static constexpr std::size_t K_ = 8;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m256i, 16> &s)
    {
        __m256i c =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
                static_cast<MCKL_INT64>(std::get<2>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0x0>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 1));
        std::get<0x1>(s) =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<7>(ctr)),
                static_cast<MCKL_INT64>(std::get<6>(ctr)),
                static_cast<MCKL_INT64>(std::get<5>(ctr)),
                static_cast<MCKL_INT64>(std::get<4>(ctr)));
        std::get<0x2>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 2));
        std::get<0x3>(s) = std::get<0x1>(s);
        std::get<0x4>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 3));
        std::get<0x5>(s) = std::get<0x1>(s);
        std::get<0x6>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 4));
        std::get<0x7>(s) = std::get<0x1>(s);
        std::get<0x8>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 5));
        std::get<0x9>(s) = std::get<0x1>(s);
        std::get<0xA>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 6));
        std::get<0xB>(s) = std::get<0x1>(s);
        std::get<0xC>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 7));
        std::get<0xD>(s) = std::get<0x1>(s);
        std::get<0xE>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 8));
        std::get<0xF>(s) = std::get<0x1>(s);
    }
}; // class IncrementBlockSI256

template <typename T>
class IncrementBlockSI256<T, 16, 4, 64>
{
    static constexpr std::size_t K_ = 16;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m256i, 16> &s)
    {
        __m256i c =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x3>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x2>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x0>(ctr)));

        std::get<0x0>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 1));
        std::get<0x1>(s) =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x7>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x6>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x5>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x4>(ctr)));
        std::get<0x2>(s) =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0xB>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xA>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x9>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x8>(ctr)));
        std::get<0x3>(s) =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0xF>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xE>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xD>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xC>(ctr)));
        std::get<0x4>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 2));
        std::get<0x5>(s) = std::get<1>(s);
        std::get<0x6>(s) = std::get<2>(s);
        std::get<0x7>(s) = std::get<3>(s);
        std::get<0x8>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 3));
        std::get<0x9>(s) = std::get<1>(s);
        std::get<0xA>(s) = std::get<2>(s);
        std::get<0xB>(s) = std::get<3>(s);
        std::get<0xC>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 4));
        std::get<0xD>(s) = std::get<1>(s);
        std::get<0xE>(s) = std::get<2>(s);
        std::get<0xF>(s) = std::get<3>(s);
    }
}; // class IncrementBlockSI256

template <typename T>
class IncrementBlockSI256<T, 32, 2, 64>
{
    static constexpr std::size_t K_ = 32;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m256i, 16> &s)
    {
        __m256i c =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x03>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x02>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x01>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x00>(ctr)));

        std::get<0x0>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 1));
        std::get<0x1>(s) =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x07>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x06>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x05>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x04>(ctr)));
        std::get<0x2>(s) =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x0B>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x0A>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x09>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x08>(ctr)));
        std::get<0x3>(s) =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x0F>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x0E>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x0D>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x0C>(ctr)));
        std::get<0x4>(s) =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x13>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x12>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x11>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x10>(ctr)));
        std::get<0x5>(s) =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x17>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x16>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x15>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x14>(ctr)));
        std::get<0x6>(s) =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x1B>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x1A>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x19>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x18>(ctr)));
        std::get<0x7>(s) =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x1F>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x1E>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x1D>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x1C>(ctr)));
        std::get<0x8>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 2));
        std::get<0x9>(s) = std::get<0x1>(s);
        std::get<0xA>(s) = std::get<0x2>(s);
        std::get<0xB>(s) = std::get<0x3>(s);
        std::get<0xC>(s) = std::get<0x4>(s);
        std::get<0xD>(s) = std::get<0x5>(s);
        std::get<0xE>(s) = std::get<0x6>(s);
        std::get<0xF>(s) = std::get<0x7>(s);
    }
}; // class IncrementBlockSI256

template <typename T>
class IncrementBlockSI256<T, 64, 1, 64>
{
    static constexpr std::size_t K_ = 64;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m256i, 16> &s)
    {
        std::memcpy(s.data(), ctr.data(), 512);
        std::get<0>(s) =
            _mm256_add_epi64(std::get<0>(s), _mm256_set_epi64x(0, 0, 0, 1));
    }
}; // class IncrementBlockSI256

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_INCREMENT_AVX2_64_F_HPP
