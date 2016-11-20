//============================================================================
// MCKL/include/mckl/random/internal/threefry_unroll.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_UNROLL_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_UNROLL_HPP

#define MCKL_RANDOM_INTERNAL_THREEFRY_UNROLL(N, s, par)                       \
    MCKL_INLINE_CALL rbox<N + 0x0>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0x0>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0x1>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0x1>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0x2>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0x2>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0x3>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0x3>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0x4>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0x4>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0x5>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0x5>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0x6>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0x6>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0x7>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0x7>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0x8>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0x8>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0x9>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0x9>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0xA>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0xA>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0xB>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0xB>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0xC>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0xC>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0xD>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0xD>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0xE>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0xE>(s, par);                                   \
    MCKL_INLINE_CALL rbox<N + 0xF>(s);                                        \
    MCKL_INLINE_CALL kbox<N + 0xF>(s, par);

#define MCKL_RANDOM_INTERNAL_THREEFRY_UNROLL_ROUND(N, s, par)                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_UNROLL(N + 0x00, s, par)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_UNROLL(N + 0x10, s, par)                    \
    round<N + 0x20>(                                                          \
        s, par, std::integral_constant<bool, N + 0x20 <= Rounds>());

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_UNROLL_HPP
