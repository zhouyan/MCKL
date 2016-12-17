//============================================================================
// MCKL/example/core/include/core_memory.hpp
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

#ifndef MCKL_EXAMPLE_CORE_MEMORY_HPP
#define MCKL_EXAMPLE_CORE_MEMORY_HPP

#include <mckl/core/memory.hpp>
#include <mckl/random/rng.hpp>
#include <mckl/random/uniform_int_distribution.hpp>
#include <mckl/random/uniform_real_distribution.hpp>
#include <mckl/utility/stop_watch.hpp>

inline void core_memory_check(const std::string &func, std::size_t align,
    std::size_t bytes, const mckl::StopWatch &watch, bool passed)
{
    const double perf = watch.has_cycles() ? (1.0 * watch.cycles() / bytes) :
                                             (1e-9 * bytes / watch.seconds());

    std::cout << std::setw(80) << std::left << func;
    std::cout << std::setw(10) << std::right << align;
    std::cout << std::setw(10) << std::right << perf;
    std::cout << std::setw(15) << std::right << (passed ? "Passed" : "Failed");
    std::cout << std::endl;
}

template <typename Alloc>
inline void core_memory_allocate(std::size_t N, std::size_t M,
    const std::string &tname, const std::string &mname)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);

    Alloc alloc;
    mckl::Vector<typename Alloc::pointer> ptr;
    ptr.reserve(M);
    std::size_t n = 0;
    mckl::StopWatch watch;

    watch.start();
    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = rsize(rng);
        n += K;
        ptr.push_back(alloc.allocate(K));
    }
    watch.stop();

    const std::string func(
        "Allocator<" + tname + ", " + mname + "<" + tname + ">>::allocate");
    const std::size_t bytes = n * sizeof(typename Alloc::value_type);
    constexpr std::uintptr_t align = Alloc::memory_type::alignment();
    bool passed = true;
    for (auto p : ptr) {
        passed = passed && reinterpret_cast<std::uintptr_t>(p) % align == 0;
        alloc.deallocate(p);
    }
    core_memory_check(func, align, bytes, watch, passed);
}

template <typename Alloc>
inline void core_memory_vector(std::size_t N, std::size_t M,
    const std::string &tname, const std::string &mname)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);

    using Vec = mckl::Vector<typename Alloc::value_type, Alloc>;
    mckl::Vector<Vec> vec;
    vec.reserve(M);
    std::size_t n = 0;
    mckl::StopWatch watch;

    watch.start();
    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = rsize(rng);
        n += K;
        vec.emplace_back(Vec(K));
    }
    watch.stop();

    const std::string func("Vector<" + tname + ", Allocator<" + tname + ", " +
        mname + "<" + tname + ">>>(size_t)");
    const std::size_t bytes = n * sizeof(typename Alloc::value_type);
    constexpr std::uintptr_t align = Alloc::memory_type::alignment();
    bool passed = true;
    for (auto v : vec) {
        passed =
            passed && reinterpret_cast<std::uintptr_t>(v.data()) % align == 0;
    }
    core_memory_check(func, align, bytes, watch, passed);
}

template <typename Alloc>
inline void core_memory_vector_val(std::size_t N, std::size_t M,
    const std::string &tname, const std::string &mname)
{
    mckl::RNG rng;
    mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);
    mckl::UniformRealDistribution<double> runif(
        static_cast<double>(N / 2), static_cast<double>(N));

    using Vec = mckl::Vector<typename Alloc::value_type, Alloc>;
    auto val = static_cast<typename Alloc::value_type>(runif(rng));
    mckl::Vector<Vec> vec;
    vec.reserve(M);
    std::size_t n = 0;
    mckl::StopWatch watch;

    watch.start();
    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = rsize(rng);
        n += K;
        vec.emplace_back(Vec(K, val));
    }
    watch.stop();

    const std::string func("Vector<" + tname + ", Allocator<" + tname + ", " +
        mname + "<" + tname + ">>>(size_t, " + tname + ")");
    const std::size_t bytes = n * sizeof(typename Alloc::value_type);
    constexpr std::uintptr_t align = Alloc::memory_type::alignment();
    bool passed = true;
    for (auto v : vec) {
        passed =
            passed && reinterpret_cast<std::uintptr_t>(v.data()) % align == 0;
        for (auto &x : v)
            passed = passed && x == val;
    }
    core_memory_check(func, align, bytes, watch, passed);
}

template <typename T>
inline void core_memory(std::size_t N, std::size_t M, const std::string &tname)
{
    constexpr std::size_t alignment = mckl::AlignOf<T>;
    N /= sizeof(T);

    std::cout << std::fixed << std::setprecision(2);

    core_memory_allocate<mckl::Allocator<T, mckl::MemorySTD<alignment>>>(
        N, M, tname, "MemorySTD");
#if MCKL_HAS_POSIX || defined(MCKL_MSVC)
    core_memory_allocate<mckl::Allocator<T, mckl::MemorySYS<alignment>>>(
        N, M, tname, "MemorySYS");
#endif
#if MCKL_HAS_TBB
    core_memory_allocate<mckl::Allocator<T, mckl::MemoryTBB<alignment>>>(
        N, M, tname, "MemoryTBB");
#endif
    std::cout << std::string(115, '-') << std::endl;

    core_memory_vector<mckl::Allocator<T, mckl::MemorySTD<alignment>>>(
        N, M, tname, "MemorySTD");
#if MCKL_HAS_POSIX || defined(MCKL_MSVC)
    core_memory_vector<mckl::Allocator<T, mckl::MemorySYS<alignment>>>(
        N, M, tname, "MemorySYS");
#endif
#if MCKL_HAS_TBB
    core_memory_vector<mckl::Allocator<T, mckl::MemoryTBB<alignment>>>(
        N, M, tname, "MemoryTBB");
#endif
    std::cout << std::string(115, '-') << std::endl;

    core_memory_vector_val<mckl::Allocator<T, mckl::MemorySTD<alignment>>>(
        N, M, tname, "MemorySTD");
#if MCKL_HAS_POSIX || defined(MCKL_MSVC)
    core_memory_vector_val<mckl::Allocator<T, mckl::MemorySYS<alignment>>>(
        N, M, tname, "MemorySYS");
#endif
#if MCKL_HAS_TBB
    core_memory_vector_val<mckl::Allocator<T, mckl::MemoryTBB<alignment>>>(
        N, M, tname, "MemoryTBB");
#endif
    std::cout << std::string(115, '-') << std::endl;
}

inline void core_memory(std::size_t N, std::size_t M)
{
    std::cout << std::string(115, '=') << std::endl;
    std::cout << std::setw(80) << std::left << "Function";
    std::cout << std::setw(10) << std::right << "Alignment";
    std::cout << std::setw(10) << std::right
              << (mckl::StopWatch::has_cycles() ? "cpB" : "GB/s");
    std::cout << std::setw(15) << std::right << "Determinstics";
    std::cout << std::endl;
    std::cout << std::string(115, '-') << std::endl;

    core_memory<int>(N, M, "int");
}

#endif // MCKL_EXAMPLE_CORE_MEMORY_HPP
