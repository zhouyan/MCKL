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
                e.second.reserve(n);
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
                e.second.clear();
    }

    std::map<std::string, Vector<typename estimator_type::value_type>>
        summary() const
    {
        using T = typename estimator_type::value_type;
        const std::size_t n = static_cast<const Derived *>(this)->num_iter();

        std::map<std::string, Vector<T>> df;
        Vector<T> data(n);

        for (const auto &est : estimator_) {
            for (const auto &e : est) {
                for (std::size_t i = 0; i != e.second.dim(); ++i) {
                    e.second.read_variable(i, data.begin());
                    df[e.first + "." + std::to_string(i)] = data;
                }
            }
        }

        return df;
    }

    template <typename CharT, typename Traits>
    std::basic_ostream<CharT, Traits> &print(
        std::basic_ostream<CharT, Traits> &os, char sepchar = ' ') const
    {
        const std::size_t n = static_cast<const Derived *>(this)->num_iter();

        if (!os || n == 0)
            return os;

        const auto df = summary();
        for (const auto &v : df)
            os << v.first << sepchar;
        os << '\n';
        for (std::size_t i = 0; i != n; ++i) {
            for (const auto &v : df)
                os << v.second[i] << sepchar;
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

    Vector<std::pair<std::string, estimator_type>> &estimator(std::size_t step)
    {
        return estimator_.at(step);
    }

    const Vector<std::pair<std::string, estimator_type>> &estimator(
        std::size_t step) const
    {
        return estimator_.at(step);
    }

    template <typename Eval>
    void eval(std::size_t step, Eval &&eval)
    {
        eval_.at(step).push_back(std::forward<Eval>(eval));
    }

    template <typename Estimator>
    std::string estimator(
        std::size_t step, Estimator &&estimator, const std::string &name)
    {
        auto find = [this, step](const std::string &vname) {
            return std::find_if(estimator_.at(step).begin(),
                estimator_.at(step).end(),
                [&vname](auto &p) { return p.first == vname; });
        };

        std::string vname(name);
        if (name.empty()) {
            const std::string v("V");
            int i = 0;
            while (find(v + std::to_string(i)) != estimator_.at(step).end())
                ++i;
            vname = "V" + std::to_string(i);
        } else {
            auto f = find(vname);
            if (f != estimator_.at(step).end())
                estimator_.at(step).erase(f);
        }
        estimator_.at(step).emplace_back(
            vname, std::forward<Estimator>(estimator));

        return vname;
    }

    const estimator_type &estimator(std::size_t step, const std::string &name)
    {
        auto exact = std::find_if(estimator_.at(step).begin(),
            estimator_.at(step).end(),
            [&name](auto &p) { return p.first == name; });
        if (exact != estimator_.at(step).end())
            return exact->second;

        auto partial = std::find_if(estimator_.at(step).begin(),
            estimator_.at(step).end(), [&name](auto &p) {
                return p.first.find(name) != std::string::npos;
            });
        if (partial != estimator_.at(step).end())
            return partial->second;

        runtime_assert(
            false, "**Sampler::estimator** not found with the given name");

        return estimator_.at(step).front().second;
    }

    private:
    std::size_t num_iter_;
    Vector<Vector<eval_type>> eval_;
    Vector<Vector<std::pair<std::string, estimator_type>>> estimator_;
}; // class Sampler

} // namespace mckl

#endif // MCKL_CORE_SAMPLER_HPP
