//============================================================================
// MCKL/include/mckl/random/internal/increment_generic.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_INCREMENT_GENERIC_HPP
#define MCKL_RANDOM_INTERNAL_INCREMENT_GENERIC_HPP

#include <mckl/random/internal/common.hpp>

namespace mckl
{

namespace internal
{

template <std::size_t, typename T, std::size_t K>
MCKL_FLATTEN inline void increment_single(std::array<T, K> &, std::false_type)
{
}

template <std::size_t N, typename T, std::size_t K>
MCKL_FLATTEN inline void increment_single(
    std::array<T, K> &ctr, std::true_type)
{
    if (++std::get<N>(ctr) != 0)
        return;

    increment_single<N + 1>(ctr, std::integral_constant<bool, N + 1 < K>());
}

} // namespace mckl::internal

/// \brief Increment a counter by one
/// \ingroup Random
template <typename T, std::size_t K>
MCKL_FLATTEN inline void increment(std::array<T, K> &ctr)
{
    internal::increment_single<0>(ctr, std::true_type());
}

/// \brief Increment a counter by given steps
/// \ingroup Random
template <typename T, std::size_t K, T NSkip>
MCKL_FLATTEN inline void increment(
    std::array<T, K> &ctr, std::integral_constant<T, NSkip>)
{
    if (ctr.front() < std::numeric_limits<T>::max() - NSkip) {
        ctr.front() += NSkip;
    } else {
        ctr.front() += NSkip;
        internal::increment_single<1>(
            ctr, std::integral_constant<bool, 1 < K>());
    }
}

/// \brief Increment a counter by given steps
/// \ingroup Random
template <typename T, std::size_t K>
MCKL_FLATTEN inline void increment(std::array<T, K> &ctr, T nskip)
{
    if (ctr.front() < std::numeric_limits<T>::max() - nskip) {
        ctr.front() += nskip;
    } else {
        ctr.front() += nskip;
        internal::increment_single<1>(
            ctr, std::integral_constant<bool, 1 < K>());
    }
}

namespace internal
{

template <std::size_t, typename T, std::size_t K, std::size_t Blocks>
MCKL_FLATTEN inline void increment_block_set(const std::array<T, K> &,
    std::array<std::array<T, K>, Blocks> &, std::false_type)
{
}

template <std::size_t B, typename T, std::size_t K, std::size_t Blocks>
MCKL_FLATTEN inline void increment_block_set(const std::array<T, K> &ctr,
    std::array<std::array<T, K>, Blocks> &ctr_block, std::true_type)
{
    std::get<B>(ctr_block) = ctr;
    increment_block_set<B + 1>(
        ctr, ctr_block, std::integral_constant<bool, B + 1 < Blocks>());
}

template <std::size_t, typename T, std::size_t K, std::size_t Blocks>
MCKL_FLATTEN inline void increment_block_add(
    std::array<std::array<T, K>, Blocks> &, std::false_type)
{
}

template <std::size_t B, typename T, std::size_t K, std::size_t Blocks>
MCKL_FLATTEN inline void increment_block_add(
    std::array<std::array<T, K>, Blocks> &ctr_block, std::true_type)
{
    increment(std::get<B>(ctr_block), std::integral_constant<T, B + 1>());
    increment_block_add<B + 1>(
        ctr_block, std::integral_constant<bool, B + 1 < Blocks>());
}

template <std::size_t, typename T, std::size_t K, std::size_t Blocks>
MCKL_FLATTEN inline void increment_block_add_safe(
    std::array<std::array<T, K>, Blocks> &, std::false_type)
{
}

template <std::size_t B, typename T, std::size_t K, std::size_t Blocks>
MCKL_FLATTEN inline void increment_block_add_safe(
    std::array<std::array<T, K>, Blocks> &ctr_block, std::true_type)
{
    std::get<B>(ctr_block).front() += B + 1;
    increment_block_add_safe<B + 1>(
        ctr_block, std::integral_constant<bool, B + 1 < Blocks>());
}

template <typename T, std::size_t K, std::size_t Blocks>
MCKL_FLATTEN inline void increment_block(const std::array<T, K> &ctr,
    std::array<std::array<T, K>, Blocks> &ctr_block)
{
    increment_block_set<0>(
        ctr, ctr_block, std::integral_constant<bool, 0 < Blocks>());
    increment_block_add<0>(
        ctr_block, std::integral_constant<bool, 0 < Blocks>());
}

template <typename T, std::size_t K, std::size_t Blocks,
    int = std::numeric_limits<T>::digits>
class IncrementBlock
{
    public:
    static constexpr bool aligned(void *) { return true; }

    MCKL_FLATTEN static void eval(const std::array<T, K> &ctr,
        std::array<std::array<T, K>, Blocks> &ctr_block)
    {
        increment_block_set<0>(
            ctr, ctr_block, std::integral_constant<bool, 0 < Blocks>());
        increment_block_add_safe<0>(
            ctr_block, std::integral_constant<bool, 0 < Blocks>());
    }
}; // class IncrementBlock

} // namespace mckl::internal

/// \brief Increment a counter by a given steps, and store each step in an
/// array of counters
/// \ingroup Random
template <typename T, std::size_t K, std::size_t Blocks>
MCKL_FLATTEN inline void increment(
    std::array<T, K> &ctr, std::array<std::array<T, K>, Blocks> &ctr_block)
{
    const bool safe =
        internal::IncrementBlock<T, K, Blocks>::aligned(ctr_block.data()) &&
        ctr.front() < std::numeric_limits<T>::max() - static_cast<T>(Blocks);
    safe ? internal::IncrementBlock<T, K, Blocks>::eval(ctr, ctr_block) :
           internal::increment_block(ctr, ctr_block);
    ctr = ctr_block.back();
}

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_INCREMENT_GENERIC_HPP
