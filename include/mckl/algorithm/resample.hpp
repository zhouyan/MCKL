//============================================================================
// MCKL/include/mckl/algorithm/resample.hpp
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

#ifndef MCKL_ALGORITHM_RESAMPLE_HPP
#define MCKL_ALGORITHM_RESAMPLE_HPP

#include <mckl/internal/common.hpp>
#include <mckl/core/particle.hpp>
#include <mckl/random/u01_distribution.hpp>

namespace mckl {

/// \brief Resampling schemes
/// \ingroup Resample
enum class ResampleScheme {
    Multinomial,        ///< Multinomial resampling
    Stratified,         ///< Stratified resampling
    Systematic,         ///< Systematic resampling
    Residual,           ///< Residual resampling
    ResidualStratified, ///< Stratified resampling on residuals
    ResidualSystematic  ///< Systematic resampling on residuals
};                      // enum ResampleScheme

constexpr ResampleScheme Multinomial = ResampleScheme::Multinomial;
constexpr ResampleScheme Stratified = ResampleScheme::Stratified;
constexpr ResampleScheme Systematic = ResampleScheme::Systematic;
constexpr ResampleScheme Residual = ResampleScheme::Residual;
constexpr ResampleScheme ResidualStratified =
    ResampleScheme::ResidualStratified;
constexpr ResampleScheme ResidualSystematic =
    ResampleScheme::ResidualSystematic;

namespace internal {

template <std::size_t K, typename RealType>
inline void u01_trans_sorted_impl(std::size_t n0, std::size_t n,
    const RealType *u01, RealType *r, std::size_t N, RealType &lmax)
{
    if (n0 == n)
        return;

    alignas(32) std::array<RealType, K> s;
    std::size_t j = 0;
    std::size_t m = N - n0;
    log(n - n0, u01, r);
    for (std::size_t i = n0; i != n; ++i, ++j, --m) {
        lmax += r[j] / m;
        s[j] = lmax;
    }
    exp(n - n0, s.data(), s.data());
    sub(n - n0, const_one<RealType>(), s.data(), r);
}

template <std::size_t K, typename RealType, typename RNGType>
inline void u01_rand_sorted_impl(RNGType &rng, std::size_t n0, std::size_t n,
    RealType *r, std::size_t N, RealType &lmax)
{
    if (n0 == n)
        return;

    u01_distribution(rng, n - n0, r);
    u01_trans_sorted_impl<K>(n0, n, r, r, N, lmax);
}

template <typename RealType>
inline void u01_trans_stratified_impl(std::size_t n0, std::size_t n,
    const RealType *u01, RealType *r, RealType delta)
{
    if (n0 == n)
        return;

    std::size_t j = 0;
    for (std::size_t i = n0; i != n; ++i, ++j)
        r[j] = u01[j] + static_cast<RealType>(i);
    mul(n - n0, delta, r, r);
}

template <typename RealType, typename RNGType>
inline void u01_rand_stratified_impl(
    RNGType &rng, std::size_t n0, std::size_t n, RealType *r, RealType delta)
{
    if (n0 == n)
        return;

    u01_distribution(rng, n - n0, r);
    u01_trans_stratified_impl(n0, n, r, r, delta);
}

template <typename RealType>
inline void u01_trans_systematic_impl(
    std::size_t n0, std::size_t n, RealType u, RealType *r, RealType delta)
{
    if (n0 == n)
        return;

    std::size_t j = 0;
    for (std::size_t i = n0; i != n; ++i, ++j)
        r[j] = static_cast<RealType>(i);
    muladd(n - n0, r, delta, u, r);
}

} // namespace mckl::internal

/// \brief Tranform a sequence of standard uniform random numbers to sorted
/// sequence
/// \ingroup Resample
template <typename RealType>
inline void u01_trans_sorted(std::size_t N, const RealType *u01, RealType *r)
{
    static_assert(std::is_floating_point<RealType>::value,
        "**u01_trans_sorted** used with RealType other than floating point "
        "types");

    if (N == 0)
        return;

    const std::size_t k = internal::BufferSize<RealType>::value;
    const std::size_t m = N / k;
    std::size_t n0 = 0;
    RealType lmax = 0;
    for (std::size_t i = 0; i != m; ++i, n0 += k, u01 += k, r += k)
        internal::u01_trans_sorted_impl<k>(n0, n0 + k, u01, r, N, lmax);
    internal::u01_trans_sorted_impl<k>(n0, N, u01, r, N, lmax);
}

/// \brief Transform a sequence of standard uniform random numbers to a
/// stratified sequence
/// \ingroup Resample
template <typename RealType>
inline void u01_trans_stratified(
    std::size_t N, const RealType *u01, RealType *r)
{
    static_assert(std::is_floating_point<RealType>::value,
        "**u01_trans_stratified** used with RealType other than floating "
        "point types");

    if (N == 0)
        return;

    const std::size_t k = internal::BufferSize<RealType>::value;
    const std::size_t m = N / k;
    std::size_t n0 = 0;
    const RealType delta = 1 / static_cast<RealType>(N);
    for (std::size_t i = 0; i != m; ++i, n0 += k, u01 += k, r += k)
        internal::u01_trans_stratified_impl(n0, n0 + k, u01, r, delta);
    internal::u01_trans_stratified_impl(n0, N, u01, r, delta);
}

/// \brief Transform a single standard uniform random number to a systematic
/// sequence
/// \ingroup Resample
template <typename RealType>
inline void u01_trans_systematic(
    std::size_t N, const RealType *u01, RealType *r)
{
    static_assert(std::is_floating_point<RealType>::value,
        "**u01_trans_systematic** used with RealType other than floating "
        "point types");

    if (N == 0)
        return;

    const std::size_t k = internal::BufferSize<RealType>::value;
    const std::size_t m = N / k;
    std::size_t n0 = 0;
    const RealType delta = 1 / static_cast<RealType>(N);
    const RealType u = u01[0] * delta;
    for (std::size_t i = 0; i != m; ++i, n0 += k, r += k)
        internal::u01_trans_systematic_impl(n0, n0 + k, u, r, delta);
    internal::u01_trans_systematic_impl(n0, N, u, r, delta);
}

/// \brief Generate sorted standard uniform numbers with \f$O(N)\f$ cost
/// \ingroup Resample
template <typename RealType, typename RNGType>
inline void u01_rand_sorted(RNGType &rng, std::size_t N, RealType *r)
{
    static_assert(std::is_floating_point<RealType>::value,
        "**u01_rand_sorted** used with RealType other than floating point "
        "types");

    if (N == 0)
        return;

    const std::size_t k = internal::BufferSize<RealType>::value;
    const std::size_t m = N / k;
    std::size_t n0 = 0;
    RealType lmax = 0;
    for (std::size_t i = 0; i != m; ++i, n0 += k, r += k)
        internal::u01_rand_sorted_impl<k>(rng, n0, n0 + k, r, N, lmax);
    internal::u01_rand_sorted_impl<k>(rng, n0, N, r, N, lmax);
}

/// \brief Generate stratified standard uniform numbers
/// \ingroup Resample
template <typename RealType, typename RNGType>
inline void u01_rand_stratified(RNGType &rng, std::size_t N, RealType *r)
{
    static_assert(std::is_floating_point<RealType>::value,
        "**u01_rand_stratified** used with RealType other than floating point "
        "types");

    const std::size_t k = internal::BufferSize<RealType>::value;
    const std::size_t m = N / k;
    std::size_t n0 = 0;
    const RealType delta = 1 / static_cast<RealType>(N);
    for (std::size_t i = 0; i != m; ++i, n0 += k, r += k)
        internal::u01_rand_stratified_impl(rng, n0, n0 + k, r, delta);
    internal::u01_rand_stratified_impl(rng, n0, N, r, delta);
}

/// \brief Generate systematic standard uniform numbers
/// \ingroup Resample
template <typename RealType, typename RNGType>
inline void u01_rand_systematic(RNGType &rng, std::size_t N, RealType *r)
{
    static_assert(std::is_floating_point<RealType>::value,
        "**u01_rand_systematic** used with RealType other than floating point "
        "types");

    U01Distribution<RealType> ru01;
    RealType u01 = ru01(rng);
    u01_trans_systematic(N, &u01, r);
}

/// \brief Sorted of standard uniform numbers
/// \ingroup Resample
class U01SequenceSorted
{
  public:
    template <typename RealType>
    void operator()(std::size_t N, const RealType *u01, RealType *r) const
    {
        u01_trans_sorted(N, u01, r);
    }

    template <typename RealType, typename RNG>
    void operator()(RNG &rng, std::size_t N, RealType *r) const
    {
        u01_rand_sorted(rng, N, r);
    }
}; // class U01SequenceSorted

/// \brief Stratified standard uniform numbers
/// \ingroup Resample
class U01SequenceStratified
{
  public:
    template <typename RealType>
    void operator()(std::size_t N, const RealType *u01, RealType *r) const
    {
        u01_trans_stratified(N, u01, r);
    }

    template <typename RealType, typename RNG>
    void operator()(RNG &rng, std::size_t N, RealType *r) const
    {
        u01_rand_stratified(rng, N, r);
    }
}; // class U01SequenceStratified

/// \brief Systematic standard uniform numbers
/// \ingroup Resample
class U01SequenceSystematic
{
  public:
    template <typename RealType>
    void operator()(std::size_t N, const RealType *u01, RealType *r) const
    {
        u01_trans_systematic(N, u01, r);
    }

    template <typename RealType, typename RNG>
    void operator()(RNG &rng, std::size_t N, RealType *r) const
    {
        u01_rand_systematic(rng, N, r);
    }
}; // class U01SequenceSystematic

/// \brief Transform normalized weights to normalized residual and integrals,
/// \ingroup Resample
///
/// \param N Sample size before resampling
/// \param M Sample size after resampling
/// \param weight N-vector of normalized weights
/// \param resid N-vector of normalized residuals
/// \param integ N-vector of integral parts
/// \return The number of remaining elements to be resampled
template <typename InputIter, typename OutputIterR, typename OutputIterI>
inline std::size_t resample_trans_residual(std::size_t N, std::size_t M,
    InputIter weight, OutputIterR resid, OutputIterI integ)
{
    using resid_type = typename std::iterator_traits<OutputIterR>::value_type;
    using integ_type = typename std::iterator_traits<OutputIterI>::value_type;

    static_assert(std::is_floating_point<resid_type>::value,
        "**resample_trans_residual** used resid other than floating point "
        "types");

    resid_type sum_resid = 0;
    integ_type sum_integ = 0;
    OutputIterR resid_i = resid;
    OutputIterI integ_i = integ;
    const resid_type coeff = static_cast<resid_type>(M);
    for (std::size_t i = 0; i != N; ++i, ++weight, ++resid_i, ++integ_i) {
        const resid_type w = coeff * static_cast<resid_type>(*weight);
        resid_type integral;
        *resid_i = std::modf(w, &integral);
        *integ_i = static_cast<integ_type>(integral);
        sum_resid += *resid_i;
        sum_integ += *integ_i;
    }

    const resid_type mul_resid = 1 / sum_resid;
    for (std::size_t i = 0; i != N; ++i, ++resid)
        *resid *= mul_resid;

    return M - static_cast<std::size_t>(sum_integ);
}

/// \brief Transform uniform [0, 1) sequence into replication numbers
/// \ingroup Resample
///
/// \param N Sample size before resampling
/// \param M Sample size after resampling
/// \param weight N-vector of normalized weights
/// \param u01seq M ordered uniform [0, 1) random numbers
/// \param replication N-vector of replication numbers
template <typename InputIter, typename OutputIter, typename U01SeqType>
inline OutputIter resample_trans_u01_rep(std::size_t N, std::size_t M,
    InputIter weight, U01SeqType &&u01seq, OutputIter replication)
{
    using real_type = typename std::iterator_traits<InputIter>::value_type;
    using rep_type = typename std::iterator_traits<OutputIter>::value_type;

    if (N == 0)
        return replication;

    if (N == 1) {
        *replication++ = static_cast<rep_type>(M);
        return replication;
    }

    OutputIter rep = std::fill_n(replication, N, const_zero<rep_type>());
    if (M == 0)
        return rep;

    real_type accw = 0;
    std::size_t j = 0;
    for (std::size_t i = 0; i != N - 1; ++i, ++weight, ++replication) {
        accw += *weight;
        while (j != M && static_cast<real_type>(u01seq[j]) < accw) {
            *replication += 1;
            ++j;
        }
    }
    *replication++ = static_cast<rep_type>(M - j);

    return replication;
}

/// \brief Transform replication numbers into parent indices
/// \ingroup Resample
///
/// \param N Sample size before resampling
/// \param M Sample size after resampling
/// \param replication N-vector of replication numbers
/// \param index M-vector of parent indices
template <typename InputIter, typename OutputIter>
inline OutputIter resample_trans_rep_index(
    std::size_t N, std::size_t M, InputIter replication, OutputIter index)
{
    using rep_type = typename std::iterator_traits<InputIter>::value_type;
    using idx_type = typename std::iterator_traits<OutputIter>::value_type;

    if (N == 0 || M == 0)
        return index;

    const std::size_t K = std::min(N, M);
    rep_type time = 0;
    std::size_t src = 0;
    InputIter rep = replication;

    auto seek = [N, K, &time, &src, &rep]() {
        if (src < K && *rep < time + 2) {
            time = 0;
            do {
                ++src;
                ++rep;
            } while (src < K && *rep < time + 2);
        }
        if (src >= K && *rep < time + 1) {
            time = 0;
            do {
                ++src;
                ++rep;
            } while (src < N && *rep < time + 1);
        }
    };

    for (std::size_t dst = 0; dst != K; ++dst, ++replication, ++index) {
        if (*replication > 0) {
            *index = static_cast<idx_type>(dst);
        } else {
            seek();
            *index = static_cast<idx_type>(src);
            ++time;
        }
    }

    for (std::size_t dst = K; dst < M; ++dst, ++index) {
        seek();
        *index = static_cast<idx_type>(src);
        ++time;
    }

    return index;
}

/// \brief SMCSampler<T>::eval_type subtype
/// \ingroup Resample
template <typename T>
class ResampleEval
{
  public:
    using eval_type = std::function<void(std::size_t, std::size_t,
        typename Particle<T>::rng_type &, const double *,
        typename Particle<T>::size_type *)>;

    ResampleEval() = default;

    /// \brief Construct a `Sampler::move_type` object
    ///
    /// \param eval A resampling algorithm evaluation object, see interface of
    /// ResampleAlgorithm
    explicit ResampleEval(const eval_type &eval) : eval_(eval) {}

    /// \brief Set a new evaluation object of type eval_type
    template <typename Eval>
    void selection(Eval &&eval)
    {
        eval_ = std::forward<Eval>(eval);
    }

    /// \brief Resample a particle collection
    void operator()(std::size_t, Particle<T> &particle) const
    {
        runtime_assert(static_cast<bool>(eval_),
            "**ResampleEval::operator()** invalid evaluation object");

        using size_type = typename Particle<T>::size_type;

        const std::size_t N = static_cast<std::size_t>(particle.size());
        Vector<size_type> rep(N);
        Vector<size_type> idx(N);
        eval_(N, N, particle.rng(), particle.weight().data(), rep.data());
        resample_trans_rep_index(N, N, rep.data(), idx.data());
        particle.select(particle.size(), idx.data());
    }

  private:
    eval_type eval_;
}; // class ResampleEval

/// \brief Resampling algorithm
/// \ingroup Resample
template <typename U01SeqType, bool Residual>
class ResampleAlgorithm
{
  public:
    /// \brief Generate replication numbers from normalized weights
    ///
    /// \param N Sample size before resampling
    /// \param M Sample size after resampling
    /// \param rng An RNG engine
    /// \param weight N-vector of normalized weights
    /// \param replication N-vector of replication numbers
    template <typename RNGType, typename InputIter, typename OutputIter>
    void operator()(std::size_t N, std::size_t M, RNGType &rng,
        InputIter weight, OutputIter replication) const
    {
        eval(N, M, rng, weight, replication,
            std::integral_constant<bool, Residual>());
    }

  private:
    U01SeqType u01seq_;

    template <typename RNGType, typename InputIter, typename OutputIter>
    void eval(std::size_t N, std::size_t M, RNGType &rng, InputIter weight,
        OutputIter replication, std::false_type) const
    {
        using real_type = typename std::iterator_traits<InputIter>::value_type;

        Vector<real_type> u01(M);
        u01seq_(rng, M, u01.data());
        resample_trans_u01_rep(N, M, weight, u01.data(), replication);
    }

    template <typename RNGType, typename InputIter, typename OutputIter>
    void eval(std::size_t N, std::size_t M, RNGType &rng, InputIter weight,
        OutputIter replication, std::true_type) const
    {
        using real_type = typename std::iterator_traits<InputIter>::value_type;
        using rep_type = typename std::iterator_traits<OutputIter>::value_type;

        Vector<real_type> resid(N);
        Vector<rep_type> integ(N);
        std::size_t R =
            resample_trans_residual(N, M, weight, resid.data(), integ.data());

        Vector<real_type> u01(R);
        u01seq_(rng, R, u01.data());
        resample_trans_u01_rep(N, R, resid.data(), u01.data(), replication);
        for (std::size_t i = 0; i != N; ++i, ++replication)
            *replication += integ[i];
    }
}; // class ResampleAlgorithm

/// \brief Multinomial resampling
/// \ingroup Resample
using ResampleMultinomial = ResampleAlgorithm<U01SequenceSorted, false>;

/// \brief Stratified resampling
/// \ingroup Resample
using ResampleStratified = ResampleAlgorithm<U01SequenceStratified, false>;

/// \brief Systematic resampling
/// \ingroup Resample
using ResampleSystematic = ResampleAlgorithm<U01SequenceSystematic, false>;

/// \brief Residual resampling
/// \ingroup Resample
using ResampleResidual = ResampleAlgorithm<U01SequenceSorted, true>;

/// \brief Residual stratified resampling
/// \ingroup Resample
using ResampleResidualStratified =
    ResampleAlgorithm<U01SequenceStratified, true>;

/// \brief Residual systematic resampling
/// \ingroup Resample
using ResampleResidualSystematic =
    ResampleAlgorithm<U01SequenceSystematic, true>;

/// \brief Record and trace resampling index
/// \ingroup Resample
template <typename IntType = std::size_t>
class ResampleIndex
{
  public:
    using index_type = IntType;

    ResampleIndex() : num_iter_(0) {}

    /// \brief Number of iterations recorded
    std::size_t num_iter() const { return num_iter_; }

    /// \brief The sample size of the last iteration
    std::size_t size() const
    {
        if (index_.size() == 0)
            return 0;
        return index_.back().size();
    }

    /// \brief The sample size of a given iteration
    std::size_t size(std::size_t iter) const
    {
        if (iter >= index_.size())
            return 0;
        return index_[iter].size();
    }

    /// \brief Reset history
    void reset() { num_iter_ = 0; }

    /// \brief Release memory
    void clear() { index_.clear(); }

    /// \brief Append an identity resampling index
    void push_back(std::size_t N)
    {
        ++num_iter_;
        resize_identity(N);
        if (index_.size() < num_iter_)
            index_.push_back(identity_);
        else
            index_[num_iter_ - 1] = identity_;
    }

    /// \brief Append a resampling index
    template <typename InputIter>
    void push_back(std::size_t N, InputIter first)
    {
        ++num_iter_;
        if (index_.size() < num_iter_)
            index_.emplace_back(N);
        else
            index_[num_iter_ - 1].resize(N);
        std::copy_n(first, N, index_[num_iter_ - 1].begin());
    }

    index_type index(std::size_t id) const
    {
        return index(id, num_iter_ - 1, 0);
    }

    index_type index(std::size_t id, std::size_t iter_back) const
    {
        return index(id, iter_back, 0);
    }

    /// \brief Get the index given the particle ID and iteration number
    index_type index(
        std::size_t id, std::size_t iter_back, std::size_t iter) const
    {
        runtime_assert(iter <= iter_back && iter_back < num_iter(),
            "**ResampleIndex::index** iteration numbers out of range");

        index_type idx = index_.back()[id];
        while (iter_back > iter) {
            --iter_back;
            idx = index_[iter_back][idx];
        }

        return idx;
    }

    std::size_t index_matrix_nrow() const
    {
        return index_matrix_nrow(num_iter_ - 1);
    }

    std::size_t index_matrix_nrow(std::size_t iter_back) const
    {
        runtime_assert(iter_back < num_iter(),
            "**ResampleIndex::index_matrix_nrow** iteration numbers out of "
            "range");

        return index_[iter_back].size();
    }

    std::size_t index_matrix_ncol() const
    {
        return index_matrix_ncol(num_iter_ - 1, 0);
    }

    std::size_t index_matrix_ncol(std::size_t iter_back) const
    {
        return index_matrix_ncol(iter_back, 0);
    }

    std::size_t index_matrix_ncol(
        std::size_t iter_back, std::size_t iter) const
    {
        runtime_assert(iter <= iter_back && iter_back < num_iter(),
            "**ResampleIndex::index_matrix_ncol** iteration numbers out of "
            "range");

        return iter_back - iter + 1;
    }

    Vector<index_type> index_matrix(MatrixLayout layout) const
    {
        return index_matrix(layout, num_iter_ - 1, 0);
    }

    Vector<index_type> index_matrix(
        MatrixLayout layout, std::size_t iter_back) const
    {
        return index_matrix(layout, iter_back, 0);
    }

    /// \brief Get the resampling index matrix.
    Vector<index_type> index_matrix(
        MatrixLayout layout, std::size_t iter_back, std::size_t iter) const
    {
        runtime_assert(iter <= iter_back && iter_back < num_iter(),
            "**ResampleIndex::index_matrix** iteration numbers out of range");

        Vector<index_type> idxmat(
            index_matrix_nrow(iter_back) * index_matrix_ncol(iter_back, iter));
        read_index_matrix(layout, idxmat.begin(), iter_back, iter);

        return idxmat;
    }

    template <typename RandomIter>
    RandomIter read_index_matrix(MatrixLayout layout, RandomIter first) const
    {
        return read_index_matrix(layout, first, num_iter_ - 1, 0);
    }

    template <typename RandomIter>
    RandomIter read_index_matrix(
        MatrixLayout layout, RandomIter first, std::size_t iter_back) const
    {
        return read_index_matrix(layout, first, iter_back, 0);
    }

    /// \brief Read the resampling index matrix into an random access iterator
    ///
    /// \details
    /// The matrix, say \f$M\f$ is assume to be \f$N\f$ by \f$R\f$, where
    /// \f$R\f$ is the number of iterations between `iter` and `iter_back`,
    /// inclusive; and \f$N\f$ is the sample size at iteration `iter_back`. The
    /// output is equivalent to set the \f$M_{i,j}\f$ to
    /// `index(i, j, iter_back)`. Note that, column major will be more
    /// efficient.
    template <typename RandomIter>
    RandomIter read_index_matrix(MatrixLayout layout, RandomIter first,
        std::size_t iter_back, std::size_t iter) const
    {
        runtime_assert(iter <= iter_back && iter_back < num_iter(),
            "**ResampleIndex::read_index_matrix** iteration numbers out of "
            "range");

        using difference_type =
            typename std::iterator_traits<RandomIter>::difference_type;
        const std::size_t N = index_matrix_nrow(iter_back);
        const std::size_t R = index_matrix_ncol(iter_back, iter);

        if (layout == RowMajor) {
            RandomIter back = first + static_cast<difference_type>(R - 1);
            const index_type *idx = index_[iter_back].data();
            for (std::size_t i = 0; i != N; ++i)
                back[static_cast<difference_type>(i * R)] = idx[i];
            for (std::size_t r = 1; r != R; ++r) {
                const std::size_t j = iter_back - r;
                RandomIter last = back;
                --back;
                idx = index_[j].data();
                for (std::size_t i = 0; i != N; ++i) {
                    back[static_cast<difference_type>(i * R)] =
                        idx[static_cast<std::size_t>(
                            last[static_cast<difference_type>(i * R)])];
                }
            }
        }

        if (layout == ColMajor) {
            RandomIter back =
                first + static_cast<difference_type>(N * (R - 1));
            const index_type *idx = index_[iter_back].data();
            std::copy_n(idx, N, back);
            for (std::size_t r = 1; r != R; ++r) {
                const std::size_t j = iter_back - r;
                RandomIter last = back;
                back -= static_cast<difference_type>(N);
                idx = index_[j].data();
                for (std::size_t i = 0; i != N; ++i) {
                    back[static_cast<difference_type>(i)] =
                        idx[static_cast<std::size_t>(
                            last[static_cast<difference_type>(i)])];
                }
            }
        }

        return first + static_cast<difference_type>(N * R);
    }

  private:
    std::size_t num_iter_;
    Vector<index_type> identity_;
    Vector<Vector<index_type>> index_;

    void resize_identity(std::size_t N)
    {
        std::size_t n = identity_.size();
        identity_.resize(N);
        if (n < N)
            for (std::size_t i = n; i != N; ++i)
                identity_[i] = static_cast<index_type>(i);
    }
}; // class ResampleIndex

} // namespace mckl

#endif // MCKL_ALGORITHM_RESAMPLE_HPP
