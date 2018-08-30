//============================================================================
// MCKL/include/mckl/random/stable_distribution.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
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

#ifndef MCKL_RANDOM_STABLE_DISTRIBUTION_HPP
#define MCKL_RANDOM_STABLE_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01_distribution.hpp>

namespace mckl {

namespace internal {

template <typename RealType>
inline bool stable_distribution_check_param(
    RealType alpha, RealType beta, RealType, RealType b)
{
    if (alpha <= 0) {
        return false;
    }
    if (alpha > 2) {
        return false;
    }
    if (beta < -1) {
        return false;
    }
    if (beta > 1) {
        return false;
    }
    if (b <= 0) {
        return false;
    }
    return true;
}

enum StableDistributionAlgorithm {
    StableDistributionAlgorithm1,
    StableDistributionAlgorithmA
}; // enum StableDistributionAlgorithm

MCKL_PUSH_CLANG_WARNING("-Wpadded")
template <typename RealType>
class StableDistributionConstant
{
  public:
    StableDistributionConstant(
        RealType alpha = 1, RealType beta = 0, RealType = 0, RealType b = 1)
    {
        MCKL_PUSH_CLANG_WARNING("-Wfloat-equal")
        MCKL_PUSH_INTEL_WARNING(1572) // floating-point comparison
        algorithm_ = alpha == 1 ? StableDistributionAlgorithm1 :
                                  StableDistributionAlgorithmA;
        MCKL_POP_CLANG_WARNING
        MCKL_POP_INTEL_WARNING

        RealType zeta = -beta * std::tan(const_pi_by2<RealType>() * alpha);
        xi_ = c_ = 0;
        switch (algorithm_) {
            case StableDistributionAlgorithm1:
                xi_ = const_pi_by2<RealType>();
                c_ = 1 / const_pi_by2<RealType>() * std::log(b);
                break;
            case StableDistributionAlgorithmA:
                xi_ = 1 / alpha * std::atan(-zeta);
                c_ = std::pow(1 + zeta * zeta, 1 / (2 * alpha));
                break;
        }
    }

    RealType xi() const { return xi_; }
    RealType c() const { return c_; }
    StableDistributionAlgorithm algorithm() const { return algorithm_; }

    friend bool operator==(const StableDistributionConstant<RealType> &c1,
        const StableDistributionConstant<RealType> &c2)
    {
        MCKL_PUSH_CLANG_WARNING("-Wfloat-equal")
        MCKL_PUSH_INTEL_WARNING(1572) // floating-point comparison
        if (c1.xi_ != c2.xi_) {
            return false;
        }
        if (c1.c_ != c2.c_) {
            return false;
        }
        if (c1.algorithm_ != c2.algorithm_) {
            return false;
        }
        return true;
        MCKL_POP_CLANG_WARNING
        MCKL_POP_INTEL_WARNING
    }

  private:
    RealType xi_;
    RealType c_;
    StableDistributionAlgorithm algorithm_;
}; // class StableDistributionConstant
MCKL_POP_CLANG_WARNING

template <std::size_t K, typename RealType, typename RNGType>
inline void stable_distribution_impl_1(RNGType &rng, std::size_t n,
    RealType *r, RealType, RealType beta, RealType, RealType,
    const StableDistributionConstant<RealType> &constant)
{
    alignas(MCKL_ALIGNMENT) std::array<RealType, K * 3> s;
    const RealType xi = constant.xi();
    const RealType c = constant.c();
    RealType *const u = s.data();
    RealType *const w = s.data() + n;
    RealType *const t = s.data() + n * 2;
    u01_oo_distribution(rng, n, u);
    muladd(n, u, const_pi<RealType>(), -const_pi_by2<RealType>(), u);
    u01_oo_distribution(rng, n, w);
    log(n, w, w);
    mul(n, -const_one<RealType>(), w, w);
    muladd(n, u, beta, const_pi_by2<RealType>(), t);
    tan(n, u, r);
    mul(n, t, r, r);
    cos(n, u, u);
    mul(n, w, u, u);
    div(n, const_pi_by2<RealType>(), t, t);
    mul(n, u, t, t);
    muladd(n, t, -beta, r, r);
    muladd(n, r, 1 / xi, c, r);
}

template <std::size_t K, typename RealType, typename RNGType>
inline void stable_distribution_impl_a(RNGType &rng, std::size_t n,
    RealType *r, RealType alpha, RealType, RealType, RealType,
    const StableDistributionConstant<RealType> &constant)
{
    alignas(MCKL_ALIGNMENT) std::array<RealType, K * 4> s;
    const RealType xi = constant.xi();
    const RealType c = constant.c();
    RealType *const u = s.data();
    RealType *const w = s.data() + n;
    RealType *const p = s.data() + n * 2;
    RealType *const t = s.data() + n * 3;
    u01_oo_distribution(rng, n, u);
    muladd(n, u, const_pi<RealType>(), -const_pi_by2<RealType>(), u);
    u01_oo_distribution(rng, n, w);
    log(n, w, w);
    mul(n, -const_one<RealType>(), w, w);
    muladd(n, u, alpha, alpha * xi, t);
    sin(n, t, r);
    cos(n, u, p);
    pow(n, p, 1 / alpha, p);
    div(n, r, p, r);
    sub(n, u, t, u);
    cos(n, u, u);
    div(n, u, w, p);
    pow(n, p, (1 - alpha) / alpha, p);
    mul(n, p, r, r);
    mul(n, c, r, r);
}

template <std::size_t K, typename RealType, typename RNGType>
inline void stable_distribution_impl(RNGType &rng, std::size_t n, RealType *r,
    RealType alpha, RealType beta, RealType a, RealType b)
{
    const StableDistributionConstant<RealType> constant(alpha, beta, a, b);
    switch (constant.algorithm()) {
        case StableDistributionAlgorithm1:
            stable_distribution_impl_1<K>(
                rng, n, r, alpha, beta, a, b, constant);
            break;
        case StableDistributionAlgorithmA:
            stable_distribution_impl_a<K>(
                rng, n, r, alpha, beta, a, b, constant);
            break;
    }
    muladd(n, r, b, a, r);
}

} // namespace mckl::internal

MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_4(Stable, stable, RealType, RealType,
    alpha, RealType, beta, RealType, a, RealType, b)

/// \brief Stable distribution
/// \ingroup Distribution
template <typename RealType>
class StableDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(Stable)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_4(Stable, stable, RealType, result_type,
        alpha, 1, result_type, beta, 0, result_type, a, 0, result_type, b, 1)

  public:
    result_type min() const
    {
        return std::numeric_limits<result_type>::lowest();
    }

    result_type max() const { return std::numeric_limits<result_type>::max(); }

    void reset()
    {
        constant_ = internal::StableDistributionConstant<RealType>(
            alpha(), beta(), a(), b());
    }

  private:
    internal::StableDistributionConstant<RealType> constant_;

    bool is_equal(const distribution_type &other) const
    {
        return constant_ == other.constant_;
    }

    template <typename CharT, typename Traits>
    void ostream(std::basic_ostream<CharT, Traits> &) const
    {
    }

    template <typename CharT, typename Traits>
    void istream(std::basic_istream<CharT, Traits> &)
    {
        reset();
    }

    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        if (param == param_) {
            return generate(rng, param_, constant_);
        }

        internal::StableDistributionConstant<RealType> constant(
            param.alpha(), param.beta(), param.a(), param.b());

        return generate(rng, param, constant);
    }

    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param,
        const internal::StableDistributionConstant<RealType> &constant)
    {
        result_type r = 0;
        switch (constant.algorithm()) {
            case internal::StableDistributionAlgorithm1:
                r = generate_1(rng, param, constant);
                break;
            case internal::StableDistributionAlgorithmA:
                r = generate_a(rng, param, constant);
                break;
        }

        return param.a() + param.b() * r;
    }

    template <typename RNGType>
    result_type generate_1(RNGType &rng, const param_type &param,
        const internal::StableDistributionConstant<RealType> &constant)
    {
        U01OODistribution<RealType> u01;
        result_type u =
            u01(rng) * const_pi<result_type>() - const_pi_by2<result_type>();
        result_type w = -std::log(u01(rng));
        result_type t = const_pi_by2<result_type>() + param.beta() * u;
        result_type r = t * std::tan(u);
        t = (const_pi_by2<result_type>() / t) * (w * std::cos(u));
        r -= param.beta() * t;

        return r / constant.xi() + constant.c();
    }

    template <typename RNGType>
    result_type generate_a(RNGType &rng, const param_type &param,
        const internal::StableDistributionConstant<RealType> &constant)
    {
        U01OODistribution<RealType> u01;
        result_type u =
            u01(rng) * const_pi<result_type>() - const_pi_by2<result_type>();
        result_type w = -std::log(u01(rng));
        result_type t = param.alpha() * (constant.xi() + u);
        result_type r = std::sin(t);
        result_type p = std::cos(u);
        r /= std::pow(p, 1 / param.alpha());
        p = std::cos(u - t) / w;
        r *= std::pow(p, (1 - param.alpha()) / param.alpha());

        return r * constant.c();
    }
}; // class StableDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(Stable, RealType)

} // namespace mckl

#endif // MCKL_RANDOM_STABLE_DISTRIBUTION_HPP
