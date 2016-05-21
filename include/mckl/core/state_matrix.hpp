//============================================================================
// MCKL/include/mckl/core/state_matrix.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2016, Yan Zhou
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

namespace mckl
{

namespace internal
{

template <std::size_t Dim>
class StateMatrixDim
{
    protected:
    void swap(StateMatrixDim<Dim> &) noexcept {}

    std::size_t get_dim() const { return Dim; }

    static void set_dim(std::size_t) {}
}; // class StateMatrixDim

template <>
class StateMatrixDim<Dynamic>
{
    protected:
    StateMatrixDim() : dim_(0) {}

    void swap(StateMatrixDim<Dynamic> &other) noexcept
    {
        std::swap(dim_, other.dim_);
    }

    std::size_t get_dim() const { return dim_; }

    void set_dim(std::size_t dim) { dim_ = dim; }

    private:
    std::size_t dim_;
}; // class StateMatrixDim

} // namespace mckl::internal

/// \brief Base type of StateMatrix
/// \ingroup Core
template <MatrixLayout Layout, std::size_t Dim, typename T>
class StateMatrixBase : private internal::StateMatrixDim<Dim>
{
    public:
    using size_type = std::size_t;
    using value_type = T;
    using pack_type = Vector<T>;

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
        value_type &operator()(size_type j) const
        {
            return this->particle().state()(
                static_cast<size_type>(this->i()), j);
        }

        /// \brief `this->particle().state().at(this->i(), j)`
        value_type &at(size_type j) const
        {
            runtime_assert(
                j < dim(), "**StateMatrix::at** index out of range");

            return operator()(j);
        }
    }; // class particle_index_type

    /// \brief The numbrer of particles
    size_type size() const { return size_; }

    /// \brief The dimension
    size_type dim() const { return this->get_dim(); }

    /// \brief Synonym of `dim()`
    size_type row_size() const { return dim(); }

    /// \brief Synonym of `size()`
    size_type col_size() const { return size(); }

    /// \brief Reserve space for specified sample size
    void reserve(size_type N) { reserve(N, dim()); }

    /// \brief Reserve space for specified sample size and dimension
    ///
    /// \details
    /// `dim` is ignored unless `Dim == Dynamic`.
    void reserve(size_type N, size_type dim)
    {
        data_.reserve(N * (Dim == Dynamic ? dim : this->dim()));
    }

    /// \brief Release memory no longer needed
    void shrink_to_fit() { data_.shrink_to_fit(); }

    /// \brief Pointer to the upper left corner of the matrix
    value_type *data() { return data_.data(); }

    /// \brief Pointer to the upper left corner of the matrix
    const value_type *data() const { return data_.data(); }

    /// \brief Swap two StateMatrix objects
    void swap(StateMatrixBase<Layout, Dim, T> &other) noexcept
    {
        internal::StateMatrixDim<Dim>::swap(other);
        std::swap(size_, other.size_);
        data_.swap(other.data_);
    }

    protected:
    explicit StateMatrixBase(size_type N) : size_(N), data_(N * Dim) {}

    StateMatrixBase(size_type N, size_type dim) : size_(N)
    {
        static_assert(Dim == Dynamic,
            "**StateMatrix::StateMatrix** used with an object with fixed "
            "dimension");

        resize_data(N, dim);
    }

    StateMatrixBase(const StateMatrixBase<Layout, Dim, T> &) = default;

    StateMatrixBase<Layout, Dim, T> &operator=(
        const StateMatrixBase<Layout, Dim, T> &) = default;

    StateMatrixBase(StateMatrixBase<Layout, Dim, T> &&) = default;

    StateMatrixBase<Layout, Dim, T> &operator=(
        StateMatrixBase<Layout, Dim, T> &&other) = default;

    void resize_data(size_type N, size_type dim)
    {
        size_ = N;
        this->set_dim(dim);
        data_.resize(N * dim);
    }

    size_type data_size() const { return data_.size(); }

    friend bool operator==(const StateMatrixBase<Layout, Dim, T> &state1,
        const StateMatrixBase<Layout, Dim, T> &state2)
    {
        if (state1.dim() != state2.dim())
            return false;
        if (state1.size_ != state2.size_)
            return false;
        if (state1.data_ != state2.data_)
            return false;
        return true;
    }

    friend bool operator!=(const StateMatrixBase<Layout, Dim, T> &state1,
        const StateMatrixBase<Layout, Dim, T> &state2)
    {
        return !(state1 == state2);
    }

    private:
    size_type size_;
    Vector<T> data_;
}; // class StateMatrixBase

/// \brief Swap two StateMatrix objects
/// \ingroup Core
template <MatrixLayout Layout, std::size_t Dim, typename T>
inline void swap(StateMatrixBase<Layout, Dim, T> &state1,
    StateMatrixBase<Layout, Dim, T> &state2) noexcept
{
    state1.swap(state2);
}

/// \brief Particle::value_type subtype
/// \ingroup Core
template <std::size_t Dim, typename T>
class StateMatrix<RowMajor, Dim, T> : public StateMatrixBase<RowMajor, Dim, T>
{
    public:
    using typename StateMatrixBase<RowMajor, Dim, T>::size_type;
    using typename StateMatrixBase<RowMajor, Dim, T>::value_type;
    using typename StateMatrixBase<RowMajor, Dim, T>::pack_type;

    /// \brief Construct a matrix with `N` rows and `Dim` columns
    explicit StateMatrix(size_type N = 0)
        : StateMatrixBase<RowMajor, Dim, T>(N)
    {
    }

    /// \brief Construct a matrix with `N` rows and `dim` columns, only usable
    /// when `Dim == Dynamic`
    StateMatrix(size_type N, size_type dim)
        : StateMatrixBase<RowMajor, Dim, T>(N, dim)
    {
    }

    /// \brief Change the sample size
    void resize(size_type N) { resize_both(N, this->dim()); }

    /// \brief Change the sample size and dimension, only usable when `Dim ==
    /// Dynamic`
    void resize(size_type N, size_type dim)
    {
        static_assert(Dim == Dynamic,
            "**StateMatrix::resize** used with an object with fixed "
            "dimension");

        resize_both(N, dim);
    }

    /// \brief Change the dimension, only usable when `Dim == Dynamic`
    void resize_dim(size_type dim)
    {
        static_assert(Dim == Dynamic,
            "**StateMatrix::resize_dim** used with an object with fixed "
            "dimension");

        resize_both(this->size(), dim);
    }

    /// \brief The element at row `i` and column `j`
    value_type &operator()(size_type i, size_type j)
    {
        return this->data()[i * this->dim() + j];
    }

    /// \brief The element at row `i` and column `j`
    const value_type &operator()(size_type i, size_type j) const
    {
        return this->data()[i * this->dim() + j];
    }

    /// \brief The element at row `i` and column `j`, with assertion
    value_type &at(size_type i, size_type j)
    {
        runtime_assert(i < this->size() && j < this->dim(),
            "**StateMatrix::at** index out of range");

        return operator()(i, j);
    }

    /// \brief The element at row `i` and column `j`, with assertion
    const value_type &at(size_type i, size_type j) const
    {
        runtime_assert(i < this->size() && j < this->dim(),
            "**StateMatrix::at** index out of range");

        return operator()(i, j);
    }

    /// \brief The stride size of row-wise access
    ///
    /// \details
    /// To iterate over a specific row `i`,
    /// ~~~{.cpp}
    /// auto stride = state.row_stride();
    /// auto data = state.row_data(i);
    /// auto size = state.row_size(); // or state.dim();
    /// for (j = 0; j != size; ++j, data += stride)
    ///     /* *data is the same as state(i, j) */;
    /// ~~~
    size_type row_stride() const { return 1; }

    /// \brief Pointer to the beginning of a row
    value_type *row_data(size_type i)
    {
        return this->data() + i * this->dim();
    }

    /// \brief Pointer to the beginning of a row
    const value_type *row_data(size_type i) const
    {
        return this->data() + i * this->dim();
    }

    /// \brief The stride size of column-wise access
    ///
    /// \details
    /// To iterate over a specific column `j`,
    /// ~~~{.cpp}
    /// auto stride = state.col_stride();
    /// auto data = state.col_data(j);
    /// auto size = state.col_size(); // or state.size();
    /// for (i = 0; i != size; ++i, data += stride)
    ///     /* *data is the same as state(i, j); */;
    /// ~~~
    size_type col_stride() const { return this->dim(); }

    /// \brief Pointer to the beginning of a column
    value_type *col_data(size_type j) { return this->data() + j; }

    /// \brief Pointer to the beginning of a column
    const value_type *col_data(size_type j) const { return this->data() + j; }

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
        size_type n = static_cast<size_type>(N);
        if (this->size() == 0 || internal::is_nullptr(index)) {
            this->resize(n);
            return;
        }

        if (n > this->size())
            this->resize(n);
        for (size_type dst = 0; dst != n; ++dst, ++index)
            duplicate(static_cast<size_type>(*index), dst);
        if (n < this->size())
            this->resize(n);

        return;
    }

    /// \brief Duplicate a sample
    ///
    /// \param src The index of sample to be duplicated
    /// \param dst The index of sample to be eliminated
    void duplicate(size_type src, size_type dst)
    {
        if (src == dst)
            return;

        duplicate_dispatch(src, dst, std::integral_constant < bool,
            Dim == Dynamic || 8 < Dim > ());
    }

    template <typename OutputIter>
    OutputIter read_row(size_type i, OutputIter first) const
    {
        return std::copy_n(row_data(i), this->dim(), first);
    }

    template <typename OutputIter>
    OutputIter read_col(size_type j, OutputIter first) const
    {
        using vtype = typename std::iterator_traits<OutputIter>::value_type;

        const size_type stride = col_stride();
        const value_type *src = col_data(j);
        for (size_type i = 0; i != this->size(); ++i, ++first, src += stride)
            *first = static_cast<vtype>(*src);

        return first;
    }

    template <typename OutputIter>
    OutputIter read(MatrixLayout layout, OutputIter first) const
    {
        runtime_assert(layout == RowMajor || layout == ColMajor,
            "**StateMatrix::read** invalid layout parameter");

        if (layout == RowMajor)
            first = std::copy_n(this->data(), this->data_size(), first);

        if (layout == ColMajor)
            for (size_type j = 0; j != this->dim(); ++j)
                first = read_col(j, first);

        return first;
    }

    pack_type state_pack(size_type i) const
    {
        pack_type pack(this->dim());
        std::copy_n(row_data(i), this->dim(), pack.data());

        return pack;
    }

    void state_unpack(size_type i, const pack_type &pack)
    {
        runtime_assert(pack.size() >= this->dim(),
            "**StateMatrix::unpack** pack size is too small");

        const value_type *ptr = pack.data();
        std::copy_n(ptr, this->dim(), row_data(i));
    }

    void state_unpack(size_type i, pack_type &&pack)
    {
        runtime_assert(pack.size() >= this->dim(),
            "**StateMatrix::unpack** pack size is too small");

        const value_type *ptr = pack.data();
        std::move(ptr, ptr + this->dim(), row_data(i));
    }

    private:
    void resize_both(size_type N, size_type dim)
    {
        if (N == this->size() && dim == this->dim())
            return;

        if (dim == this->dim()) {
            this->resize_data(N, dim);
            return;
        }

        StateMatrix<RowMajor, Dim, T> tmp;
        tmp.resize_data(N, dim);
        const size_type K = std::min(N, this->size());
        const size_type D = std::min(dim, this->dim());
        if (D > 0)
            for (size_type k = 0; k != K; ++k)
                std::copy_n(row_data(k), D, tmp.row_data(k));
        *this = std::move(tmp);
    }

    void duplicate_dispatch(size_type src, size_type dst, std::true_type)
    {
        std::copy(row_data(src), row_data(src) + this->dim(), row_data(dst));
    }

    void duplicate_dispatch(size_type src, size_type dst, std::false_type)
    {
        duplicate_j<0>(row_data(src), row_data(dst),
            std::integral_constant<bool, 0 < Dim>());
    }

    template <std::size_t D>
    void duplicate_j(const value_type *, value_type *, std::false_type)
    {
    }

    template <std::size_t D>
    void duplicate_j(const value_type *src, value_type *dst, std::true_type)
    {
        dst[D] = src[D];
        duplicate_j<D + 1>(
            src, dst, std::integral_constant<bool, D + 1 < Dim>());
    }
}; // class StateMatrix

/// \brief Particle::value_type subtype
/// \ingroup Core
template <std::size_t Dim, typename T>
class StateMatrix<ColMajor, Dim, T> : public StateMatrixBase<ColMajor, Dim, T>
{
    public:
    using typename StateMatrixBase<ColMajor, Dim, T>::size_type;
    using typename StateMatrixBase<ColMajor, Dim, T>::value_type;
    using typename StateMatrixBase<ColMajor, Dim, T>::pack_type;

    /// \brief Construct a matrix with `N` rows and `Dim` columns
    explicit StateMatrix(size_type N = 0)
        : StateMatrixBase<ColMajor, Dim, T>(N)
    {
    }

    /// \brief Construct a matrix with `N` rows and `dim` columns, only usable
    /// when `Dim == Dynamic`
    StateMatrix(size_type N, size_type dim)
        : StateMatrixBase<ColMajor, Dim, T>(N, dim)
    {
    }

    /// \brief Change the sample size
    void resize(size_type N) { resize_both(N, this->dim()); }

    /// \brief Change the sample size and dimension, only usable when `Dim ==
    /// Dynamic`
    void resize(size_type N, size_type dim)
    {
        static_assert(Dim == Dynamic,
            "**StateMatrix::resize** used with an object with fixed "
            "dimension");

        resize_both(N, dim);
    }

    /// \brief Change the dimension, only usable when `Dim == Dynamic`
    void resize_dim(size_type dim)
    {
        static_assert(Dim == Dynamic,
            "**StateMatrix::resize_dim** used with an object with fixed "
            "dimension");

        resize_both(this->size(), dim);
    }

    /// \brief The element at row `i` and column `j`
    value_type &operator()(size_type i, size_type j)
    {
        return this->data()[i + j * this->size()];
    }

    /// \brief The element at row `i` and column `j`
    const value_type &operator()(size_type i, size_type j) const
    {
        return this->data()[i + j * this->size()];
    }

    /// \brief The element at row `i` and column `j`, with assertion
    value_type &at(size_type i, size_type j)
    {
        runtime_assert(i < this->size() && j < this->dim(),
            "**StateMatrix::at** index out of range");

        return operator()(i, j);
    }

    /// \brief The element at row `i` and column `j`, with assertion
    const value_type &at(size_type i, size_type j) const
    {
        runtime_assert(i < this->size() && j < this->dim(),
            "**StateMatrix::at** index out of range");

        return operator()(i, j);
    }

    /// \brief The stride size of row-wise access
    ///
    /// \details
    /// To iterate over a specific row `i`,
    /// ~~~{.cpp}
    /// auto stride = state.row_stride();
    /// auto data = state.row_data(i);
    /// auto size = state.row_size(); // or state.dim();
    /// for (j = 0; j != size; ++j, data += stride)
    ///     /* *data is the same as state(i, j) */;
    /// ~~~
    size_type row_stride() const { return this->size(); }

    /// \brief Pointer to the beginning of a row
    value_type *row_data(size_type i) { return this->data() + i; }

    /// \brief Pointer to the beginning of a row
    const value_type *row_data(size_type i) const { return this->data() + i; }

    /// \brief The stride size of column-wise access
    ///
    /// \details
    /// To iterate over a specific column `j`,
    /// ~~~{.cpp}
    /// auto stride = state.col_stride();
    /// auto data = state.col_data(j);
    /// auto size = state.col_size(); // or state.size();
    /// for (i = 0; i != size; ++i, data += stride)
    ///     /* *data is the same as state(i, j); */;
    /// ~~~
    size_type col_stride() const { return 1; }

    /// \brief Pointer to the beginning of a column
    value_type *col_data(size_type j)
    {
        return this->data() + j * this->size();
    }

    /// \brief Pointer to the beginning of a column
    const value_type *col_data(size_type j) const
    {
        return this->data() + j * this->size();
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
        size_type n = static_cast<size_type>(N);
        if (this->size() == 0 || internal::is_nullptr(index)) {
            this->resize(n);
            return;
        }

        InputIter idx = index;
        if (n == this->size()) {
            for (size_type j = 0; j != this->dim(); ++j) {
                idx = index;
                const value_type *src = col_data(j);
                value_type *dst = col_data(j);
                for (size_type i = 0; i != n; ++i, ++idx)
                    dst[i] = src[*idx];
            }
        } else {
            StateMatrix<ColMajor, Dim, T> tmp;
            tmp.resize_data(N, this->dim());
            for (size_type j = 0; j != this->dim(); ++j) {
                idx = index;
                const value_type *src = col_data(j);
                value_type *dst = tmp.col_data(j);
                for (size_type i = 0; i != n; ++i, ++idx)
                    dst[i] = src[*idx];
            }
            *this = std::move(tmp);
        }

        return;
    }

    /// \brief Duplicate a sample
    ///
    /// \param src The index of sample to be duplicated
    /// \param dst The index of sample to be eliminated
    void duplicate(size_type src, size_type dst)
    {
        if (src == dst)
            return;

        duplicate_dispatch(src, dst, std::integral_constant < bool,
            Dim == Dynamic || 8 < Dim > ());
    }

    template <typename OutputIter>
    OutputIter read_row(size_type i, OutputIter first) const
    {
        using vtype = typename std::iterator_traits<OutputIter>::value_type;

        const size_type stride = row_stride();
        const value_type *src = row_data(i);
        for (size_type j = 0; j != this->dim(); ++j, ++first, src += stride)
            *first = static_cast<vtype>(*src);
    }

    template <typename OutputIter>
    OutputIter read_col(size_type j, OutputIter first) const
    {
        return std::copy_n(col_data(j), this->size(), first);
    }

    template <typename OutputIter>
    OutputIter read(MatrixLayout layout, OutputIter first) const
    {
        runtime_assert(layout == RowMajor || layout == ColMajor,
            "**StateMatrix::read** invalid layout parameter");

        if (layout == RowMajor)
            for (size_type i = 0; i != this->size(); ++i)
                first = read_row(i, first);

        if (layout == ColMajor)
            first = std::copy_n(this->data(), this->data_size(), first);

        return first;
    }

    pack_type state_pack(size_type i) const
    {
        pack_type pack(this->dim());
        for (size_type j = 0; j != this->dim(); ++j)
            pack[j] = operator()(i, j);

        return pack;
    }

    void state_unpack(size_type i, const pack_type &pack)
    {
        runtime_assert(pack.size() >= this->dim(),
            "**StateMatrix::unpack** pack size is too small");

        for (size_type j = 0; j != this->dim(); ++j)
            operator()(i, j) = pack[j];
    }

    void state_unpack(size_type i, pack_type &&pack)
    {
        runtime_assert(pack.size() >= this->dim(),
            "**StateMatrix::unpack** pack size is too small");

        for (size_type j = 0; j != this->dim(); ++j)
            operator()(i, j) = std::move(pack[j]);
    }

    private:
    void resize_both(size_type N, size_type dim)
    {
        if (N == this->size() && dim == this->dim())
            return;

        if (N == this->size()) {
            this->resize_data(N, dim);
            return;
        }

        StateMatrix<ColMajor, Dim, T> tmp;
        tmp.resize_data(N, dim);
        const size_type K = std::min(N, this->size());
        const size_type D = std::min(dim, this->dim());
        if (K > 0)
            for (size_type d = 0; d != D; ++d)
                std::copy_n(col_data(d), K, tmp.col_data(d));
        *this = std::move(tmp);
    }

    void duplicate_dispatch(size_type src, size_type dst, std::true_type)
    {
        for (size_type d = 0; d != this->dim(); ++d)
            operator()(dst, d) = operator()(src, d);
    }

    void duplicate_dispatch(size_type src, size_type dst, std::false_type)
    {
        duplicate_j<0>(this->data() + src, this->data() + dst,
            std::integral_constant<bool, 0 < Dim>());
    }

    template <std::size_t D>
    void duplicate_j(const value_type *, value_type *, std::false_type)
    {
    }

    template <std::size_t D>
    void duplicate_j(const value_type *src, value_type *dst, std::true_type)
    {
        dst[D * this->size()] = src[D * this->size()];
        duplicate_j<D + 1>(
            src, dst, std::integral_constant<bool, D + 1 < Dim>());
    }
}; // class StateMatrix

} // namespace mckl

#endif // MCKL_CORE_STATE_MATRIX_HPP
