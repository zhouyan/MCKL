//============================================================================
// MCKL/include/mckl/core/particle.hpp
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

#ifndef MCKL_CORE_PARTICLE_HPP
#define MCKL_CORE_PARTICLE_HPP

#include <mckl/internal/common.hpp>
#include <mckl/core/is_equal.hpp>
#include <mckl/core/iterator.hpp>
#include <mckl/core/weight.hpp>
#include <mckl/random/rng_set.hpp>
#include <mckl/random/seed.hpp>

namespace mckl {

template <typename>
class Particle;

/// \brief A thin wrapper over a complete Particle
/// \ingroup Core
template <typename T>
class ParticleIndexBase
{
  public:
    ParticleIndexBase() : pptr_(nullptr), i_(0) {}

    ParticleIndexBase(typename Particle<T>::size_type i, Particle<T> *pptr)
        : pptr_(pptr), i_(i)
    {
    }

    Particle<T> &particle() const { return *pptr_; }

    Particle<T> *particle_ptr() const { return pptr_; }

    T &state() const { return pptr_->state(); }

    typename Particle<T>::size_type i() const { return i_; }

    typename Particle<T>::rng_type &rng() const { return pptr_->rng(i_); }

  private:
    Particle<T> *pptr_;
    typename Particle<T>::size_type i_;
}; // class ParticleIndexBase

/// \brief ParticleIndex base class trait
/// \ingroup Traits
MCKL_DEFINE_TYPE_TEMPLATE_DISPATCH_TRAIT(
    ParticleIndexBaseType, particle_index_type, ParticleIndexBase)

/// \brief A thin wrapper over a complete Particle
/// \ingroup Core
///
/// \details
/// This class also serves as an random access iterator
template <typename T>
class ParticleIndex final : public ParticleIndexBaseType<T>
{
  public:
    using difference_type =
        std::make_signed_t<typename Particle<T>::size_type>;
    using value_type = ParticleIndex;
    using pointer = const ParticleIndex *;
    using reference = const ParticleIndex &;
    using iterator_category = std::random_access_iterator_tag;

    ParticleIndex() = default;

    ParticleIndex(typename Particle<T>::size_type i, Particle<T> *pptr)
        : ParticleIndexBaseType<T>(i, pptr)
    {
    }

    /// \brief Dereference operator returns a reference to the index itself
    reference operator*() const noexcept { return *this; }

    /// \brief Member selection operator returns a pointer the index itself
    pointer operator->() const noexcept { return this; }

    /// \brief Subscript operator return a new index
    template <typename IntType>
    ParticleIndex operator[](IntType n)
    {
        return ParticleIndex(static_cast<typename Particle<T>::size_type>(
                                 static_cast<difference_type>(this->i()) +
                                 static_cast<difference_type>(n)),
            this->particle_ptr());
    }

    friend bool operator==(
        const ParticleIndex &idx1, const ParticleIndex &idx2)
    {
        runtime_assert(idx1.particle_ptr() == idx2.particle_ptr(),
            "Compare two ParticleIndex objects from two different particle "
            "systems");

        return idx1.i() == idx2.i();
    }

    friend bool operator!=(
        const ParticleIndex &idx1, const ParticleIndex &idx2)
    {
        runtime_assert(idx1.particle_ptr() == idx2.particle_ptr(),
            "Compare two ParticleIndex objects from two different particle "
            "systems");

        return idx1.i() != idx2.i();
    }

    friend bool operator<(const ParticleIndex &idx1, const ParticleIndex &idx2)
    {
        runtime_assert(idx1.particle_ptr() == idx2.particle_ptr(),
            "Compare two ParticleIndex objects from two different particle "
            "systems");

        return idx1.i() < idx2.i();
    }

    friend bool operator>(const ParticleIndex &idx1, const ParticleIndex &idx2)
    {
        runtime_assert(idx1.particle_ptr() == idx2.particle_ptr(),
            "Compare two ParticleIndex objects from two different particle "
            "systems");

        return idx1.i() > idx2.i();
    }

    friend bool operator<=(
        const ParticleIndex &idx1, const ParticleIndex &idx2)
    {
        runtime_assert(idx1.particle_ptr() == idx2.particle_ptr(),
            "Compare two ParticleIndex objects from two different particle "
            "systems");

        return idx1.i() <= idx2.i();
    }

    friend bool operator>=(
        const ParticleIndex &idx1, const ParticleIndex &idx2)
    {
        runtime_assert(idx1.particle_ptr() == idx2.particle_ptr(),
            "Compare two ParticleIndex objects from two different particle "
            "systems");

        return idx1.i() >= idx2.i();
    }

    friend ParticleIndex &operator++(ParticleIndex &idx)
    {
        return idx = ParticleIndex(idx.i() + 1, idx.particle_ptr());
    }

    friend ParticleIndex operator++(ParticleIndex &idx, int)
    {
        ParticleIndex ret(idx);
        ++idx;

        return ret;
    }

    friend ParticleIndex &operator--(ParticleIndex &idx)
    {
        return idx = ParticleIndex(idx.i() - 1, idx.particle_ptr());
    }

    friend ParticleIndex operator--(ParticleIndex &idx, int)
    {
        ParticleIndex ret(idx);
        --idx;

        return ret;
    }

    template <typename IntType>
    friend ParticleIndex operator+(const ParticleIndex &idx, IntType n)
    {
        return ParticleIndex(static_cast<typename Particle<T>::size_type>(
                                 static_cast<difference_type>(idx.i()) +
                                 static_cast<difference_type>(n)),
            idx.particle_ptr());
    }

    template <typename IntType>
    friend ParticleIndex operator+(IntType n, const ParticleIndex &idx)
    {
        return idx + n;
    }

    template <typename IntType>
    friend ParticleIndex operator-(const ParticleIndex &idx, IntType n)
    {
        return idx + (-static_cast<difference_type>(n));
    }

    template <typename IntType>
    friend ParticleIndex &operator+=(ParticleIndex &idx, IntType n)
    {
        return idx = idx + n;
    }

    template <typename IntType>
    friend ParticleIndex &operator-=(ParticleIndex &idx, IntType n)
    {
        return idx = idx - n;
    }

    friend difference_type operator-(
        const ParticleIndex &idx1, const ParticleIndex &idx2)
    {
        runtime_assert(idx1.particle_ptr() == idx2.particle_ptr(),
            "Substract two ParticleIndex objects from two different "
            "particle systems");

        return static_cast<difference_type>(idx1.i()) -
            static_cast<difference_type>(idx2.i());
    }
}; // class ParticleIndex

/// \brief Range of ParticleIndex
/// \ingroup Core
template <typename T>
class ParticleRange : public Range<ParticleIndex<T>>
{
  public:
    using Range<ParticleIndex<T>>::Range;

    Particle<T> &particle() const { return this->begin()->particle(); }

    Particle<T> *particle_ptr() const { return this->begin()->particle_ptr(); }

    typename Particle<T>::size_type ibegin() const
    {
        return this->begin()->i();
    }

    typename Particle<T>::size_type iend() const { return this->end()->i(); }
}; // class ParticleRange

/// \brief Particle system
/// \ingroup Core
///
/// \tparam T The sample collection type
///
/// \details
/// A collection of particles is formed by,
/// * The collection of samples
/// * The weights of samples
/// * A collection of RNG engines for parallel use
template <typename T>
class Particle
{
  public:
    using size_type = SizeType<T>;
    using state_type = T;
    using weight_type = WeightType<T>;
    using rng_set_type = RNGSetType<T>;
    using rng_type = typename rng_set_type::rng_type;

    /// \brief Default constructor
    Particle()
        : weight_(0), rng_set_(0), rng_(Seed<rng_type>::instance().get())
    {
    }

    /// \brief Construct a particle system
    ///
    /// \details
    /// All arguments are passed down to the constructor of `T`
    template <typename... Args>
    explicit Particle(size_type N, Args &&... args)
        : state_(N, std::forward<Args>(args)...)
        , weight_(static_cast<SizeType<weight_type>>(N))
        , rng_set_(static_cast<SizeType<rng_set_type>>(N))
        , rng_(Seed<rng_type>::instance().get())
    {
    }

    /// \brief Clone the Particle except the RNG engines
    Particle clone() const
    {
        Particle particle(*this);
        particle.rng_set_.reset();
        particle.rng_.seed(Seed<rng_type>::instance().get());

        return particle;
    }

    /// \brief The number of particles
    size_type size() const { return state_.size(); }

    /// \brief Resize by selecting according to user supplied index vector
    ///
    /// \param n The new sample size
    /// \param index N-vector of parent indices
    template <typename InputIter>
    void select(size_type n, InputIter index)
    {
        state_.select(n, index);
        weight_.resize(static_cast<SizeType<weight_type>>(n));
        weight_.set_equal();
        rng_set_.resize(static_cast<SizeType<rng_set_type>>(n));
    }

    /// \brief Read and write access to the state collection object
    state_type &state() { return state_; }

    /// \brief Read only access to the state collection object
    const state_type &state() const { return state_; }

    /// \brief Read and write access to the weight collection object
    weight_type &weight() { return weight_; }

    /// \brief Read only access to the weight collection object
    const weight_type &weight() const { return weight_; }

    /// \brief Read and write access to the RNG collection object
    rng_set_type &rng_set() { return rng_set_; }

    /// \brief Read only access to the RNG collection object
    const rng_set_type &rng_set() const { return rng_set_; }

    /// \brief Get an (parallel) RNG stream for a given particle
    rng_type &rng(size_type i)
    {
        return rng_set_[static_cast<std::size_t>(i)];
    }

    /// \brief Get an (parallel) RNG stream for a given particle
    const rng_type &rng(size_type i) const
    {
        return rng_set_[static_cast<std::size_t>(i)];
    }

    /// \brief Get the (sequential) RNG used stream for resampling
    rng_type &rng() { return rng_; }

    /// \brief Get the (sequential) RNG used stream for resampling
    const rng_type &rng() const { return rng_; }

    /// \brief Get a ParticleIndex<T> object for the i-th particle
    ParticleIndex<T> at(size_type i)
    {
        runtime_assert<std::out_of_range>(
            i <= size(), "**Particle::at** index out of range");

        return operator[](i);
    }

    /// \brief Get a ParticleIndex<T> object for the i-th particle
    ParticleIndex<T> operator[](size_type i)
    {
        return ParticleIndex<T>(i, this);
    }

    /// \brief Get a ParticleIndex<T> object for the first particle
    ParticleIndex<T> begin() { return operator[](0); }

    /// \brief Get a ParticleIndex<T> object for one pass the last particle
    ParticleIndex<T> end() { return operator[](size()); }

    /// \brief Get a Range<ParticleItrator<T>> object of all particles
    ParticleRange<T> range(size_type grainsize = 1)
    {
        return ParticleRange<T>(begin(), end(), grainsize);
    }

    /// \brief Get a ParticleRange<T> object given a range denoted by integral
    /// values
    ParticleRange<T> range(
        size_type ibegin, size_type iend, size_type grainsize = 1)
    {
        return ParticleRange<T>(operator[](ibegin), operator[](iend),
            grainsize);
    }

    friend bool operator==(const Particle &p1, const Particle &p2)
    {
        return p1.weight_ == p2.weight_ && p1.state_ == p2.state_;
    }

    friend bool operator!=(const Particle &p1, const Particle &p2)
    {
        return !(p1 == p2);
    }

  private:
    state_type state_;
    weight_type weight_;
    rng_set_type rng_set_;
    rng_type rng_;
}; // class Particle

template <typename T>
inline bool is_equal(const Particle<T> &p1, const Particle<T> &p2)
{
    return is_equal(p1.weight(), p2.weight()) &&
        is_equal(p1.state(), p2.state());
}

} // namespace mckl

#endif // MCKL_CORE_PARTICLE_HPP
