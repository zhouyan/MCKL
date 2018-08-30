//============================================================================
// MCKL/include/mckl/internal/defines.hpp
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

#ifndef MCKL_INTERNAL_DEFINES_HPP
#define MCKL_INTERNAL_DEFINES_HPP

#include <mckl/internal/config.h>
#include <type_traits>

namespace mckl {

/// \brief A placeholder type
/// \ingroup Definitions
class Virtual;

/// \brief Matrix layout
/// \ingroup Definitions
enum class MatrixLayout { RowMajor = 101, ColMajor = 102 };
constexpr MatrixLayout RowMajor = MatrixLayout::RowMajor;
constexpr MatrixLayout ColMajor = MatrixLayout::ColMajor;

/// \brief Matrix Transpose
enum class MatrixTranspose { NoTrans = 111, Trans = 112, ConjTrans = 113 };

/// \brief Matrix Upper/lower triangular
enum class MatrixUpLo { Upper = 121, Lower = 122 };

/// \brief Matrix diagonal elements
enum class MatrixDiag { NonUnit = 131, Unit = 132 };

/// \brief Symmetric matrix operator side
enum class MatrixSide { Left = 141, Right = 142 };

template <typename T, std::size_t K = 1>
class BufferSize
    : public std::integral_constant<std::size_t,
          8192 / (sizeof(T) * K) == 0 ? 1 : 8192 / (sizeof(T) * K)>
{
}; // class BufferSize;

} // namespace mckl

#endif // MCKL_INTERNAL_DEFINES_HPP
