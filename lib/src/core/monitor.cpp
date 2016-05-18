//============================================================================
// MCKL/lib/src/core/monitor.cpp
//----------------------------------------------------------------------------
//                         MCKL: Monte Carlo Kernel Library
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
#include "libmckl.hpp"

namespace mckl
{

inline MonitorC::eval_type cast(mckl_monitor_eval_type fptr)
{
    return [fptr](
        std::size_t iter, std::size_t dim, ParticleC &particle, double *r) {
        mckl_particle c_particle = {&particle};
        fptr(iter, dim, c_particle, r);
    };
}

} // namespace mckl

extern "C" {

mckl_monitor mckl_monitor_new(size_t dim, mckl_monitor_eval_type eval,
    int record_only, MCKLMonitorStage stage)
{
    return {new ::mckl::MonitorC(dim, ::mckl::cast(eval), record_only != 0,
        static_cast<::mckl::MonitorStage>(stage))};
}

void mckl_monitor_delete(mckl_monitor *monitor_ptr)
{
    delete ::mckl::cast(monitor_ptr);
    monitor_ptr->ptr = nullptr;
}

void mckl_monitor_assign(mckl_monitor monitor, mckl_monitor other)
{
    ::mckl::cast(monitor) = ::mckl::cast(other);
}

size_t mckl_monitor_dim(mckl_monitor monitor)
{
    return ::mckl::cast(monitor).dim();
}

int mckl_monitor_record_only(mckl_monitor monitor)
{
    return ::mckl::cast(monitor).record_only();
}

MCKLMonitorStage mckl_monitor_stage(mckl_monitor monitor)
{
    return static_cast<MCKLMonitorStage>(::mckl::cast(monitor).stage());
}

size_t mckl_monitor_iter_size(mckl_monitor monitor)
{
    return ::mckl::cast(monitor).iter_size();
}

void mckl_monitor_reserve(mckl_monitor monitor, size_t num)
{
    ::mckl::cast(monitor).reserve(num);
}

int mckl_monitor_empty(mckl_monitor monitor)
{
    return ::mckl::cast(monitor).empty();
}

void mckl_monitor_set_name(mckl_monitor monitor, size_t id, const char *name)
{
    ::mckl::cast(monitor).name(id) = name;
}

size_t mckl_monitor_get_name(mckl_monitor monitor, size_t id, char *name)
{
    std::string str(::mckl::cast(monitor).name(id));
    std::size_t size = (str.size() + 1) * sizeof(char);
    if (name != nullptr)
        std::memcpy(name, str.c_str(), size);

    return size;
}

size_t mckl_monitor_index(mckl_monitor monitor, size_t iter)
{
    return ::mckl::cast(monitor).index(iter);
}

void mckl_monitor_read_index(mckl_monitor monitor, size_t *first)
{
    ::mckl::cast(monitor).read_index(first);
}

double mckl_monitor_record(mckl_monitor monitor, size_t id, size_t iter)
{
    return ::mckl::cast(monitor).record(id, iter);
}

void mckl_monitor_read_record(mckl_monitor monitor, size_t id, double *first)
{
    ::mckl::cast(monitor).read_record(id, first);
}

void mckl_monitor_read_record_matrix(
    mckl_monitor monitor, MCKLMatrixLayout layout, double *first)
{
    ::mckl::cast(monitor).read_record_matrix(
        static_cast<::mckl::MatrixLayout>(layout), first);
}

void mckl_monitor_eval(mckl_monitor monitor, mckl_monitor_eval_type new_eval,
    int record_only, MCKLMonitorStage stage)
{
    ::mckl::cast(monitor).eval(::mckl::cast(new_eval), record_only != 0,
        static_cast<::mckl::MonitorStage>(stage));
}

void mckl_monitor_compute(mckl_monitor monitor, size_t iter,
    mckl_particle particle, MCKLMonitorStage stage)
{
    ::mckl::cast(monitor)(iter, ::mckl::cast(particle),
        static_cast<::mckl::MonitorStage>(stage));
}

void mckl_monitor_clear(mckl_monitor monitor)
{
    ::mckl::cast(monitor).clear();
}

} // extern "C"
