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
#elif defined(MCKL_MSVC)
#include <malloc.h>
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
#ifndef MCKL_ALIGNMENT_MIN
#define MCKL_ALIGNMENT_MIN 16
#endif

#if MCKL_ALIGNMENT < MCKL_ALIGNMENT_MIN
#undef MCKL_ALIGNEMNT
#define MCKL_ALIGNMENT MCKL_ALIGNMENT_MIN
#endif

/// \brief Default allocation type
/// \ingroup Config
#ifndef MCKL_MEMORY_TYPE
#if MCKL_USE_JEMALLOC
#define MCKL_MEMORY_TYPE ::mckl::MemoryJEM
#elif MCKL_USE_TBB_MALLOC
#define MCKL_MEMORY_TYPE ::mckl::MemoryTBB
#elif MCKL_HAS_POSIX || defined(MCKL_MSVC)
#define MCKL_MEMORY_TYPE ::mckl::MemorySYS
#else
#define MCKL_MEMORY_TYPE ::mckl::MemorySTD
#endif
#endif

namespace mckl
{

/// \brief Alignment of types
///
/// \details
/// * For scalar types, define `MCKL_ALIGNMENT` as `value`
/// * For all other types, define the maximum of `MCKL_MIN_ALIGNMENT` and
/// `alignof(T)` as `value`.
template <typename T>
constexpr std::size_t AlignOf =
    std::integral_constant<std::size_t, std::is_scalar<T>::value ?
            (alignof(T) > MCKL_ALIGNMENT ? alignof(T) : MCKL_ALIGNMENT) :
            (alignof(T) > MCKL_ALIGNMENT_MIN ? alignof(T) :
                                               MCKL_ALIGNMENT_MIN)>::value;

/// \brief Memory allocation using `std::malloc` and `std::free`
/// \ingroup Core
///
/// \details
/// Memory allocated through this class is aligned but some bytes might be
/// wasted in each allocation.
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

    static void *malloc(std::size_t n)
    {
        if (n < 1)
            n = 1;
        n = (n / Alignment + (n % Alignment == 0 ? 0 : 1)) * Alignment;

        std::size_t bytes = n + Alignment + sizeof(void *);
        if (bytes < n)
            return nullptr;

        void *orig_ptr = std::malloc(bytes);
        if (orig_ptr == nullptr)
            return nullptr;

        uintptr_t address = reinterpret_cast<uintptr_t>(orig_ptr);
        uintptr_t offset = Alignment - (address + sizeof(void *)) % Alignment;
        void *ptr =
            reinterpret_cast<void *>(address + offset + sizeof(void *));
        void **orig = reinterpret_cast<void **>(address + offset);
        *orig = orig_ptr;

        return ptr;
    }

    static void free(void *ptr)
    {
        if (ptr != nullptr) {
            std::free(*reinterpret_cast<void **>(
                reinterpret_cast<uintptr_t>(ptr) - sizeof(void *)));
        }
    }
}; // class MemorySTD

#if MCKL_HAS_POSIX

/// \brief Aligned memory using native system aligned memory allocation
/// \ingroup Core
///
/// \details
/// This class use `posix_memalign` and `free` on POSIX systems (Mac OS X,
/// Linux, etc.) and `_aligned_malloc` and `_aligned_free` on Windows.
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

    static void *malloc(std::size_t n)
    {
        if (n < 1)
            n = 1;
        n = (n / Alignment + (n % Alignment == 0 ? 0 : 1)) * Alignment;

        void *ptr = nullptr;
        if (::posix_memalign(&ptr, Alignment, n) != 0)
            ptr = nullptr;

        return ptr;
    }

    static void free(void *ptr)
    {
        if (ptr != nullptr)
            ::free(ptr);
    }
}; // class MemorySYS

#elif defined(MCKL_MSVC)

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

    static void *malloc(std::size_t n)
    {
        if (n < 1)
            n = 1;
        n = (n / Alignment + (n % Alignment == 0 ? 0 : 1)) * Alignment;

        return _aligned_malloc(n, Alignment);
    }

    static void free(void *ptr)
    {
        if (ptr != nullptr)
            _aligned_free(ptr);
    }
}; // class MemorySYS

#endif // MCKL_HAS_POSIX

#if MCKL_HAS_JEMALLOC

/// \brief Aligned memory using jemalloc
/// \ingroup Core
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

    static void *malloc(std::size_t n)
    {
        if (n < 1)
            n = 1;
        n = (n / Alignment + (n % Alignment == 0 ? 0 : 1)) * Alignment;

        return ::je_aligned_alloc(Alignment, n);
    }

    static void free(void *ptr)
    {
        if (ptr != nullptr)
            ::je_free(ptr);
    }
}; // class MemoryTBB

#endif // MCKL_HAS_JEMALLOC

#if MCKL_HAS_TBB

/// \brief Aligned memory using Intel TBB `scalable_aligned_malloc` and
/// `scalable_aligned_free`.
/// \ingroup Core
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

    static void *malloc(std::size_t n)
    {
        if (n < 1)
            n = 1;
        n = (n / Alignment + (n % Alignment == 0 ? 0 : 1)) * Alignment;

        return scalable_aligned_malloc(n, Alignment);
    }

    static void free(void *ptr)
    {
        if (ptr != nullptr)
            scalable_aligned_free(ptr);
    }
}; // class MemoryTBB

#endif // MCKL_HAS_TBB

/// \brief Default Memory type
/// \ingroup Core
template <std::size_t Alignment>
using Memory = MCKL_MEMORY_TYPE<Alignment>;

/// \brief Allocator
/// \ingroup Core
///
/// \tparam T The value type
/// \tparam Mem The memory allocation class.
///
/// \details
/// `Mem` must provides two static member functions, `malloc` and `free`. The
/// member function `malloc` shall behave similar to `std::malloc`. It shall
/// return a reachable non-null pointer even if the size is zero. It shall
/// return a null pointer if it fails to allocate the memory. The member
/// function `free` shall behave just like `std::free`. It shall be able to
/// handle a null pointer as its input.
template <typename T, typename Mem = Memory<AlignOf<T>>>
class Allocator : public std::allocator<T>
{
    public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = typename std::add_lvalue_reference_t<T>;
    using const_reference = typename std::add_lvalue_reference_t<const T>;
    using is_always_equal = std::true_type;
    using memory_type = Mem;

    template <typename U>
    struct rebind {
        using other = Allocator<U, Mem>;
    };

    Allocator() = default;
    Allocator(const Allocator<T, Mem> &) = default;
    Allocator(Allocator<T, Mem> &&) = default;
    Allocator<T, Mem> &operator=(const Allocator<T, Mem> &) = default;
    Allocator<T, Mem> &operator=(Allocator<T, Mem> &&) = default;

    template <typename U>
    Allocator(const Allocator<U, Mem> &other) noexcept(
        noexcept(std::allocator<T>(static_cast<std::allocator<U>>(other))))
        : std::allocator<T>(static_cast<std::allocator<U>>(other))
    {
    }

    template <typename U>
    Allocator(Allocator<U, Mem> &&other) noexcept(noexcept(
        std::allocator<T>(std::move(static_cast<std::allocator<U>>(other)))))
        : std::allocator<T>(std::move(static_cast<std::allocator<U>>(other)))
    {
    }

    pointer allocate(size_type n, const void * = nullptr)
    {
        n = n > 1 ? n : 1;
        size_type bytes = n * sizeof(value_type);
        if (bytes < n)
            throw std::bad_alloc();

        pointer ptr = static_cast<pointer>(Mem::malloc(bytes));
        if (ptr == nullptr)
            throw std::bad_alloc();

        return ptr;
    }

    void deallocate(pointer ptr, size_type = 0)
    {
        if (ptr != nullptr)
            Mem::free(ptr);
    }

    template <typename U>
    void construct(U *ptr) noexcept(
        std::is_nothrow_default_constructible<U>::value)
    {
        construct_dispatch(ptr, std::is_scalar<U>());
    }

    template <typename U, typename... Args>
    void construct(U *ptr, Args &&... args) noexcept(
        std::is_nothrow_constructible<U, Args...>::value)
    {
        ::new (static_cast<void *>(ptr)) U(std::forward<Args>(args)...);
    }

    private:
    template <typename U>
    void construct_dispatch(U *, std::true_type) noexcept
    {
    }

    template <typename U>
    void construct_dispatch(U *ptr, std::false_type) noexcept(
        std::is_nothrow_default_constructible<U>::value)
    {
        ::new (static_cast<void *>(ptr)) U;
    }
}; // class Allocator

template <typename Mem>
class Allocator<void, Mem>
{
    using value_type = void;
    using pointer = void *;
    using const_pointer = const void *;
    using memory_type = Mem;

    template <class U>
    struct rebind {
        using other = Allocator<U, Mem>;
    };
}; // class Allocator

template <typename Mem>
class Allocator<const void, Mem>
{
    using value_type = const void;
    using pointer = const void *;
    using const_pointer = const void *;
    using memory_type = Mem;

    template <class U>
    struct rebind {
        using other = Allocator<U, Mem>;
    };
}; // class Allocator

template <typename T1, typename T2, typename Mem1, typename Mem2>
inline constexpr bool operator==(
    const Allocator<T1, Mem1> &, const Allocator<T2, Mem2> &)
{
    return std::is_same<Mem1, Mem2>::value;
}

template <typename T1, typename T2, typename Mem1, typename Mem2>
inline constexpr bool operator!=(
    const Allocator<T1, Mem1> &, const Allocator<T2, Mem2> &)
{
    return !std::is_same<Mem1, Mem2>::value;
}

/// \brief std::vector with Allocator as default allocator
/// \ingroup Core
template <typename T, typename Alloc = Allocator<T>>
using Vector = std::vector<T, Alloc>;

} // namespace mckl

#endif // MCKL_CORE_MEMORY_HPP
