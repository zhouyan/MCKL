//============================================================================
// MCKL/include/mckl/core/state_matrix.hpp
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

#ifndef MCKL_CORE_STATE_MATRIX_HPP
#define MCKL_CORE_STATE_MATRIX_HPP

#include <mckl/internal/common.hpp>
#include <mckl/core/matrix.hpp>
#include <mckl/core/particle.hpp>

namespace mckl
{

/// \brief Particle::state_type subtype
/// \ingroup Core
///
/// \tparam Layout The storage layout, either RowMajor or ColMajor
/// \tparam Dim The initial dimension of the samples
/// \tparam T The value type of the samples
///
/// \details
/// Let \f$X\f$ be \f$D\f$-vector of samples of type `T`. A collection of
/// \f$N\f$ samples is stored as an \f$N\f$ by \f$D\f$ matrix. The value of the
/// template parameter `Dim` only specifies the initial value of \f$D\f$.
template <MatrixLayout Layout, typename T, std::size_t Dim = 0>
class StateMatrix : public Matrix<T, Layout>
{
    using layout_dispatch = std::integral_constant<MatrixLayout, Layout>;
    using row_major = std::integral_constant<MatrixLayout, RowMajor>;
    using col_major = std::integral_constant<MatrixLayout, ColMajor>;

  public:
    using matrix_type = Matrix<T, Layout>;
    using value_type = typename matrix_type::value_type;
    using size_type = typename matrix_type::size_type;
    using difference_type = typename matrix_type::difference_type;
    using reference = typename matrix_type::reference;
    using pointer = typename matrix_type::pointer;

    /// \brief ParticleIndex base class
    template <typename S>
    class particle_index_type : public ParticleIndexBase<S>
    {
      public:
        using iterator = typename matrix_type::row_iterator;
        using const_iterator = typename matrix_type::const_row_iterator;
        using reverse_iterator = typename matrix_type::reverse_row_iterator;
        using const_reverse_iterator =
            typename matrix_type::const_reverse_row_iterator;

        particle_index_type() = default;

        particle_index_type(
            typename Particle<S>::size_type i, Particle<S> *pptr)
            : ParticleIndexBase<S>(i, pptr)
        {
        }

        /// \brief `this->state().row_begin(this->i())`
        iterator begin() const { return this->state().row_begin(this->i()); }

        /// \brief `this->state().row_cbegin(this->i())`
        const_iterator cbegin() const
        {
            return this->state().row_cbegin(this->i());
        }

        /// \brief `this->state().row_end(this->i())`
        iterator end() const { return this->state().row_end(this->i()); }

        /// \brief `this->state().row_cend(this->i())`
        const_iterator cend() const
        {
            return this->state().row_cend(this->i());
        }

        /// \brief `this->state().row_rbegin(this->i())`
        reverse_iterator rbegin() const
        {
            return this->state().row_rbegin(this->i());
        }

        /// \brief `this->state().row_crbegin(this->i())`
        const_reverse_iterator crbegin() const
        {
            return this->state().row_crbegin(this->i());
        }

        /// \brief `this->state().row_rend(this->i())`
        reverse_iterator rend() const
        {
            return this->state().row_rend(this->i());
        }

        /// \brief `this->state().row_crend(this->i())`
        const_reverse_iterator crend() const
        {
            return this->state().row_crend(this->i());
        }

        /// \brief `this->state().at(this->i(), j)`
        reference at(size_type j) const
        {
            return this->state().at(this->i(), j);
        }

        /// \brief `this->state()(this->i(), j)`
        reference operator()(size_type j) const
        {
            return this->state()(this->i(), j);
        }

        /// \brief `this->state().row_data(this->i())`
        pointer data() const { return this->state().row_data(this->i()); }

        /// \brief `this->state().row_stride()`
        size_type stride() const { return this->state().row_stride(); }

        /// \brief `this->state().empty()`
        bool empty() const { return this->state().empty(); }

        /// \brief `this->state().dim()`
        size_type dim() const { return this->state().dim(); }
    }; // class particle_index_type

    /// \brief `Matrix::Matrix(N, Dim)`
    explicit StateMatrix(size_type N = 0) : matrix_type(N, Dim) {}

    /// \brief `Matrix::Matrix(N, dim)`
    StateMatrix(size_type N, size_type dim) : matrix_type(N, dim)
    {
        static_assert(Dim == 0,
            "**StateMatrix::StateMatrix** used with an object with fixed "
            "dimension");
    }

    /// \brief `Matrix::nrow()`
    size_type size() const { return this->nrow(); }

    /// \brief `Matrix::ncol()`
    size_type dim() const { return this->ncol(); }

    using matrix_type::reserve;

    /// \brief `Matrix::reserve(N, dim())`
    void reserve(size_type N) { this->reserve(N, dim()); }

    using matrix_type::resize;

    /// \brief `Matrix::resize(N, dim())`
    void resize(size_type N) { this->resize(N, dim()); }

    /// \brief Select samples in-place
    ///
    /// \param n The new sample size
    /// \param index N-vector of parent index
    ///
    /// \pre Let \f$a_i\f$ denote the value of `index[i]`, and
    /// \f$r_i = \sum_{j=1}^N \mathbb{I}_{\{i\}}(a_j)\f$. Then it is required
    /// that \f$a_i = i\f$ for all \f$r_i > 0\f$.
    ///
    /// \post `size() == N`
    /// \post For \f$i = 1,\dots,N\f$, \f$Y_i = X_{a_i}\f$, where \f$Y_i\f$ is
    /// the \f$i\f$-th row of the new matrix and \f$X_j\f$ is the \f$j\f$-th
    /// row of the original matrix
    template <typename InputIter>
    void select(size_type n, InputIter index)
    {
        select_dispatch(n, index, layout_dispatch());
    }

    /// \brief Duplicate a sample
    ///
    /// \param src The index of sample to be duplicated
    /// \param dst The index of sample to be eliminated
    ///
    /// \post Let \f$i\f$ and \f$j\f$ be the values of `src` and `dst`,
    /// respectivley, then \f$X_j = X_i\f$, where \f$X_i\f$ is the \f$i\f$-th
    /// row of the matrix
    void duplicate(size_type src, size_type dst)
    {
        if (src == dst)
            return;

        duplicate_dispatch(src, dst, layout_dispatch(),
            std::integral_constant<bool, (Dim == 0 || 8 < Dim)>());
    }

  private:
    template <typename InputIter>
    void select_dispatch(size_type n, InputIter index, row_major)
    {
        if (size() == 0 || internal::is_nullptr(index)) {
            resize(n);
            return;
        }

        if (n > size())
            resize(n);
        for (size_type dst = 0; dst != n; ++dst, ++index)
            duplicate(static_cast<size_type>(*index), dst);
        if (n < size())
            resize(n);

        return;
    }

    template <typename InputIter>
    void select_dispatch(size_type n, InputIter index, col_major)
    {
        if (size() == 0 || internal::is_nullptr(index)) {
            resize(n);
            return;
        }

        InputIter idx = index;
        if (n == size()) {
            for (size_type j = 0; j != dim(); ++j) {
                idx = index;
                const value_type *src = this->col_data(j);
                value_type *dst = this->col_data(j);
                for (size_type i = 0; i != n; ++i, ++idx)
                    dst[i] = src[*idx];
            }
        } else {
            Matrix<T, ColMajor> tmp(n, dim());
            for (size_type j = 0; j != dim(); ++j) {
                idx = index;
                const value_type *src = this->col_data(j);
                value_type *dst = tmp.col_data(j);
                for (size_type i = 0; i != n; ++i, ++idx)
                    dst[i] = src[*idx];
            }
            Matrix<T, ColMajor>::operator=(std::move(tmp));
        }

        return;
    }

    void duplicate_dispatch(
        size_type src, size_type dst, row_major, std::true_type)
    {
        std::copy_n(this->row_data(src), dim(), this->row_data(dst));
    }

    void duplicate_dispatch(
        size_type src, size_type dst, row_major, std::false_type)
    {
        duplicate_j<0>(this->row_data(src), this->row_data(dst), row_major(),
            std::integral_constant<bool, 0 < Dim>());
    }

    template <std::size_t>
    void duplicate_j(
        const value_type *, value_type *, row_major, std::false_type)
    {
    }

    template <std::size_t D>
    void duplicate_j(
        const value_type *src, value_type *dst, row_major, std::true_type)
    {
        dst[D] = src[D];
        duplicate_j<D + 1>(src, dst, row_major(),
            std::integral_constant<bool, D + 1 < Dim>());
    }

    void duplicate_dispatch(
        size_type src, size_type dst, col_major, std::true_type)
    {
        for (size_type d = 0; d != dim(); ++d)
            this->operator()(dst, d) = this->operator()(src, d);
    }

    void duplicate_dispatch(
        size_type src, size_type dst, col_major, std::false_type)
    {
        duplicate_j<0>(this->row_data(src), this->col_data(dst), col_major(),
            std::integral_constant<bool, 0 < Dim>());
    }

    template <std::size_t>
    void duplicate_j(
        const value_type *, value_type *, col_major, std::false_type)
    {
    }

    template <std::size_t D>
    void duplicate_j(
        const value_type *src, value_type *dst, col_major, std::true_type)
    {
        dst[D * size()] = src[D * size()];
        duplicate_j<D + 1>(
            src, dst, std::integral_constant<bool, D + 1 < Dim>());
    }
}; // class StateMatrix

} // namespace mckl

#endif // MCKL_CORE_STATE_MATRIX_HPP
