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

/// \brief Iterator adapter which increment the base iterator in multiple steps
/// \ingroup Core
///
/// \tparam Base An iterator type
template <typename Base>
class StepIterator
{
    public:
    using difference_type = std::make_signed_t<
        typename std::iterator_traits<Base>::difference_type>;
    using value_type = typename std::iterator_traits<Base>::value_type;
    using pointer = typename std::iterator_traits<Base>::pointer;
    using reference = typename std::iterator_traits<Base>::reference;
    using iterator_category =
        typename std::iterator_traits<Base>::iterator_category;

    /// \brief Default constructor
    StepIterator() noexcept(std::is_nothrow_default_constructible<Base>::value)
        : base_(Base()), step_(1)
    {
    }

    /// \brief Convert from a base iterator
    ///
    /// \param base The base iterator
    /// \param step The step size. Each call of the increment/decrement
    /// operator of the newly constructed iterator will advance it by a
    /// distance equal to `step`.
    ///
    /// \pre `Base` is copy constructible
    explicit StepIterator(Base base, difference_type step = 1) noexcept(
        std::is_nothrow_copy_constructible<Base>::value)
        : base_(base), step_(step)
    {
    }

    /// \brief Convert from compatible StepIterator
    ///
    /// \pre T is convertible to Base
    template <typename T>
    StepIterator(const StepIterator<T> &other) noexcept(
        std::is_nothrow_copy_constructible<Base>::value)
        : base_(other.base()), step_(other.step())
    {
    }

    /// \brief Return the underlying base iterator
    Base base() const { return base_; }

    /// \brief Return the increment step size
    difference_type step() const { return step_; }

    /// \brief Dereference the base iterator
    reference operator*() const { return *base_; }

    /// \brief Member selection through the base iterator
    Base operator->() const { return base_; }

    /// \brief Equivalent to `*((*this) + n)`
    ///
    /// \pre `Base` is an input iterator
    /// \pre `Base` is a bidirectional iterator if `n * step() < 0`
    template <typename IntType>
    reference operator[](IntType n) const
    {
        return *((*this) + n);
    }

    /// \brief Compare equality
    ///
    /// \pre `Base` is equality comparable
    /// \pre `iter1.step() == iter2.step()`
    friend bool operator==(
        const StepIterator &iter1, const StepIterator &iter2)
    {
        runtime_assert(iter1.step_ == iter2.step_,
            "Compare two StepIterator objects with different steps");

        return iter1.base_ == iter2.base_;
    }

    /// \brief Compare inequality
    ///
    /// \pre `Base` is inequality comparable
    /// \pre `iter1.step() == iter2.step()`
    friend bool operator!=(
        const StepIterator &iter1, const StepIterator &iter2)
    {
        runtime_assert(iter1.step_ == iter2.step_,
            "Compare two StepIterator objects with different steps");

        return iter1.base_ != iter2.base_;
    }

    /// \brief Compare less than relation
    ///
    /// \pre `Base` is less than comparable
    /// \pre `iter1.step() == iter2.step()`
    friend bool operator<(const StepIterator &iter1, const StepIterator &iter2)
    {
        runtime_assert(iter1.step_ == iter2.step_,
            "Compare two StepIterator objects with different steps");

        return iter1.base_ < iter2.base_;
    }

    /// \brief Compare less than relation or equality
    ///
    /// \pre `Base` is less than or equality comparable
    /// \pre `iter1.step() == iter2.step()`
    friend bool operator<=(
        const StepIterator &iter1, const StepIterator &iter2)
    {
        runtime_assert(iter1.step_ == iter2.step_,
            "Compare two StepIterator objects with different steps");

        return iter1.base_ <= iter2.base_;
    }

    /// \brief Compare greater than relation
    ///
    /// \pre `Base` is greater than comparable
    /// \pre `iter1.step() == iter2.step()`
    friend bool operator>(const StepIterator &iter1, const StepIterator &iter2)
    {
        runtime_assert(iter1.step_ == iter2.step_,
            "Compare two StepIterator objects with different steps");

        return iter1.base_ > iter2.base_;
    }

    /// \brief Compare greater than relation or equality
    ///
    /// \pre `Base` is greater than or equality comparable
    /// \pre `iter1.step() == iter2.step()`
    friend bool operator>=(
        const StepIterator &iter1, const StepIterator &iter2)
    {
        runtime_assert(iter1.step_ == iter2.step_,
            "Compare two StepIterator objects with different steps");

        return iter1.base_ >= iter2.base_;
    }

    /// \brief Prefix increment: advance the base iterator by a distance of
    /// `iter.step()`
    ///
    /// \pre `Base` is an input iterator
    /// \pre `Base` is a bidirectional iterator if `iter.step() < 0`
    friend StepIterator &operator++(StepIterator &iter)
    {
        using std::advance;

        advance(iter.base_, iter.step_);

        return iter;
    }

    /// \brief Postfix increment
    ///
    /// \pre `Base` is an input iterator
    /// \pre `Base` is a bidirectional iterator if `iter.step() < 0`
    friend StepIterator operator++(StepIterator &iter, int)
    {
        StepIterator ret(iter);
        ++iter;

        return ret;
    }

    /// \brief Prefix decrement: advance the base iterator by a distance of
    /// `-iter.step()`
    ///
    /// \pre `Base` is a input iterator
    /// \pre `Base` is a bidirectional iterator if `iter.step() > 0`
    friend StepIterator &operator--(StepIterator &iter)
    {
        using std::advance;

        advance(iter.base_, -iter.step_);

        return iter;
    }

    /// \brief Postfix decrement
    ///
    /// \pre `Base` is a input iterator
    /// \pre `Base` is a bidirectional iterator if `iter.step() > 0`
    friend StepIterator operator--(StepIterator &iter, int)
    {
        StepIterator ret(iter);
        --iter;

        return ret;
    }

    /// \brief Addition with integer: advance the base iterator by a distance
    /// of `n * iter.step()`
    ///
    /// \pre `Base` is an input iterator
    /// \pre `Base` is a bidirectional iterator if `n * iter.step() < 0`
    template <typename IntType>
    friend StepIterator operator+(const StepIterator &iter, IntType n)
    {
        using std::advance;

        StepIterator ret(iter);
        advance(ret.base_, static_cast<difference_type>(n) * ret.step_);

        return ret;
    }

    /// \brief Addition with integer
    ///
    /// \pre `Base` is an input iterator
    /// \pre `Base` is a bidirectional iterator if `n * iter.step() < 0`
    template <typename IntType>
    friend StepIterator operator+(IntType n, const StepIterator &iter)
    {
        return iter + n;
    }

    /// \brief Subtraction with integer: advance the base iterator by a
    /// distance `-n * iter.step()`
    ///
    /// \pre `Base` is an input iterator
    /// \pre `Base` is a bidirectional iterator if `n * iter.step() > 0`
    template <typename IntType>
    friend StepIterator operator-(const StepIterator &iter, IntType n)
    {
        return iter + (-static_cast<difference_type>(n));
    }

    /// \brief Compound addition with integer
    ///
    /// \pre `Base` is an input iterator
    /// \pre `Base` is a bidirectional iterator if `n * iter.step() < 0`
    template <typename IntType>
    friend StepIterator &operator+=(StepIterator &iter, IntType n)
    {
        return iter = iter + n;
    }

    /// \brief Compound subtraction with integer
    ///
    /// \pre `Base` is an input iterator
    /// \pre `Base` is a bidirectional iterator if `n * iter.step() > 0`
    template <typename IntType>
    friend StepIterator &operator-=(StepIterator &iter, IntType n)
    {
        return iter = iter - n;
    }

    /// \brief Difference between two iterators
    ///
    /// \pre `Base` is an input iterator
    /// \pre `iter1` is reachable by incrementing `iter2`
    /// \pre `iter1.step() == iter2.step()`
    friend difference_type operator-(
        const StepIterator &iter1, const StepIterator &iter2)
    {
        runtime_assert(iter1.step_ == iter2.step_,
            "Subtract two StepIterator objects with different steps");

        using std::distance;

        return distance(iter2.base_, iter1.base_) / iter1.step_;
    }

    private:
    Base base_;
    difference_type step_;
}; // class StepIterator

template <typename T, bool = std::is_integral<T>::value>
class Range;

/// \brief Range of integers
/// \ingroup Core
///
/// \tparam IntType An integral type
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

    /// \brief Default constructor
    Range() noexcept : begin_(0), end_(0), grainsize_(1) {}

    /// \brief Construct a new range with a pair of integers
    ///
    /// \param begin The lower bound of the range
    /// \param end The upper bound of the range
    /// \param grainsize The minimum size of a divisible range
    ///
    /// \pre `begin <= end`
    Range(iterator begin, iterator end, size_type grainsize = 1)
        : begin_(begin), end_(end), grainsize_(grainsize)
    {
        runtime_assert(begin_ <= end_,
            "**Range::Range** used with invalid pair of integers");
    }

    /// \brief Copy Constructor
    Range(const Range &) = default;

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

    /// \brief If the range is divisible, i.e., `grainsize() <= size()`
    bool is_divisible() const { return grainsize_ <= size(); }

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
/// \tparam Iterator An iterator type
///
/// \details
/// A range of iterators denote the half-open interval \f$[b, e)\f$ where
/// `distance(b, e) >= 0`.
template <typename Iterator>
class Range<Iterator, false>
{
    public:
    using size_type = std::size_t;
    using iterator = Iterator;
    using value_type = typename std::iterator_traits<iterator>::value_type;
    using pointer = typename std::iterator_traits<iterator>::pointer;
    using reference = typename std::iterator_traits<iterator>::reference;

    /// \brief Default constructor
    Range() : begin_(iterator()), end_(begin_), grainsize_(1) {}

    /// \brief Construct a new range with a pair of iterators
    ///
    /// \param begin The lower bound of the range
    /// \param end The upper bound of the range
    /// \param grainsize The minimum size of a divisible range
    ///
    /// \pre `Iterator` is an input iterator
    /// \pre `distance(begin, end) >= 0`
    Range(iterator begin, iterator end, size_type grainsize = 1)
        : begin_(begin), end_(end), grainsize_(grainsize)
    {
        using std::distance;

        runtime_assert(distance(begin_, end_) >= 0,
            "**Range::Range** used with invalid pair of iterators");
    }

    /// \brief Copy constructor
    Range(const Range &) = default;

    /// \brief Copy construct a new range which split the original
    ///
    /// \pre `Iterator is an input iterator
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

    /// \brief If the range is divisible, i.e., `grainsize() <= size()`
    bool is_divisible() const { return grainsize_ <= size(); }

    /// \brief The lower bound of the range
    iterator begin() const { return begin_; }

    /// \brief The upper bound of the range
    iterator end() const { return end_; }

    /// \brief Access an element within the range with bound checking
    ///
    /// \pre `Iterator` is an input iterator
    /// \pre `n >= 0 && n < size()`
    reference at(size_type n) const
    {
        runtime_assert<std::out_of_range>(
            n < size(), "**Range::at** index out of range");

        return operator[](n);
    }

    /// \brief Access an element within the range
    ///
    /// \pre `Iterator` is an input iterator
    /// \pre `n >= 0 && n < size()`
    reference operator[](size_type n) const
    {
        using std::advance;

        iterator iter = begin_;
        advance(iter, n);

        return *iter;
    }

    /// \brief Access the first element in the range
    reference front() const { return *begin_; }

    /// \brief Access the last element in the range
    ///
    /// \pre `Iterator` is a bidirectional iterator
    reference back() const { return *(--end_); }

    private:
    iterator begin_;
    iterator end_;
    size_type grainsize_;
}; // class Range

} // namespace mckl

#endif // MCKL_CORE_ITERATOR_HPP
