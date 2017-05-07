//============================================================================
// MCKL/include/mckl/internal/compiler.h
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

#ifndef MCKL_INTERNAL_COMPILER_H
#define MCKL_INTERNAL_COMPILER_H

#include <mckl/internal/compiler/platform.h>
#if defined(__OPENCL_VERSION__)
#define MCKL_OPENCL
#include <mckl/internal/compiler/opencl.h>
#elif defined(__INTEL_COMPILER)
#define MCKL_INTEL
#include <mckl/internal/compiler/intel.h>
#elif defined(__clang__)
#define MCKL_CLANG
#include <mckl/internal/compiler/clang.h>
#elif defined(__GNUC__)
#define MCKL_GCC
#include <mckl/internal/compiler/gcc.h>
#endif
#include <mckl/internal/compiler/byte_order.h>
#include <mckl/internal/compiler/intrin.h>

#ifndef MCKL_INLINE
#define MCKL_INLINE
#endif

#ifndef MCKL_INLINE_CALL
#define MCKL_INLINE_CALL
#endif

#define MCKL_PRAGMA(x) _Pragma(#x)

#ifdef MCKL_CLANG
#define MCKL_PUSH_CLANG_WARNING(warning)                                      \
    MCKL_PRAGMA(clang diagnostic push)                                        \
    MCKL_PRAGMA(clang diagnostic ignored warning)
#define MCKL_POP_CLANG_WARNING MCKL_PRAGMA(clang diagnostic pop)
#else
#define MCKL_PUSH_CLANG_WARNING(warning)
#define MCKL_POP_CLANG_WARNING
#endif

#ifdef MCKL_GCC
#define MCKL_PUSH_GCC_WARNING(warning)                                        \
    MCKL_PRAGMA(GCC diagnostic push)                                          \
    MCKL_PRAGMA(GCC diagnostic ignored warning)
#define MCKL_POP_GCC_WARNING MCKL_PRAGMA(GCC diagnostic pop)
#else
#define MCKL_PUSH_GCC_WARNING(warning)
#define MCKL_POP_GCC_WARNING
#endif

#ifdef MCKL_INTEL
#define MCKL_PUSH_INTEL_WARNING(wid)                                          \
    MCKL_PRAGMA(warning(push))                                                \
    MCKL_PRAGMA(warning(disable : wid))
#define MCKL_POP_INTEL_WARNING MCKL_PRAGMA(warning(pop))
#else
#define MCKL_PUSH_INTEL_WARNING(wid)
#define MCKL_POP_INTEL_WARNING
#endif

MCKL_PUSH_CLANG_WARNING("-Wc++98-compat")
MCKL_PUSH_CLANG_WARNING("-Wc++98-compat-pedantic")
MCKL_PUSH_CLANG_WARNING("-Wc++11-compat")
MCKL_PUSH_CLANG_WARNING("-Wc++11-compat-pedantic")

MCKL_PUSH_INTEL_WARNING(383) // reference to temporary used
MCKL_PUSH_INTEL_WARNING(444) // destructor for base class is not virtual
MCKL_PUSH_INTEL_WARNING(981) // operands are evaluated in unspecified order

#endif // MCKL_INTERNAL_COMPILER_HPP
