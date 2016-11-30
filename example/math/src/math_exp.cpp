//============================================================================
// MCKL/example/math/src/math_exp.cpp
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

MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, exp, vd_exp)

int main(int argc, char **argv)
{
    mckl::Vector<std::pair<double, double>> bounds;
    bounds.push_back(std::make_pair(-707, 707));
    bounds.push_back(std::make_pair(-708.39, -707));
    bounds.push_back(std::make_pair(-707, -500));
    bounds.push_back(std::make_pair(-500, -1));
    bounds.push_back(std::make_pair(-1, -DBL_MIN));
    bounds.push_back(std::make_pair(-DBL_MIN, DBL_MIN));
    bounds.push_back(std::make_pair(DBL_MIN, 1));
    bounds.push_back(std::make_pair(1, 500));
    bounds.push_back(std::make_pair(500, 707));
    bounds.push_back(std::make_pair(707, 709.43));
    math_asm(argc, argv, math_asm_vd_exp, bounds);

    return 0;
}
