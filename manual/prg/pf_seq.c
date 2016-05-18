#include "pf.h"

static inline void pf_init(size_t t, mckl_particle particle)
{
    pf_normal(particle, 2, 1);
    const size_t n = mckl_particle_size(particle);
    for (size_t i = 0; i < n; ++i) {
        const mckl_particle_index idx = mckl_particle_get_index(particle, i);
        idx.state[PosX] = pf_pos_x[i];
        idx.state[PosY] = pf_pos_y[i];
        idx.state[VelX] = pf_vel_x[i];
        idx.state[VelY] = pf_vel_y[i];
    }
}

static inline void pf_move(size_t t, mckl_particle particle)
{
    pf_normal(particle, sqrt(0.02), sqrt(0.001));
    const size_t n = mckl_particle_size(particle);
    for (size_t i = 0; i < n; ++i) {
        mckl_particle_index idx = mckl_particle_get_index(particle, i);
        idx.state[PosX] += pf_pos_x[i] + 0.1 * idx.state[VelX];
        idx.state[PosY] += pf_pos_y[i] + 0.1 * idx.state[VelY];
        idx.state[VelX] += pf_vel_x[i];
        idx.state[VelY] += pf_vel_y[i];
    }
}

static inline void pf_weight(size_t t, mckl_particle particle)
{
    const size_t n = mckl_particle_size(particle);
    for (size_t i = 0; i < n; ++i)
        pf_inc_w[i] =
            pf_log_likelihood(t, mckl_particle_get_index(particle, i));
    mckl_weight_add_log(mckl_particle_weight(particle), pf_inc_w, 1);
}

static inline void pf_eval(
    size_t t, size_t dim, mckl_particle particle, double *r)
{
    const size_t n = mckl_particle_size(particle);
    for (size_t i = 0; i < n; ++i) {
        mckl_particle_index idx = mckl_particle_get_index(particle, i);
        *r++ = idx.state[PosX];
        *r++ = idx.state[PosY];
    }
}

int main(int argc, char **argv)
{
    size_t n = 10000;
    if (argc > 1)
        n = (size_t) atoi(argv[1]);

    pf_malloc(n);

    mckl_sampler sampler = mckl_sampler_new(n, 4);
    mckl_sampler_resample_scheme(sampler, MCKLStratified, 0.5);
    mckl_sampler_eval(sampler, pf_init, MCKLSamplerInit);
    mckl_sampler_eval(sampler, pf_move, MCKLSamplerMove);
    mckl_sampler_eval(sampler, pf_weight, MCKLSamplerInit | MCKLSamplerMove);
    mckl_monitor pf_pos = mckl_monitor_new(2, pf_eval, 0, MCKLMonitorMCMC);
    mckl_sampler_set_monitor(sampler, "pos", pf_pos);
    mckl_monitor_delete(&pf_pos);
    mckl_sampler_initialize(sampler);
    mckl_sampler_iterate(sampler, pf_obs_size - 1);
    mckl_sampler_print_f(sampler, "pf.out", '\t');
    mckl_sampler_delete(&sampler);

    pf_free();

    return 0;
}
