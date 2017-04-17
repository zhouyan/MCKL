.. ============================================================================
..  MCKL/docs/random_rng.rst
.. ----------------------------------------------------------------------------
..  MCKL: Monte Carlo Kernel Library
.. ----------------------------------------------------------------------------
..  Copyright (c) 2013-2017, Yan Zhou
..  All rights reserved.

..  Redistribution and use in source and binary forms, with or without
..  modification, are permitted provided that the following conditions are met:

..    Redistributions of source code must retain the above copyright notice,
..    this list of conditions and the following disclaimer.

..    Redistributions in binary form must reproduce the above copyright notice,
..    this list of conditions and the following disclaimer in the documentation
..    and/or other materials provided with the distribution.

..  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
..  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
..  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
..  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
..  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
..  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
..  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
..  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
..  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
..  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
..  POSSIBILITY OF SUCH DAMAGE.
.. ============================================================================

.. _chap-Performance of Random Number Generators:

***************************************
Performance of Random Number Generators
***************************************

The performance is measured on an MacBook Pro with an Intel Core i7-4960HQ CPU
running macOS Sierra (version 10.12.1).

Three compilers are tested, LLVM clang (version Apple 8.0.0), GNU GCC (version
6.3.0), and Intel C++ compiler (version 17.0.1). They are labeled as “LLVM”,
“GNU” and “Intel”, respectively.

Two usage cases of RNGs are considered. The first is generating random integers
within a loop, each iteration generate a single random integer,

.. code-block:: cpp

    // RNGType is the type of RNG tested
    RNGType rng;
    ::mckl::UniformBitsDistribution<RNGType::result_type> ubits;
    for (std:size_t i = 0; i != n; ++i)
        r[i] = ::mckl::rand(rng, ubits);

For RNGs that output random integers uniform on the set :math:`\{0,\dots,2^W -
1\}`, where :math:`W` is the number of bits in its output integer type, this is
equivalent to

.. code-block:: cpp

    for (std::size_t i = 0; i != n; ++i)
        r[i] = rng();

The second is the vectorized performance,

.. code-block:: cpp

    ::mckl::rand(rng, ubits, n, r.data());

For RNGs that output random integers uniform on the set :math:`\{0,\dots,2^W -
1\}`, where :math:`W` is the number of bits in its output integer type, this is
equivalent to

.. code-block:: cpp

    ::mckl::rand(rng, n, r.data());

See :ref:`sec-Vectorized Random Number Generating`. In both cases, we repeat
the simulations 100 times, each time with :math:`n` chosen randomly between
5,000 and 10,000. The total number of cycles of the 100 simulations are
recorded, and then divided by the total number of bytes generated. It gives the
performance measurement in cpB. The two cases are labeled “Single” and “Batch”,
respectively, in the tables below.

.. _tab-Performance of RNGs in the Standard Library:

.. csv-table:: Performance of RNGs in the Standard Library
    :delim: &
    :header-rows: 1
    :header: ,,Single,,,Batch,

    RNG                       & LLVM   & GNU    & Intel  & LLVM   & GNU    & Intel
    ``mt19937``               & 4.56   & 4.21   & 5.04   & 4.56   & 3.99   & 5.03
    ``mt19937_64``            & 2.35   & 2.21   & 2.46   & 2.35   & 2.18   & 2.46
    ``minstd_rand0``          & 9.68   & 10.2   & 10.9   & 9.69   & 10.1   & 10.9
    ``minstd_rand``           & 7.61   & 8.55   & 9.59   & 7.56   & 8.56   & 9.59
    ``ranlux24_base``         & 12.5   & 5.99   & 13.7   & 12.6   & 6.15   & 12.1
    ``ranlux48_base``         & 5.85   & 3.11   & 6.64   & 5.93   & 3.07   & 6.77
    ``ranlux24``              & 130    & 94.3   & 131    & 130    & 93.8   & 113
    ``ranlux48``              & 223    & 159    & 243    & 224    & 159    & 219
    ``knuth_b``               & 43.8   & 49.6   & 30.8   & 37.9   & 48.8   & 30.3

.. _tab-Performance of RNGs in the Random123 Library:

.. csv-table:: Performance of RNGs in the Random123 Library
    :delim: &
    :header-rows: 1
    :header: ,,Single,,,Batch,

    RNG                       & LLVM   & GNU    & Intel  & LLVM   & GNU    & Intel
    ``r123::AESNI4x32``       & 6.59   & 1.56   & 8.93   & 6.61   & 1.52   & 8.83
    ``r123::ARS4x32``         & 5.14   & 1.18   & 6.40   & 5.19   & 1.18   & 6.30
    ``r123::Philox2x32``      & 10.1   & 3.04   & 12.6   & 10.1   & 3.05   & 12.6
    ``r123::Philox4x32``      & 6.15   & 2.81   & 13.1   & 6.17   & 2.85   & 13.1
    ``r123::Philox2x64``      & 2.19   & 1.77   & 10.0   & 2.15   & 1.56   & 10.4
    ``r123::Philox4x64``      & 2.25   & 1.65   & 12.0   & 2.25   & 1.64   & 12.0
    ``r123::Threefry2x32``    & 11.7   & 4.59   & 10.9   & 11.7   & 4.59   & 10.9
    ``r123::Threefry4x32``    & 6.96   & 3.86   & 8.08   & 6.84   & 3.87   & 8.01
    ``r123::Threefry2x64``    & 3.38   & 2.25   & 3.17   & 3.30   & 2.32   & 3.17
    ``r123::Threefry4x64``    & 2.63   & 1.97   & 6.75   & 2.53   & 1.97   & 6.72

.. _tab-Performance of AESEngine:

.. csv-table:: Performance of ``AESEngine``
    :delim: &
    :header-rows: 1
    :header: ,,Single,,,Batch,

    RNG                       & LLVM   & GNU    & Intel  & LLVM   & GNU    & Intel
    ``AES128``                & 1.53   & 1.90   & 1.87   & 0.63   & 0.63   & 0.64
    ``AES192``                & 2.18   & 2.28   & 2.25   & 0.76   & 0.76   & 0.76
    ``AES256``                & 2.57   & 2.63   & 2.34   & 0.89   & 0.89   & 0.89
    ``ARS``                   & 1.04   & 1.32   & 1.01   & 0.32   & 0.32   & 0.32
    ``AES128_64``             & 1.14   & 1.36   & 1.48   & 0.63   & 0.63   & 0.63
    ``AES192_64``             & 1.62   & 1.62   & 1.78   & 0.76   & 0.76   & 0.76
    ``AES256_64``             & 1.94   & 1.96   & 2.06   & 0.88   & 0.88   & 0.88
    ``ARS_64``                & 0.73   & 0.86   & 0.67   & 0.32   & 0.32   & 0.32

.. _tab-Performance of PhiloxEngine:

.. csv-table:: Performance of ``PhiloxEngine``
    :delim: &
    :header-rows: 1
    :header: ,,Single,,,Batch,

    RNG                       & LLVM   & GNU    & Intel  & LLVM   & GNU    & Intel
    ``Philox2x32``            & 4.71   & 4.53   & 5.50   & 0.61   & 0.61   & 0.61
    ``Philox4x32``            & 3.79   & 6.26   & 4.17   & 0.63   & 0.63   & 0.63
    ``Philox2x64``            & 2.70   & 2.37   & 3.00   & 1.43   & 1.42   & 1.43
    ``Philox4x64``            & 2.62   & 2.67   & 2.22   & 1.45   & 1.45   & 1.45
    ``Philox2x32_64``         & 4.08   & 4.08   & 5.32   & 0.61   & 0.60   & 0.61
    ``Philox4x32_64``         & 3.46   & 6.07   & 4.06   & 0.63   & 0.63   & 0.63
    ``Philox2x64_64``         & 2.09   & 2.08   & 2.82   & 1.42   & 1.42   & 1.42
    ``Philox4x64_64``         & 2.19   & 2.16   & 2.08   & 1.45   & 1.45   & 1.45

.. _tab-Performance of ThreefryEngine:

.. csv-table:: Performance of ``ThreefryEngine``
    :delim: &
    :header-rows: 1
    :header: ,,Single,,,Batch,

    RNG                       & LLVM   & GNU    & Intel  & LLVM   & GNU    & Intel
    ``Threefry2x32``          & 7.37   & 6.64   & 7.19   & 0.99   & 0.99   & 0.94
    ``Threefry4x32``          & 5.33   & 6.80   & 5.33   & 1.01   & 0.98   & 0.98
    ``Threefry2x64``          & 3.73   & 3.35   & 4.30   & 0.94   & 0.92   & 0.88
    ``Threefry4x64``          & 3.26   & 2.89   & 3.52   & 0.95   & 0.91   & 0.86
    ``Threefry8x64``          & 3.05   & 2.21   & 3.13   & 0.89   & 0.86   & 0.86
    ``Threefry16x64``         & 4.00   & 3.38   & 3.45   & 0.92   & 1.02   & 0.86
    ``Threefish256``          & 8.80   & 8.96   & 9.16   & 2.98   & 2.78   & 2.81
    ``Threefish512``          & 6.75   & 6.53   & 6.81   & 2.92   & 2.78   & 2.79
    ``Threefish1024``         & 10.6   & 9.28   & 9.37   & 3.41   & 3.84   & 3.26
    ``Threefry2x32_64``       & 6.53   & 5.50   & 6.61   & 0.98   & 0.98   & 0.94
    ``Threefry4x32_64``       & 4.78   & 6.75   & 4.85   & 1.01   & 0.99   & 0.96
    ``Threefry2x64_64``       & 3.29   & 3.13   & 4.03   & 0.92   & 0.91   & 0.88
    ``Threefry4x64_64``       & 2.57   & 2.62   & 2.95   & 0.94   & 0.89   & 0.85
    ``Threefry8x64_64``       & 1.97   & 1.93   & 2.50   & 0.88   & 0.86   & 0.85
    ``Threefry16x64_64``      & 2.50   & 2.50   & 2.43   & 0.91   & 0.99   & 0.85
    ``Threefish256_64``       & 8.05   & 8.36   & 8.67   & 2.95   & 2.75   & 2.86
    ``Threefish512_64``       & 5.77   & 5.60   & 6.23   & 2.92   & 2.78   & 2.91
    ``Threefish1024_64``      & 8.73   & 8.32   & 8.48   & 3.38   & 3.80   & 3.24

.. _tab-Performance of MKLEngine:

.. csv-table:: Performance of ``MKLEngine``
    :delim: &
    :header-rows: 1
    :header: ,,Single,,,Batch,

    RNG                       & LLVM   & GNU    & Intel  & LLVM   & GNU    & Intel
    ``MKL_ARS5``              & 2.37   & 2.41   & 2.37   & 0.41   & 0.41   & 0.41
    ``MKL_PHILOX4X32X10``     & 2.95   & 3.00   & 2.97   & 0.77   & 0.77   & 0.77
    ``MKL_MCG59``             & 2.10   & 2.13   & 2.10   & 0.44   & 0.44   & 0.44
    ``MKL_MT19937``           & 1.99   & 2.10   & 2.09   & 0.32   & 0.32   & 0.32
    ``MKL_MT2203``            & 1.97   & 2.02   & 1.99   & 0.25   & 0.25   & 0.25
    ``MKL_SFMT19937``         & 1.97   & 2.02   & 2.00   & 0.22   & 0.22   & 0.22
    ``MKL_NONDETERM``         & 44.0   & 45.8   & 45.8   & 42.3   & 44.2   & 43.8
    ``MKL_ARS5_64``           & 1.15   & 1.14   & 1.13   & 0.39   & 0.39   & 0.39
    ``MKL_PHILOX4X32X10_64``  & 1.58   & 1.58   & 1.51   & 0.76   & 0.76   & 0.76
    ``MKL_MCG59_64``          & 0.98   & 0.98   & 0.94   & 0.42   & 0.42   & 0.42
    ``MKL_MT19937_64``        & 0.92   & 0.91   & 0.91   & 0.32   & 0.32   & 0.32
    ``MKL_MT2203_64``         & 0.88   & 0.83   & 0.80   & 0.25   & 0.25   & 0.25
    ``MKL_SFMT19937_64``      & 0.89   & 0.89   & 0.85   & 0.20   & 0.20   & 0.20
    ``MKL_NONDETERM_64``      & 43.0   & 44.8   & 43.7   & 42.6   & 44.4   & 43.2

.. _tab-Performance of Non-Determinstic RNGs:

.. csv-table:: Performance of Non-Determinstic RNGs
    :delim: &
    :header-rows: 1
    :header: ,,Single,,,Batch,

    RNG                       & LLVM   & GNU    & Intel  & LLVM   & GNU    & Intel
    ``RDRAND16``              & 177    & 175    & 172    & 168    & 175    & 172
    ``RDRAND32``              & 88.9   & 85.2   & 87.3   & 84.1   & 85.2   & 87.0
    ``RDRAND64``              & 44.8   & 42.6   & 42.6   & 42.3   & 42.6   & 42.6
