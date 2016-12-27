//============================================================================
// MCKL/include/mckl/core/matrix.hpp
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

#ifndef MCKL_CORE_MATRIX_HPP
#define MCKL_CORE_MATRIX_HPP

#include <mckl/internal/common.hpp>

namespace mckl
{

namespace internal
{

template <typename T>
class MatrixIterator : public std::iterator<std::random_access_iterator_tag, T>
{
    public:
    MatrixIterator() noexcept : ptr_(nullptr), stride_(1) {}

    template <typename IntType>
    MatrixIterator(T *ptr, IntType stride) noexcept
        : ptr_(ptr), stride_(static_cast<std::ptrdiff_t>(stride))
    {
    }

    operator MatrixIterator<const T>()
    {
        return MatrixIterator<const T>(ptr_, stride_);
    }

    T &operator*() const { return *ptr_; }

    T *operator->() const { return ptr_; }

    friend bool operator==(
        const MatrixIterator &iter1, const MatrixIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**MatrixIterator::operator==** used with two iterators with "
            "different strides");

        return iter1.ptr_ == iter2.ptr_;
    }

    friend bool operator!=(
        const MatrixIterator &iter1, const MatrixIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**MatrixIterator::operator!=** used with two iterators with "
            "different strides");

        return iter1.ptr_ != iter2.ptr_;
    }

    friend bool operator<(
        const MatrixIterator &iter1, const MatrixIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**MatrixIterator::operator<** used with two iterators with "
            "different strides");

        return iter1.ptr_ < iter2.ptr_;
    }

    friend bool operator<=(
        const MatrixIterator &iter1, const MatrixIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**MatrixIterator::operator<=** used with two iterators with "
            "different strides");

        return iter1.ptr_ <= iter2.ptr_;
    }

    friend bool operator>(
        const MatrixIterator &iter1, const MatrixIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**MatrixIterator::operator>** used with two iterators with "
            "different strides");

        return iter1.ptr_ > iter2.ptr_;
    }

    friend bool operator>=(
        const MatrixIterator &iter1, const MatrixIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**MatrixIterator::operator>=** used with two iterators with "
            "different strides");

        return iter1.ptr_ >= iter2.ptr_;
    }

    friend MatrixIterator &operator++(MatrixIterator &iter)
    {
        iter.ptr_ += iter.stride_;

        return iter;
    }

    friend MatrixIterator operator++(MatrixIterator &iter, int)
    {
        MatrixIterator ret(iter);
        iter.ptr_ += iter.stride_;

        return ret;
    }

    friend MatrixIterator &operator--(MatrixIterator &iter)
    {
        iter.ptr_ -= iter.stride_;

        return iter;
    }

    friend MatrixIterator operator--(MatrixIterator &iter, int)
    {
        MatrixIterator ret(iter);
        iter.ptr_ -= iter.stride_;

        return ret;
    }

    template <typename IntType>
    friend MatrixIterator operator+(const MatrixIterator &iter, IntType n)
    {
        MatrixIterator ret(iter);
        ret.ptr_ += ret.stride_ * static_cast<std::ptrdiff_t>(n);

        return ret;
    }

    template <typename IntType>
    friend MatrixIterator operator+(IntType n, const MatrixIterator &iter)
    {
        return iter + n;
    }

    template <typename IntType>
    friend MatrixIterator operator-(const MatrixIterator &iter, IntType n)
    {
        return iter + (-static_cast<std::ptrdiff_t>(n));
    }

    template <typename IntType>
    friend MatrixIterator &operator+=(MatrixIterator &iter, IntType n)
    {
        iter = iter + n;

        return iter;
    }

    template <typename IntType>
    friend MatrixIterator &operator-=(MatrixIterator &iter, IntType n)
    {
        iter = iter - n;

        return iter;
    }

    friend std::ptrdiff_t operator-(
        const MatrixIterator &iter1, const MatrixIterator &iter2)
    {
        runtime_assert(iter1.stride_ == iter2.stride_,
            "**MatrixIterator::operator-** used with two iterators with "
            "different strides");

        return (iter1.ptr_ - iter2.ptr_) / iter1.stride_;
    }

    private:
    T *ptr_;
    std::ptrdiff_t stride_;
}; // class MatrixIterator

} // namespace mckl::internal

/// \brief Matrix container
/// \ingroup Core
template <MatrixLayout Layout, typename T, typename Alloc = Allocator<T>>
class Matrix
{
    using layout_dispatch = std::integral_constant<MatrixLayout, Layout>;
    using row_major = std::integral_constant<MatrixLayout, RowMajor>;
    using col_major = std::integral_constant<MatrixLayout, ColMajor>;

    Vector<T, Alloc> data_;
    std::size_t nrow_;
    std::size_t ncol_;

    public:
    using size_type = std::size_t;
    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;

    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using row_iterator = std::conditional_t<Layout == RowMajor, pointer,
        internal::MatrixIterator<T>>;
    using const_row_iterator = std::conditional_t<Layout == RowMajor,
        const_pointer, internal::MatrixIterator<const T>>;
    using reverse_row_iterator = std::reverse_iterator<row_iterator>;
    using const_reverse_row_iterator =
        std::reverse_iterator<const_row_iterator>;

    using col_iterator = std::conditional_t<Layout == ColMajor, pointer,
        internal::MatrixIterator<T>>;
    using const_col_iterator = std::conditional_t<Layout == ColMajor,
        const_pointer, internal::MatrixIterator<const T>>;
    using reverse_col_iterator = std::reverse_iterator<col_iterator>;
    using const_reverse_col_iterator =
        std::reverse_iterator<const_col_iterator>;

    using transpose_type =
        Matrix<Layout == RowMajor ? ColMajor : RowMajor, T, Alloc>;

    /// \brief Construct an empty matrix
    Matrix() noexcept(
        std::is_nothrow_default_constructible<Vector<T, Alloc>>::value)
        : nrow_(0), ncol_(0)
    {
    }

    /// \brief Construct an `nrow` by `ncol` matrix
    Matrix(size_type nrow, size_type ncol)
        : data_(nrow * ncol), nrow_(nrow), ncol_(ncol)
    {
    }

    /// \brief Copy constructor
    Matrix(const Matrix &) = default;

    /// \brief Copy constructor using a matrix with a different layout
    Matrix(const transpose_type &other)
        : data_(other.nrow() * other.ncol())
        , nrow_(other.nrow())
        , ncol_(other.ncol())
    {
        for (std::size_t i = 0; i != nrow_; ++i)
            for (std::size_t j = 0; j != ncol_; ++j)
                operator()(i, j) = other(i, j);
    }

    /// \brief Move constructor
    Matrix(Matrix &&other) noexcept(
        std::is_nothrow_move_constructible<Vector<T, Alloc>>::value)
        : data_(std::move(other.data_)), nrow_(other.nrow_), ncol_(other.ncol_)
    {
        other.nrow_ = 0;
        other.ncol_ = 0;
    }

    /// \brief Move constructor using a matrix with a different layout
    Matrix(transpose_type &&other)
        : data_(other.nrow() * other.ncol())
        , nrow_(other.nrow())
        , ncol_(other.ncol())
    {
        for (std::size_t i = 0; i != nrow_; ++i)
            for (std::size_t j = 0; j != ncol_; ++j)
                operator()(i, j) = std::move(other(i, j));
    }

    /// \brief Copy assignment operator
    Matrix &operator=(const Matrix &) = default;

    /// \brief Copy assignment operator using a matrix with a different layout
    Matrix &operator=(const transpose_type &other)
    {
        resize(other.nrow_, other.ncol_);
        for (std::size_t i = 0; i != nrow_; ++i)
            for (std::size_t j = 0; j != ncol_; ++j)
                operator()(i, j) = other(i, j);

        return *this;
    }

    /// \brief Move assignment operator
    Matrix &operator=(Matrix &&other) noexcept(
        noexcept(data_.swap(other.data_)))
    {
        if (this != &other) {
            data_.swap(other.data_);
            std::swap(nrow_, other.nrow_);
            std::swap(ncol_, other.ncol_);
        }

        return *this;
    }

    /// \brief Move assignment operator using a matrix with a different layout
    Matrix &operator=(transpose_type &&other)
    {
        resize(other.nrow_, other.ncol_);
        for (std::size_t i = 0; i != nrow_; ++i)
            for (std::size_t j = 0; j != ncol_; ++j)
                operator()(i, j) = std::move(other(i, j));

        return *this;
    }

    /// \brief Iterator to the upper-left corner of the matrix
    iterator begin() { return data(); }

    /// \brief Iterator to the upper-left corner of the matrix
    const_iterator begin() const { return cbegin(); }

    /// \brief Iterator to the upper-left corner of the matrix
    const_iterator cbegin() const { return data(); }

    /// \brief Iterator to one pass the lower-right corner of the matrix
    iterator end() { return begin() + nrow_ * ncol_; }

    /// \brief Iterator to one pass the lower-right corner of the matrix
    const_iterator end() const { return cend(); }

    /// \brief Iterator to one pass the lower-right corner of the matrix
    const_iterator cend() const { return begin() + nrow_ * ncol_; }

    /// \brief Iterator to the lower-right corner of the matrix
    reverse_iterator rbegin() { return reverse_iterator(end()); }

    /// \brief Iterator to the lower-right corner of the matrix
    const_reverse_iterator rbegin() const { return crbegin(); }

    /// \brief Iterator to the lower-right corner of the matrix
    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(cend());
    }

    /// \brief Iterator one before the upper-left corner of the matrix
    reverse_iterator rend() { return reverse_iterator(begin()); }

    /// \brief Iterator one before the upper-left corner of the matrix
    const_reverse_iterator rend() const { return crend(); }

    /// \brief Iterator one before the upper-left corner of the matrix
    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(cbegin());
    }

    /// \brief Iterator to the beginning of a given row
    row_iterator row_begin(size_type i)
    {
        return row_begin_dispatch(i, layout_dispatch());
    }

    /// \brief Iterator to the beginning of a given row
    const_row_iterator row_begin(size_type i) const { return row_cbegin(i); }

    /// \brief Iterator to the beginning of a given row
    const_row_iterator row_cbegin(size_type i) const
    {
        return row_begin_dispatch(i, layout_dispatch());
    }

    /// \brief Iterator to one pass the end of a given row
    row_iterator row_end(size_type i) { return row_begin(i) + ncol_; }

    /// \brief Iterator to one pass the end of a given row
    const_row_iterator row_end(size_type i) const { return row_cend(i); }

    /// \brief Iterator to one pass the end of a given row
    const_row_iterator row_cend(size_type i) const
    {
        return row_cbegin(i) + ncol_;
    }

    /// \brief Iterator to the end of a given row
    reverse_row_iterator row_rbegin(size_type i)
    {
        return reverse_row_iterator(row_end(i));
    }

    /// \brief Iterator to the end of a given row
    const_reverse_row_iterator row_rbegin(size_type i) const
    {
        return row_crbegin(i);
    }

    /// \brief Iterator to the end of a given row
    const_reverse_row_iterator row_crbegin(size_type i) const
    {
        return const_reverse_row_iterator(row_cend(i));
    }

    /// \brief Iterator to one before the beginning of a given row
    reverse_row_iterator row_rend(size_type i)
    {
        return reverse_row_iterator(row_begin(i));
    }

    /// \brief Iterator to one before the beginning of a given row
    const_reverse_row_iterator row_rend(size_type i) const
    {
        return row_crend(i);
    }

    /// \brief Iterator to one before the beginning of a given row
    const_reverse_row_iterator row_crend(size_type i) const
    {
        return const_reverse_row_iterator(row_cbegin(i));
    }

    /// \brief Iterator to the beginning of a given column
    col_iterator col_begin(size_type i)
    {
        return col_begin_dispatch(i, layout_dispatch());
    }

    /// \brief Iterator to the beginning of a given column
    const_col_iterator col_begin(size_type j) const { return col_cbegin(j); }

    /// \brief Iterator to the beginning of a given column
    const_col_iterator col_cbegin(size_type j) const
    {
        return col_begin_dispatch(j, layout_dispatch());
    }

    /// \brief Iterator to one pass the end of a given column
    col_iterator col_end(size_type j) { return col_begin(j) + nrow_; }

    /// \brief Iterator to one pass the end of a given column
    const_col_iterator col_end(size_type j) const { return col_cend(j); }

    /// \brief Iterator to one pass the end of a given column
    const_col_iterator col_cend(size_type j) const
    {
        return col_cbegin(j) + nrow_;
    }

    /// \brief Iterator to the end of a given column
    reverse_col_iterator col_rbegin(size_type j)
    {
        return reverse_col_iterator(col_end(j));
    }

    /// \brief Iterator to the end of a given column
    const_reverse_col_iterator col_rbegin(size_type j) const
    {
        return col_crbegin(j);
    }

    /// \brief Iterator to the end of a given column
    const_reverse_col_iterator col_crbegin(size_type j) const
    {
        return const_reverse_col_iterator(col_cend(j));
    }

    /// \brief Iterator to one before the beginning of a given column
    reverse_col_iterator col_rend(size_type j)
    {
        return reverse_col_iterator(col_begin(j));
    }

    /// \brief Iterator to one before the beginning of a given column
    const_reverse_col_iterator col_rend(size_type j) const
    {
        return col_crend(j);
    }

    /// \brief Iterator to one before the beginning of a given column
    const_reverse_col_iterator col_crend(size_type j) const
    {
        return const_reverse_col_iterator(col_cbegin(j));
    }

    /// \brief The element at row `i` and column `j`
    reference at(size_type i, size_type j)
    {
        runtime_assert<std::out_of_range>(
            i < nrow_, "**Matrix::at** row index out of range");
        runtime_assert<std::out_of_range>(
            j < ncol_, "**Matrix::at** column index out of range");

        return at_dispatch(i, j, layout_dispatch());
    }

    /// \brief The element at row `i` and column `j`
    const_reference at(size_type i, size_type j) const
    {
        runtime_assert<std::out_of_range>(
            i < nrow_, "**Matrix::at** row index out of range");
        runtime_assert<std::out_of_range>(
            j < ncol_, "**Matrix::at** column index out of range");

        return at_dispatch(i, j, layout_dispatch());
    }

    /// \brief The element at row `i` and column `j`
    reference operator()(size_type i, size_type j)
    {
        return at_dispatch(i, j, layout_dispatch());
    }

    /// \brief The element at row `i` and column `j`
    const_reference operator()(size_type i, size_type j) const
    {
        return at_dispatch(i, j, layout_dispatch());
    }

    /// \brief Pointer to the upper left corner of the matrix
    pointer data() { return data_.data(); }

    /// \brief Pointer to the upper left corner of the matrix
    const_pointer data() const { return data_.data(); }

    /// \brief Pointer to the first element of a row
    pointer row_data(size_type i)
    {
        return row_data_dispatch(i, layout_dispatch());
    }

    /// \brief Pointer to the first element of a column
    const_pointer row_data(size_type i) const
    {
        return row_data_dispatch(i, layout_dispatch());
    }

    /// \brief The stride of row-wise access through `row_data`
    size_type row_stride() const
    {
        return row_stride_dispatch(layout_dispatch());
    }

    /// \brief Pointer to the beginning of a column
    pointer col_data(size_type j)
    {
        return col_data_dispatch(j, layout_dispatch());
    }

    /// \brief Pointer to the beginning of a column
    const_pointer col_data(size_type j) const
    {
        return col_data_dispatch(j, layout_dispatch());
    }

    /// \brief The stride size of column-wise access through `col_data`
    size_type col_stride() const
    {
        return col_stride_dispatch(layout_dispatch());
    }

    /// \brief If the matrix is empty, either zero rows or zero columns or both
    bool empty() const { return nrow_ == 0 || ncol_ == 0; }

    /// \brief The number of rows
    size_type nrow() const { return nrow_; }

    /// \brief The number of columns
    size_type ncol() const { return ncol_; }

    /// \brief Reserve space for the matrix given the new number of rows and
    /// columns
    void reserve(size_type n, size_type m) { data_.reserve(n * m); }

    /// \brief Resize the matrix given new number of rows and columns
    void resize(size_type nrow, size_type ncol)
    {
        if (nrow * ncol == 0) {
            data_.clear();
            nrow_ = nrow;
            ncol_ = ncol;
            return;
        }

        if (nrow == nrow_ && ncol_ == ncol)
            return;

        resize_dispatch(nrow, ncol, layout_dispatch());
    }

    /// \brief Release memory no longer needed
    void shrink_to_fit() { data_.shrink_to_fit(); }

    /// \brief Clear the matrix
    void clear()
    {
        data_.clear();
        nrow_ = 0;
        ncol_ = 0;
    }

    /// \brief Swap two matrices
    void swap(Matrix &other) noexcept(noexcept(data_.swap(other.data_)))
    {
        std::swap(nrow_, other.nrow_);
        std::swap(ncol_, other.ncol_);
        data_.swap(other.data_);
    }

    /// \brief Swap contents of two matrices
    friend void swap(Matrix &m1, Matrix &m2) noexcept(noexcept(m1.swap(m2)))
    {
        m1.swap(m2);
    }

    /// \brief Compare equality of two matrices
    friend bool operator==(const Matrix &m1, const Matrix &m2)
    {
        if (m1.nrow_ != m2.nrow_)
            return false;
        if (m1.ncol_ != m2.ncol_)
            return false;
        return m1.data_ == m2.data_;
    }

    /// \brief Compare inequality of two matrices
    friend bool operator!=(const Matrix &m1, const Matrix &m2)
    {
        return !(m1 == m2);
    }

    private:
    // Layout == RowMajor

    pointer row_begin_dispatch(size_type i, row_major) { return row_data(i); }

    const_pointer row_begin_dispatch(size_type i, row_major) const
    {
        return row_data(i);
    }

    internal::MatrixIterator<T> col_begin_dispatch(size_type j, row_major)
    {
        return internal::MatrixIterator<T>(col_data(j), col_stride());
    }

    internal::MatrixIterator<const T> col_begin_dispatch(
        size_type j, row_major) const
    {
        return internal::MatrixIterator<const T>(col_data(j), col_stride());
    }

    void resize_dispatch(size_type nrow, size_type ncol, row_major)
    {
        if (ncol == ncol_) {
            data_.resize(nrow * ncol);
            nrow_ = nrow;
            return;
        }

        Matrix tmp(nrow, ncol);
        const size_type n = std::min(nrow, nrow_);
        const size_type m = std::min(ncol, ncol_);
        for (size_type i = 0; i != n; ++i)
            std::copy_n(row_data(i), m, tmp.row_data(i));
        swap(tmp);
    }

    reference at_dispatch(size_type i, size_type j, row_major)
    {
        return data_[i * ncol_ + j];
    }

    const_reference at_dispatch(size_type i, size_type j, row_major) const
    {
        return data_[i * ncol_ + j];
    }

    size_type row_stride_dispatch(row_major) const { return 1; }

    pointer row_data_dispatch(size_type i, row_major)
    {
        return data_.data() + i * ncol_;
    }

    const_pointer row_data_dispatch(size_type i, row_major) const
    {
        return data_.data() + i * ncol_;
    }

    size_type col_stride_dispatch(row_major) const { return ncol_; }

    pointer col_data_dispatch(size_type j, row_major)
    {
        return data_.data() + j;
    }

    const_pointer col_data_dispatch(size_type j, row_major) const
    {
        return data_.data() + j;
    }

    // Layout == ColMajor

    internal::MatrixIterator<T> row_begin_dispatch(size_type i, col_major)
    {
        return internal::MatrixIterator<T>(row_data(i), row_stride());
    }

    internal::MatrixIterator<const T> row_begin_dispatch(
        size_type i, col_major) const
    {
        return internal::MatrixIterator<const T>(row_data(i), row_stride());
    }

    pointer col_begin_dispatch(size_type j, col_major) { return col_data(j); }

    const_pointer col_begin_dispatch(size_type j, col_major) const
    {
        return col_data(j);
    }

    void resize_dispatch(size_type nrow, size_type ncol, col_major)
    {
        if (nrow == nrow_) {
            data_.resize(nrow * ncol);
            ncol_ = ncol;
            return;
        }

        Matrix tmp(nrow, ncol);
        const size_type n = std::min(nrow, nrow_);
        const size_type m = std::min(ncol, ncol_);
        for (size_type i = 0; i != m; ++i)
            std::copy_n(col_data(i), n, tmp.col_data(i));
        swap(tmp);
    }

    reference at_dispatch(size_type i, size_type j, col_major)
    {
        return data_[j * nrow_ + i];
    }

    const_reference at_dispatch(size_type i, size_type j, col_major) const
    {
        return data_[j * nrow_ + i];
    }

    size_type row_stride_dispatch(col_major) const { return nrow_; }

    pointer row_data_dispatch(size_type i, col_major)
    {
        return data_.data() + i;
    }

    const_pointer row_data_dispatch(size_type i, col_major) const
    {
        return data_.data() + i;
    }

    size_type col_stride_dispatch(col_major) const { return 1; }

    pointer col_data_dispatch(size_type j, col_major)
    {
        return data_.data() + j * nrow_;
    }

    const_pointer col_data_dispatch(size_type j, col_major) const
    {
        return data_.data() + j * nrow_;
    }
}; // class Matrix

} // namespace mckl

#endif // MCKL_CORE_MATRIX_HPP
