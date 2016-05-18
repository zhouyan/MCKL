#include "pf.h"

static inline void pf_init_first(size_t t, mckl_particle particle)
{
    pf_normal(particle, 2, 1);
}

static inline void pf_init_each(size_t t, mckl_particle_index idx)
{
    idx.state[PosX] = pf_pos_x[idx.i];
    idx.state[PosY] = pf_pos_y[idx.i];
    idx.state[VelX] = pf_vel_x[idx.i];
    idx.state[VelY] = pf_vel_y[idx.i];
}

static inline void pf_move_first(size_t t, mckl_particle particle)
{
    pf_normal(particle, sqrt(0.02), sqrt(0.001));
}

static inline void pf_move_each(size_t t, mckl_particle_index idx)
{
    idx.state[PosX] += pf_pos_x[idx.i] + 0.1 * idx.state[VelX];
    idx.state[PosY] += pf_pos_y[idx.i] + 0.1 * idx.state[VelY];
    idx.state[VelX] += pf_vel_x[idx.i];
    idx.state[VelY] += pf_vel_y[idx.i];
}

static inline void pf_weight_each(size_t t, mckl_particle_index idx)
{
    pf_inc_w[idx.i] = pf_log_likelihood(t, idx);
}

static inline void pf_weight_last(size_t t, mckl_particle particle)
{
    mckl_weight_add_log(mckl_particle_weight(particle), pf_inc_w, 1);
}

static inline void pf_eval_each(
    size_t t, size_t dim, mckl_particle_index idx, double *r)
{
    r[0] = idx.state[PosX];
    r[1] = idx.state[PosY];
}

int main(int argc, char **argv)
{
    size_t n = 10000;
    if (argc > 1)
        n = (size_t) atoi(argv[1]);

    pf_malloc(n);

    mckl_sampler_eval_smp_type pf_init = {pf_init_each, pf_init_first, NULL};
    mckl_sampler_eval_smp_type pf_move = {pf_move_each, pf_move_first, NULL};
    mckl_sampler_eval_smp_type pf_weight = {
        pf_weight_each, NULL, pf_weight_last};
    mckl_monitor_eval_smp_type pf_eval = {pf_eval_each, NULL, NULL};

    mckl_sampler sampler = mckl_sampler_new(n, 4);
    mckl_sampler_resample_scheme(sampler, MCKLStratified, 0.5);
    mckl_sampler_eval_smp(MCKLBackendTBB, sampler, pf_init, MCKLSamplerInit);
    mckl_sampler_eval_smp(MCKLBackendTBB, sampler, pf_move, MCKLSamplerMove);
    mckl_sampler_eval_smp(
        MCKLBackendTBB, sampler, pf_weight, MCKLSamplerInit | MCKLSamplerMove);
    mckl_monitor pf_pos =
        mckl_monitor_new_smp(MCKLBackendTBB, 2, pf_eval, 0, MCKLMonitorMCMC);
    mckl_sampler_set_monitor(sampler, "pos", pf_pos);
    mckl_monitor_delete(&pf_pos);
    mckl_sampler_initialize(sampler);
    mckl_sampler_iterate(sampler, pf_obs_size - 1);
    mckl_sampler_print_f(sampler, "pf.out", '\t');
    mckl_sampler_delete(&sampler);

    pf_free();

    return 0;
}
