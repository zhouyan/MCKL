//============================================================================
// MCKL/include/mckl/internal/traits.hpp
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

#ifndef MCKL_INTERNAL_TRAITS_HPP
#define MCKL_INTERNAL_TRAITS_HPP

#include <mckl/internal/config.h>
#include <mckl/internal/defines.hpp>
#include <type_traits>

#define MCKL_DEFINE_TYPE_DISPATCH_TRAIT(Outer, Inner, Default)                \
    template <typename T>                                                     \
    class Outer##Trait;                                                       \
                                                                              \
    namespace internal                                                        \
    {                                                                         \
                                                                              \
    template <typename T>                                                     \
    class Has##Outer##Impl                                                    \
    {                                                                         \
        class char2                                                           \
        {                                                                     \
            char c1;                                                          \
            char c2;                                                          \
        };                                                                    \
                                                                              \
        template <typename U>                                                 \
        static char test(typename U::Inner *);                                \
                                                                              \
        template <typename U>                                                 \
        static char2 test(...);                                               \
                                                                              \
        public:                                                               \
        static constexpr bool value =                                         \
            sizeof(test<T>(nullptr)) == sizeof(char);                         \
    };                                                                        \
                                                                              \
    template <typename T>                                                     \
    class Has##Outer                                                          \
        : public std::integral_constant<bool, Has##Outer##Impl<T>::value>     \
    {                                                                         \
    };                                                                        \
                                                                              \
    template <typename T, bool>                                               \
    class Outer##Dispatch;                                                    \
                                                                              \
    template <typename T>                                                     \
    class Outer##Dispatch<T, false>                                           \
    {                                                                         \
        public:                                                               \
        using type = Default;                                                 \
    };                                                                        \
                                                                              \
    template <typename T>                                                     \
    class Outer##Dispatch<T, true>                                            \
    {                                                                         \
        public:                                                               \
        using type = typename T::Inner;                                       \
    };                                                                        \
                                                                              \
    } /* namespace interanl */                                                \
                                                                              \
    template <typename T>                                                     \
    class Outer##Trait                                                        \
    {                                                                         \
        public:                                                               \
        static constexpr bool value = internal::Has##Outer<T>::value;         \
        using type = typename internal::Outer##Dispatch<T, value>::type;      \
    };                                                                        \
                                                                              \
    template <typename T>                                                     \
    using Outer = typename Outer##Trait<T>::type;

#define MCKL_DEFINE_TYPE_TEMPLATE_DISPATCH_TRAIT(Outer, Inner, Default)       \
    template <typename T>                                                     \
    class Outer##Trait;                                                       \
                                                                              \
    namespace internal                                                        \
    {                                                                         \
                                                                              \
    template <typename T>                                                     \
    class Has##Outer##Impl                                                    \
    {                                                                         \
        class char2                                                           \
        {                                                                     \
            char c1;                                                          \
            char c2;                                                          \
        };                                                                    \
                                                                              \
        template <typename U>                                                 \
        static char test(typename U::template Inner<T> *);                    \
                                                                              \
        template <typename U>                                                 \
        static char2 test(...);                                               \
                                                                              \
        public:                                                               \
        static constexpr bool value =                                         \
            sizeof(test<T>(nullptr)) == sizeof(char);                         \
    };                                                                        \
                                                                              \
    template <typename T>                                                     \
    class Has##Outer                                                          \
        : public std::integral_constant<bool, Has##Outer##Impl<T>::value>     \
    {                                                                         \
    };                                                                        \
                                                                              \
    template <typename T, bool>                                               \
    class Outer##Dispatch;                                                    \
                                                                              \
    template <typename T>                                                     \
    class Outer##Dispatch<T, false>                                           \
    {                                                                         \
        public:                                                               \
        using type = Default<T>;                                              \
    };                                                                        \
                                                                              \
    template <typename T>                                                     \
    class Outer##Dispatch<T, true>                                            \
    {                                                                         \
        public:                                                               \
        using type = typename T::template Inner<T>;                           \
    };                                                                        \
    }                                                                         \
                                                                              \
    template <typename T>                                                     \
    class Outer##Trait                                                        \
    {                                                                         \
        public:                                                               \
        static constexpr bool value = internal::Has##Outer<T>::value;         \
        using type = typename internal::Outer##Dispatch<T, value>::type;      \
    };                                                                        \
                                                                              \
    template <typename T>                                                     \
    using Outer = typename Outer##Trait<T>::type;

namespace mckl
{

/// \brief Particle::size_type etc., traits
/// \ingroup Traits
MCKL_DEFINE_TYPE_DISPATCH_TRAIT(SizeType, size_type, std::size_t)

namespace internal
{

template <typename T, std::size_t K = 1>
class BufferSize
    : public std::integral_constant<std::size_t,
          8192 / (sizeof(T) * K) == 0 ? 1 : 8192 / (sizeof(T) * K)>
{
}; // class BufferSize;

template <typename T>
using is_blas_floating_point = std::integral_constant<bool,
    std::is_same<typename std::remove_cv<T>::type, float>::value ||
        std::is_same<typename std::remove_cv<T>::type, double>::value>;

template <typename T, typename RNGType,
    typename KeyType = typename RNGType::result_type>
using is_seed_seq = std::integral_constant<bool, std::is_class<T>::value &&
        !std::is_convertible<T, RNGType>::value &&
        !std::is_convertible<T, typename RNGType::result_type>::value &&
        !std::is_convertible<T, KeyType>::value>;

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_INTERNAL_TRAITS_HPP
