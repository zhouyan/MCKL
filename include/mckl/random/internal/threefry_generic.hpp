//============================================================================
// MCKL/include/mckl/random/internal/threefry_generic.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_common.hpp>
#include <mckl/random/internal/threefry_constants.hpp>
#include <mckl/random/internal/threefry_generic_16x64.hpp>
#include <mckl/random/internal/threefry_generic_2x32.hpp>
#include <mckl/random/internal/threefry_generic_2x64.hpp>
#include <mckl/random/internal/threefry_generic_4x32.hpp>
#include <mckl/random/internal/threefry_generic_4x64.hpp>
#include <mckl/random/internal/threefry_generic_8x64.hpp>
#include <mckl/random/internal/threefry_unroll.hpp>
#include <mckl/random/increment.hpp>

namespace mckl {

namespace internal {

template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    int = std::numeric_limits<T>::digits>
class ThreefryGeneratorGenericImpl
{
  public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        alignas(MCKL_ALIGNMENT) union {
            std::array<T, K> s;
            std::array<char, sizeof(T) * K> r;
        } buf;

        std::memcpy(buf.s.data(), plain, sizeof(T) * K);
        union_le<char>(buf.s);
        MCKL_RANDOM_INTERNAL_THREEFRY_UNROLL_ROUND(0, buf.s, par);
        union_le<T>(buf.r);
        std::memcpy(cipher, buf.s.data(), sizeof(T) * K);
    }

    template <typename ResultType>
    static void eval(
        Counter<T, K> &ctr, ResultType *r, const std::array<T, K + 4> &par)
    {
        alignas(MCKL_ALIGNMENT) union {
            std::array<T, K> s;
            Counter<T, K> c;
            std::array<ResultType, sizeof(T) * K / sizeof(ResultType)> r;
        } buf;

        MCKL_INLINE_CALL increment(ctr);
        buf.c = ctr;
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        union_le<typename Counter<T, K>::value_type>(buf.s);
#endif
        MCKL_RANDOM_INTERNAL_THREEFRY_UNROLL_ROUND(0, buf.s, par);
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        union_le<T>(buf.r);
#endif
        std::memcpy(r, buf.r.data(), sizeof(T) * K);
    }

    template <typename ResultType>
    static void eval(Counter<T, K> &ctr, std::size_t n, ResultType *r,
        const std::array<T, K + 4> &par)
    {
        constexpr std::size_t rstride = sizeof(T) * K / sizeof(ResultType);

        for (std::size_t i = 0; i != n; ++i, r += rstride) {
            eval(ctr, r, par);
        }
    }

  private:
    template <std::size_t>
    static void round(
        std::array<T, K> &, const std::array<T, K + 4> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void round(
        std::array<T, K> &s, const std::array<T, K + 4> &par, std::true_type)
    {
        MCKL_RANDOM_INTERNAL_THREEFRY_UNROLL_ROUND(N, s, par);
    }

    template <std::size_t N>
    MCKL_INLINE static void kbox(
        std::array<T, K> &s, const std::array<T, K + 4> &par)
    {
        kbox<N>(s, par,
            std::integral_constant<bool, (N % 4 == 0 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void kbox(
        std::array<T, K> &, const std::array<T, K + 4> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void kbox(
        std::array<T, K> &s, const std::array<T, K + 4> &par, std::true_type)
    {
        ThreefryKBox<T, K, N>::eval(s, par);
    }

    template <std::size_t N>
    MCKL_INLINE static void rbox(std::array<T, K> &s)
    {
        rbox<N>(s, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void rbox(std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void rbox(std::array<T, K> &s, std::true_type)
    {
        ThreefryRBox<T, K, N, Constants>::eval(s);
    }
}; // class ThreefryGeneratorGenericImpl

template <typename T>
class ThreefryGeneratorGenericImpl<T, 2, 20, ThreefryConstants<T, 2>, 32>
    : public Threefry2x32GeneratorGenericImpl<T>
{
}; // class ThreefryGeneratorGenericImpl

template <typename T>
class ThreefryGeneratorGenericImpl<T, 4, 20, ThreefryConstants<T, 4>, 32>
    : public Threefry4x32GeneratorGenericImpl<T>
{
}; // class ThreefryGeneratorGenericImpl

template <typename T>
class ThreefryGeneratorGenericImpl<T, 2, 20, ThreefryConstants<T, 2>, 64>
    : public Threefry2x64GeneratorGenericImpl<T>
{
}; // class ThreefryGeneratorGenericImpl

template <typename T>
class ThreefryGeneratorGenericImpl<T, 4, 20, ThreefryConstants<T, 4>, 64>
    : public Threefry4x64GeneratorGenericImpl<T>
{
}; // class ThreefryGeneratorGenericImpl

template <typename T>
class ThreefryGeneratorGenericImpl<T, 4, 72, ThreefryConstants<T, 4>, 64>
    : public Threefish256GeneratorGenericImpl<T>
{
}; // class ThreefryGeneratorGenericImpl

template <typename T>
class ThreefryGeneratorGenericImpl<T, 8, 20, ThreefryConstants<T, 8>, 64>
    : public Threefry8x64GeneratorGenericImpl<T>
{
}; // class ThreefryGeneratorGenericImpl

template <typename T>
class ThreefryGeneratorGenericImpl<T, 8, 72, ThreefryConstants<T, 8>, 64>
    : public Threefish512GeneratorGenericImpl<T>
{
}; // class ThreefryGeneratorGenericImpl

template <typename T>
class ThreefryGeneratorGenericImpl<T, 16, 20, ThreefryConstants<T, 16>, 64>
    : public Threefry16x64GeneratorGenericImpl<T>
{
}; // class ThreefryGeneratorGenericImpl

template <typename T>
class ThreefryGeneratorGenericImpl<T, 16, 80, ThreefryConstants<T, 16>, 64>
    : public Threefish1024GeneratorGenericImpl<T>
{
}; // class ThreefryGeneratorGenericImpl

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_HPP
