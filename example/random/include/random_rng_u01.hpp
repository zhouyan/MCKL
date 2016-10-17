//============================================================================
// MCKL/example/random/include/random_rng_u01.hpp
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

#ifndef MCKL_EXAMPLE_RANDOM_RNG_U01_HPP
#define MCKL_EXAMPLE_RANDOM_RNG_U01_HPP

extern void random_rng_load_seed(const std::string &);
extern void random_rng_store_seed(const std::string &);
extern double random_rng_std();
extern double random_rng_u01();
extern double random_rng_u01cc();
extern double random_rng_u01co();
extern double random_rng_u01oc();
extern double random_rng_u01oo();

class RandomRNG
{
    public:
    template <typename T>
    void seed(T)
    {
    }
};

class RandomSTD
{
    public:
    using result_type = double;

    template <typename T>
    double operator()(T)
    {
        return random_rng_std();
    }
};

class RandomU01
{
    public:
    using result_type = double;

    template <typename T>
    double operator()(T)
    {
        return random_rng_u01();
    }
};

class RandomU01CC
{
    public:
    using result_type = double;

    template <typename T>
    double operator()(T)
    {
        return random_rng_u01cc();
    }
};

class RandomU01CO
{
    public:
    using result_type = double;

    template <typename T>
    double operator()(T)
    {
        return random_rng_u01co();
    }
};

class RandomU01OC
{
    public:
    using result_type = double;

    template <typename T>
    double operator()(T)
    {
        return random_rng_u01oc();
    }
};

class RandomU01OO
{
    public:
    using result_type = double;

    template <typename T>
    double operator()(T)
    {
        return random_rng_u01oo();
    }
};

#endif // MCKL_EXAMPLE_RANDOM_RNG_U01_HPP
