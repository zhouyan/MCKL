//============================================================================
// MCKL/include/mckl/core/sampler.hpp
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

#ifndef MCKL_CORE_SAMPLER_HPP
#define MCKL_CORE_SAMPLER_HPP

#include <mckl/internal/common.hpp>
#include <mckl/core/matrix.hpp>

namespace mckl
{

template <typename>
class SamplerTrait;

/// \brief Sampler for iterative Monte Carlo algorithms
/// \ingroup Core
template <typename Derived>
class Sampler
{
    public:
    using eval_type = typename SamplerTrait<Derived>::eval_type;
    using estimator_type = typename SamplerTrait<Derived>::estimator_type;

    void reserve(std::size_t n)
    {
        for (auto &est : estimator_)
            for (auto &e : est)
                e.reserve(n);
    }

    void reset()
    {
        for (auto &eval : eval_)
            eval.clear();
        for (auto &est : estimator_)
            est.clear();
    }

    void clear()
    {
        for (auto &est : estimator_)
            for (auto &e : est)
                e.clear();
    }

    template <MatrixLayout Layout, typename T>
    Matrix<Layout, T> summary() const
    {
        const std::size_t nrow =
            static_cast<const Derived *>(this)->num_iter();

        std::size_t ncol = 0;
        for (auto &est : estimator_)
            for (auto &e : est)
                ncol += e.dim();

        Matrix<Layout, T> mat(nrow, ncol);
        if (nrow * ncol == 0)
            return mat;

        if (Layout == RowMajor) {
            for (std::size_t i = 0; i != nrow; ++i) {
                T *first = mat.row_data(i);
                for (auto &est : estimator_)
                    for (auto &e : est)
                        first = std::copy(e.row_begin(i), e.row_end(i), first);
            }
        } else {
            T *first = mat.col_data(0);
            for (auto &est : estimator_)
                for (auto &e : est)
                    for (std::size_t j = 0; j != e.dim(); ++j)
                        first = std::copy(e.col_begin(j), e.col_end(j), first);
        }

        return mat;
    }

    template <typename CharT, typename Traits>
    std::basic_ostream<CharT, Traits> &print(
        std::basic_ostream<CharT, Traits> &os, char sepchar = ' ') const
    {
        const auto mat = summary<RowMajor, double>();
        const std::size_t n = mat.nrow();
        const std::size_t m = mat.ncol();

        if (!os || n * m == 0)
            return os;

        auto v = mat.data();
        for (std::size_t i = 0; i != n; ++i) {
            for (std::size_t j = 0; j != m; ++j)
                os << *v++ << sepchar;
            os << '\n';
        }

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os, const Sampler<Derived> &sampler)
    {
        return sampler.print(os);
    }

    protected:
    Sampler(std::size_t steps) : num_iter_(0), eval_(steps), estimator_(steps)
    {
    }

    Vector<eval_type> &eval(std::size_t step) { return eval_.at(step); }

    const Vector<eval_type> &eval(std::size_t step) const
    {
        return eval_.at(step);
    }

    Vector<estimator_type> &estimator(std::size_t step)
    {
        return estimator_.at(step);
    }

    const Vector<estimator_type> &estimator(std::size_t step) const
    {
        return estimator_.at(step);
    }

    template <typename Eval>
    std::size_t eval(std::size_t step, Eval &&eval,
        std::enable_if_t<!std::is_integral<Eval>::value> * = nullptr)
    {
        eval_.at(step).push_back(std::forward<Eval>(eval));

        return eval_.at(step).size() - 1;
    }

    eval_type &eval(std::size_t step, std::size_t k)
    {
        return eval_.at(step).at(k);
    }

    const eval_type &eval(std::size_t step, std::size_t k) const
    {
        return eval_.at(step).at(k);
    }

    template <typename Estimator>
    std::size_t estimator(std::size_t step, Estimator &&estimator,
        std::enable_if_t<!std::is_integral<Estimator>::value> * = nullptr)
    {
        estimator_.at(step).push_back(std::forward<Estimator>(estimator));

        return estimator_.at(step).size() - 1;
    }

    estimator_type &estimator(std::size_t step, std::size_t k)
    {
        return estimator_.at(step).at(k);
    }

    const estimator_type &estimator(std::size_t step, std::size_t k) const
    {
        return estimator_.at(step).at(k);
    }

    private:
    std::size_t num_iter_;
    Vector<Vector<eval_type>> eval_;
    Vector<Vector<estimator_type>> estimator_;
}; // class Sampler

} // namespace mckl

#endif // MCKL_CORE_SAMPLER_HPP
