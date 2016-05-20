//============================================================================
// MCKL/include/mckl/capi/utility.h
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

#ifndef MCKL_UTILITY_UTILITY_H
#define MCKL_UTILITY_UTILITY_H

#include <mckl/internal/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/// \addtogroup C_API_Utility_AlignedMemory
/// @{

/// \brief `mckl::AlignedMemory::aligned_malloc`
void *mckl_malloc(size_t n, size_t alignment);

/// \brief `mckl::AlignedMemory::aligned_free`
void mckl_free(void *ptr);

/// @} C_API_Utility_AlignedMemory

/// \addtogroup C_API_Utility_Covariance
/// @{

/// \brief `mckl::Covariance::Covariance`
mckl_covariance mckl_covariance_new(void);

/// \brief `mckl::Covariance::~Covariance`
void mckl_covariance_delete(mckl_covariance *covariance_ptr);

/// \brief `mckl::Covariance::operator=`
void mckl_covariance_assign(mckl_covariance covariance, mckl_covariance other);

/// \brief `mckl::Covariance::operator()`
void mckl_covariance_compute(mckl_covariance covariance,
    MCKLMatrixLayout layout, size_t n, size_t p, const double *x,
    const double *w, double *mean, double *cov, MCKLMatrixLayout cov_layout,
    int cov_upper, int cov_packed);

/// @} C_API_Utility_Covariance

/// \addtogroup C_API_Utility_HDF5
/// @{

/// \brief `mckl::hdf5store`
void mckl_hdf5store_state_matrix(mckl_state_matrix state_matrix,
    const char *filename, const char *dataname, int append);

/// \brief `mckl::hdf5store`
void mckl_hdf5store_particle(mckl_particle particle, const char *filename,
    const char *dataname, int append);

/// \brief `mckl::hdf5store`
void mckl_hdf5store_monitor(mckl_monitor monitor, const char *filename,
    const char *dataname, int append);

/// \brief `mckl::hdf5store`
void mckl_hdf5store_sampler(mckl_sampler sampler, const char *filename,
    const char *dataname, int append);

/// @} C_API_Utility_HDF5

/// \addtogroup C_API_Utility_StopWatch
/// @{

/// \brief `mckl::StopWatch::StopWatch`
mckl_stop_watch mckl_stop_watch_new(void);

/// \brief `mckl::StopWatch::~StopWatch`
void mckl_stop_watch_delete(mckl_stop_watch *stop_watch_ptr);

/// \brief `mckl::StopWatch::operator=`
void mckl_stop_watch_assign(mckl_stop_watch stop_watch, mckl_stop_watch other);

/// \brief `mckl::StopWatch::running`
int mckl_stop_watch_running(mckl_stop_watch stop_watch);

/// \brief `mckl::StopWatch::start`
int mckl_stop_watch_start(mckl_stop_watch stop_watch);

/// \brief `mckl::StopWatch::stop`
int mckl_stop_watch_stop(mckl_stop_watch stop_watch);

/// \brief `mckl::StopWatch::reset`
void mckl_stop_watch_reset(mckl_stop_watch stop_watch);

/// \brief `mckl::StopWatch::cycles`
double mckl_stop_watch_cycles(mckl_stop_watch stop_watch);

/// \brief `mckl::StopWatch::nanoseconds`
double mckl_stop_watch_nanoseconds(mckl_stop_watch stop_watch);

/// \brief `mckl::StopWatch::microseconds`
double mckl_stop_watch_microseconds(mckl_stop_watch stop_watch);

/// \brief `mckl::StopWatch::milliseconds`
double mckl_stop_watch_milliseconds(mckl_stop_watch stop_watch);

/// \brief `mckl::StopWatch::seconds`
double mckl_stop_watch_seconds(mckl_stop_watch stop_watch);

/// \brief `mckl::StopWatch::minutes`
double mckl_stop_watch_minutes(mckl_stop_watch stop_watch);

/// \brief `mckl::StopWatch::hours`
double mckl_stop_watch_hours(mckl_stop_watch stop_watch);

/// @} C_API_Utility_StopWatch

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MCKL_UTILITY_UTILITY_H
