//============================================================================
// MCKL/include/mckl/random/internal/increment_generic.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_INCREMENT_GENERIC_HPP
#define MCKL_RANDOM_INTERNAL_INCREMENT_GENERIC_HPP

#include <mckl/random/internal/common.hpp>

namespace mckl
{

namespace internal
{

template <std::size_t, typename T, std::size_t K>
inline void increment1(std::array<T, K> &, std::false_type)
{
}

template <std::size_t N, typename T, std::size_t K>
inline void increment1(std::array<T, K> &ctr, std::true_type)
{
    if (++std::get<N>(ctr))
        return;

    increment1<N + 1>(ctr, std::integral_constant<bool, N + 1 < K>());
}

template <typename T, std::size_t K>
class Increment
{
  public:
    static void eval(std::array<T, K> &ctr)
    {
        internal::increment1<0>(ctr, std::true_type());
    }
}; // class Increment

template <typename T>
class Increment<T, 1>
{
  public:
    static void eval(std::array<T, 1> &ctr) { ++std::get<0>(ctr); }
}; // class Increment

template <typename T>
class Increment<T, 2>
{
  public:
    static void eval(std::array<T, 2> &ctr)
    {
        if (++std::get<0>(ctr))
            return;
        ++std::get<1>(ctr);
    }
}; // class Increment

template <typename T>
class Increment<T, 4>
{
  public:
    static void eval(std::array<T, 4> &ctr)
    {
        if (++std::get<0>(ctr))
            return;
        if (++std::get<1>(ctr))
            return;
        if (++std::get<2>(ctr))
            return;
        ++std::get<3>(ctr);
    }
}; // class Increment

template <typename T>
class Increment<T, 8>
{
  public:
    static void eval(std::array<T, 8> &ctr)
    {
        if (++std::get<0>(ctr))
            return;
        if (++std::get<1>(ctr))
            return;
        if (++std::get<2>(ctr))
            return;
        if (++std::get<3>(ctr))
            return;
        if (++std::get<4>(ctr))
            return;
        if (++std::get<5>(ctr))
            return;
        if (++std::get<6>(ctr))
            return;
        ++std::get<7>(ctr);
    }
}; // class Increment

template <typename T>
class Increment<T, 16>
{
  public:
    static void eval(std::array<T, 16> &ctr)
    {
        if (++std::get<0x0>(ctr))
            return;
        if (++std::get<0x1>(ctr))
            return;
        if (++std::get<0x2>(ctr))
            return;
        if (++std::get<0x3>(ctr))
            return;
        if (++std::get<0x4>(ctr))
            return;
        if (++std::get<0x5>(ctr))
            return;
        if (++std::get<0x6>(ctr))
            return;
        if (++std::get<0x7>(ctr))
            return;
        if (++std::get<0x8>(ctr))
            return;
        if (++std::get<0x9>(ctr))
            return;
        if (++std::get<0xA>(ctr))
            return;
        if (++std::get<0xB>(ctr))
            return;
        if (++std::get<0xC>(ctr))
            return;
        if (++std::get<0xD>(ctr))
            return;
        if (++std::get<0xE>(ctr))
            return;
        ++std::get<0xF>(ctr);
    }
}; // class Increment

} // namespace mckl::internal

/// \brief Increment a counter by given steps
/// \ingroup Random
template <typename T, std::size_t K, T NSkip>
inline void increment(std::array<T, K> &ctr, std::integral_constant<T, NSkip>)
{
    if (std::get<0>(ctr) < std::numeric_limits<T>::max() - NSkip) {
        std::get<0>(ctr) += NSkip;
    } else {
        std::get<0>(ctr) += NSkip;
        internal::increment1<1>(ctr, std::integral_constant<bool, 1 < K>());
    }
}

/// \brief Increment a counter by given steps
/// \ingroup Random
template <typename T, std::size_t K>
inline void increment(std::array<T, K> &ctr, T nskip)
{
    if (std::get<0>(ctr) < std::numeric_limits<T>::max() - nskip) {
        std::get<0>(ctr) += nskip;
    } else {
        std::get<0>(ctr) += nskip;
        internal::increment1<1>(ctr, std::integral_constant<bool, 1 < K>());
    }
}

namespace internal
{

template <typename T, std::size_t K, std::size_t Blocks>
class IncrementBlock
{
  public:
    static void eval(
        std::array<T, K> &ctr, std::array<std::array<T, K>, Blocks> &ctr_block)
    {
        eval<0>(ctr, ctr_block, std::integral_constant<bool, 0 < Blocks>());
    }

  private:
    template <std::size_t>
    static void eval(std::array<T, K> &,
        std::array<std::array<T, K>, Blocks> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<T, K> &ctr,
        std::array<std::array<T, K>, Blocks> &ctr_block, std::true_type)
    {
        Increment<T, K>::eval(ctr);
        std::get<N>(ctr_block) = ctr;
        eval<N + 1>(
            ctr, ctr_block, std::integral_constant<bool, N + 1 < Blocks>());
    }
}; // class IncrementBlock

template <typename T, std::size_t K>
class IncrementBlock<T, K, 1>
{
  public:
    static void eval(
        std::array<T, K> &ctr, std::array<std::array<T, K>, 1> &ctr_block)
    {
        Increment<T, K>::eval(ctr);
        std::get<0>(ctr_block) = ctr;
    }
}; // class IncrementBlock

template <typename T, std::size_t K>
class IncrementBlock<T, K, 2>
{
  public:
    static void eval(
        std::array<T, K> &ctr, std::array<std::array<T, K>, 2> &ctr_block)
    {
        std::get<0>(ctr_block) = ctr;
        std::get<1>(ctr_block) = ctr;
        increment(std::get<0>(ctr_block), std::integral_constant<T, 1>());
        increment(std::get<1>(ctr_block), std::integral_constant<T, 2>());
        ctr = std::get<1>(ctr_block);
    }
}; // class IncrementBlock

template <typename T, std::size_t K>
class IncrementBlock<T, K, 4>
{
  public:
    static void eval(
        std::array<T, K> &ctr, std::array<std::array<T, K>, 4> &ctr_block)
    {
        std::get<0>(ctr_block) = ctr;
        std::get<1>(ctr_block) = ctr;
        std::get<2>(ctr_block) = ctr;
        std::get<3>(ctr_block) = ctr;
        increment(std::get<0>(ctr_block), std::integral_constant<T, 1>());
        increment(std::get<1>(ctr_block), std::integral_constant<T, 2>());
        increment(std::get<2>(ctr_block), std::integral_constant<T, 3>());
        increment(std::get<3>(ctr_block), std::integral_constant<T, 4>());
        ctr = std::get<3>(ctr_block);
    }
}; // class IncrementBlock

template <typename T, std::size_t K>
class IncrementBlock<T, K, 8>
{
  public:
    static void eval(
        std::array<T, K> &ctr, std::array<std::array<T, K>, 8> &ctr_block)
    {
        std::get<0>(ctr_block) = ctr;
        std::get<1>(ctr_block) = ctr;
        std::get<2>(ctr_block) = ctr;
        std::get<3>(ctr_block) = ctr;
        std::get<4>(ctr_block) = ctr;
        std::get<5>(ctr_block) = ctr;
        std::get<6>(ctr_block) = ctr;
        std::get<7>(ctr_block) = ctr;
        increment(std::get<0>(ctr_block), std::integral_constant<T, 1>());
        increment(std::get<1>(ctr_block), std::integral_constant<T, 2>());
        increment(std::get<2>(ctr_block), std::integral_constant<T, 3>());
        increment(std::get<3>(ctr_block), std::integral_constant<T, 4>());
        increment(std::get<4>(ctr_block), std::integral_constant<T, 5>());
        increment(std::get<5>(ctr_block), std::integral_constant<T, 6>());
        increment(std::get<6>(ctr_block), std::integral_constant<T, 7>());
        increment(std::get<7>(ctr_block), std::integral_constant<T, 8>());
        ctr = std::get<7>(ctr_block);
    }
}; // class IncrementBlock

template <typename T, std::size_t K>
class IncrementBlock<T, K, 16>
{
  public:
    static void eval(
        std::array<T, K> &ctr, std::array<std::array<T, K>, 16> &ctr_block)
    {
        std::get<0x0>(ctr_block) = ctr;
        std::get<0x1>(ctr_block) = ctr;
        std::get<0x2>(ctr_block) = ctr;
        std::get<0x3>(ctr_block) = ctr;
        std::get<0x4>(ctr_block) = ctr;
        std::get<0x5>(ctr_block) = ctr;
        std::get<0x6>(ctr_block) = ctr;
        std::get<0x7>(ctr_block) = ctr;
        std::get<0x8>(ctr_block) = ctr;
        std::get<0x9>(ctr_block) = ctr;
        std::get<0xA>(ctr_block) = ctr;
        std::get<0xB>(ctr_block) = ctr;
        std::get<0xC>(ctr_block) = ctr;
        std::get<0xD>(ctr_block) = ctr;
        std::get<0xE>(ctr_block) = ctr;
        std::get<0xF>(ctr_block) = ctr;
        increment(std::get<0x0>(ctr_block), std::integral_constant<T, 0x01>());
        increment(std::get<0x1>(ctr_block), std::integral_constant<T, 0x02>());
        increment(std::get<0x2>(ctr_block), std::integral_constant<T, 0x03>());
        increment(std::get<0x3>(ctr_block), std::integral_constant<T, 0x04>());
        increment(std::get<0x4>(ctr_block), std::integral_constant<T, 0x05>());
        increment(std::get<0x5>(ctr_block), std::integral_constant<T, 0x06>());
        increment(std::get<0x6>(ctr_block), std::integral_constant<T, 0x07>());
        increment(std::get<0x7>(ctr_block), std::integral_constant<T, 0x08>());
        increment(std::get<0x8>(ctr_block), std::integral_constant<T, 0x09>());
        increment(std::get<0x9>(ctr_block), std::integral_constant<T, 0x0A>());
        increment(std::get<0xA>(ctr_block), std::integral_constant<T, 0x0B>());
        increment(std::get<0xB>(ctr_block), std::integral_constant<T, 0x0C>());
        increment(std::get<0xC>(ctr_block), std::integral_constant<T, 0x0D>());
        increment(std::get<0xD>(ctr_block), std::integral_constant<T, 0x0E>());
        increment(std::get<0xE>(ctr_block), std::integral_constant<T, 0x0F>());
        increment(std::get<0xF>(ctr_block), std::integral_constant<T, 0x10>());
        ctr = std::get<0xF>(ctr_block);
    }
}; // class IncrementBlock

} // namespace mckl::internal

/// \brief Increment a counter by one
/// \ingroup Random
template <typename T, std::size_t K>
MCKL_INLINE inline void increment(std::array<T, K> &ctr)
{
    internal::Increment<T, K>::eval(ctr);
}

/// \brief Increment a counter by a given steps, and store each step in an
/// array of counters
/// \ingroup Random
template <typename T, std::size_t K, std::size_t Blocks>
inline void increment(
    std::array<T, K> &ctr, std::array<std::array<T, K>, Blocks> &ctr_block)
{
    internal::IncrementBlock<T, K, Blocks>::eval(ctr, ctr_block);
}

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_INCREMENT_GENERIC_HPP
