//============================================================================
// MCKL/example/core/include/core_matrix.hpp
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

#ifndef MCKL_EXAMPLE_CORE_MATRIX_HPP
#define MCKL_EXAMPLE_CORE_MATRIX_HPP

#include <mckl/core/matrix.hpp>
#include <mckl/random/rng.hpp>
#include <mckl/random/uniform_int_distribution.hpp>
#include <mckl/utility/stop_watch.hpp>

template <mckl::MatrixLayout Layout>
inline void core_matrix(std::size_t N, std::size_t M)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::uint64_t> rsize(0, N);

    bool pass1 = true;
    bool pass2 = true;
    bool pass3 = true;
    bool pass4 = true;
    bool pass5 = true;
    bool pass6 = true;
    mckl::StopWatch watch1;
    mckl::StopWatch watch2;
    mckl::StopWatch watch3;
    mckl::StopWatch watch4;
    mckl::StopWatch watch5;
    mckl::StopWatch watch6;
    std::uint64_t l = 0;
    for (std::size_t k = 0; k != M; ++k) {
        const std::uint64_t n = rsize(rng);
        const std::uint64_t m = rsize(rng);
        l += n * m;
        mckl::Matrix<std::uint64_t, Layout> mat(
            static_cast<std::size_t>(n), static_cast<std::size_t>(m));
        for (std::uint64_t i = 0; i != n; ++i) {
            for (std::uint64_t j = 0; j != m; ++j) {
                mat(static_cast<std::size_t>(i), static_cast<std::size_t>(j)) =
                    (i << 32) + j;
            }
        }
        const mckl::Matrix<std::uint64_t, Layout> cmat(mat);

        constexpr std::uint64_t zero = 0;

        watch1.start();
        std::uint64_t r1 = ((m * n * (n - 1) / 2) << 32) + n * m * (m - 1) / 2;
        std::uint64_t s1 = std::accumulate(mat.begin(), mat.end(), zero);
        pass1 = pass1 && s1 == r1;
        watch1.stop();
        s1 = std::accumulate(mat.cbegin(), mat.cend(), zero);
        pass1 = pass1 && s1 == r1;
        s1 = std::accumulate(cmat.begin(), cmat.end(), zero);
        pass1 = pass1 && s1 == r1;
        s1 = 0;
        for (auto v : mat)
            s1 += v;
        pass1 = pass1 && s1 == r1;
        s1 = 0;
        for (auto v : cmat)
            s1 += v;
        pass1 = pass1 && s1 == r1;

        watch2.start();
        std::uint64_t r2 = ((m * n * (n - 1) / 2) << 32) + n * m * (m - 1) / 2;
        std::uint64_t s2 = std::accumulate(mat.rbegin(), mat.rend(), zero);
        pass2 = pass2 && s2 == r2;
        watch2.stop();
        s2 = std::accumulate(mat.crbegin(), mat.crend(), zero);
        pass2 = pass2 && s2 == r2;
        s2 = std::accumulate(cmat.rbegin(), cmat.rend(), zero);
        pass2 = pass2 && s2 == r2;

        watch3.start();
        for (std::uint64_t i = 0; i != n; ++i) {
            std::uint64_t r = ((m * i) << 32) + m * (m - 1) / 2;
            std::uint64_t s =
                std::accumulate(mat.row_begin(static_cast<std::size_t>(i)),
                    mat.row_end(static_cast<std::size_t>(i)), zero);
            pass3 = pass3 && s == r;
        }
        watch3.stop();
        for (std::uint64_t i = 0; i != n; ++i) {
            std::uint64_t r = ((m * i) << 32) + m * (m - 1) / 2;
            std::uint64_t s =
                std::accumulate(mat.row_cbegin(static_cast<std::size_t>(i)),
                    mat.row_cend(static_cast<std::size_t>(i)), zero);
            pass3 = pass3 && s == r;
        }
        for (std::uint64_t i = 0; i != n; ++i) {
            std::uint64_t r = ((m * i) << 32) + m * (m - 1) / 2;
            std::uint64_t s =
                std::accumulate(cmat.row_begin(static_cast<std::size_t>(i)),
                    cmat.row_end(static_cast<std::size_t>(i)), zero);
            pass3 = pass3 && s == r;
        }
        for (std::uint64_t i = 0; i != n; ++i) {
            std::uint64_t r = ((m * i) << 32) + m * (m - 1) / 2;
            std::uint64_t s = 0;
            for (auto v : mat.row(static_cast<std::size_t>(i)))
                s += v;
            pass3 = pass3 && s == r;
        }
        for (std::uint64_t i = 0; i != n; ++i) {
            std::uint64_t r = ((m * i) << 32) + m * (m - 1) / 2;
            std::uint64_t s = 0;
            for (auto v : mat.crow(static_cast<std::size_t>(i)))
                s += v;
            pass3 = pass3 && s == r;
        }
        for (std::uint64_t i = 0; i != n; ++i) {
            std::uint64_t r = ((m * i) << 32) + m * (m - 1) / 2;
            std::uint64_t s = 0;
            for (auto v : cmat.row(static_cast<std::size_t>(i)))
                s += v;
            pass3 = pass3 && s == r;
        }

        watch4.start();
        for (std::uint64_t i = 0; i != n; ++i) {
            std::uint64_t r = ((m * i) << 32) + m * (m - 1) / 2;
            std::uint64_t s =
                std::accumulate(mat.row_rbegin(static_cast<std::size_t>(i)),
                    mat.row_rend(static_cast<std::size_t>(i)), zero);
            pass4 = pass4 && s == r;
        }
        watch4.stop();
        for (std::uint64_t i = 0; i != n; ++i) {
            std::uint64_t r = ((m * i) << 32) + m * (m - 1) / 2;
            std::uint64_t s =
                std::accumulate(mat.row_crbegin(static_cast<std::size_t>(i)),
                    mat.row_crend(static_cast<std::size_t>(i)), zero);
            pass4 = pass4 && s == r;
        }
        for (std::uint64_t i = 0; i != n; ++i) {
            std::uint64_t r = ((m * i) << 32) + m * (m - 1) / 2;
            std::uint64_t s =
                std::accumulate(cmat.row_rbegin(static_cast<std::size_t>(i)),
                    cmat.row_rend(static_cast<std::size_t>(i)), zero);
            pass4 = pass4 && s == r;
        }
        for (std::uint64_t i = 0; i != n; ++i) {
            std::uint64_t r = ((m * i) << 32) + m * (m - 1) / 2;
            std::uint64_t s = 0;
            for (auto v : mat.rrow(static_cast<std::size_t>(i)))
                s += v;
            pass4 = pass4 && s == r;
        }
        for (std::uint64_t i = 0; i != n; ++i) {
            std::uint64_t r = ((m * i) << 32) + m * (m - 1) / 2;
            std::uint64_t s = 0;
            for (auto v : mat.crrow(static_cast<std::size_t>(i)))
                s += v;
            pass4 = pass4 && s == r;
        }
        for (std::uint64_t i = 0; i != n; ++i) {
            std::uint64_t r = ((m * i) << 32) + m * (m - 1) / 2;
            std::uint64_t s = 0;
            for (auto v : cmat.rrow(static_cast<std::size_t>(i)))
                s += v;
            pass4 = pass4 && s == r;
        }

        watch5.start();
        for (std::uint64_t j = 0; j != m; ++j) {
            std::uint64_t r = ((n * (n - 1) / 2) << 32) + n * j;
            std::uint64_t s =
                std::accumulate(mat.col_begin(static_cast<std::size_t>(j)),
                    mat.col_end(static_cast<std::size_t>(j)), zero);
            pass5 = pass5 && s == r;
        }
        watch5.stop();
        for (std::uint64_t j = 0; j != m; ++j) {
            std::uint64_t r = ((n * (n - 1) / 2) << 32) + n * j;
            std::uint64_t s =
                std::accumulate(mat.col_cbegin(static_cast<std::size_t>(j)),
                    mat.col_cend(static_cast<std::size_t>(j)), zero);
            pass5 = pass5 && s == r;
        }
        for (std::uint64_t j = 0; j != m; ++j) {
            std::uint64_t r = ((n * (n - 1) / 2) << 32) + n * j;
            std::uint64_t s =
                std::accumulate(cmat.col_begin(static_cast<std::size_t>(j)),
                    cmat.col_end(static_cast<std::size_t>(j)), zero);
            pass5 = pass5 && s == r;
        }
        for (std::uint64_t j = 0; j != m; ++j) {
            std::uint64_t r = ((n * (n - 1) / 2) << 32) + n * j;
            std::uint64_t s = 0;
            for (auto v : mat.col(j))
                s += v;
            pass5 = pass5 && s == r;
        }
        for (std::uint64_t j = 0; j != m; ++j) {
            std::uint64_t r = ((n * (n - 1) / 2) << 32) + n * j;
            std::uint64_t s = 0;
            for (auto v : mat.ccol(j))
                s += v;
            pass5 = pass5 && s == r;
        }
        for (std::uint64_t j = 0; j != m; ++j) {
            std::uint64_t r = ((n * (n - 1) / 2) << 32) + n * j;
            std::uint64_t s = 0;
            for (auto v : cmat.col(j))
                s += v;
            pass5 = pass5 && s == r;
        }

        watch6.start();
        for (std::uint64_t j = 0; j != m; ++j) {
            std::uint64_t r = ((n * (n - 1) / 2) << 32) + n * j;
            std::uint64_t s =
                std::accumulate(mat.col_rbegin(static_cast<std::size_t>(j)),
                    mat.col_rend(static_cast<std::size_t>(j)), zero);
            pass6 = pass6 && s == r;
        }
        watch6.stop();
        for (std::uint64_t j = 0; j != m; ++j) {
            std::uint64_t r = ((n * (n - 1) / 2) << 32) + n * j;
            std::uint64_t s =
                std::accumulate(mat.col_crbegin(static_cast<std::size_t>(j)),
                    mat.col_crend(static_cast<std::size_t>(j)), zero);
            pass6 = pass6 && s == r;
        }
        for (std::uint64_t j = 0; j != m; ++j) {
            std::uint64_t r = ((n * (n - 1) / 2) << 32) + n * j;
            std::uint64_t s =
                std::accumulate(cmat.col_rbegin(static_cast<std::size_t>(j)),
                    cmat.col_rend(static_cast<std::size_t>(j)), zero);
            pass6 = pass6 && s == r;
        }
        for (std::uint64_t j = 0; j != m; ++j) {
            std::uint64_t r = ((n * (n - 1) / 2) << 32) + n * j;
            std::uint64_t s = 0;
            for (auto v : mat.rcol(j))
                s += v;
            pass6 = pass6 && s == r;
        }
        for (std::uint64_t j = 0; j != m; ++j) {
            std::uint64_t r = ((n * (n - 1) / 2) << 32) + n * j;
            std::uint64_t s = 0;
            for (auto v : mat.crcol(j))
                s += v;
            pass6 = pass6 && s == r;
        }
        for (std::uint64_t j = 0; j != m; ++j) {
            std::uint64_t r = ((n * (n - 1) / 2) << 32) + n * j;
            std::uint64_t s = 0;
            for (auto v : cmat.rcol(j))
                s += v;
            pass6 = pass6 && s == r;
        }
    }

    double p1 = 0;
    double p2 = 0;
    double p3 = 0;
    double p4 = 0;
    double p5 = 0;
    double p6 = 0;
    if (mckl::StopWatch::has_cycles()) {
        p1 = 1.0 * watch1.cycles() / l;
        p2 = 1.0 * watch2.cycles() / l;
        p3 = 1.0 * watch3.cycles() / l;
        p4 = 1.0 * watch4.cycles() / l;
        p5 = 1.0 * watch5.cycles() / l;
        p6 = 1.0 * watch6.cycles() / l;
    } else {
        p1 = 1e-16 * l / watch1.seconds();
        p2 = 1e-16 * l / watch2.seconds();
        p3 = 1e-16 * l / watch3.seconds();
        p4 = 1e-16 * l / watch4.seconds();
        p5 = 1e-16 * l / watch5.seconds();
        p6 = 1e-16 * l / watch6.seconds();
    }

    std::cout << std::setw(30) << std::left << "iterator";
    std::cout << std::setw(10) << std::right << p1;
    std::cout << std::setw(15) << std::right << (pass1 ? "Passed" : "Failed");
    std::cout << std::endl;

    std::cout << std::setw(30) << std::left << "reverse_iterator";
    std::cout << std::setw(10) << std::right << p2;
    std::cout << std::setw(15) << std::right << (pass2 ? "Passed" : "Failed");
    std::cout << std::endl;

    std::cout << std::setw(30) << std::left << "row_iterator";
    std::cout << std::setw(10) << std::right << p3;
    std::cout << std::setw(15) << std::right << (pass3 ? "Passed" : "Failed");
    std::cout << std::endl;

    std::cout << std::setw(30) << std::left << "reverse_row_iterator";
    std::cout << std::setw(10) << std::right << p4;
    std::cout << std::setw(15) << std::right << (pass4 ? "Passed" : "Failed");
    std::cout << std::endl;

    std::cout << std::setw(30) << std::left << "col_iterator";
    std::cout << std::setw(10) << std::right << p5;
    std::cout << std::setw(15) << std::right << (pass5 ? "Passed" : "Failed");
    std::cout << std::endl;

    std::cout << std::setw(30) << std::left << "reverse_col_iterator";
    std::cout << std::setw(10) << std::right << p6;
    std::cout << std::setw(15) << std::right << (pass6 ? "Passed" : "Failed");
    std::cout << std::endl;
}

inline void core_matrix(std::size_t N, std::size_t M)
{
    std::cout << std::fixed << std::setprecision(5);

    std::string perf;
    if (mckl::StopWatch::has_cycles())
        perf = "cpE";
    else
        perf = "ME/s";

    std::cout << std::string(55, '=') << std::endl;
    std::cout << std::setw(30) << std::left << "RowMajor";
    std::cout << std::setw(10) << std::right << perf;
    std::cout << std::setw(15) << std::right << "Determinstics";
    std::cout << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    core_matrix<mckl::RowMajor>(N, M);
    std::cout << std::string(55, '-') << std::endl;

    std::cout << std::string(55, '=') << std::endl;
    std::cout << std::setw(30) << std::left << "ColMajor";
    std::cout << std::setw(10) << std::right << perf;
    std::cout << std::setw(15) << std::right << "Determinstics";
    std::cout << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    core_matrix<mckl::ColMajor>(N, M);
    std::cout << std::string(55, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_CORE_MATRIX_HPP
