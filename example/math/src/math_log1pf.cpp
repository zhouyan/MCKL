//============================================================================
// MCKL/example/math/src/math_log1pf.cpp
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

#include "math_asm.hpp"

MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, float, log1p, vs_log1p)

int main(int argc, char **argv)
{
    math_asm_vs_log1p_check(0xBF7FFFFFU, 0x7F7FFFFFU);

    const float sqrt2m2 = static_cast<float>(std::sqrt(2.0l) / 2.0l - 1.0l);
    const float sqrt2m1 = static_cast<float>(std::sqrt(2.0l) - 1.0l);

    mckl::Vector<MathBound<float>> bounds;
    bounds.push_back(MathBound<float>(-1, sqrt2m2, "", "sqrt(2) / 2 - 1"));
    bounds.push_back(MathBound<float>(sqrt2m2, -FLT_MIN, "sqrt(2) / 2 - 1"));
    bounds.push_back(MathBound<float>(-FLT_MIN, FLT_MIN));
    bounds.push_back(MathBound<float>(FLT_MIN, sqrt2m1, "", "sqrt(2) - 1"));
    bounds.push_back(MathBound<float>(sqrt2m1, 1e0, "sqrt(2) - 1"));
    bounds.push_back(MathBound<float>(1e0, 1e1));
    bounds.push_back(MathBound<float>(1e1, 1e2));
    bounds.push_back(MathBound<float>(1e2, 1e3));
    bounds.push_back(MathBound<float>(1e3, 1e4));
    math_asm(argc, argv, math_asm_vs_log1p, bounds);

    return 0;
}
