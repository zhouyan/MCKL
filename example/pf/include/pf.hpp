//============================================================================
// MCKL/example/pf/include/pf.hpp
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

#ifndef MCKL_EXAMPLE_PF_HPP
#define MCKL_EXAMPLE_PF_HPP

#include <mckl/mckl.hpp>

template <typename>
std::string pf_backend_name();

template <>
std::string pf_backend_name<mckl::BackendSEQ>()
{
    return "BackendSEQ";
}

template <>
std::string pf_backend_name<mckl::BackendSTD>()
{
    return "BackendSTD";
}

#if MCKL_HAS_OMP
template <>
std::string pf_backend_name<mckl::BackendOMP>()
{
    return "BackendOMP";
}
#endif

#if MCKL_HAS_TBB
template <>
std::string pf_backend_name<mckl::BackendTBB>()
{
    return "BackendTBB";
}
#endif

template <mckl::ResampleScheme>
std::string pf_scheme_name();

template <>
std::string pf_scheme_name<mckl::Multinomial>()
{
    return "Multinomial";
}

template <>
std::string pf_scheme_name<mckl::Residual>()
{
    return "Residual";
}

template <>
std::string pf_scheme_name<mckl::ResidualStratified>()
{
    return "ResidualStratified";
}

template <>
std::string pf_scheme_name<mckl::ResidualSystematic>()
{
    return "ResidualSystematic";
}

template <>
std::string pf_scheme_name<mckl::Stratified>()
{
    return "Stratified";
}

template <>
std::string pf_scheme_name<mckl::Systematic>()
{
    return "Systematic";
}

template <mckl::MatrixLayout>
std::string pf_layout_name();

template <>
std::string pf_layout_name<mckl::RowMajor>()
{
    return "RowMajor";
}

template <>
std::string pf_layout_name<mckl::ColMajor>()
{
    return "ColMajor";
}

template <typename>
std::string pf_rng_set_name();

template <>
std::string pf_rng_set_name<mckl::RNGSetVector<>>()
{
    return "RNGSetVector";
}

#if MCKL_HAS_TBB
template <>
std::string pf_rng_set_name<mckl::RNGSetTBB<>>()
{
    return "RNGSetTBB";
}

template <>
std::string pf_rng_set_name<mckl::RNGSetTBBKPI<>>()
{
    return "RNGSetTBBKPI";
}
#endif

#endif // MCKL_EXAMPLE_PF_HPP
