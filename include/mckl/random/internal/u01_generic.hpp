//============================================================================
// MCKL/include/mckl/random/internal/u01_generic.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_U01_GENERIC_HPP
#define MCKL_RANDOM_INTERNAL_U01_GENERIC_HPP

#include <mckl/random/internal/common.hpp>

namespace mckl {

namespace internal {

template <int W, typename UIntType>
using U01UIntLeastType = std::conditional_t<W <= 32, std::uint32_t,
    std::conditional_t<W <= 64, std::uint64_t, UIntType>>;

template <typename UIntType, typename, typename, typename>
class U01GenericImpl;

template <typename UIntType, typename RealType>
class U01GenericImpl<UIntType, RealType, Closed, Closed>
{
  public:
    static RealType eval(UIntType u)
    {
        constexpr int W = std::numeric_limits<UIntType>::digits;
        constexpr int M = std::numeric_limits<RealType>::digits;
        constexpr int P = W - 1 < M ? W - 1 : M;
        constexpr int V = P + 1;
        constexpr int L = V < W ? 1 : 0;
        constexpr int R = V < W ? W - 1 - V : 0;

        using UIntLeastType = U01UIntLeastType<W - R, UIntType>;

        return trans(static_cast<UIntLeastType>((u << L) >> (R + L)),
                   std::integral_constant<bool, (V < W)>()) *
            Pow2<RealType, -(P + 1)>::value;
    }

    static void eval(std::size_t n, const UIntType *u, RealType *r)
    {
        for (std::size_t i = 0; i != n; ++i)
            r[i] = eval(u[i]);
    }

  private:
    template <typename UIntLeastType>
    static RealType trans(UIntLeastType u, std::true_type)
    {
        return static_cast<RealType>((u & 1) + u);
    }

    template <typename UIntLeastType>
    static RealType trans(UIntLeastType u, std::false_type)
    {
        return static_cast<RealType>(u & 1) + static_cast<RealType>(u);
    }
}; // class U01GenericImpl

template <typename UIntType, typename RealType>
class U01GenericImpl<UIntType, RealType, Closed, Open>
{
  public:
    static RealType eval(UIntType u)
    {
        constexpr int W = std::numeric_limits<UIntType>::digits;
        constexpr int M = std::numeric_limits<RealType>::digits;
        constexpr int P = W < M ? W : M;
        constexpr int R = W - P;

        using UIntLeastType = U01UIntLeastType<W - R, UIntType>;

        return static_cast<RealType>(static_cast<UIntLeastType>(u >> R)) *
            Pow2<RealType, -P>::value;
    }

    static void eval(std::size_t n, const UIntType *u, RealType *r)
    {
        for (std::size_t i = 0; i != n; ++i)
            r[i] = eval(u[i]);
    }
}; // class U01GenericImpl

template <typename UIntType, typename RealType>
class U01GenericImpl<UIntType, RealType, Open, Closed>
{
  public:
    static RealType eval(UIntType u)
    {
        constexpr int W = std::numeric_limits<UIntType>::digits;
        constexpr int M = std::numeric_limits<RealType>::digits;
        constexpr int P = W < M ? W : M;
        constexpr int R = W - P;

        using UIntLeastType = U01UIntLeastType<W - R, UIntType>;

        return static_cast<RealType>(static_cast<UIntLeastType>(u >> R)) *
            Pow2<RealType, -P>::value +
            Pow2<RealType, -P>::value;
    }

    static void eval(std::size_t n, const UIntType *u, RealType *r)
    {
        for (std::size_t i = 0; i != n; ++i)
            r[i] = eval(u[i]);
    }
}; // class U01GenericImpl

template <typename UIntType, typename RealType>
class U01GenericImpl<UIntType, RealType, Open, Open>
{
  public:
    static RealType eval(UIntType u)
    {
        constexpr int W = std::numeric_limits<UIntType>::digits;
        constexpr int M = std::numeric_limits<RealType>::digits;
        constexpr int P = W + 1 < M ? W + 1 : M;
        constexpr int R = W + 1 - P;

        using UIntLeastType = U01UIntLeastType<W - R, UIntType>;

        return static_cast<RealType>(static_cast<UIntLeastType>(u >> R)) *
            Pow2<RealType, -(P - 1)>::value +
            Pow2<RealType, -P>::value;
    }

    static void eval(std::size_t n, const UIntType *u, RealType *r)
    {
        for (std::size_t i = 0; i != n; ++i)
            r[i] = eval(u[i]);
    }
}; // class U01GenericImpl

template <typename UIntType, typename RealType, int Q>
class U01CanonicalGenericImpl
{
  public:
    static RealType eval(const UIntType *u)
    {
        return eval<0>(u, std::true_type());
    }

    static void eval(std::size_t n, const UIntType *u, RealType *r)
    {
        for (std::size_t i = 0; i != n; ++i, u += Q)
            r[i] = eval(u);
    }

  private:
    template <std::size_t>
    static RealType eval(const UIntType *, std::false_type)
    {
        return 0;
    }

    template <std::size_t N>
    static RealType eval(const UIntType *u, std::true_type)
    {
        constexpr int W = std::numeric_limits<UIntType>::digits;

        return static_cast<RealType>(u[N]) *
            Pow2<RealType, -static_cast<int>((Q - N) * W)>::value +
            eval<N + 1>(u, std::integral_constant<bool, N + 1 < Q>());
    }
}; // class U01CanonicalGenericImpl

} // namespace mckl::internal

} // namespace mckl;

#endif // MCKL_RANDOM_INTERNAL_U01_GENERIC_HPP
