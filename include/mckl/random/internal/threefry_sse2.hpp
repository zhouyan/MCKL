//============================================================================
// MCKL/include/mckl/random/internal/threefry_sse2.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_HPP

#include <mckl/random/internal/threefry_generic.hpp>
#include <mckl/random/internal/threefry_sse2_32.hpp>
#include <mckl/random/internal/threefry_sse2_64.hpp>

namespace mckl {

namespace internal {

template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    int = std::numeric_limits<T>::digits>
class ThreefryGeneratorSSE2Impl
    : public ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>
{
}; // class ThreefryGeneratorSSE2Impl

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class ThreefryGeneratorSSE2Impl<T, K, Rounds, Constants, 32>
    : public std::conditional_t<K != 0 && 16 % K == 0,
          ThreefryGeneratorSSE2Impl32<T, K, Rounds, Constants>,
          ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>>
{
}; // class ThreefryGeneratorImplSSE2Impl

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class ThreefryGeneratorSSE2Impl<T, K, Rounds, Constants, 64>
    : public std::conditional_t<K != 0 && 16 % K == 0,
          ThreefryGeneratorSSE2Impl64<T, K, Rounds, Constants>,
          ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>>
{
}; // class ThreefryGeneratorImplSSE2Impl

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_HPP
