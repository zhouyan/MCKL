//============================================================================
// MCKL/include/mckl/internal/common.hpp
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

#ifndef MCKL_INTERNAL_COMMON_HPP
#define MCKL_INTERNAL_COMMON_HPP

#include <mckl/internal/config.h>

#include <mckl/internal/assert.hpp>
#include <mckl/internal/defines.hpp>
#include <mckl/internal/forward.hpp>
#include <mckl/internal/traits.hpp>

#include <mckl/internal/byte_order.hpp>
#include <mckl/internal/cblas.hpp>
#include <mckl/internal/const_math.hpp>
#include <mckl/internal/iostream.hpp>

#if MCKL_HAS_SSE2
#include <mckl/internal/sse2.hpp>
#endif

#if MCKL_HAS_AVX2
#include <mckl/internal/avx2.hpp>
#endif

#if MCKL_HAS_AESNI
#include <mckl/internal/aesni.hpp>
#endif

#if MCKL_HAS_FMA
#include <mckl/internal/fma.hpp>
#endif

#include <mckl/math.hpp>
#include <mckl/utility/aligned_memory.hpp>

// Utilities
#include <bitset>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

// Numeric limits
#include <cfloat>
#include <cinttypes>
#include <climits>
#include <cstdint>
#include <limits>

// String
#include <cctype>
#include <cstring>
#include <string>

// Containers
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Algorithms
#include <algorithm>

// Iterators
#include <iterator>

// Numerics
#include <cfenv>
#include <cmath>
#include <complex>
#include <numeric>
#include <random>
#include <ratio>

// Input/Output
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

// Atomic operation
#include <atomic>

// Thread support
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>

#endif // MCKL_INTERNAL_COMMON_HPP
