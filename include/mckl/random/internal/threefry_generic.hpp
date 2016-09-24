//============================================================================
// MCKL/include/mckl/random/threefry_generic.hpp
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
    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par)
    {
        eval<0>(state, par, std::integral_constant<bool, 0 < K>());
        state.back() += s_;
    }

    private:
    static constexpr T s_ = N / 4;

    template <std::size_t>
    static void eval(
        std::array<T, K> &, const std::array<T, K + 1> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par,
        std::true_type)
    {
        std::get<I>(state) += std::get<(s_ + I) % (K + 1)>(par);
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
}; // class ThreefryPBox

template <typename T, std::size_t N>
class ThreefryPBox<T, 2, N, ThreefryConstants<T, 2>>
{
    public:
    static void eval(std::array<T, 2> &) {}
}; // class ThreefryPBox

template <typename T, std::size_t N>
class ThreefryPBox<T, 4, N, ThreefryConstants<T, 4>>
{
    public:
    static void eval(std::array<T, 4> &state)
    {
        std::swap(std::get<1>(state), std::get<3>(state));
    }
}; // class ThreefryPBox

template <typename T, std::size_t N>
class ThreefryPBox<T, 8, N, ThreefryConstants<T, 8>>
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
}; // class ThreefryPBox

template <typename T, std::size_t N>
class ThreefryPBox<T, 16, N, ThreefryConstants<T, 16>>
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
}; // class ThreefryPBox

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class ThreefryGeneratorGenericImpl
{
    public:
    static constexpr std::size_t blocks() { return 8; }

    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par)
    {
        eval<0>(state, par, std::integral_constant<bool, 0 <= Rounds>());
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K + 1> &par)
    {
        eval(std::get<0>(state), par);
        eval(std::get<1>(state), par);
        eval(std::get<2>(state), par);
        eval(std::get<3>(state), par);
        eval(std::get<4>(state), par);
        eval(std::get<5>(state), par);
        eval(std::get<6>(state), par);
        eval(std::get<7>(state), par);
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
