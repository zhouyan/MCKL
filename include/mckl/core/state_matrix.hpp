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

namespace mckl
{

/// \brief Particle::state_type subtype
/// \ingroup Core
template <MatrixLayout Layout, std::size_t Dim, typename T>
class StateMatrix : public Matrix<Layout, T>
{
    using major = std::integral_constant<MatrixLayout, Layout>;
    using row_major = std::integral_constant<MatrixLayout, RowMajor>;
    using col_major = std::integral_constant<MatrixLayout, ColMajor>;

    public:
    using size_type = typename Matrix<Layout, T>::size_type;
    using value_type = typename Matrix<Layout, T>::value_type;

    template <typename S>
    class particle_index_type : public ParticleIndexBase<S>
    {
        public:
        particle_index_type(
            typename Particle<S>::size_type i, Particle<S> *pptr)
            : ParticleIndexBase<S>(i, pptr)
        {
        }

        /// \brief `this->particle().state().dim()`
        std::size_t dim() const { return this->particle().state().dim(); }

        /// \brief `this->particle().state().row_stride()`
        size_type stride() const
        {
            return this->particle().state().row_stride();
        }

        /// \brief `this->particle().state().row_data(this->i())`
        value_type *data() const
        {
            return this->particle().state().row_data(this->i());
        }

        /// \brief `this->particle().state()(this->i(), j)`
        value_type &operator[](size_type j) const
        {
            return this->particle().state()(
                static_cast<size_type>(this->i()), j);
        }

        /// \brief `this->particle().state()(this->i(), j)`
        value_type &operator()(size_type j) const { return operator[](j); }

        /// \brief `this->particle().state().at(this->i(), j)`
        value_type &at(size_type j) const
        {
            runtime_assert(
                j < dim(), "**StateMatrix::at** index out of range");

            return operator[](j);
        }
    }; // class particle_index_type

    explicit StateMatrix(size_type N) : Matrix<Layout, T>(N, Dim) {}

    StateMatrix(size_type N, size_type dim) : Matrix<Layout, T>(N, dim)
    {
        static_assert(Dim == 0,
            "**StateMatrix::StateMatrix** used with an object with fixed "
            "dimension");
    }

    /// \brief The numbrer of samples
    size_type size() const { return this->nrow(); }

    /// \brief The dimension
    size_type dim() const { return this->ncol(); }

    /// \brief Number of elements in the matrix
    size_type data_size() const { return this->nrow() * this->ncol(); }

    /// \brief Reserve space for specified sample size
    void reserve(size_type N) { this->reserve_nrow(N); }

    /// \brief Reserve space for specified sample size and dimension
    ///
    /// \details
    /// `dim` is ignored unless `Dim == 0`.
    void reserve(size_type N, size_type dim)
    {
        Matrix<Layout, T>::reserve(N, Dim == 0 ? dim : dim());
    }

    using Matrix<Layout, T>::resize;

    /// \brief Change the sample size
    void resize(size_type N) { this->resize_nrow(N); }

    /// \brief Change the dimension, only usable when `Dim == 0`
    void resize_dim(size_type dim)
    {
        static_assert(Dim == 0,
            "**StateMatrix::resize_dim** used with an object with fixed "
            "dimension");

        this->resize_ncol(dim);
    }

    /// \brief Select samples
    ///
    /// \param N The new sample size
    /// \param index N-vector of parent index
    ///
    /// \details
    /// Let \f$a_i\f$ denote the value of `index[i]`, and
    /// \f$r_i = \sum_{j=1}^N \mathbb{I}_{\{i\}}(a_j)\f$. Then it is required
    /// that \f$a_i = i\f$ for all \f$r_i > 0\f$.
    template <typename IntType, typename InputIter>
    void select(IntType N, InputIter index)
    {
        select_dispatch(N, index, major());
    }

    /// \brief Duplicate a sample
    ///
    /// \param src The index of sample to be duplicated
    /// \param dst The index of sample to be eliminated
    void duplicate(size_type src, size_type dst)
    {
        if (src == dst)
            return;

        duplicate_dispatch(src, dst, major(),
            std::integral_constant<bool, (Dim == 0 || 8 < Dim)>());
    }

    private:
    template <typename IntType, typename InputIter>
    void select_dispatch(IntType N, InputIter index, row_major)
    {
        size_type n = static_cast<size_type>(N);
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

    template <typename IntType, typename InputIter>
    void select_dispatch(IntType N, InputIter index, col_major)
    {
        size_type n = static_cast<size_type>(N);
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
            Matrix<ColMajor, T> tmp(N, dim());
            for (size_type j = 0; j != dim(); ++j) {
                idx = index;
                const value_type *src = this->col_data(j);
                value_type *dst = tmp.col_data(j);
                for (size_type i = 0; i != n; ++i, ++idx)
                    dst[i] = src[*idx];
            }
            Matrix<ColMajor, T>::operator=(std::move(tmp));
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
