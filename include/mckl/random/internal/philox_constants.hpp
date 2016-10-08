//============================================================================
// MCKL/include/mckl/random/internal/philox_constants.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_COSNTANTS_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_COSNTANTS_HPP

namespace mckl
{

namespace internal
{

template <typename, std::size_t, int>
class PhiloxConstantsImpl;

template <typename T>
class PhiloxConstantsImpl<T, 2, 32>
{
    public:
    static constexpr T weyl[1] = {UINT32_C(0x9E3779B9)};

    static constexpr T multiplier[1] = {UINT32_C(0xD256D193)};
}; // class PhiloxConstantsImpl

template <typename T>
class PhiloxConstantsImpl<T, 4, 32>
{
    public:
    static constexpr T weyl[2] = {UINT32_C(0x9E3779B9), UINT32_C(0xBB67AE85)};

    static constexpr T multiplier[2] = {
        UINT32_C(0xCD9E8D57), UINT32_C(0xD2511F53)};
}; // class PhiloxConstantsImpl

template <typename T>
class PhiloxConstantsImpl<T, 2, 64>
{
    public:
    static constexpr T weyl[1] = {UINT64_C(0x9E3779B97F4A7C15)};

    static constexpr T multiplier[1] = {UINT64_C(0xD2B74407B1CE6E93)};
}; // class PhiloxConstantsImpl

template <typename T>
class PhiloxConstantsImpl<T, 4, 64>
{
    public:
    static constexpr T weyl[2] = {
        UINT64_C(0x9E3779B97F4A7C15), UINT64_C(0xBB67AE8584CAA73B)};

    static constexpr T multiplier[2] = {
        UINT64_C(0xCA5A826395121157), UINT64_C(0xD2E7470EE14C6C93)};
}; // class PhiloxConstantsImpl

} // namespace mckl::internal

/// \brief Default Philox constants
/// \ingroup Philox
template <typename T, std::size_t K>
using PhiloxConstants =
    internal::PhiloxConstantsImpl<T, K, std::numeric_limits<T>::digits>;

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_PHILOX_COSNTANTS_HPP
