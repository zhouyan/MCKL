//============================================================================
// MCKL/example/math/src/math_expm1f.cpp
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

MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, float, expm1, vs_expm1)

int main(int argc, char **argv)
{
    union {
        std::uint32_t u;
        float x;
    };
    u = 0xC2AEAC4FU;

    union {
        std::uint32_t v;
        float y;
    };
    v = 0x42B0C0A5U;

    const float ln2 = static_cast<float>(std::log(2.0l));
    const float ln2by2 = static_cast<float>(std::log(2.0l) / 2.0l);

    math_asm_vs_expm1_check(u, v);

    mckl::Vector<MathBound<float>> bounds;
    bounds.push_back(MathBound<float>(x, -87));
    bounds.push_back(MathBound<float>(-87, -70));
    bounds.push_back(MathBound<float>(-70, -1));
    bounds.push_back(MathBound<float>(-1, -ln2, "", "-ln(2)"));
    bounds.push_back(MathBound<float>(-ln2, -ln2by2, "-ln(2)", "-ln(2) / 2"));
    bounds.push_back(MathBound<float>(-ln2by2, -FLT_MIN, "-ln(2) / 2"));
    bounds.push_back(MathBound<float>(-FLT_MIN, FLT_MIN));
    bounds.push_back(MathBound<float>(FLT_MIN, ln2by2, "", "ln(2) / 2"));
    bounds.push_back(MathBound<float>(ln2by2, ln2, "ln(2) / 2", "ln(2)"));
    bounds.push_back(MathBound<float>(ln2, 1, "ln(2)"));
    bounds.push_back(MathBound<float>(1, 70));
    bounds.push_back(MathBound<float>(70, 87));
    bounds.push_back(MathBound<float>(87, y));
    math_asm(argc, argv, math_asm_vs_expm1, bounds);

    return 0;
}
