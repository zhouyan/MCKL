//============================================================================
// MCKL/example/random/src/random_skein_perf.cpp
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

#include <mckl/random/aes.hpp>
#include <mckl/random/skein.hpp>
#include <mckl/utility/stop_watch.hpp>
#include <iomanip>
#include <iostream>

inline std::size_t random_skein_count_duplicates(
    std::size_t n, const std::uint64_t *v)
{
    std::set<std::uint64_t> s;
    for (std::size_t i = 0; i != n; ++i) {
        s.insert(v[i]);
    }

    return n - s.size();
}

template <typename Hasher>
inline void random_skein_perf(const std::string &name)
{
    constexpr std::size_t n = 1 << 20;
    constexpr std::size_t m = 1 << 2;
    ::mckl::Vector<::mckl::Vector<std::uint64_t>> msg(m);
    ::mckl::Vector<std::uint64_t> ret(n);
    ::mckl::ARS_64 rng;
    for (std::size_t i = 0; i != m; ++i) {
        msg[i].resize(n);
        ::mckl::rand(rng, n, msg[i].data());
    }

    double mb = 1.0 * n * m * sizeof(std::uint64_t) / (1 << 20);

    ::mckl::StopWatch watch;

    watch.reset();
    watch.start();
    Hasher hasher(CHAR_BIT * sizeof(std::uint64_t));
    ::mckl::Vector<typename Hasher::state_type> states(n);
    std::fill_n(states.data(), n, hasher.init());
    std::size_t N = CHAR_BIT * sizeof(std::uint64_t);
    for (std::size_t j = 0; j != m; ++j) {
        auto type = static_cast<int>(Hasher::type_field::cfg() + j + 1);
        for (std::size_t i = 0; i != n; ++i) {
            hasher.update(
                states[i], typename Hasher::param_type(N, &msg[j][i], type));
        }
    }
    for (std::size_t i = 0; i != n; ++i)
        hasher.output(states[i], &ret[i]);
    watch.stop();
    auto p1 = mb / watch.seconds();
    auto d1 = random_skein_count_duplicates(n, ret.data());

    watch.reset();
    watch.start();
    states.resize(m);
    std::fill_n(states.data(), m, hasher.init());
    for (std::size_t j = 0; j != m; ++j) {
        auto type = static_cast<int>(Hasher::type_field::cfg() + j + 1);
        hasher.update(states[j],
            typename Hasher::param_type(N * n, msg[j].data(), type));
    }
    for (std::size_t j = 0; j != m; ++j) {
        hasher.output(states[j], &ret[j]);
    }
    watch.stop();
    auto p2 = mb / watch.seconds();
    auto d2 = random_skein_count_duplicates(m, ret.data());

    watch.reset();
    watch.start();
    ::mckl::Vector<std::string> str;
    str.reserve(n);
    ::mckl::Vector<char> s(m * sizeof(std::uint64_t));
    std::hash<std::string> shasher;
    for (std::size_t i = 0; i != n; ++i) {
        auto r = reinterpret_cast<std::uint64_t *>(s.data());
        for (std::size_t j = 0; j != m; ++j)
            *r++ = msg[j][i];
        str.emplace_back(s.data(), s.size());
    }
    for (std::size_t i = 0; i != n; ++i) {
        ret[i] = shasher(str[i]);
    }
    watch.stop();
    auto p3 = mb / watch.seconds();
    auto d3 = random_skein_count_duplicates(n, ret.data());

    watch.reset();
    watch.start();
    str.clear();
    s.resize(n * sizeof(std::uint64_t));
    for (std::size_t j = 0; j != m; ++j) {
        auto r = reinterpret_cast<std::uint64_t *>(s.data());
        for (std::size_t i = 0; i != n; ++i)
            *r++ = msg[j][i];
        str.emplace_back(s.data(), s.size());
    }
    for (std::size_t j = 0; j != m; ++j) {
        ret[j] = shasher(str[j]);
    }
    watch.stop();
    auto p4 = mb / watch.seconds();
    auto d4 = random_skein_count_duplicates(m, ret.data());

    std::cout << std::setw(20) << std::left << name << std::fixed;
    std::cout << std::setw(20) << std::right << p1 << " (" << d1 << ")";
    std::cout << std::setw(20) << std::right << p2 << " (" << d2 << ")";
    std::cout << std::setw(20) << std::right << p3 << " (" << d3 << ")";
    std::cout << std::setw(20) << std::right << p4 << " (" << d4 << ")";
    std::cout << std::endl;
}

int main()
{
    random_skein_perf<::mckl::Skein256::mono_hasher>("Skein256");

    random_skein_perf<::mckl::Skein512::mono_hasher>("Skein512");

    random_skein_perf<::mckl::Skein1024::mono_hasher>("Skein1024");

    random_skein_perf<
        ::mckl::Skein<::mckl::Threefry2x32::generator_type>::mono_hasher>(
        "Skein2x32");

    random_skein_perf<
        ::mckl::Skein<::mckl::Threefry4x32::generator_type>::mono_hasher>(
        "Skein4x32");

    random_skein_perf<
        ::mckl::Skein<::mckl::Threefry2x64::generator_type>::mono_hasher>(
        "Skein2x64");

    random_skein_perf<
        ::mckl::Skein<::mckl::Threefry4x64::generator_type>::mono_hasher>(
        "Skein4x64");

    random_skein_perf<
        ::mckl::Skein<::mckl::Threefry4x64::generator_type>::mono_hasher>(
        "Skein4x64");

    random_skein_perf<
        ::mckl::Skein<::mckl::Threefry8x64::generator_type>::mono_hasher>(
        "Skein8x64");

    random_skein_perf<
        ::mckl::Skein<::mckl::Threefry16x64::generator_type>::mono_hasher>(
        "Skein16x64");

    return 0;
}
