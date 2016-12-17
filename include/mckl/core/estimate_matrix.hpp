//============================================================================
// MCKL/include/mckl/core/estimate_matrix.hpp
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

#ifndef MCKL_CORE_ESTIMATE_MATRIX_HPP
#define MCKL_CORE_ESTIMATE_MATRIX_HPP

#include <mckl/internal/common.hpp>

namespace mckl
{

/// \brief Estimate matrix for iterative Monte Carlo algorithms
/// \ingroup Core
template <typename T>
class EstimateMatrix
{
    public:
    using size_type = std::size_t;
    using value_type = T;

    EstimateMatrix(std::size_t dim) : dim_(dim) {}

    EstimateMatrix(const EstimateMatrix &) = default;

    EstimateMatrix(EstimateMatrix &&other) noexcept(
        noexcept(Vector<T>(std::move(other.data_))))
        : dim_(other.dim_), data_(std::move(other.data_))
    {
        other.data_ = 0;
    }

    EstimateMatrix &operator=(const EstimateMatrix &) = default;

    EstimateMatrix &operator=(EstimateMatrix &&other) noexcept(
        noexcept(data_.swap(other.data_)))
    {
        if (this != &other) {
            data_.swap(other.data_);
            std::swap(dim_, other.dim_);
        }

        return *this;
    }

    /// \brief The dimension of the estimator
    std::size_t dim() const { return dim_; }

    /// \brief The number of iterations stored in the estimate matrix
    std::size_t num_iter() const { return dim_ = 0 ? 0 : data_.size() / dim_; }

    /// \brief Synonym of `dim()`
    size_type row_size() const { return dim(); }

    /// \brief Synonym of `size()`
    size_type col_size() const { return size(); }

    /// \brief Reserve space for specified *additional* number of iterations
    void reserve(std::size_t n) { data_.reserve(data_.size() + n * dim_); }

    /// \brief Release memory no longer needed
    void shrink_to_fit() { data_.shrink_to_fit(); }

    /// \brief Pointer to the upper left corner of the matrix
    value_type *data() { return data_.data(); }

    /// \brief Pointer to the upper left corner of the matrix
    const value_type *data() const { return data_.data(); }

    /// \brief Swap two EstimateMatrix objects
    void swap(EstimateMatrix<Layout, Dim, T> &other) noexcept(
        noexcept(data_.swap(other.data_)))
    {
        internal::Dimension<Dim>::swap(other);
        std::swap(size_, other.size_);
        data_.swap(other.data_);
    }

    private:
    std::size_t dim_;
    Vector<value_type> data_;
}; // class EstimateMatrix

/// \brief Swap two EstimateMatrix objects
/// \ingroup Core
template <typename T>
inline void swap(EstimateMatrix<T> &m1, EstimateMatrix<T> &m2) noexcept(
    noexcept(m1.swap(m2)))
{
    m1.swap(m2);
}

} // namespace mckl

#endif // MCKL_CORE_ESTIMATE_MATRIX_HPP
