//============================================================================
// MCKL/include/mckl/random/internal/threefry_generic.hpp
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

template <typename T, std::size_t K, std::size_t N, typename Constants>
class ThreefryKBox
{
    public:
    template <std::size_t I>
    static T key(const std::array<T, K + 1> &par)
    {
        static constexpr std::size_t S = N / 4;

        static constexpr T t[3] = {Constants::tweaks[0], Constants::tweaks[1],
            Constants::tweaks[0] ^ Constants::tweaks[1]};

        static constexpr T p = (I + 1 == K ? S : 0) +
            (I + 2 == K ? t[(S + 1) % 3] : 0) + (I + 3 == K ? t[S % 3] : 0);

        return std::get<(S + I) % (K + 1)>(par) + p;
    }

    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par)
    {
        eval<0>(state, par, std::integral_constant<bool, 0 < K>());
    }

    private:
    template <std::size_t>
    static void eval(
        std::array<T, K> &, const std::array<T, K + 1> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par,
        std::true_type)
    {
        std::get<I>(state) += key<I>(par);
        eval<I + 1>(state, par, std::integral_constant<bool, I + 1 < K>());
    }
}; // class ThreefryKBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class ThreefrySBox
{
    public:
    static void eval(std::array<T, K> &state)
    {
        eval<0>(state, std::integral_constant<bool, 1 < K>());
    }

    private:
    template <std::size_t>
    static void eval(std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(std::array<T, K> &state, std::true_type)
    {
        static constexpr int L = Constants::rotate[I / 2][(N - 1) % 8];
        static constexpr int R = std::numeric_limits<T>::digits - L;

        T x = std::get<I + 1>(state);
        std::get<I>(state) += x;
        x = (x << L) | (x >> R);
        x ^= std::get<I>(state);
        std::get<I + 1>(state) = x;
        eval<I + 2>(state, std::integral_constant<bool, I + 3 < K>());
    }
}; // class ThreefrySBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class ThreefryPBox
{
    public:
    static void eval(std::array<T, K> &state)
    {
        std::array<T, K> tmp;
        eval<0>(state, tmp, std::integral_constant<bool, 0 < N>());
        state = tmp;
    }

    static void eval(T *state)
    {
        std::array<T, K> tmp;
        eval<0>(state, tmp, std::integral_constant<bool, 0 < N>());
        std::copy(tmp.begin(), tmp.end(), state);
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
        static constexpr std::size_t P = Constants::permute[I];

        std::get<I>(tmp) = std::get<P>(state);
        eval<I + 1>(state, tmp, std::integral_constant<bool, I + 1 < K>());
    }

    template <std::size_t>
    static void eval(const T *, std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(const T *state, std::array<T, K> &tmp, std::true_type)
    {
        static constexpr std::size_t P = Constants::permute[I];

        std::get<I>(tmp) = state[P];
        eval<I + 1>(state, tmp, std::integral_constant<bool, I + 1 < K>());
    }
}; // class ThreefryPBox

template <typename T, typename U, std::size_t N>
class ThreefryPBox<T, 2, N, ThreefryConstants<U, 2>>
{
    public:
    static void eval(std::array<T, 2> &) {}

    static void eval(T *) {}
}; // class ThreefryPBox

template <typename T, typename U, std::size_t N>
class ThreefryPBox<T, 4, N, ThreefryConstants<U, 4>>
{
    public:
    static void eval(std::array<T, 4> &state)
    {
        std::swap(std::get<1>(state), std::get<3>(state));
    }

    static void eval(T *state) { std::swap(state[1], state[3]); }
}; // class ThreefryPBox

template <typename T, typename U, std::size_t N>
class ThreefryPBox<T, 8, N, ThreefryConstants<U, 8>>
{
    public:
    static void eval(std::array<T, 8> &state)
    {
        T x0 = std::get<0>(state);
        T x3 = std::get<3>(state);
        std::get<0>(state) = std::get<2>(state);
        std::get<2>(state) = std::get<4>(state);
        std::get<3>(state) = std::get<7>(state);
        std::get<4>(state) = std::get<6>(state);
        std::get<6>(state) = x0;
        std::get<7>(state) = x3;
    }

    static void eval(T *state)
    {
        T x0 = state[0];
        T x3 = state[3];
        state[0] = state[2];
        state[2] = state[4];
        state[3] = state[7];
        state[4] = state[6];
        state[6] = x0;
        state[7] = x3;
    }
}; // class ThreefryPBox

template <typename T, typename U, std::size_t N>
class ThreefryPBox<T, 16, N, ThreefryConstants<U, 16>>
{
    public:
    static void eval(std::array<T, 16> &state)
    {
        T x1 = std::get<1>(state);
        T x3 = std::get<3>(state);
        T x4 = std::get<4>(state);
        T x5 = std::get<5>(state);
        T x7 = std::get<7>(state);
        T x8 = std::get<8>(state);
        std::get<1>(state) = std::get<9>(state);
        std::get<3>(state) = std::get<13>(state);
        std::get<4>(state) = std::get<6>(state);
        std::get<5>(state) = std::get<11>(state);
        std::get<6>(state) = x4;
        std::get<7>(state) = std::get<15>(state);
        std::get<8>(state) = std::get<10>(state);
        std::get<9>(state) = x7;
        std::get<10>(state) = std::get<12>(state);
        std::get<11>(state) = x3;
        std::get<12>(state) = std::get<14>(state);
        std::get<13>(state) = x5;
        std::get<14>(state) = x8;
        std::get<15>(state) = x1;
    }

    static void eval(T *state)
    {
        T x1 = state[1];
        T x3 = state[3];
        T x4 = state[4];
        T x5 = state[5];
        T x7 = state[7];
        T x8 = state[8];
        state[1] = state[9];
        state[3] = state[13];
        state[4] = state[6];
        state[5] = state[11];
        state[6] = x4;
        state[7] = state[15];
        state[8] = state[10];
        state[9] = x7;
        state[10] = state[12];
        state[11] = x3;
        state[12] = state[14];
        state[13] = x5;
        state[14] = x8;
        state[15] = x1;
    }
}; // class ThreefryPBox

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class ThreefryGeneratorGenericImpl
{
    public:
    static constexpr bool batch() { return false; }

    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par)
    {
        eval<0>(state, par, std::integral_constant<bool, 0 <= Rounds>());
    }

    private:
    template <std::size_t>
    static void eval(
        std::array<T, K> &, const std::array<T, K + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par,
        std::true_type)
    {
        sbox<N>(state);
        pbox<N>(state);
        kbox<N>(state, par);
        eval<N + 1>(
            state, par, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t N>
    static void kbox(std::array<T, K> &state, const std::array<T, K + 1> &par)
    {
        kbox<N>(state, par,
            std::integral_constant<bool, (N % 4 == 0 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void kbox(
        std::array<T, K> &, const std::array<T, K + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void kbox(std::array<T, K> &state, const std::array<T, K + 1> &par,
        std::true_type)
    {
        ThreefryKBox<T, K, N, Constants>::eval(state, par);
    }

    template <std::size_t N>
    static void sbox(std::array<T, K> &state)
    {
        sbox<N>(state, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void sbox(std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void sbox(std::array<T, K> &state, std::true_type)
    {
        ThreefrySBox<T, K, N, Constants>::eval(state);
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
        ThreefryPBox<T, K, N, Constants>::eval(state);
    }
}; // class ThreefryGeneratorGenericImpl

template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    int = std::numeric_limits<T>::digits>
class ThreefryGeneratorImpl
    : public ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>
{
}; // class PhiloxGeneratorImpl
