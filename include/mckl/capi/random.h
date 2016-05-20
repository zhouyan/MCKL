//============================================================================
// MCKL/include/mckl/capi/random.h
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

#ifndef MCKL_RANDOM_RANDOM_H
#define MCKL_RANDOM_RANDOM_H

#include <mckl/internal/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/// \addtogroup C_API_Random_RNG
/// @{

/// \brief The maximum of an integer that can be passed to where `MCKLRNGType`
/// value is expected
int mckl_rng_type_max();

/// \brief Check if a given RNGType is defined within the library
int mckl_rng_type_check(MCKLRNGType type);

/// \brief `RNGType::RNGType`
mckl_rng mckl_rng_new(unsigned seed, MCKLRNGType type);

/// \brief `RNGType::~RNGType`
void mckl_rng_delete(mckl_rng *rng_ptr);

/// \brief `RNGType::operator=`
void mckl_rng_assign(mckl_rng rng, mckl_rng other);

/// \brief `RNGType::seed`
void mckl_rng_seed(mckl_rng rng, unsigned seed);

/// \brief `RNGType::discard`
void mckl_rng_discard(mckl_rng rng, unsigned nskip);

/// \brief `RNGType::operator==`
int mckl_rng_is_eq(mckl_rng rng1, mckl_rng rng2);

/// \brief `RNGType::operator!=`
int mckl_rng_is_neq(mckl_rng rng1, mckl_rng rng2);

/// \brief `RNGType::operator<<`
size_t mckl_rng_save(mckl_rng rng, void *mem);

/// \brief `RNGType::operator>>`
void mckl_rng_load(mckl_rng rng, void *mem);

/// \brief `RNGType::operator<<` directly to an external file
void mckl_rng_save_f(mckl_rng rng, const char *filename);

/// \brief `RNGType::operator>>` directly from an external file*/
void mckl_rng_load_f(mckl_rng rng, const char *filename);

/// @} C_API_Random_RNG

/// \addtogroup C_API_Random_Seed
/// @{

/// \brief `mckl::Seed::operator()`
void mckl_seed(mckl_rng rng);

/// \brief `mckl::Seed::get`
unsigned mckl_seed_get(void);

/// \brief `mckl::Seed::set`
void mckl_seed_set(unsigned seed);

/// \brief `mckl::Seed::modulo`
void mckl_seed_modulo(unsigned div, unsigned rem);

/// \brief `mckl::Seed::operator<<`
size_t mckl_seed_save(void *mem);

/// \brief `mckl::Seed::operator>>`
void mckl_seed_load(const void *mem);

/// \brief `mckl::Seed::operator<<` directly to an external file
void mckl_seed_save_f(const char *filename);

/// \brief `mckl::Seed::operator>>` directly from an external file
void mckl_seed_load_f(const char *filename);

/// @} C_API_Random_Seed

/// \addtogroup C_API_Random_Distribution
/// @{

/// \brief `mckl::uniform_bits_distribution`
void mckl_rand(mckl_rng rng, size_t n, unsigned *r);

/// \brief `mckl::uniform_bits_distribution`
void mckl_rand_64(mckl_rng rng, size_t n, unsigned long long *r);

/// \brief `std::bernoulli_distribution`
void mckl_rand_bernoulli(mckl_rng rng, size_t n, int *r, double p);

/// \brief `std::binomial_distribution<int>`
void mckl_rand_binomial(mckl_rng rng, size_t n, int *r, int t, double p);

/// \brief `std::binomial_distribution<long long>`
void mckl_rand_binomial_64(
    mckl_rng rng, size_t n, long long *r, long long t, double p);

/// \brief `mckl::DiscreteDistribution<int>`
void mckl_rand_discrete(mckl_rng rng, size_t n, int *r, size_t m,
    const double *weight, int normalized);

/// \brief `mckl::DiscreteDistribution<long long>`
void mckl_rand_discrete_64(mckl_rng rng, size_t n, long long *r, size_t m,
    const double *weight, int normalized);

/// \brief `std::geometric_distribution<int>`
void mckl_rand_geometric(mckl_rng rng, size_t n, int *r, double p);

/// \brief `std::geometric_distribution<long long>`
void mckl_rand_geometric_64(mckl_rng rng, size_t n, long long *r, double p);

/// \brief `std::negative_binomial_distribution<int>`
void mckl_rand_negative_binomial(
    mckl_rng rng, size_t n, int *r, int k, double p);

/// \brief `std::negative_binomial_distribution<long long>`
void mckl_rand_negative_binomial_64(
    mckl_rng rng, size_t n, long long *r, long long k, double p);

/// \brief `std::poisson_distribution<int>`
void mckl_rand_poisson(mckl_rng rng, size_t n, int *r, double mean);

/// \brief `std::poisson_distribution<long long>`
void mckl_rand_poisson_64(mckl_rng rng, size_t n, long long *r, double mean);

/// \brief `std::uniform_int_distribution<long long>`
void mckl_rand_uniform_int(mckl_rng rng, size_t n, int *r, int a, int b);

/// \brief `std::uniform_int_distribution<long long>`
void mckl_rand_uniform_int_64(
    mckl_rng rng, size_t n, long long *r, long long a, long long b);

/// \brief `mckl::arcsine_distribution<double>`
void mckl_rand_arcsine(
    mckl_rng rng, size_t n, double *r, double alpha, double beta);

/// \brief `mckl::beta_distribution<double>`
void mckl_rand_beta(
    mckl_rng rng, size_t n, double *r, double alpha, double beta);

/// \brief `mckl::cauchy_distribution<double>`
void mckl_rand_cauchy(mckl_rng rng, size_t n, double *r, double a, double b);

/// \brief `mckl::chi_squared_distribution<double>`
void mckl_rand_chi_squared(mckl_rng rng, size_t n, double *r, double df);

/// \brief `mckl::exponential_distribution<double>`
void mckl_rand_exponential(mckl_rng rng, size_t n, double *r, double lambda);

/// \brief `mckl::extreme_value_distribution<double>`
void mckl_rand_extreme_value(
    mckl_rng rng, size_t n, double *r, double a, double b);

/// \brief `mckl::fisher_f_distribution<double>`
void mckl_rand_fisher_f(
    mckl_rng rng, size_t n, double *r, double df1, double df2);

/// \brief `mckl::gamma_distribution<double>`
void mckl_rand_gamma(
    mckl_rng rng, size_t n, double *r, double alpha, double beta);

/// \brief `mckl::laplace_distribution<double>`
void mckl_rand_laplace(mckl_rng rng, size_t n, double *r, double a, double b);

/// \brief `mckl::levy_distribution<double>`
void mckl_rand_levy(mckl_rng rng, size_t n, double *r, double a, double b);

/// \brief `mckl::logistic_distribution<double>`
void mckl_rand_logistic(mckl_rng rng, size_t n, double *r, double a, double b);

/// \brief `mckl::lognormal_distribution<double>`
void mckl_rand_lognormal(
    mckl_rng rng, size_t n, double *r, double m, double s);

/// \brief `mckl::normal_distribution<double>`
void mckl_rand_normal(
    mckl_rng rng, size_t n, double *r, double mean, double stddev);

/// \brief `mckl::normal_mv_distribution<double>`
void mckl_rand_normal_mv(mckl_rng rng, size_t n, double *r, size_t dim,
    const double *mean, const double *chol);

/// \brief `mckl::pareto_distribution<double>`
void mckl_rand_pareto(mckl_rng rng, size_t n, double *r, double a, double b);

/// \brief `mckl::rayleigh_distribution<double>`
void mckl_rand_rayleigh(mckl_rng rng, size_t n, double *r, double b);

/// \brief `mckl::student_t_distribution<double>`
void mckl_rand_student_t(mckl_rng rng, size_t n, double *r, double df);

/// \brief `mckl::u01_distribution<double>`
void mckl_rand_u01(mckl_rng rng, size_t n, double *r);

/// \brief `mckl::u01_cc_distribution<double>`
void mckl_rand_u01_cc(mckl_rng rng, size_t n, double *r);

/// \brief `mckl::u01_co_distribution<double>`
void mckl_rand_u01_co(mckl_rng rng, size_t n, double *r);

/// \brief `mckl::u01_oc_distribution<double>`
void mckl_rand_u01_oc(mckl_rng rng, size_t n, double *r);

/// \brief `mckl::u01_oo_distribution<double>`
void mckl_rand_u01_oo(mckl_rng rng, size_t n, double *r);

/// \brief `mckl::uniform_real_distribution<double>`
void mckl_rand_uniform_real(
    mckl_rng rng, size_t n, double *r, double a, double b);

/// \brief `mckl::weibull_distribution<double>`
void mckl_rand_weibull(mckl_rng rng, size_t n, double *r, double a, double b);

/// @} C_API_Random_Distribution

/// \addtogroup C_API_Random_MKL
/// @{

/// \brief `mckl::mkl_brng`
int mckl_mkl_brng(MCKLRNGType type);

/// @} C_API_Random_MKL

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MCKL_RANDOM_RANDOM_H
