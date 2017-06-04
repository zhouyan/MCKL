//============================================================================
// MCKL/include/mckl/randomc/philox.h
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

#ifndef MCKL_RANDOMC_PHILOX_H
#define MCKL_RANDOMC_PHILOX_H

#include <mckl/internal/config.h>

#ifndef UINT32_C
#error __STDC_CONSTANT_MACROS not defined before #include<stdint.h>
#endif

/// \brief Philox2x32 counter type
/// \ingroup PhiloxC
typedef struct {
    uint32_t v[2];
} mckl_philox2x32_ctr_t;

/// \brief Philox4x32 counter type
/// \ingroup PhiloxC
typedef struct {
    uint32_t v[4];
} mckl_philox4x32_ctr_t;

/// \brief Philox2x32 key type
/// \ingroup PhiloxC
typedef struct {
    uint32_t v[1];
} mckl_philox2x32_key_t;

/// \brief Philox4x32 key type
/// \ingroup PhiloxC
typedef struct {
    uint32_t v[2];
} mckl_philox4x32_key_t;

typedef struct {
    uint32_t v[1];
} mckl_philox2x32_par_t;

typedef struct {
    uint32_t v[2];
} mckl_philox4x32_par_t;

/// \brief Philox2x32 RNG state structure
/// \ingroup PhiloxC
typedef struct {
    mckl_philox2x32_ctr_t state;
    mckl_philox2x32_ctr_t ctr;
    mckl_philox2x32_key_t key;
    uint32_t index;
} mckl_philox2x32;

/// \brief Philox4x32 RNG state structure
/// \ingroup PhiloxC
typedef struct {
    mckl_philox4x32_ctr_t state;
    mckl_philox4x32_ctr_t ctr;
    mckl_philox4x32_key_t key;
    uint32_t index;
} mckl_philox4x32;

static inline void mckl_philox2x32_inc(mckl_philox2x32_ctr_t *ctr)
{
    if (++ctr->v[0] != 0) {
        return;
    }
    if (++ctr->v[1] != 0) {
        return;
    }
}

static inline void mckl_philox4x32_inc(mckl_philox4x32_ctr_t *ctr)
{
    if (++ctr->v[0] != 0) {
        return;
    }
    if (++ctr->v[1] != 0) {
        return;
    }
    if (++ctr->v[2] != 0) {
        return;
    }
    if (++ctr->v[3] != 0) {
        return;
    }
}

static inline void mckl_philox2x32_initpar(
    const mckl_philox2x32_key_t *key, mckl_philox2x32_par_t *par)
{
    par->v[0] = key->v[0];
}

static inline void mckl_philox4x32_initpar(
    const mckl_philox4x32_key_t *key, mckl_philox4x32_par_t *par)
{
    par->v[0] = key->v[0];
    par->v[1] = key->v[1];
}

static inline void mckl_philox2x32_bumpkey(mckl_philox2x32_par_t *par)
{
    par->v[0] += UINT32_C(0x9E3779B9);
}

static inline void mckl_philox4x32_bumpkey(mckl_philox4x32_par_t *par)
{
    par->v[0] += UINT32_C(0x9E3779B9);
    par->v[1] += UINT32_C(0xBB67AE85);
}

static inline void mckl_philox2x32_round(
    mckl_philox2x32_ctr_t *state, const mckl_philox2x32_par_t *par)
{
#ifdef __cplusplus
    uint64_t p = static_cast<uint64_t>(state->v[0]) * UINT64_C(0xD256D193);
    uint32_t hi = static_cast<uint32_t>(p >> 32);
    uint32_t lo = static_cast<uint32_t>(p);
#else
    uint64_t p = ((uint64_t)(state->v[0])) * UINT64_C(0xD256D193);
    uint32_t hi = (uint32_t)(p >> 32);
    uint32_t lo = (uint32_t)(p);
#endif
    hi ^= par->v[0];
    state->v[0] = hi ^ (state->v[1]);
    state->v[1] = lo;
}

static inline void mckl_philox4x32_round(
    mckl_philox4x32_ctr_t *state, const mckl_philox4x32_par_t *par)
{
#ifdef __cplusplus
    uint64_t p0 = static_cast<uint64_t>(state->v[0]) * UINT64_C(0xD2511F53);
    uint64_t p2 = static_cast<uint64_t>(state->v[2]) * UINT64_C(0xCD9E8D57);
    uint32_t hi0 = static_cast<uint32_t>(p2 >> 32);
    uint32_t lo1 = static_cast<uint32_t>(p2);
    uint32_t hi2 = static_cast<uint32_t>(p0 >> 32);
    uint32_t lo3 = static_cast<uint32_t>(p0);
#else
    uint64_t p0 = ((uint64_t)(state->v[0])) * UINT64_C(0xD2511F53);
    uint64_t p2 = ((uint64_t)(state->v[2])) * UINT64_C(0xCD9E8D57);
    uint32_t hi0 = (uint32_t)(p2 >> 32);
    uint32_t lo1 = (uint32_t)(p2);
    uint32_t hi2 = (uint32_t)(p0 >> 32);
    uint32_t lo3 = (uint32_t)(p0);
#endif
    hi0 ^= par->v[0];
    hi2 ^= par->v[1];
    state->v[0] = hi0 ^ (state->v[1]);
    state->v[1] = lo1;
    state->v[2] = hi2 ^ (state->v[3]);
    state->v[3] = lo3;
}

/// \brief Generate Philox2x32 RNG state
/// \ingroup PhiloxC
static inline void mckl_philox2x32_gen(const mckl_philox2x32_ctr_t *ctr,
    const mckl_philox2x32_key_t *key, mckl_philox2x32_ctr_t *state)
{
    *state = *ctr;
    mckl_philox2x32_par_t par;
    mckl_philox2x32_initpar(key, &par);

    mckl_philox2x32_round(state, &par);
    mckl_philox2x32_bumpkey(&par);
    mckl_philox2x32_round(state, &par);
    mckl_philox2x32_bumpkey(&par);
    mckl_philox2x32_round(state, &par);
    mckl_philox2x32_bumpkey(&par);
    mckl_philox2x32_round(state, &par);
    mckl_philox2x32_bumpkey(&par);
    mckl_philox2x32_round(state, &par);
    mckl_philox2x32_bumpkey(&par);
    mckl_philox2x32_round(state, &par);
    mckl_philox2x32_bumpkey(&par);
    mckl_philox2x32_round(state, &par);
    mckl_philox2x32_bumpkey(&par);
    mckl_philox2x32_round(state, &par);
    mckl_philox2x32_bumpkey(&par);
    mckl_philox2x32_round(state, &par);
    mckl_philox2x32_bumpkey(&par);
    mckl_philox2x32_round(state, &par);
}

/// \brief Generate Philox4x32 RNG state
/// \ingroup PhiloxC
static inline void mckl_philox4x32_gen(const mckl_philox4x32_ctr_t *ctr,
    const mckl_philox4x32_key_t *key, mckl_philox4x32_ctr_t *state)
{
    *state = *ctr;
    mckl_philox4x32_par_t par;
    mckl_philox4x32_initpar(key, &par);

    mckl_philox4x32_round(state, &par);
    mckl_philox4x32_bumpkey(&par);
    mckl_philox4x32_round(state, &par);
    mckl_philox4x32_bumpkey(&par);
    mckl_philox4x32_round(state, &par);
    mckl_philox4x32_bumpkey(&par);
    mckl_philox4x32_round(state, &par);
    mckl_philox4x32_bumpkey(&par);
    mckl_philox4x32_round(state, &par);
    mckl_philox4x32_bumpkey(&par);
    mckl_philox4x32_round(state, &par);
    mckl_philox4x32_bumpkey(&par);
    mckl_philox4x32_round(state, &par);
    mckl_philox4x32_bumpkey(&par);
    mckl_philox4x32_round(state, &par);
    mckl_philox4x32_bumpkey(&par);
    mckl_philox4x32_round(state, &par);
    mckl_philox4x32_bumpkey(&par);
    mckl_philox4x32_round(state, &par);
}

/// \brief Initialize Philox2x32 RNG state
/// \ingroup PhiloxC
static inline void mckl_philox2x32_init(mckl_philox2x32 *rng, uint32_t seed)
{
    rng->ctr.v[0] = 0;
    rng->ctr.v[1] = 0;
    rng->key.v[0] = seed;
    rng->index = 2;
}

/// \brief Initialize Philox4x32 RNG state
/// \ingroup PhiloxC
static inline void mckl_philox4x32_init(mckl_philox4x32 *rng, uint32_t seed)
{
    rng->ctr.v[0] = 0;
    rng->ctr.v[1] = 0;
    rng->ctr.v[2] = 0;
    rng->ctr.v[3] = 0;
    rng->key.v[0] = seed;
    rng->key.v[1] = 0;
    rng->index = 4;
}

/// \brief Generate random 32-bit integers from Philox2x32 RNG
/// \ingroup PhiloxC
static inline uint32_t mckl_philox2x32_rand(mckl_philox2x32 *rng)
{
    if (rng->index == 2) {
        mckl_philox2x32_inc(&rng->ctr);
        mckl_philox2x32_gen(&rng->ctr, &rng->key, &rng->state);
        rng->index = 0;
    }

    return rng->state.v[rng->index++];
}

/// \brief Generate random 32-bit integers from Philox4x32 RNG
/// \ingroup PhiloxC
static inline uint32_t mckl_philox4x32_rand(mckl_philox4x32 *rng)
{
    if (rng->index == 4) {
        mckl_philox4x32_inc(&rng->ctr);
        mckl_philox4x32_gen(&rng->ctr, &rng->key, &rng->state);
        rng->index = 0;
    }

    return rng->state.v[rng->index++];
}

#endif // MCKL_RANDOMC_PHILOX_H
