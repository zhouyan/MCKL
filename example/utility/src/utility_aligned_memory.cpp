//============================================================================
// MCKL/example/utility/src/utility_aligned_memory.cpp
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

#include "utility_aligned_memory.hpp"

int main(int argc, char **argv)
{
    std::size_t n = 1000;
    if (argc > 1)
        n = static_cast<std::size_t>(std::atoi(argv[1]));

    std::size_t m = 1000;
    if (argc > 2)
        m = static_cast<std::size_t>(std::atoi(argv[2]));

    utility_aligned_memory<char>(n, m, "char");
    utility_aligned_memory<short>(n, m, "short");
    utility_aligned_memory<int>(n, m, "int");
    utility_aligned_memory<long>(n, m, "long");
    utility_aligned_memory<long long>(n, m, "long long");
    utility_aligned_memory<float>(n, m, "float");
    utility_aligned_memory<double>(n, m, "double");
    utility_aligned_memory<long double>(n, m, "long double");

    return 0;
}
