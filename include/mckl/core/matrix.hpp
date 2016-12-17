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

/// \brief Matrix
/// \ingroup Core
template <MatrixLayout Layout, typename T>
class Matrix
{
    static_assert(Layout == RowMajor || Layout == ColMajor,
        "**Matrix** used with Layout other than RowMajor or ColMajor");

    using major = std::integral_constant<MatrixLayout, Layout>;
    using row_major = std::integral_constant<MatrixLayout, RowMajor>;
    using col_major = std::integral_constant<MatrixLayout, ColMajor>;

    public:
    using size_type = std::size_t;
    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;

    /// \brief Construct the matrix given number of rows and columns
    Matrix(size_type nrow, size_type ncol)
        : nrow_(nrow), ncol_(ncol), data_(nrow * ncol)
    {
    }

    Matrix(const Matrix<Layout, T> &) = default;

    Matrix(Matrix<Layout, T> &&other) noexcept(
        noexcept(Vector<T>(std::move(other.data_))))
        : nrow_(other.nrow_), ncol_(other.ncol_), data_(std::move(other.data_))
    {
        other.nrow_ = 0;
        other.ncol_ = 0;
        other.data_.clear();
    }

    Matrix<Layout, T> &operator=(const Matrix<Layout, T> &) = default;

    Matrix<Layout, T> &operator=(Matrix<Layout, T> &&other) noexcept(
        noexcept(data_.swap(other.data_)))
    {
        if (this != &other) {
            data_.swap(other.data_);
            std::swap(nrow_, other.nrow_);
            std::swap(ncol_, other.ncol_);
        }

        return *this;
    }

    /// \brief Number of rows
    size_type nrow() const { return nrow_; }

    /// \brief Number of columns
    size_type ncol() const { return ncol_; }

    /// \brief Resize the number of rows and columns
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

        resize_dispatch(nrow, ncol, major());
    }

    /// \brief Resize the number of rows
    void resize_nrow(size_type nrow) { resize(nrow, ncol_); }

    /// \brief Resize the number of columns
    void resize_ncol(size_type ncol) { resize(nrow_, ncol); }

    /// \brief Reserve space given number of rows and columns
    void reserve(size_type n, size_type m) { data_.reserve(n * m); }

    /// \brief Reserve space given number of rows
    void reserve_nrow(size_type n) { reserve(n, ncol()); }

    /// \brief Reserve space given number of columns
    void reserve_ncol(size_type m) { reserve(nrow(), m); }

    /// \brief Release memory no longer needed
    void shrink_to_fit() { data_.shrink_to_fit(); }

    /// \brief Pointer to the upper left corner of the matrix
    pointer data() { return data_.data(); }

    /// \brief Pointer to the upper left corner of the matrix
    const_pointer data() const { return data_.data(); }

    /// \brief The element at row `i` and column `j`
    reference operator()(size_type i, size_type j)
    {
        return at_dispatch(i, j, major());
    }

    /// \brief The element at row `i` and column `j`
    const_reference operator()(size_type i, size_type j) const
    {
        return at_dispatch(i, j, major());
    }

    /// \brief The element at row `i` and column `j`
    reference at(size_type i, size_type j)
    {
        runtime_assert(i < nrow_, "**Matrix::at** row index out of range");
        runtime_assert(j < ncol_, "**Matrix::at** column index out of range");

        return at_dispatch(i, j, major());
    }

    /// \brief The element at row `i` and column `j`
    const_reference at(size_type i, size_type j) const
    {
        runtime_assert(i < nrow_, "**Matrix::at** row index out of range");
        runtime_assert(j < ncol_, "**Matrix::at** column index out of range");

        return at_dispatch(i, j, major());
    }

    /// \brief The stride of row-wise access
    ///
    /// \details
    /// To iterate over a specific row `i`,
    /// ~~~{.cpp}
    /// auto stride = m.row_stride();
    /// auto data = m.row_data(i);
    /// auto size = m.row_size();
    /// for (j = 0; j != size; ++j, data += stride)
    ///     /* *data is the same as m(i, j) */;
    /// ~~~
    size_type row_stride() const { return row_stride_dispatch(major()); }

    /// \brief Pointer to the beginning of a row
    pointer row_data(size_type i) { return row_data_dispatch(i, major()); }

    /// \brief Pointer to the beginning of a row
    const_pointer row_data(size_type i) const
    {
        return row_data_dispatch(i, major());
    }

    /// \brief The stride size of column-wise access
    ///
    /// \details
    /// To iterate over a specific column `j`,
    /// ~~~{.cpp}
    /// auto stride = m.col_stride();
    /// auto data = m.col_data(j);
    /// auto size = m.col_size();
    /// for (i = 0; i != size; ++i, data += stride)
    ///     /* *data is the same as m(i, j); */;
    /// ~~~
    size_type col_stride() const { return col_stride_dispatch(major()); }

    /// \brief Pointer to the beginning of a column
    pointer col_data(size_type j) { return col_data_dispatch(j, major()); }

    /// \brief Pointer to the beginning of a column
    const_pointer col_data(size_type j) const
    {
        return col_data_dispatch(j, major());
    }

    /// \brief Swap two Matrix objects
    void swap(Matrix<Layout, T> &other) noexcept(
        noexcept(data_.swap(other.data_)))
    {
        std::swap(nrow_, other.nrow_);
        std::swap(ncol_, other.ncol_);
        data_.swap(other.data_);
    }

    /// \brief Read a row with an output iterator
    template <typename OutputIter>
    OutputIter read_row(size_type i, OutputIter first) const
    {
        return read_row_dispatch(i, first, major());
    }

    /// \brief Read a column with an output iterator
    template <typename OutputIter>
    OutputIter read_col(size_type i, OutputIter first) const
    {
        return read_col_dispatch(i, first, major());
    }

    /// \brief Read the matrix with an output iterator given a new layout
    template <typename OutputIter>
    OutputIter read(MatrixLayout layout, OutputIter first) const
    {
        runtime_assert(layout == RowMajor || layout == ColMajor,
            "**Matrix::read** used with invalid layout parameter");

        if (layout == Layout)
            return std::copy(data_.begin(), data_.end(), first);

        return transpose_dispatch(first, major());
    }

    friend bool operator==(
        const Matrix<Layout, T> &m1, const Matrix<Layout, T> &m2)
    {
        if (m1.nrow_ != m2.nrow_)
            return false;
        if (m1.ncol_ != m2.ncol_)
            return false;
        return m1.data_ == m2.data_;
    }

    friend bool operator!=(
        const Matrix<Layout, T> &m1, const Matrix<Layout, T> &m2)
    {
        return !(m1 == m2);
    }

    private:
    size_type nrow_;
    size_type ncol_;
    Vector<T> data_;

    // Layout == RowMajor

    void resize_dispatch(size_type nrow, size_type ncol, row_major)
    {
        if (ncol == ncol_) {
            data_.resize(nrow * ncol);
            nrow_ = nrow;
            return;
        }

        Matrix<Layout, T> tmp(nrow, ncol);
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

    template <typename OutputIter>
    OutputIter read_row_dispatch(
        size_type i, OutputIter first, row_major) const
    {
        return std::copy_n(row_data(i), ncol_, first);
    }

    template <typename OutputIter>
    OutputIter read_col_dispatch(
        size_type j, OutputIter first, row_major) const
    {
        const_pointer p = col_data(j);
        const size_type s = col_stride();
        for (std::size_t i = 0; i != nrow_; ++i, p += s)
            *first++ = *p;

        return first;
    }

    template <typename OutputIter>
    OutputIter transpose_dispatch(OutputIter first, row_major) const
    {
        for (size_type j = 0; j != ncol_; ++j)
            first = read_col(j, first);

        return first;
    }

    // Layout == ColMajor

    void resize_dispatch(size_type nrow, size_type ncol, col_major)
    {
        if (nrow == nrow_) {
            data_.resize(nrow * ncol);
            ncol_ = ncol;
            return;
        }

        Matrix<Layout, T> tmp(nrow, ncol);
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

    template <typename OutputIter>
    OutputIter read_row_dispatch(
        size_type i, OutputIter first, col_major) const
    {
        const_pointer p = row_data(i);
        const size_type s = row_stride();
        for (std::size_t j = 0; j != ncol_; ++j, p += s)
            *first++ = *p;

        return first;
    }

    template <typename OutputIter>
    OutputIter read_col_dispatch(
        size_type j, OutputIter first, col_major) const
    {
        return std::copy_n(col_data(j), nrow_, first);
    }

    template <typename OutputIter>
    OutputIter transpose_dispatch(OutputIter first, col_major) const
    {
        for (size_type i = 0; i != nrow_; ++i)
            first = read_row(i, first);

        return first;
    }
}; // class Matrix

} // namespace mckl

#endif // MCKL_CORE_MATRIX_HPP
