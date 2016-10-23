//============================================================================
// MCKL/include/mckl/random/internal/increment_sse2_64_f.hpp
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

template <typename T>
class IncrementBlock<T, 1, 32, 64>
{
    static constexpr std::size_t K_ = 1;
    static constexpr std::size_t blocks_ = 32;

    public:
    MCKL_FLATTEN static bool aligned(void *ptr)
    {
        return reinterpret_cast<std::uintptr_t>(ptr) % 16 == 0;
    }

    MCKL_FLATTEN static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m128i a0 =
            _mm_set1_epi64x(static_cast<MCKL_INT64>(std::get<0>(ctr)));

        __m128i c0 = _mm_add_epi64(a0, _mm_set_epi64x(0x02, 0x01));
        __m128i c1 = _mm_add_epi64(a0, _mm_set_epi64x(0x04, 0x03));
        __m128i c2 = _mm_add_epi64(a0, _mm_set_epi64x(0x06, 0x05));
        __m128i c3 = _mm_add_epi64(a0, _mm_set_epi64x(0x08, 0x07));
        __m128i c4 = _mm_add_epi64(a0, _mm_set_epi64x(0x0A, 0x09));
        __m128i c5 = _mm_add_epi64(a0, _mm_set_epi64x(0x0C, 0x0B));
        __m128i c6 = _mm_add_epi64(a0, _mm_set_epi64x(0x0E, 0x0D));
        __m128i c7 = _mm_add_epi64(a0, _mm_set_epi64x(0x10, 0x0F));
        __m128i c8 = _mm_add_epi64(a0, _mm_set_epi64x(0x12, 0x11));
        __m128i c9 = _mm_add_epi64(a0, _mm_set_epi64x(0x14, 0x13));
        __m128i cA = _mm_add_epi64(a0, _mm_set_epi64x(0x16, 0x15));
        __m128i cB = _mm_add_epi64(a0, _mm_set_epi64x(0x18, 0x17));
        __m128i cC = _mm_add_epi64(a0, _mm_set_epi64x(0x1A, 0x19));
        __m128i cD = _mm_add_epi64(a0, _mm_set_epi64x(0x1C, 0x1B));
        __m128i cE = _mm_add_epi64(a0, _mm_set_epi64x(0x1E, 0x1D));
        __m128i cF = _mm_add_epi64(a0, _mm_set_epi64x(0x20, 0x1F));

        __m128i *cptr = reinterpret_cast<__m128i *>(ctr_block.data());
        _mm_store_si128(cptr++, c0);
        _mm_store_si128(cptr++, c1);
        _mm_store_si128(cptr++, c2);
        _mm_store_si128(cptr++, c3);
        _mm_store_si128(cptr++, c4);
        _mm_store_si128(cptr++, c5);
        _mm_store_si128(cptr++, c6);
        _mm_store_si128(cptr++, c7);
        _mm_store_si128(cptr++, c8);
        _mm_store_si128(cptr++, c9);
        _mm_store_si128(cptr++, cA);
        _mm_store_si128(cptr++, cB);
        _mm_store_si128(cptr++, cC);
        _mm_store_si128(cptr++, cD);
        _mm_store_si128(cptr++, cE);
        _mm_store_si128(cptr++, cF);
    }
}; // class IncrementBlock

template <typename T>
class IncrementBlock<T, 2, 16, 64>
{
    static constexpr std::size_t K_ = 2;
    static constexpr std::size_t blocks_ = 16;

    public:
    MCKL_FLATTEN static bool aligned(void *ptr)
    {
        return reinterpret_cast<std::uintptr_t>(ptr) % 16 == 0;
    }

    MCKL_FLATTEN static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m128i a0 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)));

        __m128i c0 = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x01));
        __m128i c1 = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x02));
        __m128i c2 = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x03));
        __m128i c3 = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x04));
        __m128i c4 = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x05));
        __m128i c5 = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x06));
        __m128i c6 = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x07));
        __m128i c7 = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x08));
        __m128i c8 = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x09));
        __m128i c9 = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x0A));
        __m128i cA = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x0B));
        __m128i cB = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x0C));
        __m128i cC = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x0D));
        __m128i cD = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x0E));
        __m128i cE = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x0F));
        __m128i cF = _mm_add_epi64(a0, _mm_set_epi64x(0, 0x10));

        __m128i *cptr = reinterpret_cast<__m128i *>(ctr_block.data());
        _mm_store_si128(cptr++, c0);
        _mm_store_si128(cptr++, c1);
        _mm_store_si128(cptr++, c2);
        _mm_store_si128(cptr++, c3);
        _mm_store_si128(cptr++, c4);
        _mm_store_si128(cptr++, c5);
        _mm_store_si128(cptr++, c6);
        _mm_store_si128(cptr++, c7);
        _mm_store_si128(cptr++, c8);
        _mm_store_si128(cptr++, c9);
        _mm_store_si128(cptr++, cA);
        _mm_store_si128(cptr++, cB);
        _mm_store_si128(cptr++, cC);
        _mm_store_si128(cptr++, cD);
        _mm_store_si128(cptr++, cE);
        _mm_store_si128(cptr++, cF);
    }
}; // class IncrementBlock

template <typename T>
class IncrementBlock<T, 4, 8, 64>
{
    static constexpr std::size_t K_ = 4;
    static constexpr std::size_t blocks_ = 8;

    public:
    MCKL_FLATTEN static bool aligned(void *ptr)
    {
        return reinterpret_cast<std::uintptr_t>(ptr) % 16 == 0;
    }

    MCKL_FLATTEN static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m128i a0 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)));
        __m128i a1 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
            static_cast<MCKL_INT64>(std::get<2>(ctr)));

        __m128i c0 = _mm_add_epi64(a0, _mm_set_epi64x(0, 1));
        __m128i c2 = _mm_add_epi64(a0, _mm_set_epi64x(0, 2));
        __m128i c4 = _mm_add_epi64(a0, _mm_set_epi64x(0, 3));
        __m128i c6 = _mm_add_epi64(a0, _mm_set_epi64x(0, 4));
        __m128i c8 = _mm_add_epi64(a0, _mm_set_epi64x(0, 5));
        __m128i cA = _mm_add_epi64(a0, _mm_set_epi64x(0, 6));
        __m128i cC = _mm_add_epi64(a0, _mm_set_epi64x(0, 7));
        __m128i cE = _mm_add_epi64(a0, _mm_set_epi64x(0, 8));

        __m128i *cptr = reinterpret_cast<__m128i *>(ctr_block.data());
        _mm_store_si128(cptr++, c0);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, c2);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, c4);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, c6);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, c8);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, cA);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, cC);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, cE);
        _mm_store_si128(cptr++, a1);
    }
}; // class IncrementBlock

template <typename T>
class IncrementBlock<T, 8, 4, 64>
{
    static constexpr std::size_t K_ = 8;
    static constexpr std::size_t blocks_ = 4;

    public:
    MCKL_FLATTEN static bool aligned(void *ptr)
    {
        return reinterpret_cast<std::uintptr_t>(ptr) % 16 == 0;
    }

    MCKL_FLATTEN static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m128i a0 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)));
        __m128i a1 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
            static_cast<MCKL_INT64>(std::get<2>(ctr)));
        __m128i a2 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<5>(ctr)),
            static_cast<MCKL_INT64>(std::get<4>(ctr)));
        __m128i a3 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<7>(ctr)),
            static_cast<MCKL_INT64>(std::get<6>(ctr)));

        __m128i c0 = _mm_add_epi64(a0, _mm_set_epi64x(0, 1));
        __m128i c4 = _mm_add_epi64(a0, _mm_set_epi64x(0, 2));
        __m128i c8 = _mm_add_epi64(a0, _mm_set_epi64x(0, 2));
        __m128i cC = _mm_add_epi64(a0, _mm_set_epi64x(0, 3));

        __m128i *cptr = reinterpret_cast<__m128i *>(ctr_block.data());
        _mm_store_si128(cptr++, c0);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, a2);
        _mm_store_si128(cptr++, a3);
        _mm_store_si128(cptr++, c4);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, a2);
        _mm_store_si128(cptr++, a3);
        _mm_store_si128(cptr++, c8);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, a2);
        _mm_store_si128(cptr++, a3);
        _mm_store_si128(cptr++, cC);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, a2);
        _mm_store_si128(cptr++, a3);
    }
}; // class IncrementBlock

template <typename T>
class IncrementBlock<T, 16, 2, 64>
{
    static constexpr std::size_t K_ = 8;
    static constexpr std::size_t blocks_ = 2;

    public:
    MCKL_FLATTEN static bool aligned(void *ptr)
    {
        return reinterpret_cast<std::uintptr_t>(ptr) % 16 == 0;
    }

    MCKL_FLATTEN static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m128i a0 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x0>(ctr)));
        __m128i a1 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x3>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x2>(ctr)));
        __m128i a2 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x5>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x4>(ctr)));
        __m128i a3 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x7>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x6>(ctr)));
        __m128i a4 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x9>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x8>(ctr)));
        __m128i a5 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0xB>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xA>(ctr)));
        __m128i a6 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0xD>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xC>(ctr)));
        __m128i a7 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0xF>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xE>(ctr)));

        __m128i c0 = _mm_add_epi64(a0, _mm_set_epi64x(0, 1));
        __m128i c8 = _mm_add_epi64(a0, _mm_set_epi64x(0, 1));

        __m128i *cptr = reinterpret_cast<__m128i *>(ctr_block.data());
        _mm_store_si128(cptr++, c0);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, a2);
        _mm_store_si128(cptr++, a3);
        _mm_store_si128(cptr++, a4);
        _mm_store_si128(cptr++, a5);
        _mm_store_si128(cptr++, a6);
        _mm_store_si128(cptr++, a7);
        _mm_store_si128(cptr++, c8);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, a2);
        _mm_store_si128(cptr++, a3);
        _mm_store_si128(cptr++, a4);
        _mm_store_si128(cptr++, a5);
        _mm_store_si128(cptr++, a6);
        _mm_store_si128(cptr++, a7);
    }
}; // class IncrementBlock
