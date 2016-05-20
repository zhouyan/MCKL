//============================================================================
// MCKL/include/mckl/capi/resample.h
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

#ifndef MCKL_RESAMPLE_RESAMPLE_H
#define MCKL_RESAMPLE_RESAMPLE_H

#include <mckl/internal/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/// \addtogroup C_API_Resample
/// @{

/// \brief `mckl::resample_trans_residual`
size_t mckl_resample_trans_residual(
    size_t n, size_t m, const double *weight, double *resid, size_t *integ);

/// \brief `mckl::resample_trans_u01_rep`
void mckl_resample_trans_u01_rep(size_t n, size_t m, const double *weight,
    const double *u01, size_t *replication);

/// \brief `mckl::resample_trans_rep_index`
void mckl_resample_trans_rep_index(
    size_t n, size_t m, const size_t *replication, size_t *index);

/// \brief `mckl::ResampleMultinomial`
void mckl_resample_multinomial(size_t n, size_t m, mckl_rng rng,
    const double *weight, size_t *replication);

/// \brief `mckl::ResampleStratified`
void mckl_resample_stratified(size_t n, size_t m, mckl_rng rng,
    const double *weight, size_t *replication);

/// \brief `mckl::ResampleSystematic`
void mckl_resample_systematic(size_t n, size_t m, mckl_rng rng,
    const double *weight, size_t *replication);

/// \brief `mckl::ResampleResidual`
void mckl_resample_residual(size_t n, size_t m, mckl_rng rng,
    const double *weight, size_t *replication);

/// \brief `mckl::ResampleResidualStratified`
void mckl_resample_residual_stratified(size_t n, size_t m, mckl_rng rng,
    const double *weight, size_t *replication);

/// \brief `mckl::ResampleSystematic`
void mckl_resample_residual_systematic(size_t n, size_t m, mckl_rng rng,
    const double *weight, size_t *replication);

/// \brief `mckl::u01_trans_sorted`
void mckl_u01_trans_sorted(size_t n, const double *u01, double *r);

/// \brief `mckl::u01_trans_stratifed`
void mckl_u01_trans_stratified(size_t n, const double *u01, double *r);

/// \brief `mckl::u01_trans_systematic`
void mckl_u01_trans_systematic(size_t n, const double *u01, double *r);

/// \brief `mckl::u01_rand_sorted`
void mckl_u01_rand_sorted(mckl_rng rng, size_t n, double *r);

/// \brief `mckl::u01_rand_stratifed`
void mckl_u01_rand_stratified(mckl_rng rng, size_t n, double *r);

/// \brief `mckl::u01_rand_systematic`
void mckl_u01_rand_systematic(mckl_rng rng, size_t n, double *r);

/// @} C_API_Resample

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MCKL_RESAMPLE_RESAMPLE_H
