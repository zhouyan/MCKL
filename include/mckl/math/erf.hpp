//============================================================================
// MCKL/include/mckl/math/erf.hpp
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

#ifndef MCKL_MATH_ERF_HPP
#define MCKL_MATH_ERF_HPP

#include <mckl/internal/basic.hpp>
#include <mckl/math/constants.hpp>

namespace mckl
{

namespace internal
{

template <typename RealType>
inline RealType erfcinv_refine(RealType x, RealType z)
{
    static constexpr RealType a = static_cast<RealType>(1.12837916709551257);

    RealType err = std::erfc(x) - z;

    return x + err / (a * std::exp(-(x * x)) - x * err);
}

} // namespace mckl::internal

/// \brief Inverse complementary error function \f$\mathrm{erfc}^{-1}(y)\f$
/// \ingroup Special
template <typename RealType>
inline RealType erfcinv(RealType y)
{
    if (y >= 2)
        return -const_inf<RealType>();
    if (y <= 0)
        return const_inf<RealType>();

    static constexpr RealType a = static_cast<RealType>(0.70771);
    static constexpr RealType b = static_cast<RealType>(2.30753);
    static constexpr RealType c = static_cast<RealType>(0.27061);
    static constexpr RealType d = static_cast<RealType>(0.99229);
    static constexpr RealType e = static_cast<RealType>(0.04481);

    RealType z = y < 1 ? y : 2 - y;
    RealType t = std::sqrt(-2 * std::log(0.5 * z));
    RealType x = -a * ((b + t * c) / (1 + t * (d + t * e)) - t);
    x = internal::erfcinv_refine(x, z);
    x = internal::erfcinv_refine(x, z);

    return y < 1 ? x : -x;
}

/// \brief Inverse error function \f$\mathrm{erf}^{-1}(y)\f$
/// \ingroup Special
template <typename RealType>
inline RealType erfinv(RealType y)
{
    return erfcinv(1 - y);
}

} // namespace mckl

#endif // MCKL_MATH_ERF_HPP
