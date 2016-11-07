//============================================================================
// MCKL/include/mckl/random/internal/threefry_constants.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_CONSTANTS_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_CONSTANTS_HPP

#include <mckl/random/internal/common.hpp>

namespace mckl
{

namespace internal
{

template <typename T, int = std::numeric_limits<T>::digits>
class ThreefryConstantsParity;

template <typename T>
class ThreefryConstantsParity<T, 32>
{
    public:
    static constexpr T value = 0x1BD11BDA;
}; // class ThreefryConstantsParity

template <typename T>
class ThreefryConstantsParity<T, 64>
{
    public:
    static constexpr T value = 0x1BD11BDAA9FC1A22;
}; // class ThreefryConstantsParity

template <typename T, std::size_t, int = std::numeric_limits<T>::digits>
class ThreefryConstantsRotate;

template <typename T>
class ThreefryConstantsRotate<T, 2, 32>
{
    public:
    static constexpr int value[1][8] = {{13, 15, 26, 6, 17, 29, 16, 24}};
}; // class ThreefryConstantsRotate

template <typename T>
class ThreefryConstantsRotate<T, 4, 32>
{
    public:
    static constexpr int value[2][8] = {
        {10, 11, 13, 23, 6, 17, 25, 18}, {26, 21, 27, 5, 20, 11, 10, 20}};
}; // class ThreefryConstantsRotate

template <typename T>
class ThreefryConstantsRotate<T, 2, 64>
{
    public:
    static constexpr int value[1][8] = {{16, 42, 12, 31, 16, 32, 24, 21}};
}; // class ThreefryConstantsRotate

template <typename T>
class ThreefryConstantsRotate<T, 4, 64>
{
    public:
    static constexpr int value[2][8] = {
        {14, 52, 23, 5, 25, 46, 58, 32}, {16, 57, 40, 37, 33, 12, 22, 32}};
}; // class ThreefryConstantsRotate

template <typename T>
class ThreefryConstantsRotate<T, 8, 64>
{
    public:
    static constexpr int value[4][8] = {{46, 33, 17, 44, 39, 13, 25, 8},
        {36, 27, 49, 9, 30, 50, 29, 35}, {19, 14, 36, 54, 34, 10, 39, 56},
        {37, 42, 39, 56, 24, 17, 43, 22}};
}; // class ThreefryConstantsRotate

template <typename T>
class ThreefryConstantsRotate<T, 16, 64>
{
    public:
    static constexpr int value[8][8] = {{24, 38, 33, 5, 41, 16, 31, 9},
        {13, 19, 4, 20, 9, 34, 44, 48}, {8, 10, 51, 48, 37, 56, 47, 35},
        {47, 55, 13, 41, 31, 51, 46, 52}, {8, 49, 34, 47, 12, 4, 19, 23},
        {17, 18, 41, 28, 47, 53, 42, 31}, {22, 23, 59, 16, 44, 42, 44, 37},
        {37, 52, 17, 25, 30, 41, 25, 20}};
}; // class ThreefryConstantsRotate

template <std::size_t>
class ThreefryConstantsPermute;

template <>
class ThreefryConstantsPermute<2>
{
    public:
    static constexpr std::size_t value[2] = {0, 1};
}; // class ThreefryConstantsPermute

template <>
class ThreefryConstantsPermute<4>
{
    public:
    static constexpr std::size_t value[4] = {0, 3, 2, 1};
}; // class ThreefryConstantsPermute

template <>
class ThreefryConstantsPermute<8>
{
    public:
    static constexpr std::size_t value[8] = {2, 1, 4, 7, 6, 5, 0, 3};
}; // class ThreefryConstantsPermute

template <>
class ThreefryConstantsPermute<16>
{
    static constexpr std::size_t value[16] = {
        0, 9, 2, 13, 6, 11, 4, 15, 10, 7, 12, 3, 14, 5, 8, 1};
}; // class ThreefryConstantsPermute

} // namespace mckl::internal

/// \brief Default Threefry constants
/// \ingroup Threefry
template <typename T, std::size_t K>
class ThreefryConstants
{
    public:
    using parity = internal::ThreefryConstantsParity<T>;
    using rotate = internal::ThreefryConstantsRotate<T, K>;
    using permute = internal::ThreefryConstantsPermute<K>;
}; // class ThreefryConstants

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_CONSTANTS_HPP
