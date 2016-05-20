//============================================================================
// MCKL/lib/src/utility/covariance.cpp
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

#include <mckl/mckl.h>
#include <mckl/utility/covariance.hpp>

extern "C" {

mckl_covariance mckl_covariance_new(void)
{
    return {new ::mckl::Covariance<double>()};
}

void mckl_covariance_delete(mckl_covariance *covariance_ptr)
{
    delete reinterpret_cast<::mckl::Covariance<double> *>(covariance_ptr->ptr);
    covariance_ptr->ptr = nullptr;
}

void mckl_covariance_assign(mckl_covariance covariance, mckl_covariance other)
{
    *reinterpret_cast<::mckl::Covariance<double> *>(covariance.ptr) =
        *reinterpret_cast<::mckl::Covariance<double> *>(other.ptr);
}

void mckl_covariance_compute(mckl_covariance covariance,
    MCKLMatrixLayout layout, size_t n, size_t p, const double *x,
    const double *w, double *mean, double *cov, MCKLMatrixLayout cov_layout,
    int cov_upper, int cov_packed)
{
    (*reinterpret_cast<::mckl::Covariance<double> *>(covariance.ptr))(
        static_cast<::mckl::MatrixLayout>(layout), n, p, x, w, mean, cov,
        static_cast<::mckl::MatrixLayout>(cov_layout), cov_upper != 0,
        cov_packed != 0);
}

} // extern "C"
