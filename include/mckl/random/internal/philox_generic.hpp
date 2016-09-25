//============================================================================
// MCKL/include/mckl/random/internal/philox_generic.hpp
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

template <typename T, int = std::numeric_limits<T>::digits>
class PhiloxHiLo;

template <typename T>
class PhiloxHiLo<T, 32>
{
    public:
    static void eval(T a, T b, T &hi, T &lo)
    {
        union {
            std::uint64_t prod;
            std::array<T, 2> result;
        } buf;

        buf.prod =
            static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b);
#if MCKL_HAS_X86 // little endian
        hi = std::get<1>(buf.result);
        lo = std::get<0>(buf.result);
#else  // MCKL_HAS_X86
        hi = static_cast<T>(buf.prod >> 32);
        lo = static_cast<T>(buf.prod);
#endif // MCKL_HAS_X86
    }
}; // class PhiloxHiLo

template <typename T>
class PhiloxHiLo<T, 64>
{
    public:
    static void eval(T a, T b, T &hi, T &lo)
    {
#if MCKL_HAS_INT128

#ifdef MCKL_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif // MCKL_GCC
        union {
            unsigned MCKL_INT128 prod;
            std::array<T, 2> result;
        } buf;

        buf.prod = static_cast<unsigned MCKL_INT128>(a) *
            static_cast<unsigned MCKL_INT128>(b);
#ifdef MCKL_GCC
#pragma GCC diagnostic pop
#endif // MCKL_GCC

#if MCKL_HAS_X86 // littel endia
        hi = std::get<1>(buf.result);
        lo = std::get<0>(buf.result);
#else  // MCKL_HAS_X86
        hi = static_cast<T>(buf.prod >> 64);
        lo = static_cast<T>(buf.prod);
#endif // MCKL_HAS_X86

#elif defined(MCKL_MSVC)

        unsigned __int64 Multiplier = static_cast<unsigned __int64>(a);
        unsigned __int64 Multiplicand = static_cast<unsigned __int64>(b);
        unsigned __int64 hi_tmp = 0;
        lo = static_cast<T>(_umul128(Multiplier, Multiplicand, &hi_tmp));
        hi = static_cast<T>(hi_tmp);

#else  // MCKL_HAS_INT128

        const T lomask = (const_one<T>() << 32) - 1;
        const T ahi = a >> 32;
        const T alo = a & lomask;
        const T bhi = b >> 32;
        const T blo = b & lomask;
        const T ahbl = ahi * blo;
        const T albh = alo * bhi;
        const T ahbl_albh = ((ahbl & lomask) + (albh & lomask));

        lo = a * b;
        hi = ahi * bhi + (ahbl >> 32) + (albh >> 32);
        hi += ahbl_albh >> 32;
        hi += ((lo >> 32) < (ahbl_albh & lomask));
#endif // MCKL_HAS_INT128
    }
}; // class PhiloxHiLo

template <typename T, std::size_t K, std::size_t N, typename Constants>
class PhiloxKBox
{
    public:
    static void eval(std::array<T, K / 2> &par)
    {
        eval<0>(par, std::integral_constant<bool, 0 < K / 2>());
    }

    private:
    template <std::size_t>
    static void eval(std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(std::array<T, K / 2> &par, std::true_type)
    {
        static constexpr T w = Constants::weyl[I];

        std::get<I>(par) += w;
        eval<I + 1>(par, std::integral_constant<bool, I + 1 < K / 2>());
    }
}; // class PhiloxKBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class PhiloxSBox
{
    public:
    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &par)
    {
        eval<0>(state, par, std::integral_constant<bool, 1 < K>());
    }

    private:
    template <std::size_t>
    static void eval(
        std::array<T, K> &, const std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &par,
        std::true_type)
    {
        static constexpr T m = Constants::multiplier[I / 2];

        T x = std::get<I + 1>(state) ^ std::get<I / 2>(par);
        PhiloxHiLo<T>::eval(
            std::get<I>(state), m, std::get<I>(state), std::get<I + 1>(state));
        std::get<I>(state) ^= x;
        eval<I + 2>(state, par, std::integral_constant<bool, I + 3 < K>());
    }
}; // class PhiloxSBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class PhiloxPBox
{
    public:
    static void eval(std::array<T, K> &state)
    {
        std::array<T, K> tmp;
        eval<0>(state, tmp, std::integral_constant<bool, 0 < K>());
        state = tmp;
    }

    private:
    template <std::size_t>
    static void eval(
        const std::array<T, K> &, std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(
        const std::array<T, K> &state, std::array<T, K> &tmp, std::true_type)
    {
        static constexpr std::size_t P =
            ThreefryConstants<T, K>::permute[K - I - 1];
        static constexpr std::size_t J = K - P - 1;

        std::get<I>(tmp) = std::get<J>(state);
        eval<I + 1>(state, tmp, std::integral_constant<bool, I + 1 < K>());
    }
}; // class PhiloxPBox

template <typename T, std::size_t N, typename Constants>
class PhiloxPBox<T, 2, N, Constants>
{
    public:
    static void eval(std::array<T, 2> &) {}
}; // class PhiloxPBox

template <typename T, std::size_t N, typename Constants>
class PhiloxPBox<T, 4, N, Constants>
{
    public:
    static void eval(std::array<T, 4> &state)
    {
        std::swap(std::get<0>(state), std::get<2>(state));
    }
}; // class PhiloxPBox

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorGenericImpl
{
    public:
    static constexpr std::size_t blocks()
    {
        return sizeof(T) * K < 128 ? 128 / (sizeof(T) * K) : 1;
    }

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key)
    {
        std::array<T, K / 2> par = key;
        eval<0>(state, par, std::integral_constant<bool, 0 <= Rounds>());
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K / 2> &key)
    {
        std::array<T, K / 2> par = key;
        eval<0>(state, par, std::integral_constant<bool, 0 <= Rounds>());
    }

    private:
    template <std::size_t>
    static void eval(
        std::array<T, K> &, std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(
        std::array<T, K> &state, std::array<T, K / 2> &par, std::true_type)
    {
        kbox<N>(par);
        pbox<N>(state);
        sbox<N>(state, par);
        eval<N + 1>(
            state, par, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t>
    static void eval(std::array<std::array<T, K>, blocks()> &,
        std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<std::array<T, K>, blocks()> &state,
        std::array<T, K / 2> &par, std::true_type)
    {
        kbox<N>(par);
        pbox<N>(state);
        sbox<N>(state, par);
        eval<N + 1>(
            state, par, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t N>
    static void kbox(std::array<T, K / 2> &par)
    {
        kbox<N>(par, std::integral_constant<bool, (N > 1 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void kbox(std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void kbox(std::array<T, K / 2> &par, std::true_type)
    {
        PhiloxKBox<T, K, N, Constants>::eval(par);
    }

    template <std::size_t N>
    static void sbox(std::array<T, K> &state, const std::array<T, K / 2> &par)
    {
        sbox<N>(state, par,
            std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void sbox(
        std::array<T, K> &, const std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void sbox(std::array<T, K> &state, const std::array<T, K / 2> &par,
        std::true_type)
    {
        PhiloxSBox<T, K, N, Constants>::eval(state, par);
    }

    template <std::size_t N>
    static void pbox(std::array<T, K> &state)
    {
        pbox<N>(state, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void pbox(std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void pbox(std::array<T, K> &state, std::true_type)
    {
        PhiloxPBox<T, K, N, Constants>::eval(state);
    }

    template <std::size_t N>
    static void sbox(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K / 2> &par)
    {
        sbox<N>(state, par,
            std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void sbox(std::array<std::array<T, K>, blocks()> &,
        const std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void sbox(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K / 2> &par, std::true_type)
    {
        sbox<N, 0>(state, par, std::integral_constant<bool, 0 < blocks()>());
    }

    template <std::size_t, std::size_t>
    static void sbox(std::array<std::array<T, K>, blocks()> &,
        const std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t I>
    static void sbox(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K / 2> &par, std::true_type)
    {
        PhiloxSBox<T, K, N, Constants>::eval(std::get<I>(state), par);
        sbox<N, I + 1>(
            state, par, std::integral_constant<bool, I + 1 < blocks()>());
    }

    template <std::size_t N>
    static void pbox(std::array<std::array<T, K>, blocks()> &state)
    {
        pbox<N>(state, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void pbox(std::array<std::array<T, K>, blocks()> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void pbox(
        std::array<std::array<T, K>, blocks()> &state, std::true_type)
    {
        pbox<N, 0>(state, std::integral_constant<bool, 0 < blocks()>());
    }

    template <std::size_t, std::size_t>
    static void pbox(std::array<std::array<T, K>, blocks()> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t I>
    static void pbox(
        std::array<std::array<T, K>, blocks()> &state, std::true_type)
    {
        PhiloxPBox<T, K, N, Constants>::eval(std::get<I>(state));
        pbox<N, I + 1>(
            state, std::integral_constant<bool, I + 1 < blocks()>());
    }
}; // class PhiloxGeneratorGenericImpl

template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    int = std::numeric_limits<T>::digits>
class PhiloxGeneratorImpl
    : public PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>
{
}; // class PhiloxGeneratorImpl
