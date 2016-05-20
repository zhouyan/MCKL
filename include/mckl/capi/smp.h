//============================================================================
// MCKL/include/mckl/capi/smp.h
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

#ifndef MCKL_SMP_SMP_H
#define MCKL_SMP_SMP_H

#include <mckl/internal/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/// \addtogroup C_API_SMP
/// @{

/// \brief The maximum of an integer that can be passed to where
/// `MCKLBackendSMP` value is expected
int mckl_backend_smp_max();

/// \brief Check if a given SMP backend is defined within the library
int mckl_backend_smp_check(MCKLBackendSMP backend);

/// \brief `mckl::Sampler::eval` with `mckl::SamplerEvalSMP` as input
void mckl_sampler_eval_smp(MCKLBackendSMP backend, mckl_sampler sampler,
    mckl_sampler_eval_smp_type eval, MCKLSamplerStage);

/// \brief `mckl::Monitor` with `mckl::MonitorEvalSMP` as input
mckl_monitor mckl_monitor_new_smp(MCKLBackendSMP backend, size_t dim,
    mckl_monitor_eval_smp_type eval, int record_only, MCKLMonitorStage stage);

/// @} C_API_SMP_SEQ

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MCKL_SMP_SMP_H
