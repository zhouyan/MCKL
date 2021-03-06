//============================================================================
// MCKL/include/mckl/random/u01.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
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

#ifndef MCKL_RANDOM_U01_HPP
#define MCKL_RANDOM_U01_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/u01_generic.hpp>

#if MCKL_HAS_AVX2
#include <mckl/random/internal/u01_avx2.hpp>
#endif

#if MCKL_HAS_AVX512
#include <mckl/random/internal/u01_avx512.hpp>
#endif

namespace mckl {

namespace internal {

#if MCKL_USE_AVX512

template <typename UIntType, typename RealType, typename Lower, typename Upper>
using U01Impl = U01AVX512Impl<UIntType, RealType, Lower, Upper>;

template <typename UIntType, typename RealType, int M>
using U01CanonicalImpl = U01CanonicalAVX512Impl<UIntType, RealType, M>;

#elif MCKL_USE_AVX2

template <typename UIntType, typename RealType, typename Lower, typename Upper>
using U01Impl = U01AVX2Impl<UIntType, RealType, Lower, Upper>;

template <typename UIntType, typename RealType, int M>
using U01CanonicalImpl = U01CanonicalAVX2Impl<UIntType, RealType, M>;

#else // MCKL_USE_AVX2

template <typename UIntType, typename RealType, typename Lower, typename Upper>
using U01Impl = U01GenericImpl<UIntType, RealType, Lower, Upper>;

template <typename UIntType, typename RealType, int M>
using U01CanonicalImpl = U01CanonicalGenericImpl<UIntType, RealType, M>;

#endif // MCKL_USE_AVX2

} // namespace internal

/// \brief Convert uniform unsigned integers to floating points within [0, 1]
/// \ingroup U01
///
/// \details
/// Let \f$W\f$ be the number of digits of unsigned integer type `UIntType`.
/// Assuming the input is a uniform random number on the set
/// \f$\{0,1,\dots,2^W - 1\f$, the output is uniform over the interval
/// \f$[0,1]\f$ or one of its (half-)open interval variant. The exact output
/// depend on the template parameter `Lower` and `Upper`.
template <typename UIntType, typename RealType, typename Lower, typename Upper>
inline RealType u01(UIntType u)
{
    static_assert(std::is_unsigned<UIntType>::value,
        "**u01** used with UIntType other than unsigned integer "
        "types");

    static_assert(std::is_floating_point<RealType>::value,
        "**u01** used with RealType other than floating point "
        "types");

    return internal::U01Impl<UIntType, RealType, Lower, Upper>::eval(u);
}

/// \brief Convert uniform unsigned integers to floating points within [0, 1]
/// \ingroup U01
template <typename UIntType, typename RealType, typename Lower, typename Upper>
inline void u01(std::size_t n, const UIntType *u, RealType *r)
{
    static_assert(std::is_unsigned<UIntType>::value,
        "**u01** used with UIntType other than unsigned integer "
        "types");

    static_assert(std::is_floating_point<RealType>::value,
        "**u01** used with RealType other than floating point "
        "types");

    internal::U01Impl<UIntType, RealType, Lower, Upper>::eval(n, u, r);
}

/// \brief Convert uniform unsigned integers to floating points on [0, 1]
/// \ingroup U01
template <typename UIntType, typename RealType>
inline RealType u01_cc(UIntType u)
{
    return u01<UIntType, RealType, Closed, Closed>(u);
}

/// \brief Convert uniform unsigned integers to floating points on [0, 1)
/// \ingroup U01
template <typename UIntType, typename RealType>
inline RealType u01_co(UIntType u)
{
    return u01<UIntType, RealType, Closed, Open>(u);
}

/// \brief Convert uniform unsigned integers to floating points on (0, 1]
/// \ingroup U01
template <typename UIntType, typename RealType>
inline RealType u01_oc(UIntType u)
{
    return u01<UIntType, RealType, Open, Closed>(u);
}

/// \brief Convert uniform unsigned integers to floating points on (0, 1)
/// \ingroup U01
template <typename UIntType, typename RealType>
inline RealType u01_oo(UIntType u)
{
    return u01<UIntType, RealType, Open, Open>(u);
}

/// \brief Convert uniform unsigned integers to floating points on [0, 1]
/// \ingroup U01
template <typename UIntType, typename RealType>
inline void u01_cc(std::size_t n, const UIntType *u, RealType *r)
{
    u01<UIntType, RealType, Closed, Closed>(n, u, r);
}

/// \brief Convert uniform unsigned integers to floating points on [0, 1)
/// \ingroup U01
template <typename UIntType, typename RealType>
inline void u01_co(std::size_t n, const UIntType *u, RealType *r)
{
    u01<UIntType, RealType, Closed, Open>(n, u, r);
}

/// \brief Convert uniform unsigned integers to floating points on (0, 1]
/// \ingroup U01
template <typename UIntType, typename RealType>
inline void u01_oc(std::size_t n, const UIntType *u, RealType *r)
{
    u01<UIntType, RealType, Open, Closed>(n, u, r);
}

/// \brief Convert uniform unsigned integers to floating points on (0, 1)
/// \ingroup U01
template <typename UIntType, typename RealType>
inline void u01_oo(std::size_t n, const UIntType *u, RealType *r)
{
    u01<UIntType, RealType, Open, Open>(n, u, r);
}

/// \brief Convert uniform multiple unsigned integers to one floating points
/// within [0, 1]
/// \ingroup U01
template <typename UIntType, typename RealType, std::size_t M>
inline RealType u01_canonical(const UIntType *u)
{
    static_assert(std::is_unsigned<UIntType>::value,
        "**u01_canonical** used with UIntType other than unsigned integer "
        "types");

    static_assert(std::is_floating_point<RealType>::value,
        "**u01_canonical** used with RealType other than floating point "
        "types");

    return internal::U01CanonicalImpl<UIntType, RealType, M>::eval(u);
}

/// \brief Convert uniform multiple unsigned integers to one floating points
/// within [0, 1]
/// \ingroup U01
template <typename UIntType, typename RealType, std::size_t M>
inline void u01_canonical(std::size_t n, const UIntType *u, RealType *r)
{
    static_assert(std::is_unsigned<UIntType>::value,
        "**u01_canonical** used with UIntType other than unsigned integer "
        "types");

    static_assert(std::is_floating_point<RealType>::value,
        "**u01_canonical** used with RealType other than floating point "
        "types");

    internal::U01CanonicalImpl<UIntType, RealType, M>::eval(n, u, r);
}

} // namespace mckl

#endif //  MCKL_RANDOM_U01_HPP
