//============================================================================
// MCKL/example/math/src/math_log1p.cpp
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

#include "math_asm.hpp"

MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, log1p, vd_log1p)

int main(int argc, char **argv)
{
    math_asm_vd_log1p_check(0xBFEFFFFFFFFFFFFFULL, 0x7FEFFFFFFFFFFFFFULL);

    const double sqrt2m2 = static_cast<double>(std::sqrt(2.0l) / 2.0l - 1.0l);
    const double sqrt2m1 = static_cast<double>(std::sqrt(2.0l) - 1.0l);

    mckl::Vector<MathBound<double>> bounds;
    bounds.push_back(MathBound<double>(-1, sqrt2m2, "", "sqrt(2) / 2 - 1"));
    bounds.push_back(MathBound<double>(sqrt2m2, -DBL_MIN, "sqrt(2) / 2 - 1"));
    bounds.push_back(MathBound<double>(-DBL_MIN, DBL_MIN));
    bounds.push_back(MathBound<double>(DBL_MIN, sqrt2m1, "", "sqrt(2) - 1"));
    bounds.push_back(MathBound<double>(sqrt2m1, 1e0, "sqrt(2) - 1"));
    bounds.push_back(MathBound<double>(1e0, 1e1));
    bounds.push_back(MathBound<double>(1e1, 1e2));
    bounds.push_back(MathBound<double>(1e2, 1e3));
    bounds.push_back(MathBound<double>(1e3, 1e4));
    math_asm(argc, argv, math_asm_vd_log1p, bounds);

    return 0;
}
