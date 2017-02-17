//============================================================================
// MCKL/example/math/src/math_exp2f.cpp
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

MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, float, exp2, vs_exp2)

int main(int argc, char **argv)
{
    math_asm_vs_exp2_check(0xC2FC0000U, 0x42FE0000U);

    mckl::Vector<MathBound<float>> bounds;
    bounds.push_back(MathBound<float>(-126, -100));
    bounds.push_back(MathBound<float>(-100, -50));
    bounds.push_back(MathBound<float>(-50, -1));
    bounds.push_back(MathBound<float>(-1, -FLT_MIN));
    bounds.push_back(MathBound<float>(-FLT_MIN, FLT_MIN));
    bounds.push_back(MathBound<float>(FLT_MIN, 1));
    bounds.push_back(MathBound<float>(1, 50));
    bounds.push_back(MathBound<float>(50, 100));
    bounds.push_back(MathBound<float>(100, 127));
    math_asm(argc, argv, math_asm_vs_exp2, bounds);

    return 0;
}
