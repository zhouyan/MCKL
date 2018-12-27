//============================================================================
// MCKL/example/math/src/math_exp2.cpp
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

#include "math_asm.hpp"

MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, exp2, vd_exp2)

int main(int argc, char **argv)
{
    math_asm_vd_exp2_check(0xC08FF00000000000ULL, 0x408FF80000000000ULL);

    mckl::Vector<MathBound<double>> bounds;
    bounds.push_back(MathBound<double>(-1022, -1000));
    bounds.push_back(MathBound<double>(-1000, -500));
    bounds.push_back(MathBound<double>(-500, -1));
    bounds.push_back(MathBound<double>(-1, -DBL_MIN));
    bounds.push_back(MathBound<double>(-DBL_MIN, DBL_MIN));
    bounds.push_back(MathBound<double>(DBL_MIN, 1));
    bounds.push_back(MathBound<double>(1, 500));
    bounds.push_back(MathBound<double>(500, 1000));
    bounds.push_back(MathBound<double>(1000, 1023));
    math_asm(argc, argv, math_asm_vd_exp2, bounds);

    return 0;
}
