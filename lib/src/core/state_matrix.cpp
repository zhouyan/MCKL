//============================================================================
// MCKL/lib/src/core/state_matrix.cpp
//----------------------------------------------------------------------------
//                         MCKL: Monte Carlo Kernel Library
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
#include "libmckl.hpp"

extern "C" {

mckl_state_matrix mckl_state_matrix_new(size_t n, size_t dim)
{
    return {new ::mckl::StateMatrixC(n, dim)};
}

void mckl_state_matrix_delete(mckl_state_matrix *state_matrix_ptr)
{
    delete ::mckl::cast(state_matrix_ptr);
    state_matrix_ptr->ptr = nullptr;
}

void vsm_state_matrix_assign(
    mckl_state_matrix state_matrix, mckl_state_matrix other)
{
    ::mckl::cast(state_matrix) = ::mckl::cast(other);
}

size_t mckl_state_matrix_size(mckl_state_matrix state_matrix)
{
    return ::mckl::cast(state_matrix).size();
}

size_t mckl_state_matrix_dim(mckl_state_matrix state_matrix)
{
    return ::mckl::cast(state_matrix).dim();
}

void mckl_state_matrix_resize(
    mckl_state_matrix state_matrix, size_t n, size_t dim)
{
    ::mckl::cast(state_matrix).resize(n, dim);
}

void mckl_state_matrix_reserve(
    mckl_state_matrix state_matrix, size_t n, size_t dim)
{
    ::mckl::cast(state_matrix).reserve(n, dim);
}

void mckl_state_matrix_shrink_to_fit(mckl_state_matrix state_matrix)
{
    ::mckl::cast(state_matrix).shrink_to_fit();
}

double mckl_state_matrix_get(
    mckl_state_matrix state_matrix, size_t id, size_t pos)
{
    return ::mckl::cast(state_matrix)(id, pos);
}

void mckl_state_matrix_set(
    mckl_state_matrix state_matrix, size_t id, size_t pos, double state)
{
    ::mckl::cast(state_matrix)(id, pos) = state;
}

double *mckl_state_matrix_data(mckl_state_matrix state_matrix)
{
    return ::mckl::cast(state_matrix).data();
}

double *mckl_state_matrix_row_data(mckl_state_matrix state_matrix, size_t id)
{
    return ::mckl::cast(state_matrix).row_data(id);
}

void mckl_state_matrix_read_state(
    mckl_state_matrix state_matrix, size_t pos, double *first)
{
    ::mckl::cast(state_matrix).read_state(pos, first);
}

void mckl_state_matrix_read_state_matrix(
    mckl_state_matrix state_matrix, MCKLMatrixLayout layout, double *first)
{
    ::mckl::cast(state_matrix)
        .read_state_matrix(static_cast<::mckl::MatrixLayout>(layout), first);
}

void mckl_state_matrix_select(
    mckl_state_matrix state_matrix, size_t n, const size_t *index)
{
    ::mckl::cast(state_matrix).select(n, index);
}

void mckl_state_matrix_duplicate(
    mckl_state_matrix state_matrix, size_t src, size_t dst)
{
    ::mckl::cast(state_matrix).duplicate(src, dst);
}

} // extern "C"
