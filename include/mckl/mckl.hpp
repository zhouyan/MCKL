//============================================================================
// MCKL/include/mckl/mckl.hpp
//----------------------------------------------------------------------------
//                         MCKL: Monte Carlo Kernel Library
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

#ifndef MCKL_HPP
#define MCKL_HPP

/// \defgroup Config Configuration
/// \brief Configuration macros and default values if undefined

/// \defgroup Definitions Enumerators, placeholders and macros
/// \brief Enumerator, placeholder and macro definitions

/// \defgroup Traits Traits
/// \brief Trait classes

/// \defgroup Core Core
/// \brief Constructing samplers with operations on the whole particle set

/// \defgroup Algorithm Algorithm
/// \brief Monte Calro algorithms

/// \defgroup RandomWalk Random walk
/// \ingroup Algorithm
/// \brief Random walk Metropolis-Hastings algorithm

/// \defgroup Resample Resampling algorithms
/// \brief Resampling algorithm functor classes

/// \defgroup SMP Symmetric multiprocessing
/// \brief Parallel samplers using multi-threading on SMP architecture

/// \defgroup SEQ Sequential
/// \ingroup SMP
/// \brief Sequential samplers

/// \defgroup STD Standard library
/// \ingroup SMP
/// \brief Parallel samplers using the standard library

/// \defgroup OMP OpenMP
/// \ingroup SMP
/// \brief Parallel samplers using OpenMP

/// \defgroup TBB Intel Threading Building Blocks
/// \ingroup SMP
/// \brief Parallel samplers using Intel TBB

/// \defgroup Math Mathematics
/// \brief Mathematical utilities

/// \defgroup Constants Constants
/// \ingroup Math
/// \brief Mathematical constants

/// \defgroup vMath Vector math functions
/// \ingroup Math
/// \brief Math functions on vectors

/// \defgroup RNG Random number generating
/// \brief Random number generating engines and utilities

/// \defgroup AESNIRNG AES-NI
/// \ingroup RNG
/// \brief Random number generating using AES-NI

/// \defgroup Distribution Distribution
/// \ingroup RNG
/// \brief Distribution random varaites

/// \defgroup MKLRNG Intel Math Kernel Library
/// \ingroup RNG
/// \brief Random number generating using MKL

/// \defgroup Philox Philox
/// \ingroup RNG
/// \brief Random number generating using Random123 Philox RNG

/// \defgroup Threefry Threefry
/// \ingroup RNG
/// \brief Random number generating using Random123 Threefry RNG

/// \defgroup RDRAND Intel DRNG
/// \ingroup RNG
/// \brief Random number generating using Intel RDRAND instructions

/// \defgroup U01 U01
/// \ingroup RNG
/// \brief Converting random integers to uniform floating points

/// \defgroup U01Sequence U01 sequence
/// \ingroup RNG
/// \brief Generating ordered uniform random sequence

/// \defgroup RNGC Random number generating in C and OpenCL
/// \brief Random number generating in C and OpenCL

/// \defgroup PhiloxC Philox
/// \ingroup RNGC
/// \brief Random number generating using Random123 Philox RNG

/// \defgroup ThreefryC Threefry
/// \ingroup RNGC
/// \brief Random number generating using Random123 Threefry RNG

/// \defgroup U01C U01
/// \ingroup RNGC
/// \brief Converting random integers to uniform floating points

/// \defgroup Utility Utility
/// \brief Utilities independent of other part of the library

/// \defgroup AlignedMemory Aligned memory alignment
/// \ingroup Utility
/// \brief Memory allocation with alignment requirement

/// \defgroup Covariance Covariance
/// \ingroup Utility
/// \brief Covariance matrix estimation and manipulation

/// \defgroup HDF5 HDF5 objects I/O
/// \ingroup Utility
/// \brief Load and store objects in the HDF5 format

/// \defgroup StopWatch Stop watch
/// \ingroup Utility
/// \brief Time measurement

#include <mckl/internal/config.h>
#include <mckl/algorithm/algorithm.hpp>
#include <mckl/core/core.hpp>
#include <mckl/math/math.hpp>
#include <mckl/resample/resample.hpp>
#include <mckl/rng/rng.hpp>
#include <mckl/rngc/rngc.h>
#include <mckl/smp/smp.hpp>
#include <mckl/utility/utility.hpp>

#endif // MCKL_HPP
