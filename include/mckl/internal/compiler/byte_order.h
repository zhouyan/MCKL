//============================================================================
// MCKL/include/mckl/internal/compiler/byte_order.h
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

#ifndef MCKL_INTERNAL_COMPILER_BYTE_ORDER_H
#define MCKL_INTERNAL_COMPILER_BYTE_ORDER_H

#ifndef MCKL_HAS_LITTLE_ENDIAN
#if MCKL_HAS_X86 || MCKL_HAS_X86_64
#define MCKL_HAS_LITTLE_ENDIAN 1
#else
#define MCKL_HAS_LITTLE_ENDIAN 0
#endif
#endif

#ifndef MCKL_HAS_BIG_ENDIAN
#define MCKL_HAS_BIG_ENDIAN 0
#endif

#if MCKL_HAS_LITTLE_ENDIAN && MCKL_HAS_BIG_ENDIAN
#error The platform cannot be both little and big endian
#endif

#endif // MCKL_INTERNAL_COMPILER_BYTE_ORDER_H
