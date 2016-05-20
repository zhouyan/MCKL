//============================================================================
// MCKL/manual/prg/pf.h
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
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static const size_t PosX = 0;
static const size_t PosY = 1;
static const size_t VelX = 2;
static const size_t VelY = 3;

// Storage for data
static size_t pf_obs_size = 0;
static double *pf_obs_x = NULL;
static double *pf_obs_y = NULL;

// Temporaries used by pf_init and pf_move
static double *pf_pos_x = NULL;
static double *pf_pos_y = NULL;
static double *pf_vel_x = NULL;
static double *pf_vel_y = NULL;
static double *pf_inc_w = NULL;

static inline void pf_malloc(size_t n)
{
    FILE *data = fopen("pf.data", "r");
    pf_obs_size = 0;
    while (1) {
        double x;
        double y;
        int nx = fscanf(data, "%lg", &x);
        int ny = fscanf(data, "%lg", &y);
        if (nx == 1 && ny == 1)
            ++pf_obs_size;
        else
            break;
    }
    pf_obs_x = mckl_malloc(pf_obs_size * sizeof(double), 32);
    pf_obs_y = mckl_malloc(pf_obs_size * sizeof(double), 32);
    fseek(data, 0, SEEK_SET);
    for (size_t i = 0; i < n; ++i) {
        fscanf(data, "%lg", &pf_obs_x[i]);
        fscanf(data, "%lg", &pf_obs_y[i]);
    }
    fclose(data);

    pf_pos_x = mckl_malloc(n * sizeof(double), 32);
    pf_pos_y = mckl_malloc(n * sizeof(double), 32);
    pf_vel_x = mckl_malloc(n * sizeof(double), 32);
    pf_vel_y = mckl_malloc(n * sizeof(double), 32);
    pf_inc_w = mckl_malloc(n * sizeof(double), 32);
}

static inline void pf_free()
{
    mckl_free(pf_obs_x);
    mckl_free(pf_obs_y);
    mckl_free(pf_pos_x);
    mckl_free(pf_pos_y);
    mckl_free(pf_vel_x);
    mckl_free(pf_vel_y);
    mckl_free(pf_inc_w);
}

static inline double pf_log_likelihood(size_t t, mckl_particle_index idx)
{
    double llh_x = 10 * (idx.state[PosX] - pf_obs_x[t]);
    double llh_y = 10 * (idx.state[PosY] - pf_obs_y[t]);
    llh_x = log(1 + llh_x * llh_x / 10);
    llh_y = log(1 + llh_y * llh_y / 10);

    return -0.5 * (10 + 1) * (llh_x + llh_y);
}

static inline void pf_normal(
    mckl_particle particle, double sd_pos, double sd_vel)
{
    const mckl_rng rng = mckl_particle_rng(particle, 0);
    const size_t n = mckl_particle_size(particle);
    mckl_rand_normal(rng, n, pf_pos_x, 0, sd_pos);
    mckl_rand_normal(rng, n, pf_pos_y, 0, sd_pos);
    mckl_rand_normal(rng, n, pf_vel_x, 0, sd_vel);
    mckl_rand_normal(rng, n, pf_vel_y, 0, sd_vel);
}
