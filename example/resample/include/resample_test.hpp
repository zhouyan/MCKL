//============================================================================
// MCKL/example/resample/include/resample_test.hpp
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

#ifndef MCKL_EXAMPLE_RESAMPLE_TEST_HPP
#define MCKL_EXAMPLE_RESAMPLE_TEST_HPP

#include <mckl/core/state_matrix.hpp>
#include <mckl/core/weight.hpp>
#include <mckl/random/rng.hpp>
#include <mckl/resample.hpp>
#include <mckl/utility/stop_watch.hpp>

template <mckl::MatrixLayout Layout>
using ResampleState = mckl::StateMatrix<Layout, mckl::Dynamic, int>;

template <typename RNGType>
inline mckl::Vector<std::size_t> resample_size(
    RNGType &rng, std::size_t N, std::size_t dim, bool fixed)
{
    if (fixed)
        return mckl::Vector<std::size_t>(dim + 1, N);

    mckl::Vector<std::size_t> size;
    std::uniform_int_distribution<std::size_t> runif(N / 2, N * 2);
    for (std::size_t d = 0; d != dim; ++d)
        size.push_back(runif(rng));
    size.push_back(N);

    return size;
}

template <typename RNGType>
inline mckl::Vector<mckl::Vector<int>> resample_value(
    RNGType &rng, const mckl::Vector<std::size_t> &size)
{
    mckl::Vector<mckl::Vector<int>> value;
    const std::size_t dim = size.size() - 1;
    for (std::size_t d = 0; d != dim; ++d) {
        const std::size_t N = size[d];
        mckl::Vector<typename RNGType::result_type> tmpr(N);
        mckl::Vector<int> tmpi(N);
        mckl::rand(rng, N, tmpr.data());
        std::copy(tmpr.begin(), tmpr.end(), tmpi.begin());
        value.push_back(std::move(tmpi));
    }

    return value;
}

template <typename RNGType>
inline mckl::Vector<mckl::Vector<double>> resample_weight(
    RNGType &rng, const mckl::Vector<std::size_t> &size)
{
    mckl::Vector<mckl::Vector<double>> weight;
    const std::size_t dim = size.size() - 1;
    for (std::size_t d = 0; d != dim; ++d) {
        std::size_t N = size[d];
        mckl::Vector<double> tmp(N);
        mckl::Weight w(N);
        mckl::u01_distribution(rng, N, tmp.data());
        w.set(tmp.data());
        w.read(tmp.data());
        weight.push_back(std::move(tmp));
    }

    return weight;
}

template <typename RNGType>
inline mckl::Vector<mckl::Vector<std::size_t>> resample_index(RNGType &rng,
    const mckl::Vector<std::size_t> &size,
    const mckl::Vector<mckl::Vector<double>> &weight)
{
    mckl::Vector<mckl::Vector<size_t>> index;
    mckl::Vector<std::size_t> rep;
    mckl::Vector<std::size_t> idx;
    const std::size_t dim = size.size() - 1;
    for (std::size_t d = 0; d != dim; ++d) {
        const std::size_t N = size[d];
        const std::size_t M = size[d + 1];
        rep.resize(N);
        idx.resize(M);
        mckl::ResampleMultinomial resample;
        resample(N, M, rng, weight[d].data(), rep.data());
        mckl::resample_trans_rep_index(N, M, rep.data(), idx.data());
        index.push_back(idx);
    }

    return index;
}

#endif // MCKL_EXAMPLE_RESAMPLE_TEST_HPP
