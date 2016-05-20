//============================================================================
// MCKL/include/mckl/internal/defines.h
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

#ifndef MCKL_INTERNAL_DEFINES_H
#define MCKL_INTERNAL_DEFINES_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/// \addtogroup C_API_Definitions
/// @{

/// \brief `mckl::MatrixLayout`
typedef enum {
    MCKLRowMajor = 101, ///< `mckl::RowMajor`
    MCKLColMajor = 102  ///< `mckl::ColMajor`
} MCKLMatrixLayout;

/// \brief `mckl::ResampleScheme`
typedef enum {
    MCKLMultinomial,        ///< `mckl::Multinomial`
    MCKLStratified,         ///< `mckl::Stratified`
    MCKLSystematic,         ///< `mckl::Systematic`
    MCKLResidual,           ///< `mckl::Residual`
    MCKLResidualStratified, ///< `mckl::ResidualStratified`
    MCKLResidualSystematic  ///< `mckl::ResidualSystematic`
} MCKLResampleScheme;

/// \brief `mckl::MonitorStage`
typedef enum {
    MCKLMonitorMove,     ///< `mckl::MonitorMove`
    MCKLMonitorResample, ///< `mckl::MonitorResample`
    MCKLMonitorMCMC      ///< `mckl::MonitorMCMC`
} MCKLMonitorStage;

/// \brief `mckl::SamplerStage`
typedef enum {
    MCKLSamplerInit = 1 << 0, ///< `mckl::SamplerInit`
    MCKLSamplerMove = 1 << 1, ///< `mckl::SamplerMove`
    MCKLSamplerMCMC = 1 << 2  ///< `mckl::SamplerMCMC`
} MCKLSamplerStage;

/// \brief RNG types
typedef enum {
    MCKLRNG,    ///< `mckl::RNG`
    MCKLRNG_64, ///< `mckl::RNG_64`

    MCKLRNGMini,    ///< `mckl::RNGMini`
    MCKLRNGMini_64, ///< `mckl::RNGMini_64`

    MCKLPhilox,    ///< `mckl::Philox`
    MCKLPhilox_64, ///< `mckl::Philox_64`

    MCKLThreefry,    ///< `mckl::Threefry`
    MCKLThreefry_64, ///< `mckl::Threefry_64`

    MCKLAES128,    ///< `mckl::AES128`
    MCKLAES128_64, ///< `mckl::AES128_64`

    MCKLAES192,    ///< `mckl::AES192`
    MCKLAES192_64, ///< `mckl::AES192_64`

    MCKLAES256,    ///< `mckl::AES256`
    MCKLAES256_64, ///< `mckl::AES256_64`

    MCKLARS,    ///< `mckl::ARS`
    MCKLARS_64, ///< `mckl::ARS_64`

    MCKLPhilox2x32,    ///< `mckl::Philox2x32`
    MCKLPhilox4x32,    ///< `mckl::Philox4x32`
    MCKLPhilox2x64,    ///< `mckl::Philox2x64`
    MCKLPhilox4x64,    ///< `mckl::Philox4x64`
    MCKLPhilox2x32_64, ///< `mckl::Philox2x32_64`
    MCKLPhilox4x32_64, ///< `mckl::Philox4x32_64`
    MCKLPhilox2x64_64, ///< `mckl::Philox2x64_64`
    MCKLPhilox4x64_64, ///< `mckl::Philox4x64_64`

    MCKLThreefry2x32,     ///< `mckl::Threefry2x32`
    MCKLThreefry4x32,     ///< `mckl::Threefry4x32`
    MCKLThreefry2x64,     ///< `mckl::Threefry2x64`
    MCKLThreefry4x64,     ///< `mckl::Threefry4x64`
    MCKLThreefry8x64,     ///< `mckl::Threefry8x64`
    MCKLThreefry16x64,    ///< `mckl::Threefry16x64`
    MCKLThreefry2x32_64,  ///< `mckl::Threefry2x32_64`
    MCKLThreefry4x32_64,  ///< `mckl::Threefry4x32_64`
    MCKLThreefry2x64_64,  ///< `mckl::Threefry2x64_64`
    MCKLThreefry4x64_64,  ///< `mckl::Threefry4x64_64`
    MCKLThreefry8x64_64,  ///< `mckl::Threefry8x64_64`
    MCKLThreefry16x64_64, ///< `mckl::Threefry16x64_64`

    MCKLAES128x1,    ///< `mckl::AES128x1`
    MCKLAES128x2,    ///< `mckl::AES128x2`
    MCKLAES128x4,    ///< `mckl::AES128x4`
    MCKLAES128x8,    ///< `mckl::AES128x8`
    MCKLAES128x1_64, ///< `mckl::AES128x1_64`
    MCKLAES128x2_64, ///< `mckl::AES128x2_64`
    MCKLAES128x4_64, ///< `mckl::AES128x4_64`
    MCKLAES128x8_64, ///< `mckl::AES128x8_64`

    MCKLAES192x1,    ///< `mckl::AES192x1`
    MCKLAES192x2,    ///< `mckl::AES192x2`
    MCKLAES192x4,    ///< `mckl::AES192x4`
    MCKLAES192x8,    ///< `mckl::AES192x8`
    MCKLAES192x1_64, ///< `mckl::AES192x1_64`
    MCKLAES192x2_64, ///< `mckl::AES192x2_64`
    MCKLAES192x4_64, ///< `mckl::AES192x4_64`
    MCKLAES192x8_64, ///< `mckl::AES192x8_64`

    MCKLAES256x1,    ///< `mckl::AES256x1`
    MCKLAES256x2,    ///< `mckl::AES256x2`
    MCKLAES256x4,    ///< `mckl::AES256x4`
    MCKLAES256x8,    ///< `mckl::AES256x8`
    MCKLAES256x1_64, ///< `mckl::AES256x1_64`
    MCKLAES256x2_64, ///< `mckl::AES256x2_64`
    MCKLAES256x4_64, ///< `mckl::AES256x4_64`
    MCKLAES256x8_64, ///< `mckl::AES256x8_64`

    MCKLARSx1,    ///< `mckl::ARSx1`
    MCKLARSx2,    ///< `mckl::ARSx2`
    MCKLARSx4,    ///< `mckl::ARSx4`
    MCKLARSx8,    ///< `mckl::ARSx8`
    MCKLARSx1_64, ///< `mckl::ARSx1_64`
    MCKLARSx2_64, ///< `mckl::ARSx2_64`
    MCKLARSx4_64, ///< `mckl::ARSx4_64`
    MCKLARSx8_64, ///< `mckl::ARSx8_64`

    MCKLRDRAND16, ///< `mckl::RDRAND16`
    MCKLRDRAND32, ///< `mckl::RDRAND32`
    MCKLRDRAND64  ///< `mckl::RDRAND64`
} MCKLRNGType;

/// \brief SMP backends
typedef enum {
    MCKLBackendSEQ, ///< `mckl::BackendSEQ`
    MCKLBackendSTD, ///< `mckl::BackendSTD`
    MCKLBackendOMP, ///< `mckl::BackendOMP`
    MCKLBackendTBB  ///< `mckl::BackendTBB`
} MCKLBackendSMP;

/// \brief MCKL RNG types
typedef struct {
    void *ptr;
    MCKLRNGType type;
} mckl_rng;

/// \brief ``mckl::StateMatrix<mckl::RowMajor, mckl::Dynamic, double>`
typedef struct {
    void *ptr;
} mckl_state_matrix;

/// \brief `mckl::Weight`
typedef struct {
    void *ptr;
} mckl_weight;

/// \brief `mckl::SingleParticle`
typedef struct {
    double *state;
    size_t i;
} mckl_particle_index;

/// \brief `mckl::Particle`
typedef struct {
    void *ptr;
} mckl_particle;

/// \brief `mckl::Sampler`
typedef struct {
    void *ptr;
} mckl_sampler;

/// \brief `mckl::Monitor`
typedef struct {
    void *ptr;
} mckl_monitor;

/// \brief `mckl::Covariance<double>`
typedef struct {
    void *ptr;
} mckl_covariance;

/// \brief `mckl::StopWatch`
typedef struct {
    void *ptr;
} mckl_stop_watch;

/// \brief `mckl::Sampler::eval_type`
typedef void (*mckl_sampler_eval_type)(size_t, mckl_particle);

/// \brief `mckl::Monitor::eval_type`
typedef void (*mckl_monitor_eval_type)(
    size_t, size_t, mckl_particle, double *);

/// \brief `mckl::SamplerEvalSMP`
typedef struct {
    void (*eval_each)(size_t, mckl_particle_index);
    void (*eval_first)(size_t, mckl_particle);
    void (*eval_last)(size_t, mckl_particle);
} mckl_sampler_eval_smp_type;

/// \brief `mckl::MonitorEvalSMP`
typedef struct {
    void (*eval_each)(size_t, size_t, mckl_particle_index, double *);
    void (*eval_first)(size_t, mckl_particle);
    void (*eval_last)(size_t, mckl_particle);
} mckl_monitor_eval_smp_type;

/// @} C_API_Definitions

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MCKL_INTERNAL_DEFINES_H
