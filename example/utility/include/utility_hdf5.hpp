//============================================================================
// MCKL/example/utility/include/utility_hdf5.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
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

#ifndef MCKL_EXAMPLE_UTILITY_HDF5_HPP
#define MCKL_EXAMPLE_UTILITY_HDF5_HPP

#include <mckl/random/rng.hpp>
#include <mckl/random/u01_distribution.hpp>
#include <mckl/random/uniform_int_distribution.hpp>
#include <mckl/utility/hdf5.hpp>
#include <mckl/utility/stop_watch.hpp>

template <typename>
inline std::string utility_hdf5_typename();

template <>
inline std::string utility_hdf5_typename<float>()
{
    return "float";
}

template <>
inline std::string utility_hdf5_typename<double>()
{
    return "double";
}

template <mckl::MatrixLayout>
inline std::string utility_hdf5_layoutname();

template <>
inline std::string utility_hdf5_layoutname<mckl::RowMajor>()
{
    return "RowMajor";
}

template <>
inline std::string utility_hdf5_layoutname<mckl::ColMajor>()
{
    return "ColMajor";
}

template <typename T1, typename T2>
inline void utility_hdf5_vector(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);
    mckl::U01Distribution<T1> u01;

    const std::string filename("utility_hdf5.h5");
    const std::string dataname("data");
    mckl::StopWatch watch1;
    mckl::StopWatch watch2;
    bool passed = true;
    std::size_t n = 0;
    for (std::size_t k = 0; k != M; ++k) {
        const std::size_t m = k == 0 ? 0 : rsize(rng);
        n += m;

        mckl::Vector<T1> v1(m);
        mckl::rand(rng, u01, m, v1.data());

        mckl::HDF5File h5file(filename, false);
        watch1.start();
        mckl::hdf5store(h5file, dataname, v1);
        watch1.stop();

        mckl::Vector<T2> v2;
        watch2.start();
        mckl::hdf5load(h5file, dataname, &v2);
        watch2.stop();

        passed = passed && v1.size() == v2.size();
        if (!passed)
            break;

        mckl::Vector<float> f1(m);
        mckl::Vector<float> f2(m);
        std::copy(v1.begin(), v1.end(), f1.begin());
        std::copy(v2.begin(), v2.end(), f2.begin());
        passed = passed && f1 == f2;
        if (!passed)
            break;
    }

    const std::string n1("Vector<" + utility_hdf5_typename<T1>() + ">");
    const std::string n2("Vector<" + utility_hdf5_typename<T2>() + ">");
    const std::size_t bytes1 = n * sizeof(T1);
    const std::size_t bytes2 = n * sizeof(T2);

    std::cout << std::setw(30) << std::left << n1;
    std::cout << std::setw(30) << std::left << n2;
    std::cout << std::setw(10) << std::right
              << 1e-6 * bytes1 / watch1.seconds();
    std::cout << std::setw(10) << std::right
              << 1e-6 * bytes2 / watch2.seconds();
    std::cout << std::setw(15) << std::right << (passed ? "Passed" : "Failed");
    std::cout << std::endl;
}

template <typename T1, typename T2>
inline void utility_hdf5_append(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);
    mckl::U01Distribution<T1> u01;

    const std::string filename("utility_hdf5.h5");
    const std::string dataname("data");
    mckl::StopWatch watch1;
    mckl::StopWatch watch2;
    bool passed = true;
    std::size_t n = 0;
    for (std::size_t k = 0; k != M; ++k) {
        const std::size_t m = k == 0 ? 0 : rsize(rng);
        n += m;

        mckl::UniformIntDistribution<std::size_t> rchunk(0, m / 10);

        mckl::Vector<T1> v1;
        mckl::Vector<mckl::Vector<T1>> chunks;
        std::size_t remain = m;
        while (remain > 0) {
            std::size_t chunk = std::min(rchunk(rng), remain);
            mckl::Vector<T1> v(chunk);
            mckl::rand(rng, u01, chunk, v.data());
            v1.insert(v1.end(), v.begin(), v.end());
            chunks.push_back(std::move(v));
            remain -= chunk;
        }
        if (chunks.empty()) {
            chunks.push_back(mckl::Vector<T1>());
            chunks.push_back(mckl::Vector<T1>());
        }

        mckl::HDF5File h5file(filename, false);
        watch1.start();
        mckl::hdf5store(h5file, dataname, chunks.front(), false, true);
        for (std::size_t i = 1; i < chunks.size(); ++i) {
            mckl::hdf5append(h5file, dataname, chunks[i]);
        }
        watch1.stop();

        mckl::Vector<T2> v2;
        watch2.start();
        mckl::hdf5load(h5file, dataname, &v2);
        watch2.stop();

        passed = passed && v1.size() == v2.size();
        if (!passed)
            break;

        mckl::Vector<float> f1(m);
        mckl::Vector<float> f2(m);
        std::copy(v1.begin(), v1.end(), f1.begin());
        std::copy(v2.begin(), v2.end(), f2.begin());
        passed = passed && f1 == f2;
        if (!passed)
            break;
    }

    const std::string n1("Append<" + utility_hdf5_typename<T1>() + ">");
    const std::string n2("Append<" + utility_hdf5_typename<T2>() + ">");
    const std::size_t bytes1 = n * sizeof(T1);
    const std::size_t bytes2 = n * sizeof(T2);

    std::cout << std::setw(30) << std::left << n1;
    std::cout << std::setw(30) << std::left << n2;
    std::cout << std::setw(10) << std::right
              << 1e-6 * bytes1 / watch1.seconds();
    std::cout << std::setw(10) << std::right
              << 1e-6 * bytes2 / watch2.seconds();
    std::cout << std::setw(15) << std::right << (passed ? "Passed" : "Failed");
    std::cout << std::endl;
}

template <mckl::MatrixLayout Layout1, mckl::MatrixLayout Layout2, typename T1,
    typename T2>
inline void utility_hdf5_matrix(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(0, N);
    mckl::U01Distribution<T1> u01;

    const std::string filename("utility_hdf5.h5");
    const std::string dataname("data");
    mckl::StopWatch watch1;
    mckl::StopWatch watch2;
    bool passed = true;
    std::size_t n = 0;
    for (std::size_t k = 0; k != M; ++k) {
        const std::size_t nrow = k == 0 ? 0 : rsize(rng);
        const std::size_t ncol = k == 1 ? 0 : rsize(rng);
        n += nrow * ncol;

        mckl::Matrix<T1, Layout1> m1(nrow, ncol);
        mckl::rand(rng, u01, nrow * ncol, m1.data());

        mckl::HDF5File h5file(filename, false);
        watch1.start();
        mckl::hdf5store(h5file, dataname, m1);
        watch1.stop();

        mckl::Matrix<T2, Layout2> m2;
        watch2.start();
        mckl::hdf5load(h5file, dataname, &m2);
        watch2.stop();

        passed = passed && m1.nrow() == m2.nrow() && m1.ncol() == m2.ncol();
        if (!passed)
            break;

        mckl::Matrix<float, mckl::RowMajor> f1(nrow, ncol);
        mckl::Matrix<float, mckl::RowMajor> f2(nrow, ncol);
        for (std::size_t i = 0; i != nrow; ++i)
            for (std::size_t j = 0; j != ncol; ++j)
                f1(i, j) = static_cast<float>(m1(i, j));
        for (std::size_t i = 0; i != nrow; ++i)
            for (std::size_t j = 0; j != ncol; ++j)
                f2(i, j) = static_cast<float>(m2(i, j));
        passed = passed && f1 == f2;
        if (!passed)
            break;
    }

    const std::string n1("Matrix<" + utility_hdf5_typename<T1>() + ", " +
        utility_hdf5_layoutname<Layout1>() + ">");
    const std::string n2("Matrix<" + utility_hdf5_typename<T2>() + ", " +
        utility_hdf5_layoutname<Layout2>() + ">");
    const std::size_t bytes1 = n * sizeof(T1);
    const std::size_t bytes2 = n * sizeof(T2);

    std::cout << std::setw(30) << std::left << n1;
    std::cout << std::setw(30) << std::left << n2;
    std::cout << std::setw(10) << std::right
              << 1e-6 * bytes1 / watch1.seconds();
    std::cout << std::setw(10) << std::right
              << 1e-6 * bytes2 / watch2.seconds();
    std::cout << std::setw(15) << std::right << (passed ? "Passed" : "Failed");
    std::cout << std::endl;
}

inline void utility_hdf5(std::size_t N, std::size_t M)
{
    std::cout << std::string(95, '=') << std::endl;

    std::cout << std::setw(30) << std::left << "Data (S)";
    std::cout << std::setw(30) << std::left << "Data (L)";
    std::cout << std::setw(10) << std::right << "MB/s (S)";
    std::cout << std::setw(10) << std::right << "MB/s (L)";
    std::cout << std::setw(15) << std::right << "Determinstics" << std::endl;

    std::cout << std::string(95, '-') << std::endl;

    utility_hdf5_vector<float, float>(N * N / 4, M);
    utility_hdf5_vector<float, double>(N * N / 4, M);
    utility_hdf5_vector<double, float>(N * N / 4, M);
    utility_hdf5_vector<double, double>(N * N / 4, M);

    std::cout << std::string(95, '-') << std::endl;

    utility_hdf5_append<float, float>(N * N / 4, M);
    utility_hdf5_append<float, double>(N * N / 4, M);
    utility_hdf5_append<double, float>(N * N / 4, M);
    utility_hdf5_append<double, double>(N * N / 4, M);

    std::cout << std::string(95, '-') << std::endl;

    utility_hdf5_matrix<mckl::RowMajor, mckl::RowMajor, float, float>(N, M);
    utility_hdf5_matrix<mckl::RowMajor, mckl::RowMajor, float, double>(N, M);
    utility_hdf5_matrix<mckl::RowMajor, mckl::RowMajor, double, float>(N, M);
    utility_hdf5_matrix<mckl::RowMajor, mckl::RowMajor, double, double>(N, M);

    std::cout << std::string(95, '-') << std::endl;

    utility_hdf5_matrix<mckl::ColMajor, mckl::ColMajor, float, float>(N, M);
    utility_hdf5_matrix<mckl::ColMajor, mckl::ColMajor, float, double>(N, M);
    utility_hdf5_matrix<mckl::ColMajor, mckl::ColMajor, double, float>(N, M);
    utility_hdf5_matrix<mckl::ColMajor, mckl::ColMajor, double, double>(N, M);

    std::cout << std::string(95, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_UTILITY_HDF5_HPP
