//============================================================================
// MCKL/include/mckl/random/internal/philox_unroll.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_UNROLL_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_UNROLL_HPP

#define MCKL_RANDOM_INTERNAL_PHILOX_UNROLL(N, s, rk)                          \
    MCKL_INLINE_CALL rbox<N + 0x0>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0x1>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0x2>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0x3>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0x4>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0x5>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0x6>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0x7>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0x8>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0x9>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0xA>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0xB>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0xC>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0xD>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0xE>(s, rk);                                    \
    MCKL_INLINE_CALL rbox<N + 0xF>(s, rk);

#define MCKL_RANDOM_INTERNAL_PHILOX_UNROLL_ROUND(N, s, rk)                    \
    MCKL_RANDOM_INTERNAL_PHILOX_UNROLL(0x00, s, rk)                           \
    MCKL_NOINLINE_CALL round<N + 0x10>(                                       \
        s, rk, std::integral_constant<bool, N + 0x10 <= Rounds>());

#endif // MCKL_RANDOM_INTERNAL_PHILOX_UNROLL_HPP
