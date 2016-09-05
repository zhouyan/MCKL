//============================================================================
// MCKL/lib/src/utility/stop_watch.cpp
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

#include <mckl/mckl.h>
#include <mckl/utility/stop_watch.hpp>

mckl_stop_watch mckl_stop_watch_new(void) { return {new ::mckl::StopWatch()}; }

void mckl_stop_watch_delete(mckl_stop_watch *stop_watch_ptr)
{
    delete reinterpret_cast<::mckl::StopWatch *>(stop_watch_ptr->ptr);
    stop_watch_ptr->ptr = nullptr;
}

void mckl_stop_watch_assign(mckl_stop_watch stop_watch, mckl_stop_watch other)
{
    *reinterpret_cast<::mckl::StopWatch *>(stop_watch.ptr) =
        *reinterpret_cast<::mckl::StopWatch *>(other.ptr);
}

int mckl_stop_watch_running(mckl_stop_watch stop_watch)
{
    return reinterpret_cast<::mckl::StopWatch *>(stop_watch.ptr)->running();
}

int mckl_stop_watch_start(mckl_stop_watch stop_watch)
{
    return reinterpret_cast<::mckl::StopWatch *>(stop_watch.ptr)->start();
}

int mckl_stop_watch_stop(mckl_stop_watch stop_watch)
{
    return reinterpret_cast<::mckl::StopWatch *>(stop_watch.ptr)->stop();
}

void mckl_stop_watch_reset(mckl_stop_watch stop_watch)
{
    reinterpret_cast<::mckl::StopWatch *>(stop_watch.ptr)->reset();
}

unsigned long long mckl_stop_watch_cycles(mckl_stop_watch stop_watch)
{
    return reinterpret_cast<::mckl::StopWatch *>(stop_watch.ptr)->cycles();
}

double mckl_stop_watch_nanoseconds(mckl_stop_watch stop_watch)
{
    return reinterpret_cast<::mckl::StopWatch *>(stop_watch.ptr)
        ->nanoseconds();
}

double mckl_stop_watch_microseconds(mckl_stop_watch stop_watch)
{
    return reinterpret_cast<::mckl::StopWatch *>(stop_watch.ptr)
        ->microseconds();
}

double mckl_stop_watch_milliseconds(mckl_stop_watch stop_watch)
{
    return reinterpret_cast<::mckl::StopWatch *>(stop_watch.ptr)
        ->milliseconds();
}

double mckl_stop_watch_seconds(mckl_stop_watch stop_watch)
{
    return reinterpret_cast<::mckl::StopWatch *>(stop_watch.ptr)->seconds();
}

double mckl_stop_watch_minutes(mckl_stop_watch stop_watch)
{
    return reinterpret_cast<::mckl::StopWatch *>(stop_watch.ptr)->minutes();
}

double mckl_stop_watch_hours(mckl_stop_watch stop_watch)
{
    return reinterpret_cast<::mckl::StopWatch *>(stop_watch.ptr)->hours();
}
