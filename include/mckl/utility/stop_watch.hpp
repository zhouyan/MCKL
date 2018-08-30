//============================================================================
// MCKL/include/mckl/utility/stop_watch.hpp
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

#ifndef MCKL_UTILITY_STOP_WATCH_HPP
#define MCKL_UTILITY_STOP_WATCH_HPP

#include <mckl/internal/common.hpp>
#include <chrono>
#include <ratio>

namespace mckl {

namespace internal {

#if MCKL_USE_RDPMC

inline std::uint64_t cycle_start()
{
    unsigned a = 0;
    unsigned d = 0;
    unsigned c = 0x40000001;
    __asm__ volatile("rdpmc" : "=a"(a), "=d"(d) : "c"(c));

    return (static_cast<std::uint64_t>(d) << 32) + a;
}

inline std::uint64_t cycle_stop()
{
    unsigned a = 0;
    unsigned d = 0;
    unsigned c = 0x40000001;
    __asm__ volatile("rdpmc" : "=a"(a), "=d"(d) : "c"(c));

    return (static_cast<std::uint64_t>(d) << 32) + a;
}

#elif MCKL_USE_RDTSCP

inline std::uint64_t cycle_start()
{
    unsigned hi = 0;
    unsigned lo = 0;
    asm volatile(
        "cpuid\n\t"
        "rdtsc\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        : "=r"(hi), "=r"(lo)::"%eax", "%ebx", "%ecx", "%edx");
    return (static_cast<std::uint64_t>(hi) << 32) + lo;
}

inline std::uint64_t cycle_stop()
{
    unsigned hi = 0;
    unsigned lo = 0;
    asm volatile(
        "rdtscp\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        "cpuid\n\t"
        : "=r"(hi), "=r"(lo)::"%eax", "%ebx", "%ecx", "%edx");
    return (static_cast<std::uint64_t>(hi) << 32) + lo;
}

#elif MCKL_USE_RDTSC

inline std::uint64_t cycle_start()
{
    unsigned hi = 0;
    unsigned lo = 0;
    asm volatile(
        "cpuid\n\t"
        "rdtsc\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        : "=r"(hi), "=r"(lo)::"%eax", "%ebx", "%ecx", "%edx");
    return (static_cast<std::uint64_t>(hi) << 32) + lo;
}

inline std::uint64_t cycle_stop()
{
    unsigned hi = 0;
    unsigned lo = 0;
    asm volatile(
        "rdtsc\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        "cpuid\n\t"
        : "=r"(hi), "=r"(lo)::"%eax", "%ebx", "%ecx", "%edx");
    return (static_cast<std::uint64_t>(hi) << 32) + lo;
}

#else // MCKL_USE_RDPMC

inline std::uint64_t cycle_start() { return 0; }

inline std::uint64_t cycle_stop() { return 0; }

#endif // MCKL_USE_RDPMC

} // namespace internal

MCKL_PUSH_CLANG_WARNING("-Wpadded")
/// \brief Start and stop a StopWatch in scope (similiar to a mutex lock
/// guard)
/// \ingroup StopWatch
template <typename WatchType>
class StopWatchGuard
{
  public:
    using watch_type = WatchType;

    StopWatchGuard(watch_type &watch, bool start = true)
        : watch_(watch), start_(start)
    {
        if (start_) {
            watch_.start();
        }
    }

    ~StopWatchGuard()
    {
        if (start_) {
            watch_.stop();
        }
    }

  private:
    watch_type &watch_;
    const bool start_;
}; // class StopWatchGuard
MCKL_POP_CLANG_WARNING

MCKL_PUSH_CLANG_WARNING("-Wpadded")
/// \brief StopWatch as an adapter of C++ standard library compatible clock
/// \ingroup StopWatch
template <typename ClockType = std::chrono::high_resolution_clock>
class StopWatchClockAdapter
{
  public:
    using clock_type = ClockType;

    StopWatchClockAdapter()
        : time_(0), cycles_(0), cycles_start_(0), running_(false)
    {
        reset();
    }

    /// \brief If cycle counting is supported
    ///
    /// \details
    /// If this function returns `true`, then `cycles()` will return the number
    /// of accumulated cycles. Otherwise, it will always returns zero.
    static constexpr bool has_cycles()
    {
#if MCKL_USE_RDPMC || MCKL_USE_RDTSCP || MCKL_USE_RDTSC
        return true;
#else
        return false;
#endif
    }

    /// \brief If the watch is running
    ///
    /// \details
    /// If `start()` has been called and no `stop()` call since, then it is
    /// running, otherwise it is stoped.
    bool running() const { return running_; }

    /// \brief Start the watch, no effect if already started
    ///
    /// \return `true` if it is started by this call, and the elapsed time will
    /// be incremented next time `stop()` is called. The increment will be
    /// relative to the time point of this call. `false` if it is already
    /// started earlier.
    bool start()
    {
        if (running_) {
            return false;
        }

        running_ = true;
        time_start_ = clock_type::now();
        cycles_start_ = internal::cycle_start();

        return true;
    }

    /// \brief Stop the watch, no effect if already stopped
    ///
    /// \return `true` if it is stoped by this call, and the elapsed time has
    /// been incremented. `false` if it is already stopped or wasn't started
    /// before.
    bool stop()
    {
        std::uint64_t c = internal::cycle_stop();
        typename clock_type::time_point t = clock_type::now();

        if (!running_) {
            return false;
        }

        running_ = false;
        cycles_ += c - cycles_start_;
        time_ += t - time_start_;

        return true;
    }

    /// \brief Stop and reset the elapsed time to zero
    void reset()
    {
        start();
        time_ = typename clock_type::duration(0);
        cycles_ = 0;
        running_ = false;
    }

    /// \brief Return the accumulated cycles
    std::uint64_t cycles() const { return cycles_; }

    /// \brief Return the accumulated elapsed time in its native format
    typename clock_type::duration const time() const { return time_; }

    /// \brief Return the accumulated elapsed time in specified format
    template <typename Rep, typename Period>
    Rep time() const
    {
        using time_type = std::chrono::duration<Rep, Period>;

        return std::chrono::duration_cast<time_type>(time_).count();
    }

    /// \brief Equivalent to `time<double, std::nano>()`
    double nanoseconds() const { return time<double, std::nano>(); }

    /// \brief Equivalent to `time<double, std::micro>()`
    double microseconds() const { return time<double, std::micro>(); }

    /// \brief Equivalent to `time<double, std::milli>()`
    double milliseconds() const { return time<double, std::milli>(); }

    /// \brief Equivalent to `time<double, std::ratio<1>>()`
    double seconds() const { return time<double, std::ratio<1>>(); }

    /// \brief Equivalent to `time<double, std::ratio<60>>()`
    double minutes() const { return time<double, std::ratio<60>>(); }

    /// \brief Equivalent to `time<double, std::ratio<3600>>()`
    double hours() const { return time<double, std::ratio<3600>>(); }

  private:
    typename clock_type::duration time_;
    typename clock_type::time_point time_start_;
    std::uint64_t cycles_;
    std::uint64_t cycles_start_;
    bool running_;
}; // class StopWatchClockAdapter
MCKL_POP_CLANG_WARNING

/// \brief Stop watch using `<chrono>`
/// \ingroup StopWatch
using StopWatch = StopWatchClockAdapter<>;

} // namespace mckl

#endif // MCKL_UTILITY_STOP_WATCH_HPP
