//============================================================================
// MCKL/include/mckl/random/increment.hpp
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

#ifndef MCKL_RANDOM_INCREMENT_HPP
#define MCKL_RANDOM_INCREMENT_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/increment_generic.hpp>

#if MCKL_USE_AVX2
#include <mckl/random/internal/increment_avx2_64.hpp>
#elif MCKL_USE_SSE2
#include <mckl/random/internal/increment_sse2_64.hpp>
#endif

namespace mckl
{

namespace internal
{

template <typename T, std::size_t K,
    bool = (sizeof(T) * K) % sizeof(std::uint8_t) == 0,
    bool = (sizeof(T) * K) % sizeof(std::uint16_t) == 0,
    bool = (sizeof(T) * K) % sizeof(std::uint32_t) == 0,
    bool = (sizeof(T) * K) % sizeof(std::uint64_t) == 0>
class CounterImpl
{
    public:
    using type = std::array<T, K>;
}; // class CounterImpl

template <typename T, std::size_t K>
class CounterImpl<T, K, true, false, false, false>
{
    public:
    using type =
        std::array<std::uint8_t, sizeof(T) * K / sizeof(std::uint8_t)>;
}; // class CounterImpl

template <typename T, std::size_t K>
class CounterImpl<T, K, true, true, false, false>
{
    public:
    using type =
        std::array<std::uint16_t, sizeof(T) * K / sizeof(std::uint16_t)>;
}; // class CounterImpl

template <typename T, std::size_t K>
class CounterImpl<T, K, true, true, true, false>
{
    public:
    using type =
        std::array<std::uint32_t, sizeof(T) * K / sizeof(std::uint32_t)>;
}; // class CounterImpl

template <typename T, std::size_t K>
class CounterImpl<T, K, true, true, true, true>
{
    public:
    using type =
        std::array<std::uint64_t, sizeof(T) * K / sizeof(std::uint64_t)>;
}; // class CounterImpl

} // namespace internal

/// \brief A counter type with the same width as `std::array<T, K>` but with
/// possibly fewer elements
///
/// \ingroup Random
template <typename T, std::size_t K>
using Counter = typename internal::CounterImpl<T, K>::type;

} // namespace mckl

#endif // MCKL_RANDOM_INCREMENT_HPP
