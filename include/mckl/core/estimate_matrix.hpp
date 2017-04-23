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
#include <mckl/core/matrix.hpp>

namespace mckl
{

/// \brief Estimate matrix for iterative Monte Carlo algorithms
/// \ingroup Core
///
/// \details
/// Let \f$\eta\f$ be an estimator of dimension \f$d\f$, and \f$t\f$ be the
/// number of iterations of the algorithms. The estimates are collected in an
/// \f$t\f$ by \f$d\f$ matrix \f$E\f$, where \f$E_{i,j} = \eta_i(j)\f$, the
/// \f$j\f$-th component of the an estimate at iteration \f$i\f$.
template <typename T>
class EstimateMatrix : public Matrix<T, RowMajor>
{
    public:
    using matrix_type = Matrix<T, RowMajor>;
    using value_type = typename matrix_type::value_type;
    using size_type = typename matrix_type::size_type;
    using estimate_range = typename matrix_type::row_range;
    using variable_range = typename matrix_type::col_range;
    using const_estimate_range = typename matrix_type::const_row_range;
    using const_variable_range = typename matrix_type::const_col_range;

    EstimateMatrix(size_type dim) : matrix_type(0, dim) {}

    /// \brief The dimension of the estimator
    std::size_t dim() const noexcept { return this->ncol(); }

    /// \brief The number of iterations stored in the estimate matrix
    std::size_t num_iter() const noexcept { return this->nrow(); }

    /// \brief Clear the estimate matrix but preserve the dimension
    void clear() { this->resize(0, dim()); }

    /// \brief Reserve space for *additional* iterations
    void reserve(std::size_t n)
    {
        matrix_type::reserve(num_iter() + n, dim());
    }

    /// \brief Range of an estimate
    estimate_range estimate(size_type i) { return this->row_range(i); }

    /// \brief Range of an estimate
    const_estimate_range estimate(size_type i) const
    {
        return this->row_range(i);
    }

    /// \brief Range of a variable
    variable_range variable(size_type j) { return this->col_range(j); }

    /// \brief Range of a variable
    const_variable_range variable(size_type j) const
    {
        return this->col_range(j);
    }

    /// \brief Add space for a new estimate, return a pointer to the new row
    T *insert_estimate()
    {
        const size_type i = num_iter();
        this->resize(i + 1, dim());

        return this->row_data(i);
    }

    /// \brief Add a new estimate to the bottom of the matrix
    template <typename InputIter>
    void insert_estimate(InputIter first)
    {
        this->push_back_row(first);
    }

    /// \brief Add a new estimate into the matrix given iteration number
    ///
    /// \details
    /// Let \f$t\f$ be the number of iterations currently stored within the
    /// matrix. If \f$i < t\f$, then the estimate of \f$i\f$-th iteration is
    /// overriden. If \f$i = t\f$, then it is added to the bottom of the
    /// matrix. If \f$i > t\f$. Then the matrix will have \f$i + 1\f$
    /// iterations, and the values of estimates of iterations \f$t\f$ to
    /// \f$i - 1\f$ will be initialized as `-const_nan<T>()` if `T` is a
    /// floating point type.
    template <typename InputIter>
    void insert_estimate(size_type i, InputIter first)
    {
        const size_type t = num_iter();
        if (i < t)
            std::copy_n(first, dim(), this->row_begin(i));
        if (i == t)
            insert_estimate(first);
        if (i > t)
            insert_estimate_dispatch(t, i, first, std::is_floating_point<T>());
    }

    private:
    using matrix_type::resize;
    using matrix_type::push_back_col;

    template <typename InputIter>
    void insert_estimate_dispatch(
        size_type t, size_type i, InputIter first, std::true_type)
    {
        insert_estimate_dispatch(t, i, first, std::false_type());
        if (i > t)
            std::fill_n(this->row_begin(t), (i - t) * dim(), -const_nan<T>());
    }

    template <typename InputIter>
    void insert_estimate_dispatch(
        size_type t, size_type i, InputIter first, std::false_type)
    {
        this->resize(std::max(i + 1, t), dim());
        std::copy_n(first, dim(), this->row_begin(i));
    }
}; // class EstimateMatrix

} // namespace mckl

#endif // MCKL_CORE_ESTIMATE_MATRIX_HPP
