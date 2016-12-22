//============================================================================
// MCKL/include/mckl/core/vector.hpp
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

#ifndef MCKL_CORE_MATRIX_VECTOR_HPP
#define MCKL_CORE_MATRIX_VECTOR_HPP

#include <mckl/internal/common.hpp>

namespace mckl
{

namespace internal
{

template <typename T, std::size_t N, typename Alloc, typename Derived>
class VectorBase
{
    public:
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = typename std::allocator_traits<Alloc>::pointer;
    using const_pointer = typename std::allocator_traits<Alloc>::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    vector() noexcept(noexcept(allocator_type()))
        : capacity_(N), begin_(array_.data()), end_(begin_)
    {
    }

    explicit vector(const allocator_type &alloc) noexcept(
        noexcept(alloc_(alloc)))
        : alloc_(alloc), capcity_(N), begin_(array_.begin()), end_(begin_)
    {
    }

    private:
    std::array<value_type, N> array_;
    size_type capacity_;
    pointer begin_;
    pointer end_;
    allocator_type alloc_;
}; // class VectorStorage

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_CORE_MATRIX_VECTOR_HPP
