//============================================================================
// MCKL/include/mckl/core/memory.hpp
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

#ifndef MCKL_CORE_MEMORY_HPP
#define MCKL_CORE_MEMORY_HPP

#include <mckl/internal/config.h>

#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>
#include <vector>

#if MCKL_HAS_POSIX
#include <stdlib.h>
#endif

#if MCKL_HAS_JEMALLOC
#include <jemalloc/jemalloc.h>
#endif

#if MCKL_HAS_TBB
#include <tbb/scalable_allocator.h>
#endif

/// \brief The default alignment for scalar type
/// \ingroup Config
#ifndef MCKL_ALIGNMENT
#define MCKL_ALIGNMENT 32
#endif

/// \brief The minimum alignment for any type
/// \ingroup Config
#ifndef MCKL_MINIMUM_ALIGNMENT
#define MCKL_MINIMUM_ALIGNMENT 16
#endif

#if MCKL_ALIGNMENT < MCKL_MINIMUM_ALIGNMENT
#undef MCKL_ALIGNEMNT
#define MCKL_ALIGNMENT MCKL_MINIMUM_ALIGNMENT
#endif

/// \brief Default allocation type
/// \ingroup Config
#ifndef MCKL_MEMORY_TYPE
#if MCKL_USE_JEMALLOC
#define MCKL_MEMORY_TYPE ::mckl::MemoryJEM
#elif MCKL_USE_TBB_MALLOC
#define MCKL_MEMORY_TYPE ::mckl::MemoryTBB
#elif MCKL_HAS_POSIX
#define MCKL_MEMORY_TYPE ::mckl::MemorySYS
#else
#define MCKL_MEMORY_TYPE ::mckl::MemorySTD
#endif
#endif

namespace mckl {

namespace internal {

template <std::size_t Alignment, typename UIntType>
inline constexpr std::size_t alignment_round0(UIntType n)
{
    static_assert(Alignment != 0 && (Alignment & (Alignment - 1)) == 0,
        "**alignment_round0** used with Alignment other than a power of two "
        "positive integer");

    static_assert(Alignment >= sizeof(void *),
        "**alignment_round0** used with Alignment less than sizeof(void *)");

    static_assert(std::is_unsigned<UIntType>::value,
        "alignment_round0** used with UIntType other than unsigned integer "
        "types");

    constexpr std::size_t addn = Alignment - 1;
    constexpr std::size_t mask = ~addn;

    return (n + addn) & mask;
}

template <std::size_t Alignment, typename UIntType>
inline constexpr std::size_t alignment_round(UIntType n)
{
    return n == 0 ? Alignment : alignment_round0<Alignment>(n);
}

} // namespace mckl::internal

/// \brief Alignment of types
/// \ingroup Core
///
/// \details
/// * For scalar types, defined to the maximum of `MCKL_ALIGNMENT` and
/// `alignof(T)`
/// * For all other types, define to the maximum of `MCKL_MINIMUM_ALIGNMENT`
/// and `alignof(T)` as `value`.
template <typename T>
constexpr std::size_t AlignOf = std::integral_constant<std::size_t,
    std::is_scalar<T>::value ?
        (alignof(T) > MCKL_ALIGNMENT ? alignof(T) : MCKL_ALIGNMENT) :
        (alignof(T) > MCKL_MINIMUM_ALIGNMENT ? alignof(T) :
                                               MCKL_MINIMUM_ALIGNMENT)>::value;

/// \brief Memory allocation using the standard library
/// \ingroup Core
///
/// \tparam Alignment The alignment of the allocated memory
template <std::size_t Alignment>
class MemorySTD
{
    static_assert(Alignment != 0 && (Alignment & (Alignment - 1)) == 0,
        "**MemorySTD** used with Alignment other than a power of two positive "
        "integer");

    static_assert(Alignment >= sizeof(void *),
        "**MemorySTD** used with Alignment less than sizeof(void *)");

  public:
    static constexpr std::size_t alignment() { return Alignment; }

    static void *allocate(std::size_t n, const void * = nullptr) noexcept
    {
        const std::size_t m = internal::alignment_round0<Alignment>(
            n + sizeof(std::uintptr_t) + Alignment);
        if (m < n) {
            return nullptr;
        }

        std::uintptr_t *const address =
            static_cast<std::uintptr_t *>(std::malloc(m));
        if (address == nullptr) {
            return nullptr;
        }

        std::uintptr_t *const ptr = reinterpret_cast<std::uintptr_t *>(
            internal::alignment_round0<Alignment>(
                reinterpret_cast<std::uintptr_t>(address + 1)));
        ptr[-1] = reinterpret_cast<std::uintptr_t>(address);

        return static_cast<void *>(ptr);
    }

    static void deallocate(void *ptr, std::size_t = 0) noexcept
    {
        if (ptr != nullptr) {
            const std::uintptr_t address =
                static_cast<std::uintptr_t *>(ptr)[-1];
            std::free(reinterpret_cast<void *>(address));
        }
    }
}; // class MemorySTD

#if MCKL_HAS_POSIX

/// \brief Memory allocation using native system functions
/// \ingroup Core
///
/// \tparam Alignment The alignment of the allocated memory
template <std::size_t Alignment>
class MemorySYS
{
    static_assert(Alignment != 0 && (Alignment & (Alignment - 1)) == 0,
        "**MemorySYS** used with Alignment other than a power of two positive "
        "integer");

    static_assert(Alignment >= sizeof(void *),
        "**MemorySYS** used with Alignment less than sizeof(void *)");

  public:
    static constexpr std::size_t alignment() { return Alignment; }

    static void *allocate(std::size_t n, const void * = nullptr) noexcept
    {
        const std::size_t m = internal::alignment_round<Alignment>(n);

        if (m < n) {
            return nullptr;
        }

        void *ptr = nullptr;
        if (::posix_memalign(&ptr, Alignment, m) != 0) {
            ptr = nullptr;
        }

        return ptr;
    }

    static void deallocate(void *ptr, std::size_t = 0) noexcept
    {
        if (ptr != nullptr) {
            ::free(ptr);
        }
    }
}; // class MemorySYS

#endif // MCKL_HAS_POSIX

#if MCKL_HAS_JEMALLOC

/// \brief Memory allocation using jemalloc
/// \ingroup Core
///
/// \tparam Alignment The alignment of the allocated memory
template <std::size_t Alignment>
class MemoryJEM
{
    static_assert(Alignment != 0 && (Alignment & (Alignment - 1)) == 0,
        "**MemoryJEM** used with Alignment other than a power of two positive "
        "integer");

    static_assert(Alignment >= sizeof(void *),
        "**MemoryJEM** used with Alignment less than sizeof(void *)");

  public:
    static constexpr std::size_t alignment() { return Alignment; }

    static void *allocate(std::size_t n, const void * = nullptr) noexcept
    {
        const std::size_t m = internal::alignment_round<Alignment>(n);

        return m < n ? nullptr : ::aligned_alloc(Alignment, m);
    }

    static void deallocate(void *ptr, std::size_t = 0) noexcept
    {
        if (ptr != nullptr) {
            ::free(ptr);
        }
    }
}; // class MemoryJEM

#endif // MCKL_HAS_JEMALLOC

#if MCKL_HAS_TBB

/// \brief Memory allocation using Intel TBB
/// \ingroup Core
///
/// \tparam Alignment The alignment of the allocated memory
template <std::size_t Alignment>
class MemoryTBB
{
    static_assert(Alignment != 0 && (Alignment & (Alignment - 1)) == 0,
        "**MemoryTBB** used with Alignment other than a power of two positive "
        "integer");

    static_assert(Alignment >= sizeof(void *),
        "**MemoryTBB** used with Alignment less than sizeof(void *)");

  public:
    static constexpr std::size_t alignment() { return Alignment; }

    static void *allocate(std::size_t n, const void * = nullptr) noexcept
    {
        const std::size_t m = internal::alignment_round<Alignment>(n);

        return m < n ? nullptr : scalable_aligned_malloc(m, Alignment);
    }

    static void deallocate(void *ptr, std::size_t = 0) noexcept
    {
        if (ptr != nullptr) {
            scalable_aligned_free(ptr);
        }
    }
}; // class MemoryTBB

#endif // MCKL_HAS_TBB

/// \brief Default memory allocation policy
/// \ingroup Core
///
/// \tparam Alignment The alignment of the allocated memory
///
/// \details
/// This template is aliased to the configuration macro  `MCKL_MEMORY_TYPE`.
/// The default precedence is as the following,
/// * MemoryJEM
/// * MemoryTBB
/// * MemorySYS
/// * MemorySTD
template <std::size_t Alignment>
using Memory = MCKL_MEMORY_TYPE<Alignment>;

/// \brief Standard library compatible allocator with policies
/// \ingroup Core
///
/// \tparam T The value type
/// \tparam Mem The memory allocation policy
///
/// \sa Allocator<void, Mem>
/// \sa Allocator<const void, Mem>
/// \sa [std::allocator](http://en.cppreference.com/w/cpp/memory/allocator)
template <typename T, typename Mem = Memory<AlignOf<T>>>
class Allocator : public std::allocator<T>
{
  public:
    template <typename U>
    class rebind
    {
      public:
        using other = Allocator<U, Mem>;
    };

    Allocator() = default;

    template <typename U>
    Allocator(const Allocator<U, Mem> &other) noexcept
        : std::allocator<T>(static_cast<std::allocator<U>>(other))
    {
    }

    T *allocate(std::size_t n, const void *hint = nullptr)
    {
        const std::size_t m = n * sizeof(T);
        if (m < n) {
            throw std::bad_alloc();
        }

        T *ptr = static_cast<T *>(Mem::allocate(m, hint));
        if (ptr == nullptr) {
            throw std::bad_alloc();
        }

        return ptr;
    }

    void deallocate(T *ptr, std::size_t size = 0) noexcept(
        noexcept(Mem::deallocate(ptr, size)))
    {
        Mem::deallocate(ptr, size);
    }

    template <typename U>
    void construct(U *p)
    {
        construct_dispatch(p, std::is_scalar<U>());
    }

  private:
    template <typename U>
    void construct_dispatch(U *, std::true_type)
    {
    }

    template <typename U>
    void construct_dispatch(U *p, std::false_type)
    {
        ::new (static_cast<void *>(p)) U();
    }
}; // class Allocator

template <typename Mem>
class Allocator<void, Mem>
{
  public:
    using value_type = void;
    using pointer = void *;
    using const_pointer = const void *;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    template <typename U>
    class rebind
    {
      public:
        using other = Allocator<U, Mem>;
    };
}; // class Allocator

template <typename Mem>
class Allocator<const void, Mem>
{
  public:
    using value_type = const void;
    using pointer = const void *;
    using const_pointer = const void *;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    template <typename U>
    class rebind
    {
      public:
        using other = Allocator<U, Mem>;
    };
}; // class Allocator

/// \relates Allocator
template <typename T1, typename T2, typename Mem1, typename Mem2>
inline constexpr bool operator==(
    const Allocator<T1, Mem1> &, const Allocator<T2, Mem2> &)
{
    return std::is_same<Mem1, Mem2>::value;
}

/// \relates Allocator
template <typename T1, typename T2, typename Mem1, typename Mem2>
inline constexpr bool operator!=(
    const Allocator<T1, Mem1> &, const Allocator<T2, Mem2> &)
{
    return !std::is_same<Mem1, Mem2>::value;
}

/// \brief std::vector with Allocator as the default allocator
/// \ingroup Core
template <typename T, typename Alloc = Allocator<T>>
using Vector = std::vector<T, Alloc>;

} // namespace mckl

#endif // MCKL_CORE_MEMORY_HPP
