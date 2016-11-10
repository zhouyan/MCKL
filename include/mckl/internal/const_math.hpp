//============================================================================
// MCKL/include/mckl/internal/const_math.hpp
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

#ifndef MCKL_INTERNAL_CONST_MATH_HPP
#define MCKL_INTERNAL_CONST_MATH_HPP

#include <mckl/internal/config.h>
#include <mckl/internal/defines.hpp>
#include <mckl/utility/aligned_memory.hpp>
#include <algorithm>
#include <array>
#include <limits>
#include <type_traits>

namespace mckl
{

namespace internal
{

template <typename T, std::size_t N>
using StaticVector =
    typename std::conditional<N == Dynamic, Vector<T>, std::array<T, N>>::type;

class StirlingMatrix2
{
    public:
    StirlingMatrix2(std::size_t n, std::size_t m)
        : ncol_(m + 1), data_((n + 1) * (m + 1))
    {
        std::fill(data_.begin(), data_.end(), 0);
        get(0, 0) = 1;
        for (std::size_t j = 1; j <= m; ++j) {
            get(j, j) = 1;
            for (std::size_t i = j + 1; i <= n; ++i)
                get(i, j) = j * get(i - 1, j) + get(i - 1, j - 1);
        }
    }

    double operator()(std::size_t i, std::size_t j) const
    {
        return data_[i * ncol_ + j];
    }

    private:
    std::size_t ncol_;
    Vector<double> data_;

    double &get(std::size_t i, std::size_t j) { return data_[i * ncol_ + j]; }
}; // class StirlingMatrix

template <unsigned long long U,
    int N = std::numeric_limits<unsigned long long>::digits - 1>
class Log2L
{
    static constexpr unsigned long long M = 1ULL << N;

    public:
    static constexpr int value = M <= U ? N : Log2L<U, N - 1>::value;
}; // class Log2L

template <unsigned long long U>
class Log2L<U, 0>
{
    public:
    static constexpr int value = 0;
}; // class Log2L

template <int N>
class Log2L<0, N>
{
    public:
    static constexpr int value = N + 1;
}; // class Log2L

template <typename UIntType, UIntType U>
class Log2 : public Log2L<U, std::numeric_limits<UIntType>::digits - 1>
{
}; // class Log2

template <unsigned long long B, unsigned N, bool = N % 2 == 0>
class PowL
{
    static constexpr unsigned long long b = PowL<B, N / 2>::value;

    public:
    static constexpr unsigned long long value = b * b;
}; // class PowL

template <unsigned long long B, unsigned N>
class PowL<B, N, false>
{
    public:
    static constexpr unsigned long long value = B * PowL<B, N - 1>::value;
}; // class PowL

template <unsigned long long B>
class PowL<B, 0, true>
{
    public:
    static constexpr unsigned long long value = 1;
}; // class PowL

template <typename UIntType, unsigned long long B, unsigned N>
class Pow
{
    public:
    static constexpr UIntType value = static_cast<UIntType>(PowL<B, N>::value);
}; // class Pow

template <int P,
    int Q = (std::numeric_limits<unsigned long long>::digits <
                        std::numeric_limits<long double>::digits ?
                    std::numeric_limits<unsigned long long>::digits :
                    std::numeric_limits<long double>::digits) -
        1,
    bool = (Q < P)>
class Pow2L
{
    public:
    static constexpr long double value =
        static_cast<long double>(1ULL << Q) * Pow2L<P - Q>::value;
}; // class Pow2L

template <int P, int Q>
class Pow2L<P, Q, false>
{
    public:
    static constexpr long double value = static_cast<long double>(1ULL << P);
}; // class Pow2L

template <typename RealType, int P, bool = P >= 0>
class Pow2Impl
{
    public:
    static constexpr RealType value = static_cast<RealType>(Pow2L<P>::value);
}; // class Pow2

template <typename RealType, int P>
class Pow2Impl<RealType, P, false>
{
    public:
    static constexpr RealType value =
        static_cast<RealType>(1.0L / Pow2L<-P>::value);
}; // class Pow2

template <typename RealType, int P>
using Pow2 = Pow2Impl<RealType, P>;

template <typename UIntType, unsigned N>
class Factorial
{
    public:
    static constexpr UIntType value = N * Factorial<UIntType, N - 1>::value;
}; // class Factorial

template <typename UIntType>
class Factorial<UIntType, 1>
{
    public:
    static constexpr UIntType value = 1;
}; // class Factorial

template <typename UIntType>
class Factorial<UIntType, 0>
{
    public:
    static constexpr UIntType value = 1;
}; // class Factorial

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_INTERNAL_CONST_MATH_HPP
