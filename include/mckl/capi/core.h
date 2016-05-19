//============================================================================
// MCKL/include/mckl/core/core.h
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

#ifndef MCKL_CORE_CORE_H
#define MCKL_CORE_CORE_H

#include <mckl/internal/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/// \addtogroup C_API_Core_Weight
/// @{

/// \brief `mckl::Weight::Weight`
mckl_weight mckl_weight_new(size_t n);

/// \brief `mckl::Weight::~Weight`
void mckl_weight_delete(mckl_weight *weight_ptr);

/// \brief `mckl::Weight::operator=`
void mckl_weight_assign(mckl_weight weight, mckl_weight other);

/// \brief `mckl::Weight::size`
size_t mckl_weight_size(mckl_weight weight);

/// \brief `mckl::Weight::resize`
void mckl_weight_resize(mckl_weight weight, size_t n);

/// \brief `mckl::Weight::reserve`
void mckl_weight_reserve(mckl_weight weight, size_t n);

/// \brief `mckl::Weight::shrink_to_fit`
void mckl_weight_shrink_to_fit(mckl_weight weight);

/// \brief `mckl::Weight::ess`
double mckl_weight_ess(mckl_weight weight);

/// \brief `mckl::Weight::read_weight`
void mckl_weight_read_weight(mckl_weight weight, double *first, size_t stride);

/// \brief `mckl::Weight::data`
const double *mckl_weight_data(mckl_weight weight);

/// \brief `mckl::Weight::set_equal`
void mckl_weight_set_equal(mckl_weight weight);

/// \brief `mckl::Weight::set`
void mckl_weight_set(mckl_weight weight, const double *first, size_t stride);

/// \brief `mckl::Weight::mul`
void mckl_weight_mul(mckl_weight weight, const double *first, size_t stride);

/// \brief `mckl::Weight::set_log`
void mckl_weight_set_log(
    mckl_weight weight, const double *first, size_t stride);

/// \brief `mckl::Weight::add_log`
void mckl_weight_add_log(
    mckl_weight weight, const double *first, size_t stride);

/// \brief `mckl::Weight::draw`
size_t mckl_weight_draw(mckl_weight weight, mckl_rng rng);

/// @} C_API_Core_Weight

/// \addtogroup C_API_Core_StateMatrix
/// @{

/// \brief `mckl::StateMatrix::StateMatrix`
mckl_state_matrix mckl_state_matrix_new(size_t n, size_t dim);

/// \brief `mckl::StateMatrix::~StateMatrix`
void mckl_state_matrix_delete(mckl_state_matrix *state_matrix_ptr);

/// \brief `mckl::StateMatrix::operator=`
void vsm_state_matrix_assign(
    mckl_state_matrix state_matrix, mckl_state_matrix other);

/// \brief `mckl::StateMatrix::size`
size_t mckl_state_matrix_size(mckl_state_matrix state_matrix);

/// \brief `mckl::StateMatrix::dim`
size_t mckl_state_matrix_dim(mckl_state_matrix state_matrix);

/// \brief `mckl::StateMatrix::resize`
void mckl_state_matrix_resize(
    mckl_state_matrix state_matrix, size_t n, size_t dim);

/// \brief `mckl::StateMatrix::reserve`
void mckl_state_matrix_reserve(
    mckl_state_matrix state_matrix, size_t n, size_t dim);

/// \brief `mckl::StateMatrix::shrink_to_fit`
void mckl_state_matrix_shrink_to_fit(mckl_state_matrix state_matrix);

/// \brief `mckl::StateMatrix::state`
double mckl_state_matrix_get(
    mckl_state_matrix state_matrix, size_t id, size_t pos);

/// \brief `mckl::StateMatrix::state`
void mckl_state_matrix_set(
    mckl_state_matrix state_matrix, size_t id, size_t pos, double s);

/// \brief `mckl::StateMatrix::data`
double *mckl_state_matrix_data(mckl_state_matrix state_matrix);

/// \brief `mckl::StateMatrix::row_data`
double *mckl_state_matrix_row_data(mckl_state_matrix state_matrix, size_t id);

/// \brief `mckl::StateMatrix::row_data`
double *state_matrix_row_data(mckl_state_matrix state_matrix, size_t id);

/// \brief `mckl::StateMatrix::read_state`
void mckl_state_matrix_read_state(
    mckl_state_matrix state_matrix, size_t pos, double *first);

/// \brief `mckl::StateMatrix::read_state_matrix`
void mckl_state_matrix_read_state_matrix(
    mckl_state_matrix state_matrix, MCKLMatrixLayout layout, double *first);

/// \brief `mckl::StateMatrix::copy`
void mckl_state_matrix_select(
    mckl_state_matrix state_matrix, size_t n, const size_t *index);

/// \brief `mckl::StateMatrix::duplicate`
void mckl_state_matrix_duplicate(
    mckl_state_matrix state_matrix, size_t src, size_t dst);

/// @} C_API_Core_StateMatrix

/// \addtogroup C_API_Core_Particle
/// @{

/// \brief `mckl::Particle::Particle`
mckl_particle mckl_particle_new(size_t n, size_t dim);

/// \brief `mckl::Particle::~Particle`
void mckl_particle_delete(mckl_particle *particle_ptr);

/// \brief `mckl::Particle::operator=`
void mckl_particle_assign(mckl_particle particle, mckl_particle other);

/// \brief `mckl::Particle::clone`
mckl_particle mckl_particle_clone(mckl_particle particle);

/// \brief `mckl::Particle::size`
size_t mckl_particle_size(mckl_particle particle);

/// \brief `mckl::Particle::resize_by_index`
void mckl_particle_resize_by_index(
    mckl_particle particle, size_t n, const size_t *index);

/// \brief `mckl::Particle::resize_by_resample`
void mckl_particle_resize_by_resample(
    mckl_particle particle, size_t n, MCKLResampleScheme scheme);

/// \brief `mckl::Particle::resize_by_uniform`
void mckl_particle_resize_by_uniform(mckl_particle particle, size_t n);

/// \brief `mckl::Particle::state`
mckl_state_matrix mckl_particle_state(mckl_particle particle);

/// \brief `mckl::Particle::weight`
mckl_weight mckl_particle_weight(mckl_particle particle);

/// \brief `mckl::Particle::rng`
mckl_rng mckl_particle_rng(mckl_particle particle, size_t id);

/// \brief `mckl::Particle::index`
mckl_particle_index mckl_particle_get_index(mckl_particle particle, size_t id);

/// @} C_API_Core_Particle

/// \addtogroup C_API_Core_Monitor
/// @{

/// \brief `mckl::Monitor::Monitor`
mckl_monitor mckl_monitor_new(size_t dim, mckl_monitor_eval_type eval,
    int record_only, MCKLMonitorStage stage);

/// \brief `mckl::Monitor::~Monitor`
void mckl_monitor_delete(mckl_monitor *monitor_ptr);

/// \brief `mckl::Monitor::operator=`
void mckl_monitor_assign(mckl_monitor monitor, mckl_monitor other);

/// \brief `mckl::Monitor::dim`
size_t mckl_monitor_dim(mckl_monitor monitor);

/// \brief `mckl::Monitor::record_only`
int mckl_monitor_record_only(mckl_monitor monitor);

/// \brief `mckl::Monitor::stage`
MCKLMonitorStage mckl_monitor_stage(mckl_monitor monitor);

/// \brief `mckl::Monitor::iter_size`
size_t mckl_monitor_iter_size(mckl_monitor monitor);

/// \brief `mckl::Monitor::reserve`
void mckl_monitor_reserve(mckl_monitor monitor, size_t num);

/// \brief `mckl::Monitor::empty`
int mckl_monitor_empty(mckl_monitor monitor);

/// \brief `mckl::Monitor::name`
void mckl_monitor_set_name(mckl_monitor monitor, size_t id, const char *name);

/// \brief `mckl::Monitor::name`
size_t mckl_monitor_get_name(mckl_monitor monitor, size_t id, char *name);

/// \brief `mckl::Monitor::index`
size_t mckl_monitor_index(mckl_monitor monitor, size_t iter);

/// \brief `mckl::Monitor::read_index`
void mckl_monitor_read_index(mckl_monitor monitor, size_t *fist);

/// \brief `mckl::Monitor::record`
double mckl_monitor_record(mckl_monitor monitor, size_t id, size_t iter);

/// \brief `mckl::Monitor::read_record`
void mckl_monitor_read_record(mckl_monitor monitor, size_t id, double *first);

/// \brief `mckl::Monitor::read_record_matrix`
void mckl_monitor_read_record_matrix(
    mckl_monitor monitor, MCKLMatrixLayout layout, double *first);

/// \brief `mckl::Monitor::eval`
void mckl_monitor_eval(mckl_monitor monitor, mckl_monitor_eval_type new_eval,
    int record_only, MCKLMonitorStage stage);

/// \brief `mckl::Monitor::operator()`
void mckl_monitor_compute(mckl_monitor monitor, size_t iter,
    mckl_particle particle, MCKLMonitorStage stage);

/// \brief `mckl::Monitor::clear`
void mckl_monitor_clear(mckl_monitor monitor);

/// @} C_API_Core_Monitor

/// \addtogroup C_API_Core_Sampler
/// @{

/// \brief `mckl::Sampler::Sampler`
mckl_sampler mckl_sampler_new(size_t n, size_t dim);

/// \brief `mckl::Sampler::~Sampler`
void mckl_sampler_delete(mckl_sampler *sampler_ptr);

/// \brief `mckl::Sampler::operator=`
void mckl_sampler_assign(mckl_sampler sampler, mckl_sampler other);

/// \brief `mckl::Sampler::clone`
mckl_sampler mckl_sampler_clone(mckl_sampler sampler);

/// \brief `mckl::Sampler::size`
size_t mckl_sampler_size(mckl_sampler sampler);

/// \brief `mckl::Sampler::reserve`
void mckl_sampler_reserve(mckl_sampler sampler, size_t num);

/// \brief `mckl::Sampler::iter_size`
size_t mckl_sampler_iter_size(mckl_sampler sampler);

/// \brief `mckl::Sampler::iter_num`
size_t mckl_sampler_iter_num(mckl_sampler sampler);

/// \brief `mckl::Sampler::reset`
void mckl_sampler_reset(mckl_sampler sampler);

/// \brief `mckl::Sampler::clear`
void mckl_sampler_clear(mckl_sampler sampler);

/// \brief `mckl::Sampler::eval_clear`
void mckl_sampler_eval_clear(mckl_sampler sampler);

/// \brief `mckl::Sampler::eval`
void mckl_sampler_eval(mckl_sampler sampler, mckl_sampler_eval_type new_eval,
    MCKLSamplerStage stage);

/// \brief `mckl::Sampler::resample_method`
void mckl_sampler_resample_scheme(
    mckl_sampler sampler, MCKLResampleScheme scheme, double threshold);

/// \brief `mckl::Sampler::resample_method`
void mckl_sampler_resample_eval(
    mckl_sampler sampler, mckl_sampler_eval_type res_move, double threshold);

/// \brief `mckl::Sampler::resample_threshold`
double mckl_sampler_get_threshold(mckl_sampler sampler);

/// \brief `mckl::Sampler::resample_threshold`
void mckl_sampler_set_threshold(mckl_sampler sampler, double threshold);

/// \brief `mckl::Sampler::resample_threshold_never`
double mckl_sampler_resample_threshold_never(void);

/// \brief `mckl::Sampler::resample_threshold_always`
double mckl_sampler_resample_threshold_always(void);

/// \brief `mckl::Sampler::monitor`
void mckl_sampler_set_monitor(
    mckl_sampler sampler, const char *name, mckl_monitor mon);

/// \brief `mckl::Sampler::monitor`
mckl_monitor mckl_sampler_get_monitor(mckl_sampler sampler, const char *name);

/// \brief `mckl::Sampler::monitor_clear`
void mckl_sampler_monitor_clear(mckl_sampler sampler, const char *name);

/// \brief `mckl::Sampler::monitor_clear`
void mckl_sampler_monitor_clear_all(mckl_sampler sampler);

/// \brief `mckl::Sampler::initialize`
void mckl_sampler_initialize(mckl_sampler sampler);

/// \brief `mckl::Sampler::iterate`
void mckl_sampler_iterate(mckl_sampler sampler, size_t num);

/// \brief `mckl::Sampler::particle`
mckl_particle mckl_sampler_particle(mckl_sampler sampler);

/// \brief `mckl::Sampler::size_history`
size_t mckl_sampler_size_history(mckl_sampler sampler, size_t iter);

/// \brief `mckl::Sampler::read_size_history`
void mckl_sampler_read_size_history(mckl_sampler sampler, size_t *first);

/// \brief `mckl::Sampler::ess_history`
double mckl_sampler_ess_history(mckl_sampler sampler, size_t iter);

/// \brief `mckl::Sampler::read_ess_history`
void mckl_sampler_read_ess_history(mckl_sampler sampler, double *first);

/// \brief `mckl::Sampler::resampled_history`
int mckl_sampler_resampled_history(mckl_sampler sampler, size_t iter);

/// \brief `mckl::Sampler::read_resampled_history`
void mckl_sampler_read_resampled_history(mckl_sampler sampler, int *first);

/// \brief `mckl::Sampler::print`
size_t mckl_sampler_print(mckl_sampler sampler, char *buf, char sepchar);

/// \brief `mckl::Sampler::print` directly to an external file
void mckl_sampler_print_f(
    mckl_sampler sampler, const char *filename, char sepchar);

/// @} C_API_Core_Sampler

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MCKL_CORE_CORE_H
