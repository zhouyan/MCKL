//============================================================================
// MCKL/include/mckl/mckl.h
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

#ifndef MCKL_MCKL_H
#define MCKL_MCKL_H

#ifdef __cplusplus

#include <cstddef>
#include <cstdint>

extern "C" {

#else // __cplusplus

#include <stddef.h>
#include <stdint.h>

#endif // __cplusplus

// sqrtf.asm
void mckl_vs_sqrt(size_t, const float *, float *);

// sqrt.asm
void mckl_vd_sqrt(size_t, const double *, double *);

// expf.asm
void mckl_vs_exp(size_t, const float *, float *);
void mckl_vs_exp2(size_t, const float *, float *);
void mckl_vs_expm1(size_t, const float *, float *);

// exp.asm
void mckl_vd_exp(size_t, const double *, double *);
void mckl_vd_exp2(size_t, const double *, double *);
void mckl_vd_expm1(size_t, const double *, double *);

// logf.asm
void mckl_vs_log(size_t, const float *, float *);
void mckl_vs_log2(size_t, const float *, float *);
void mckl_vs_log10(size_t, const float *, float *);
void mckl_vs_log1p(size_t, const float *, float *);

// log.asm
void mckl_vd_log(size_t, const double *, double *);
void mckl_vd_log2(size_t, const double *, double *);
void mckl_vd_log10(size_t, const double *, double *);
void mckl_vd_log1p(size_t, const double *, double *);

// sincosf.asm
void mckl_vs_sin(size_t, const float *, float *);
void mckl_vs_cos(size_t, const float *, float *);
void mckl_vs_sincos(size_t, const float *, float *, float *);
void mckl_vs_tan(size_t, const float *, float *);

// sincos.asm
void mckl_vd_sin(size_t, const double *, double *);
void mckl_vd_cos(size_t, const double *, double *);
void mckl_vd_sincos(size_t, const double *, double *, double *);
void mckl_vd_tan(size_t, const double *, double *);

// fma.asm
void mckl_fmadd_vvv_ps(
    size_t, const float *, const float *, const float *, float *);
void mckl_fmadd_vvs_ps(size_t, const float *, const float *, float, float *);
void mckl_fmadd_vsv_ps(size_t, const float *, float, const float *, float *);
void mckl_fmadd_svv_ps(size_t, float, const float *, const float *, float *);
void mckl_fmadd_ssv_ps(size_t, float, float, const float *, float *);
void mckl_fmadd_svs_ps(size_t, float, const float *, float, float *);
void mckl_fmadd_vss_ps(size_t, const float *, float, float, float *);
void mckl_fmadd_vvv_pd(
    size_t, const double *, const double *, const double *, double *);
void mckl_fmadd_vvs_pd(
    size_t, const double *, const double *, double, double *);
void mckl_fmadd_vsv_pd(
    size_t, const double *, double, const double *, double *);
void mckl_fmadd_svv_pd(
    size_t, double, const double *, const double *, double *);
void mckl_fmadd_ssv_pd(size_t, double, double, const double *, double *);
void mckl_fmadd_svs_pd(size_t, double, const double *, double, double *);
void mckl_fmadd_vss_pd(size_t, const double *, double, double, double *);

void mckl_fmsub_vvv_ps(
    size_t, const float *, const float *, const float *, float *);
void mckl_fmsub_vvs_ps(size_t, const float *, const float *, float, float *);
void mckl_fmsub_vsv_ps(size_t, const float *, float, const float *, float *);
void mckl_fmsub_svv_ps(size_t, float, const float *, const float *, float *);
void mckl_fmsub_ssv_ps(size_t, float, float, const float *, float *);
void mckl_fmsub_svs_ps(size_t, float, const float *, float, float *);
void mckl_fmsub_vss_ps(size_t, const float *, float, float, float *);
void mckl_fmsub_vvv_pd(
    size_t, const double *, const double *, const double *, double *);
void mckl_fmsub_vvs_pd(
    size_t, const double *, const double *, double, double *);
void mckl_fmsub_vsv_pd(
    size_t, const double *, double, const double *, double *);
void mckl_fmsub_svv_pd(
    size_t, double, const double *, const double *, double *);
void mckl_fmsub_ssv_pd(size_t, double, double, const double *, double *);
void mckl_fmsub_svs_pd(size_t, double, const double *, double, double *);
void mckl_fmsub_vss_pd(size_t, const double *, double, double, double *);

void mckl_fnmadd_vvv_ps(
    size_t, const float *, const float *, const float *, float *);
void mckl_fnmadd_vvs_ps(size_t, const float *, const float *, float, float *);
void mckl_fnmadd_vsv_ps(size_t, const float *, float, const float *, float *);
void mckl_fnmadd_svv_ps(size_t, float, const float *, const float *, float *);
void mckl_fnmadd_ssv_ps(size_t, float, float, const float *, float *);
void mckl_fnmadd_svs_ps(size_t, float, const float *, float, float *);
void mckl_fnmadd_vss_ps(size_t, const float *, float, float, float *);
void mckl_fnmadd_vvv_pd(
    size_t, const double *, const double *, const double *, double *);
void mckl_fnmadd_vvs_pd(
    size_t, const double *, const double *, double, double *);
void mckl_fnmadd_vsv_pd(
    size_t, const double *, double, const double *, double *);
void mckl_fnmadd_svv_pd(
    size_t, double, const double *, const double *, double *);
void mckl_fnmadd_ssv_pd(size_t, double, double, const double *, double *);
void mckl_fnmadd_svs_pd(size_t, double, const double *, double, double *);
void mckl_fnmadd_vss_pd(size_t, const double *, double, double, double *);

void mckl_fnmsub_vvv_ps(
    size_t, const float *, const float *, const float *, float *);
void mckl_fnmsub_vvs_ps(size_t, const float *, const float *, float, float *);
void mckl_fnmsub_vsv_ps(size_t, const float *, float, const float *, float *);
void mckl_fnmsub_svv_ps(size_t, float, const float *, const float *, float *);
void mckl_fnmsub_ssv_ps(size_t, float, float, const float *, float *);
void mckl_fnmsub_svs_ps(size_t, float, const float *, float, float *);
void mckl_fnmsub_vss_ps(size_t, const float *, float, float, float *);
void mckl_fnmsub_vvv_pd(
    size_t, const double *, const double *, const double *, double *);
void mckl_fnmsub_vvs_pd(
    size_t, const double *, const double *, double, double *);
void mckl_fnmsub_vsv_pd(
    size_t, const double *, double, const double *, double *);
void mckl_fnmsub_svv_pd(
    size_t, double, const double *, const double *, double *);
void mckl_fnmsub_ssv_pd(size_t, double, double, const double *, double *);
void mckl_fnmsub_svs_pd(size_t, double, const double *, double, double *);
void mckl_fnmsub_vss_pd(size_t, const double *, double, double, double *);

// fpclassify_avx2.asm
size_t mckl_vs_find_normal(size_t, const float *);
size_t mckl_vd_find_normal(size_t, const double *);
size_t mckl_vs_find_subnormal(size_t, const float *);
size_t mckl_vd_find_subnormal(size_t, const double *);
size_t mckl_vs_find_zero(size_t, const float *);
size_t mckl_vd_find_zero(size_t, const double *);
size_t mckl_vs_find_inf(size_t, const float *);
size_t mckl_vd_find_inf(size_t, const double *);
size_t mckl_vs_find_nan(size_t, const float *);
size_t mckl_vd_find_nan(size_t, const double *);
size_t mckl_vs_find_finite(size_t, const float *);
size_t mckl_vd_find_finite(size_t, const double *);
size_t mckl_vs_count_normal(size_t, const float *);
size_t mckl_vd_count_normal(size_t, const double *);
size_t mckl_vs_count_subnormal(size_t, const float *);
size_t mckl_vd_count_subnormal(size_t, const double *);
size_t mckl_vs_count_zero(size_t, const float *);
size_t mckl_vd_count_zero(size_t, const double *);
size_t mckl_vs_count_inf(size_t, const float *);
size_t mckl_vd_count_inf(size_t, const double *);
size_t mckl_vs_count_nan(size_t, const float *);
size_t mckl_vd_count_nan(size_t, const double *);
size_t mckl_vs_count_finite(size_t, const float *);
size_t mckl_vd_count_finite(size_t, const double *);
size_t mckl_vs_find_not_normal(size_t, const float *);
size_t mckl_vd_find_not_normal(size_t, const double *);
size_t mckl_vs_find_not_subnormal(size_t, const float *);
size_t mckl_vd_find_not_subnormal(size_t, const double *);
size_t mckl_vs_find_not_zero(size_t, const float *);
size_t mckl_vd_find_not_zero(size_t, const double *);
size_t mckl_vs_find_not_inf(size_t, const float *);
size_t mckl_vd_find_not_inf(size_t, const double *);
size_t mckl_vs_find_not_nan(size_t, const float *);
size_t mckl_vd_find_not_nan(size_t, const double *);
size_t mckl_vs_find_not_finite(size_t, const float *);
size_t mckl_vd_find_not_finite(size_t, const double *);
size_t mckl_vs_count_not_normal(size_t, const float *);
size_t mckl_vd_count_not_normal(size_t, const double *);
size_t mckl_vs_count_not_subnormal(size_t, const float *);
size_t mckl_vd_count_not_subnormal(size_t, const double *);
size_t mckl_vs_count_not_zero(size_t, const float *);
size_t mckl_vd_count_not_zero(size_t, const double *);
size_t mckl_vs_count_not_inf(size_t, const float *);
size_t mckl_vd_count_not_inf(size_t, const double *);
size_t mckl_vs_count_not_nan(size_t, const float *);
size_t mckl_vd_count_not_nan(size_t, const double *);
size_t mckl_vs_count_not_finite(size_t, const float *);
size_t mckl_vd_count_not_finite(size_t, const double *);

// aes_aesni_sse2.asm
void mckl_aes128_aesni_sse2_kernel(uint64_t *, size_t, void *, const void *);
void mckl_aes192_aesni_sse2_kernel(uint64_t *, size_t, void *, const void *);
void mckl_aes256_aesni_sse2_kernel(uint64_t *, size_t, void *, const void *);
void mckl_ars_aesni_sse2_kernel(uint64_t *, size_t, void *, const void *);

// aes_aesni_avx2.asm
void mckl_aes128_aesni_avx2_kernel(uint64_t *, size_t, void *, const void *);
void mckl_aes192_aesni_avx2_kernel(uint64_t *, size_t, void *, const void *);
void mckl_aes256_aesni_avx2_kernel(uint64_t *, size_t, void *, const void *);
void mckl_ars_aesni_avx2_kernel(uint64_t *, size_t, void *, const void *);

// philox_sse2_32.asm
void mckl_philox4x32_sse2_kernel(uint64_t *, size_t, void *, const void *);
void mckl_philox2x32_sse2_kernel(uint64_t *, size_t, void *, const void *);

// philox_avx2_32.asm
void mckl_philox2x32_avx2_kernel(uint64_t *, size_t, void *, const void *);
void mckl_philox4x32_avx2_kernel(uint64_t *, size_t, void *, const void *);

// philox_bmi2_2x64.asm
void mckl_philox2x64_bmi2_kernel(uint64_t *, size_t, void *, const void *);

// philox_bmi2_4x64.asm
void mckl_philox4x64_bmi2_kernel(uint64_t *, size_t, void *, const void *);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MCKL_MCKL_H
