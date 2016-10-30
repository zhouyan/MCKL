//============================================================================
// MCKL/example/pf/src/pf.cpp
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

#include "pf.hpp"
#include <mckl/random/normal_distribution.hpp>

double pf_log_likelihood(double x, double y, double obs_x, double obs_y)
{
    const double scale = 10;
    const double nu = 10;

    double llh_x = scale * (x - obs_x);
    double llh_y = scale * (y - obs_y);

    llh_x = std::log(1 + llh_x * llh_x / nu);
    llh_y = std::log(1 + llh_y * llh_y / nu);

    return -0.5 * (nu + 1) * (llh_x + llh_y);
}

void pf_init_each(
    mckl::RNG &rng, double &pos_x, double &pos_y, double &vel_x, double &vel_y)
{
    const double sd_pos0 = 2;
    const double sd_vel0 = 1;
    mckl::NormalDistribution<double> normal_pos(0, sd_pos0);
    mckl::NormalDistribution<double> normal_vel(0, sd_vel0);

    pos_x = normal_pos(rng);
    pos_y = normal_pos(rng);
    vel_x = normal_vel(rng);
    vel_y = normal_vel(rng);
}

void pf_move_each(
    mckl::RNG &rng, double &pos_x, double &pos_y, double &vel_x, double &vel_y)
{
    const double sd_pos = std::sqrt(0.02);
    const double sd_vel = std::sqrt(0.001);
    const double delta = 0.1;
    mckl::NormalDistribution<double> normal_pos(0, sd_pos);
    mckl::NormalDistribution<double> normal_vel(0, sd_vel);

    pos_x += normal_pos(rng) + delta * vel_x;
    pos_y += normal_pos(rng) + delta * vel_y;
    vel_x += normal_vel(rng);
    vel_y += normal_vel(rng);
}

void pf_move_range(mckl::RNG &rng, std::size_t n, double *pos_x, double *pos_y,
    double *vel_x, double *vel_y, double *w, double *v)
{
    const double sd_pos = std::sqrt(0.02);
    const double sd_vel = std::sqrt(0.001);
    const double delta = 0.1;
    mckl::NormalDistribution<double> normal_pos(0, sd_pos);
    mckl::NormalDistribution<double> normal_vel(0, sd_vel);

    normal_pos(rng, n, w);
    normal_pos(rng, n, v);
    mckl::add(n, w, pos_x, pos_x);
    mckl::add(n, v, pos_y, pos_y);
    mckl::fma(n, delta, vel_x, pos_x, pos_x);
    mckl::fma(n, delta, vel_y, pos_y, pos_y);
    normal_vel(rng, n, w);
    normal_vel(rng, n, v);
    mckl::add(n, w, vel_x, vel_x);
    mckl::add(n, v, vel_y, vel_y);
}
