//============================================================================
// MCKL/include/mckl/random/internal/philox_constants.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_CONSTANTS_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_CONSTANTS_HPP

#include <mckl/random/internal/common.hpp>

namespace mckl
{

namespace internal
{

template <typename T, std::size_t, int = std::numeric_limits<T>::digits>
class PhiloxConstantsWeyl;

template <typename T>
class PhiloxConstantsWeyl<T, 2, 32>
{
  public:
    static constexpr T value[1] = {0x9E3779B9};
}; // class PhiloxConstantsWeyl

template <typename T>
class PhiloxConstantsWeyl<T, 4, 32>
{
  public:
    static constexpr T value[2] = {0x9E3779B9, 0xBB67AE85};
}; // class PhiloxConstantsWeyl

template <typename T>
class PhiloxConstantsWeyl<T, 2, 64>
{
  public:
    static constexpr T value[1] = {0x9E3779B97F4A7C15};
}; // class PhiloxConstantsWeyl

template <typename T>
class PhiloxConstantsWeyl<T, 4, 64>
{
  public:
    static constexpr T value[2] = {0x9E3779B97F4A7C15, 0xBB67AE8584CAA73B};
}; // class PhiloxConstantsWeyl

template <typename T, std::size_t, int = std::numeric_limits<T>::digits>
class PhiloxConstantsMultiplier;

template <typename T>
class PhiloxConstantsMultiplier<T, 2, 32>
{
  public:
    static constexpr T value[1] = {0xD256D193};
}; // class PhiloxConstantsMultiplier

template <typename T>
class PhiloxConstantsMultiplier<T, 4, 32>
{
  public:
    static constexpr T value[2] = {0xCD9E8D57, 0xD2511F53};
}; // class PhiloxConstantsMultiplier

template <typename T>
class PhiloxConstantsMultiplier<T, 2, 64>
{
  public:
    static constexpr T value[1] = {0xD2B74407B1CE6E93};
}; // class PhiloxConstantsMultiplier

template <typename T>
class PhiloxConstantsMultiplier<T, 4, 64>
{
  public:
    static constexpr T value[2] = {0xCA5A826395121157, 0xD2E7470EE14C6C93};
}; // class PhiloxConstantsMultiplier

} // namespace mckl::internal

/// \brief Default Philox constants
/// \ingroup Philox
template <typename T, std::size_t K>
class PhiloxConstants
{
  public:
    using weyl = internal::PhiloxConstantsWeyl<T, K>;
    using multiplier = internal::PhiloxConstantsMultiplier<T, K>;
}; // class PhiloxConstants

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_PHILOX_CONSTANTS_HPP
