//============================================================================
// MCKL/example/core/include/core_matrix.hpp
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

#ifndef MCKL_EXAMPLE_CORE_MATRIX_HPP
#define MCKL_EXAMPLE_CORE_MATRIX_HPP

#include <mckl/core/matrix.hpp>
#include <mckl/random/rng.hpp>
#include <mckl/random/uniform_int_distribution.hpp>

template <mckl::MatrixLayout Layout>
using CoreMatrix = mckl::Matrix<Layout, std::pair<std::size_t, std::size_t>>;

template <mckl::MatrixLayout Layout>
inline void core_matrix_fill(CoreMatrix<Layout> &m)
{
    for (std::size_t i = 0; i != m.nrow(); ++i)
        for (std::size_t j = 0; j != m.ncol(); ++j)
            m(i, j) = std::make_pair(i, j);
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check(const CoreMatrix<Layout> &m,
    std::size_t nrow = std::numeric_limits<std::size_t>::max(),
    std::size_t ncol = std::numeric_limits<std::size_t>::max())
{
    nrow = nrow == std::numeric_limits<std::size_t>::max() ?
        m.nrow() :
        std::min(nrow, m.nrow());
    ncol = ncol == std::numeric_limits<std::size_t>::max() ?
        m.ncol() :
        std::min(ncol, m.ncol());
    for (std::size_t i = 0; i != nrow; ++i)
        for (std::size_t j = 0; j != ncol; ++j)
            if (m(i, j) != std::make_pair(i, j))
                return false;
    return true;
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check_new(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t nrow = rsize(rng);
        std::size_t ncol = rsize(rng);
        CoreMatrix<Layout> m(nrow, ncol);
        core_matrix_fill(m);
        if (!core_matrix_check(m))
            return false;
    }
    return true;
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check_copy_new(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t nrow = rsize(rng);
        std::size_t ncol = rsize(rng);
        CoreMatrix<Layout> m0(nrow, ncol);
        core_matrix_fill(m0);
        CoreMatrix<Layout> m1(m0);
        if (!core_matrix_check(m0))
            return false;
        if (!core_matrix_check(m1))
            return false;
    }
    return true;
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check_move_new(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t nrow = rsize(rng);
        std::size_t ncol = rsize(rng);
        CoreMatrix<Layout> m0(nrow, ncol);
        core_matrix_fill(m0);
        CoreMatrix<Layout> m1(std::move(m0));
        if (!core_matrix_check(m0))
            return false;
        if (!core_matrix_check(m1))
            return false;
    }
    return true;
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check_copy_assign(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t nrow = rsize(rng);
        std::size_t ncol = rsize(rng);
        CoreMatrix<Layout> m0(nrow, ncol);
        core_matrix_fill(m0);
        CoreMatrix<Layout> m1(ncol, nrow);
        core_matrix_fill(m1);
        m1 = m0;
        if (!core_matrix_check(m0))
            return false;
        if (!core_matrix_check(m1))
            return false;
        if (m0 != m1)
            return false;
    }
    return true;
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check_move_assign(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t nrow = rsize(rng);
        std::size_t ncol = rsize(rng);
        CoreMatrix<Layout> m0(nrow, ncol);
        core_matrix_fill(m0);
        CoreMatrix<Layout> m1(ncol, nrow);
        core_matrix_fill(m1);
        m1 = std::move(m0);
        if (!core_matrix_check(m0))
            return false;
        if (!core_matrix_check(m1))
            return false;
    }
    return true;
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check_nrow(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t nrow = rsize(rng);
        std::size_t ncol = rsize(rng);
        CoreMatrix<Layout> m(nrow, ncol);
        if (m.nrow() != nrow)
            return false;
    }
    return true;
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check_ncol(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t nrow = rsize(rng);
        std::size_t ncol = rsize(rng);
        CoreMatrix<Layout> m(nrow, ncol);
        if (m.ncol() != ncol)
            return false;
    }
    return true;
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check_resize(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t nrow0 = rsize(rng);
        std::size_t ncol0 = rsize(rng);
        std::size_t nrow1 = rsize(rng);
        std::size_t ncol1 = rsize(rng);
        CoreMatrix<Layout> m(nrow0, ncol0);
        core_matrix_fill(m);
        m.resize(nrow1, ncol1);
        if (!core_matrix_check(m, nrow0, ncol0))
            return false;
    }
    return true;
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check_shrink_to_fit(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t nrow0 = rsize(rng);
        std::size_t ncol0 = rsize(rng);
        std::size_t nrow1 = rsize(rng);
        std::size_t ncol1 = rsize(rng);
        CoreMatrix<Layout> m(nrow0, ncol0);
        core_matrix_fill(m);
        m.resize(nrow1, ncol1);
        m.shrink_to_fit();
        if (!core_matrix_check(m, nrow0, ncol0))
            return false;
    }
    return true;
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check_read_row(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t nrow = 0;
        while (nrow == 0)
            nrow = rsize(rng);
        std::size_t ncol = rsize(rng);
        mckl::UniformIntDistribution<std::size_t> ridx(0, nrow - 1);
        std::size_t idx = ridx(rng);
        CoreMatrix<Layout> m(nrow, ncol);
        core_matrix_fill(m);
        mckl::Vector<std::pair<std::size_t, std::size_t>> row(ncol);
        m.read_row(idx, row.data());
        for (std::size_t j = 0; j != ncol; ++j)
            if (row[j] != std::make_pair(idx, j))
                return false;
    }
    return true;
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check_read_col(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t nrow = rsize(rng);
        std::size_t ncol = 0;
        while (ncol == 0)
            ncol = rsize(rng);
        mckl::UniformIntDistribution<std::size_t> ridx(0, ncol - 1);
        std::size_t idx = ridx(rng);
        CoreMatrix<Layout> m(nrow, ncol);
        core_matrix_fill(m);
        mckl::Vector<std::pair<std::size_t, std::size_t>> col(nrow);
        m.read_col(idx, col.data());
        for (std::size_t j = 0; j != nrow; ++j)
            if (col[j] != std::make_pair(j, idx))
                return false;
    }
    return true;
}

template <mckl::MatrixLayout Layout>
inline bool core_matrix_check_read(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t nrow = rsize(rng);
        std::size_t ncol = rsize(rng);
        CoreMatrix<Layout> m(nrow, ncol);
        core_matrix_fill(m);
        mckl::Vector<std::pair<int, int>> mat(nrow * ncol);
        m.read(mckl::RowMajor, mat.data());
        CoreMatrix<mckl::RowMajor> row(nrow, ncol);
        std::copy(mat.begin(), mat.end(), row.data());
        if (!core_matrix_check(row))
            return false;
        m.read(mckl::ColMajor, mat.data());
        CoreMatrix<mckl::ColMajor> col(nrow, ncol);
        std::copy(mat.begin(), mat.end(), col.data());
        if (!core_matrix_check(col))
            return false;
    }
    return true;
}

inline void core_matrix(std::size_t N, std::size_t M, const std::string &mf,
    bool (*check)(std::size_t, std::size_t))
{
    std::cout << std::setw(20) << std::left << mf;
    std::cout << (check(N, M) ? "Passed" : "Failed") << std::endl;
}

template <mckl::MatrixLayout Layout>
inline void core_matrix(std::size_t N, std::size_t M)
{
    core_matrix(N, M, "new", core_matrix_check_new<Layout>);
    core_matrix(N, M, "copy_new", core_matrix_check_copy_new<Layout>);
    core_matrix(N, M, "move_new", core_matrix_check_move_new<Layout>);
    core_matrix(N, M, "copy_assign", core_matrix_check_copy_assign<Layout>);
    core_matrix(N, M, "move_assign", core_matrix_check_move_assign<Layout>);
    core_matrix(N, M, "nrow", core_matrix_check_nrow<Layout>);
    core_matrix(N, M, "ncol", core_matrix_check_ncol<Layout>);
    core_matrix(N, M, "resize", core_matrix_check_resize<Layout>);
    core_matrix(
        N, M, "shrink_to_fit", core_matrix_check_shrink_to_fit<Layout>);
    core_matrix(N, M, "read_row", core_matrix_check_read_row<Layout>);
    core_matrix(N, M, "read_col", core_matrix_check_read_col<Layout>);
    core_matrix(N, M, "read", core_matrix_check_read<Layout>);
}

inline void core_matrix(std::size_t N, std::size_t M)
{
    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Layout RowMajor" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    core_matrix<mckl::RowMajor>(N, M);
    std::cout << std::string(80, '-') << std::endl;

    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Layout ColMajor" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    core_matrix<mckl::ColMajor>(N, M);
    std::cout << std::string(80, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_CORE_MATRIX_HPP
