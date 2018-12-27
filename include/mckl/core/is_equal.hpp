//============================================================================
// MCKL/include/mckl/core/is_equal.hpp
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

#ifndef MCKL_CORE_IS_EQUAL_HPP
#define MCKL_CORE_IS_EQUAL_HPP

#include <cmath>
#include <type_traits>
#include <vector>

namespace mckl {

inline bool is_equal(const float &v1, const float &v2)
{
    return v1 == v2 || (std::isnan(v1) && std::isnan(v2));
}

inline bool is_equal(const double &v1, const double &v2)
{
    return v1 == v2 || (std::isnan(v1) && std::isnan(v2));
}

inline bool is_equal(const long double &v1, const long double &v2)
{
    return v1 == v2 || (std::isnan(v1) && std::isnan(v2));
}

template <typename T, typename Alloc>
inline bool is_equal(
    const std::vector<T, Alloc> &v1, const std::vector<T, Alloc> &v2)
{
    if (v1.size() != v2.size()) {
        return false;
    }

    for (std::size_t i = 0; i != v1.size(); ++i) {
        if (!is_equal(v1[i], v2[i])) {
            return false;
        }
    }

    return true;
}

} // namespace mckl

#endif // MCKL_CORE_IS_EQUAL_HPP
