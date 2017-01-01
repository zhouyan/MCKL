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
#include <mckl/utility/stop_watch.hpp>
#include <future>
#include <thread>

template <typename T, typename Mem>
inline void core_memory(std::size_t N, std::size_t M, const std::string &tname,
    const std::string &mname)
{
    std::size_t np =
        static_cast<std::size_t>(std::thread::hardware_concurrency());
    mckl::Vector<std::future<std::pair<bool, std::size_t>>> tg;
    tg.reserve(np);

    mckl::StopWatch watch;
    watch.start();
    for (std::size_t p = 0; p != np; ++p) {
        tg.push_back(std::async(std::launch::async, [N, M]() {
            constexpr std::uintptr_t alignment = Mem::alignment();
            mckl::RNG rng;
            mckl::UniformIntDistribution<std::size_t> rsize(0, N);
            mckl::Allocator<T, Mem> alloc;
            std::size_t n = 0;
            bool passed = true;
            mckl::Vector<T *, mckl::Allocator<T *, Mem>> ptr;
            for (std::size_t k = 0; k != 10; ++k) {
                for (std::size_t i = 0; i != M; ++i) {
                    std::size_t K = rsize(rng);
                    n += K;
                    ptr.push_back(alloc.allocate(K));
                    passed = passed &&
                        reinterpret_cast<std::uintptr_t>(ptr.back()) %
                                alignment ==
                            0;
                }
                while (!ptr.empty()) {
                    alloc.deallocate(ptr.back());
                    ptr.pop_back();
                }
            }
            return std::pair<bool, std::size_t>(passed, n);
        }));
    }
    bool passed = true;
    std::size_t n = 0;
    for (std::size_t p = 0; p != np; ++p) {
        std::pair<bool, std::size_t> r = tg[p].get();
        passed = passed && r.first;
        n += r.second;
    }
    watch.stop();

    const std::string function(
        "Allocator<" + tname + ", " + mname + "<" + tname + ">>::allocate");
    const std::size_t bytes = n * sizeof(T);
    const double c = watch.has_cycles() ? (1.0 * watch.cycles() / bytes) :
                                          (1e-9 * bytes / watch.seconds());

    std::cout << std::setw(70) << std::left << function;
    std::cout << std::setw(10) << std::right << Mem::alignment();
    std::cout << std::setw(10) << std::right << sizeof(T);
    std::cout << std::setw(10) << std::right << c;
    std::cout << std::setw(15) << std::right << (passed ? "Passed" : "Failed");
    std::cout << std::endl;
}

template <typename T>
inline void core_memory(std::size_t N, std::size_t M, const std::string &tname)
{
    constexpr std::size_t alignment = mckl::AlignOf<T>;
    N /= sizeof(T);

    std::cout << std::fixed << std::setprecision(2);

    core_memory<T, mckl::MemorySTD<alignment>>(N, M, tname, "MemorySTD");
#if MCKL_HAS_POSIX || defined(MCKL_MSVC)
    core_memory<T, mckl::MemorySYS<alignment>>(N, M, tname, "MemorySYS");
#endif
#if MCKL_HAS_JEMALLOC
    core_memory<T, mckl::MemoryJEM<alignment>>(N, M, tname, "MemoryJEM");
#endif
#if MCKL_HAS_TBB
    core_memory<T, mckl::MemoryTBB<alignment>>(N, M, tname, "MemoryTBB");
#endif

    std::cout << std::string(115, '-') << std::endl;
}

inline void core_memory(std::size_t N, std::size_t M)
{
    std::cout << std::string(115, '=') << std::endl;
    std::cout << std::setw(70) << std::left << "Function";
    std::cout << std::setw(10) << std::right << "Alignment";
    std::cout << std::setw(10) << std::right << "Size";
    std::cout << std::setw(10) << std::right
              << (mckl::StopWatch::has_cycles() ? "cpB" : "GB/s");
    std::cout << std::setw(15) << std::right << "Determinstics";
    std::cout << std::endl;
    std::cout << std::string(115, '-') << std::endl;

    struct alignas(128) T {
        int c[128];
    };

    core_memory<int>(N, M, "int");
    core_memory<T>(N, M, "int[128]");
}

#endif // MCKL_EXAMPLE_CORE_MEMORY_HPP
