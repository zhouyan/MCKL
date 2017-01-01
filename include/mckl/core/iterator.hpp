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

    StrideIterator() noexcept : stride_(1) {}

    template <typename IntType>
    StrideIterator(RandomIter iter, IntType stride) noexcept
        : iter_(iter), stride_(static_cast<difference_type>(stride))
    {
    }

    template <typename Iter>
    StrideIterator(const StrideIterator<Iter> &other) noexcept
        : iter_(other.base()), stride_(other.stride())
    {
    }

    /// \brief Return the underlying random access iterator
    RandomIter base() const noexcept { return iter_; }

    /// \brief Return the stride size
    difference_type stride() const noexcept { return stride_; }

    reference operator*() const noexcept { return *iter_; }

    RandomIter operator->() const noexcept { return iter_; }

    /// \brief `base()[n * stride()]`
    template <typename IntType>
    reference operator[](IntType n) const noexcept
    {
        return *((*this) + n);
    }

    friend bool operator==(
        const StrideIterator &iter1, const StrideIterator &iter2) noexcept
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "Compare two StrideIterator objects with different strides");

        return iter1.iter_ == iter2.iter_;
    }

    friend bool operator!=(
        const StrideIterator &iter1, const StrideIterator &iter2) noexcept
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "Compare two StrideIterator objects with different strides");

        return iter1.iter_ != iter2.iter_;
    }

    friend bool operator<(
        const StrideIterator &iter1, const StrideIterator &iter2) noexcept
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "Compare two StrideIterator objects with different strides");

        return iter1.iter_ < iter2.iter_;
    }

    friend bool operator<=(
        const StrideIterator &iter1, const StrideIterator &iter2) noexcept
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "Compare two StrideIterator objects with different strides");

        return iter1.iter_ <= iter2.iter_;
    }

    friend bool operator>(
        const StrideIterator &iter1, const StrideIterator &iter2) noexcept
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "Compare two StrideIterator objects with different strides");

        return iter1.iter_ > iter2.iter_;
    }

    friend bool operator>=(
        const StrideIterator &iter1, const StrideIterator &iter2) noexcept
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "Compare two StrideIterator objects with different strides");

        return iter1.iter_ >= iter2.iter_;
    }

    /// \brief `iter.base() += iter.stride()`
    friend StrideIterator &operator++(StrideIterator &iter) noexcept
    {
        iter.iter_ += iter.stride_;

        return iter;
    }

    /// \brief `iter.base() += iter.stride()`
    friend StrideIterator operator++(StrideIterator &iter, int) noexcept
    {
        StrideIterator ret(iter);
        iter.iter_ += iter.stride_;

        return ret;
    }

    /// \brief `iter.base() -= iter.stride()`
    friend StrideIterator &operator--(StrideIterator &iter) noexcept
    {
        iter.iter_ -= iter.stride_;

        return iter;
    }

    /// \brief `iter.base() -= iter.stride()`
    friend StrideIterator operator--(StrideIterator &iter, int) noexcept
    {
        StrideIterator ret(iter);
        iter.iter_ -= iter.stride_;

        return ret;
    }

    /// \brief `iter.base() + n * iter.stride()`
    template <typename IntType>
    friend StrideIterator operator+(
        const StrideIterator &iter, IntType n) noexcept
    {
        StrideIterator ret(iter);
        ret.iter_ += ret.stride_ * static_cast<difference_type>(n);

        return ret;
    }

    /// \brief `iter.base() + n * iter.stride()`
    template <typename IntType>
    friend StrideIterator operator+(
        IntType n, const StrideIterator &iter) noexcept
    {
        return iter + n;
    }

    /// \brief `iter.base() - n * iter.stride()`
    template <typename IntType>
    friend StrideIterator operator-(
        const StrideIterator &iter, IntType n) noexcept
    {
        return iter + (-static_cast<difference_type>(n));
    }

    /// \brief `iter.base() += n * iter.stride()`
    template <typename IntType>
    friend StrideIterator &operator+=(StrideIterator &iter, IntType n) noexcept
    {
        return iter = iter + n;
    }

    /// \brief `iter.base() -= n * iter.stride()`
    template <typename IntType>
    friend StrideIterator &operator-=(StrideIterator &iter, IntType n) noexcept
    {
        return iter = iter - n;
    }

    /// \brief `(iter1.base() - iter2.base()) / iter1.stride()`
    friend difference_type operator-(
        const StrideIterator &iter1, const StrideIterator &iter2) noexcept
    {
        return (iter1.iter_ - iter2.iter_) / iter1.stride_;
    }

    private:
    RandomIter iter_;
    difference_type stride_;
}; // class StrideIterator

template <typename T, bool = std::is_integral<T>::value>
class Range;

/// \brief Range of integers
/// \ingroup Core
///
/// \details
/// A range of integer types denote the half-open interval \f$[b, e)\f$, where
/// \f$b \le e\f$.
template <typename IntType>
class Range<IntType, true>
{
    public:
    using size_type = std::size_t;
    using iterator = IntType;

    Range() : begin_(0), end_(0), grainsize_(1) {}

    Range(const Range &) = default;

    Range(iterator begin, iterator end, size_type grainsize = 1)
        : begin_(begin), end_(end), grainsize_(grainsize)
    {
        runtime_assert(begin_ <= end_,
            "**Range::Range** used with invalid pair of integers");
    }

    /// \brief Copy construct a new range which split the original
    ///
    /// \details
    /// Let the original range in `other` be \f$[b, e)\f$. The newly
    /// constructed range is \f$[h, e)\f$, where
    /// \f$h = \lfloor (e - b) / 2 \rfloor\f$. And the original range is
    /// revised to \f$[b, h)\f$.
    template <typename Split>
    Range(Range &other, Split)
        : begin_(other.begin_ + (other.end_ - other.begin_) / 2)
        , end_(other.end_)
        , grainsize_(other.grainsize_)
    {
        other.end_ = begin_;
    }

    /// \brief Return the size of the range
    ///
    /// \return Zero if `empty()`, otherwise, `end() - begin()`
    size_type size() const { return empty() ? 0 : end_ - begin_; }

    /// \brief Return the grain size
    size_type grainsize() const { return grainsize_; }

    /// \brief If the range is empty, i.e., `begin() >= end()`
    bool empty() const { return begin_ >= end_; }

    /// \brief If the range is divisible, i.e., `grainsize() < size()`
    bool is_divisible() const { return grainsize_ < size(); }

    /// \brief The lower bound of the range
    iterator begin() const { return begin_; }

    /// \brief The upper bound of the range
    iterator end() const { return end_; }

    private:
    iterator begin_;
    iterator end_;
    size_type grainsize_;
}; // class Range

/// \brief Range of iterators
/// \ingroup Core
///
/// \details
/// A range of input iterators denote the half-open interval \f$[b, e)\f$ where
/// `distance(b, e) >= 0`.
template <typename InputIter>
class Range<InputIter, false>
{
    public:
    using size_type = std::size_t;
    using iterator = InputIter;
    using value_type = typename std::iterator_traits<iterator>::value_type;
    using pointer = typename std::iterator_traits<iterator>::pointer;
    using reference = typename std::iterator_traits<iterator>::reference;

    Range() : begin_(iterator()), end_(begin_), grainsize_(1) {}

    Range(const Range &) = default;

    Range(iterator begin, iterator end, size_type grainsize = 1)
        : begin_(begin), end_(end), grainsize_(grainsize)
    {
        using std::distance;

        runtime_assert(distance(begin_, end_) >= 0,
            "**Range::Range** used with invalid pair of iterators");
    }

    /// \brief Copy construct a new range which split the original
    ///
    /// \details
    /// Let the original range in `other` be \f$[b, e)\f$. The newly
    /// constructed range is \f$[h, e)\f$, where \f$h\f$ is the iterator
    /// obtained through advancing (`std::advance`) the iterator \f$b\f$ by
    /// \f$\lfloor n / 2 \rfloor\f$, where \f$n\f$ is the distance of the pair
    /// of iterators in the original (`std::distance`).
    template <typename Split>
    Range(Range &other, Split)
        : begin_(other.begin_), end_(other.end_), grainsize_(other.grainsize_)
    {
        using std::advance;
        using std::distance;

        advance(begin_, distance(begin_, end_) / 2);
        other.end_ = begin_;
    }

    /// \brief Return the size of the range
    ///
    /// \return Zero if `empty()`, otherwise, `distance(begin(), end())`
    size_type size() const
    {
        using std::distance;

        return empty() ? 0 : static_cast<size_type>(distance(begin_, end_));
    }

    /// \brief Return the grain size
    size_type grainsize() const { return grainsize_; }

    /// \brief If the range is empty, i.e., `distance(begin(), end()) <= 0`
    bool empty() const
    {
        using std::distance;

        return distance(begin_, end_) <= 0;
    }

    /// \brief If the range is divisible, i.e., `grainsize() < size()`
    bool is_divisible() const { grainsize_ < size(); }

    /// \brief The lower bound of the range
    iterator begin() const { return begin_; }

    /// \brief The upper bound of the range
    iterator end() const { return end_; }

    /// \brief Access an element in the range given random access iterators
    /// with bound checking
    reference at(size_type i) const
    {
        runtime_assert<std::out_of_range>(
            begin_ + i < end_, "**Range::at** index out of range");

        return operator[](i);
    }

    /// \brief Access an element in the range given random access iterators
    reference operator[](size_type i) const { return *(begin_ + i); }

    /// \brief Access the first element in the range
    reference front() const { return *begin_; }

    /// \brief Access the last element in the range given bidirectory iterators
    reference back() const { return *(--end_); }

    private:
    iterator begin_;
    iterator end_;
    size_type grainsize_;
}; // class Range

} // namespace mckl

#endif // MCKL_CORE_ITERATOR_HPP
