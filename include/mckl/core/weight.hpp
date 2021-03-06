//============================================================================
// MCKL/include/mckl/core/weight.hpp
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

#ifndef MCKL_CORE_WEIGHT_HPP
#define MCKL_CORE_WEIGHT_HPP

#include <mckl/internal/common.hpp>
#include <mckl/internal/cblas.hpp>
#include <mckl/core/is_equal.hpp>
#include <mckl/random/discrete_distribution.hpp>

namespace mckl {

/// \brief Weights of samples
/// \ingroup Core
class Weight
{
  public:
    using size_type = std::size_t;

    explicit Weight(size_type N = 0) : ess_(0), data_(N) { set_equal(); }

    /// \brief Size of this Weight object
    size_type size() const { return data_.size(); }

    /// \brief Resize the Weight object
    void resize(size_type N) { data_.resize(N); }

    /// \brief Reserve space
    void reserve(size_type N) { data_.reserve(N); }

    /// \brief Shrink to fit
    void shrink_to_fit() { data_.shrink_to_fit(); }

    /// \brief Return the ESS of the particle system
    double ess() const { return ess_; }

    /// \brief Pointer to data of the normalized weight
    const double *data() const { return data_.data(); }

    /// \brief Read all normalized weights to an output iterator
    template <typename OutputIter>
    OutputIter read(OutputIter first) const
    {
        return std::copy(data_.begin(), data_.end(), first);
    }

    /// \brief Set \f$W_i = 1/N\f$
    void set_equal()
    {
        std::fill(data_.begin(), data_.end(), 1.0 / size());
        ess_ = static_cast<double>(size());
    }

    /// \brief Set \f$W_i \propto w_i\f$
    template <typename InputIter>
    void set(InputIter first)
    {
        std::copy_n(first, size(), data_.begin());
        normalize(false);
    }

    /// \brief Set exact values of ESS and normalized weights
    ///
    /// \details
    /// This will set the internal state exactly to the input. No check of if
    /// the values are really normalized or ESS is correctly calculated
    template <typename InputIter>
    void set_exact(double ess, InputIter first)
    {
        ess_ = ess;
        std::copy_n(first, size(), data_.begin());
    }

    /// \brief Set \f$W_i \propto W_i w_i\f$
    template <typename InputIter>
    void mul(InputIter first)
    {
        for (size_type i = 0; i != size(); ++i, ++first) {
            data_[i] *= *first;
        }
        normalize(false);
    }

    /// \brief Set \f$W_i \propto W_i w_i\f$
    void mul(const double *first)
    {
        ::mckl::mul(size(), first, data_.data(), data_.data());
        normalize(false);
    }

    /// \brief Set \f$W_i \propto W_i w_i\f$
    void mul(double *first) { mul(const_cast<const double *>(first)); }

    /// \brief Set \f$\log W_i = v_i + \mathrm{const.}\f$
    template <typename InputIter>
    void set_log(InputIter first)
    {
        std::copy_n(first, size(), data_.begin());
        normalize(true);
    }

    /// \brief Set \f$\log W_i = \log W_i + v_i + \mathrm{const.}\f$
    template <typename InputIter>
    void add_log(InputIter first)
    {
        log(size(), data_.data(), data_.data());
        for (size_type i = 0; i != size(); ++i) {
            data_[i] += *first;
        }
        normalize(true);
    }

    /// \brief Set \f$\log W_i = \log W_i + v_i + \mathrm{const.}\f$
    void add_log(const double *first)
    {
        log(size(), data_.data(), data_.data());
        add(size(), first, data_.data(), data_.data());
        normalize(true);
    }

    /// \brief Set \f$\log W_i = \log W_i + v_i + \mathrm{const.}\f$
    void add_log(double *first) { add_log(const_cast<const double *>(first)); }

    /// \brief Draw integer index in the range \f$[0, N)\f$ according to the
    /// weights
    template <typename RNGType>
    size_type draw(RNGType &rng) const
    {
        DiscreteDistribution<size_type> dist;

        return dist(rng, data_.begin(), data_.end(), true);
    }

    friend bool operator==(const Weight &w1, const Weight &w2)
    {
        return w1.ess_ == w2.ess_ && w1.data_ == w2.data_;
    }

    friend bool operator!=(const Weight &w1, const Weight &w2)
    {
        return !(w1 == w2);
    }

    friend bool is_equal(const Weight &w1, const Weight &w2)
    {
        return is_equal(w1.ess_, w2.ess_) && is_equal(w1.data_, w2.data_);
    }

  private:
    double ess_;
    Vector<double> data_;

    template <typename InputIter>
    double max_element(size_type n, InputIter first)
    {
        using value_type =
            typename std::iterator_traits<InputIter>::value_type;

        value_type v = -const_inf<value_type>();
        for (size_type i = 0; i != n; ++i, ++first) {
            if (v < *first) {
                v = *first;
            }
        }

        return static_cast<double>(v);
    }

    void normalize(bool use_log)
    {
        double *w = data_.data();
        double accw = 0;
        double essw = 0;
        const double lmax = use_log ? max_element(size(), w) : 0;
        const size_type k = internal::BufferSize<double>::value;
        const size_type m = size() / k;
        const size_type l = size() % k;
        for (size_type i = 0; i != m; ++i, w += k) {
            normalize(k, w, accw, essw, use_log, lmax);
        }
        normalize(l, w, accw, essw, use_log, lmax);
        ::mckl::mul(size(), 1 / accw, data_.data(), data_.data());
        ess_ = accw * accw / essw;
    }

    void normalize(size_type n, double *w, double &accw, double &essw,
        bool use_log, double lmax)
    {
        internal::size_check<MCKL_BLAS_INT>(n, "Weight::normalize");

        if (use_log) {
            sub(n, w, lmax, w);
            exp(n, w, w);
        }
        accw = std::accumulate(w, w + n, accw);
        essw +=
            internal::cblas_ddot(static_cast<MCKL_BLAS_INT>(n), w, 1, w, 1);
    }
}; // class Weight

/// \brief Particle::weight_type trait
/// \ingroup Traits
MCKL_DEFINE_TYPE_DISPATCH_TRAIT(WeightType, weight_type, Weight)

} // namespace mckl

#endif // MCKL_CORE_WEIGHT_HPP
