//============================================================================
// MCKL/example/math/src/math_logf.cpp
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

MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, float, log, vs_log)

int main(int argc, char **argv)
{
    math_asm_vs_log_check(0x00800000U, 0x7F7FFFFFU);

    const float sqrt2 = static_cast<float>(std::sqrt(2.0l));
    const float sqrt2by2 = static_cast<float>(std::sqrt(2.0l) / 2.0l);

    mckl::Vector<MathBound<float>> bounds;
    bounds.push_back(MathBound<float>(0, FLT_MIN));
    bounds.push_back(MathBound<float>(FLT_MIN, sqrt2by2, "", "sqrt(2) / 2"));
    bounds.push_back(
        MathBound<float>(sqrt2by2, sqrt2, "sqrt(2) / 2", "sqrt(2)"));
    bounds.push_back(MathBound<float>(sqrt2, 1e1, "sqrt(2)"));
    bounds.push_back(MathBound<float>(1e1, 1e2));
    bounds.push_back(MathBound<float>(1e2, 1e3));
    bounds.push_back(MathBound<float>(1e3, 1e4));
    math_asm(argc, argv, math_asm_vs_log, bounds);

    return 0;
}
