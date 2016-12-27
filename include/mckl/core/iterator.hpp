//============================================================================
// MCKL/include/mckl/core/iterator.hpp
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

#ifndef MCKL_CORE_ITERATOR_HPP
#define MCKL_CORE_ITERATOR_HPP

#include <mckl/internal/common.hpp>

namespace mckl
{

/// \brief Random access iterator adaptor for non-unity stride
/// \ingroup Core
template <typename RandomIter>
class StrideIterator
{
    static_assert(
        std::is_same<
            typename std::iterator_traits<RandomIter>::iterator_category,
            std::random_access_iterator_tag>::value,
        "**StrideIterator** used with RandomIter other than a random access "
        "iterator");

    public:
    using difference_type = std::ptrdiff_t;
    using value_type = typename std::iterator_traits<RandomIter>::value_type;
    using pointer = typename std::iterator_traits<RandomIter>::pointer;
    using reference = typename std::iterator_traits<RandomIter>::reference;
    using iterator_category = std::random_access_iterator_tag;

    /// \brief Default constructor with unity stride
    StrideIterator() noexcept : stride_(1) {}

    /// \brief Construct an iterator given a base iterator and stride size
    template <typename IntType>
    StrideIterator(RandomIter iter, IntType stride) noexcept
        : iter_(iter), stride_(static_cast<difference_type>(stride))
    {
    }

    /// \brief Convert another iterator with compatible base iterator
    template <typename Iter>
    StrideIterator(const StrideIterator<Iter> &other) noexcept
        : iter_(other.base()), stride_(other.stride())
    {
    }

    /// \brief Copy constructor
    StrideIterator(const StrideIterator &) = default;

    /// \brief Move constructor
    StrideIterator(StrideIterator &&) = default;

    /// \brief Copy assignment operator
    StrideIterator &operator=(const StrideIterator &) = default;

    /// \brief Move assignment operator
    StrideIterator &operator=(StrideIterator &&) = default;

    /// \brief The base iterator
    RandomIter base() const { return iter_; }

    /// \brief The stride size
    difference_type stride() const { return stride_; }

    /// \brief Dereference operator
    reference operator*() const { return *iter_; }

    /// \brief Member selection operator
    RandomIter operator->() const { return iter_; }

    /// \brief Subscript operator
    template <typename IntType>
    reference operator[](IntType n) const
    {
        return *((*this) + n);
    }

    /// \brief Compare equality of two iterators
    friend bool operator==(
        const StrideIterator &iter1, const StrideIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**StrideIterator::operator==** used with two iterators with "
            "different strides");

        return iter1.iter_ == iter2.iter_;
    }

    /// \brief Compare inequality of two iterators
    friend bool operator!=(
        const StrideIterator &iter1, const StrideIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**StrideIterator::operator!=** used with two iterators with "
            "different strides");

        return iter1.iter_ != iter2.iter_;
    }

    /// \brief Compare less-than of two iterators
    friend bool operator<(
        const StrideIterator &iter1, const StrideIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**StrideIterator::operator<** used with two iterators with "
            "different strides");

        return iter1.iter_ < iter2.iter_;
    }

    /// \brief Compare less-than or equality of two iterators
    friend bool operator<=(
        const StrideIterator &iter1, const StrideIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**StrideIterator::operator<=** used with two iterators with "
            "different strides");

        return iter1.iter_ <= iter2.iter_;
    }

    /// \brief Compare greater-than of two iterators
    friend bool operator>(
        const StrideIterator &iter1, const StrideIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**StrideIterator::operator>** used with two iterators with "
            "different strides");

        return iter1.iter_ > iter2.iter_;
    }

    /// \brief Compare greater-than or equality of two iterators
    friend bool operator>=(
        const StrideIterator &iter1, const StrideIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**StrideIterator::operator>=** used with two iterators with "
            "different strides");

        return iter1.iter_ >= iter2.iter_;
    }

    /// \brief Prefix increment
    friend StrideIterator &operator++(StrideIterator &iter)
    {
        iter.iter_ += iter.stride_;

        return iter;
    }

    /// \brief Postfix increment
    friend StrideIterator operator++(StrideIterator &iter, int)
    {
        StrideIterator ret(iter);
        iter.iter_ += iter.stride_;

        return ret;
    }

    /// \brief Prefix decrement
    friend StrideIterator &operator--(StrideIterator &iter)
    {
        iter.iter_ -= iter.stride_;

        return iter;
    }

    /// \brief Postfix decrement
    friend StrideIterator operator--(StrideIterator &iter, int)
    {
        StrideIterator ret(iter);
        iter.iter_ -= iter.stride_;

        return ret;
    }

    /// \brief Addition with an integer
    template <typename IntType>
    friend StrideIterator operator+(const StrideIterator &iter, IntType n)
    {
        StrideIterator ret(iter);
        ret.iter_ += ret.stride_ * static_cast<difference_type>(n);

        return ret;
    }

    /// \brief Addition with an integer
    template <typename IntType>
    friend StrideIterator operator+(IntType n, const StrideIterator &iter)
    {
        return iter + n;
    }

    /// \brief Substruction with an integer
    template <typename IntType>
    friend StrideIterator operator-(const StrideIterator &iter, IntType n)
    {
        return iter + (-static_cast<difference_type>(n));
    }

    /// \brief Compound addition and assignment
    template <typename IntType>
    friend StrideIterator &operator+=(StrideIterator &iter, IntType n)
    {
        iter = iter + n;

        return iter;
    }

    /// \brief Compound substruction and assignment
    template <typename IntType>
    friend StrideIterator &operator-=(StrideIterator &iter, IntType n)
    {
        iter = iter - n;

        return iter;
    }

    /// \brief Distance between two operators
    friend difference_type operator-(
        const StrideIterator &iter1, const StrideIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**StrideIterator::operator-** used with two iterators with "
            "different strides");

        return (iter1.iter_ - iter2.iter_) / iter1.stride_;
    }

    private:
    RandomIter iter_;
    difference_type stride_;
}; // class StrideIterator

} // namespace mckl

#endif // MCKL_CORE_ITERATOR_HPP
