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
running macOS Sierra (version 10.12.4).

Three compilers are tested, LLVM clang (version Apple 8.1.0), GNU GCC (version
6.3.0), and Intel C++ compiler (version 17.0.2). They are labeled as “LLVM”,
“GCC” and “Intel”, respectively.

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

    RNG                       & LLVM   & GCC    & Intel  & LLVM   & GCC    & Intel
    ``std::mt19937``          & 4.56   & 4.21   & 5.03   & 4.56   & 3.99   & 5.03
    ``std::mt19937_64``       & 2.35   & 2.21   & 2.46   & 2.35   & 2.18   & 2.46
    ``std::minstd_rand0``     & 9.68   & 10.1   & 10.9   & 9.68   & 10.1   & 10.9
    ``std::minstd_rand``      & 7.61   & 8.56   & 9.59   & 7.54   & 8.56   & 9.57
    ``std::ranlux24_base``    & 12.4   & 5.98   & 13.7   & 12.6   & 6.15   & 12.1
    ``std::ranlux48_base``    & 5.83   & 3.11   & 6.59   & 5.83   & 3.07   & 6.69
    ``std::ranlux24``         & 130    & 94.2   & 130    & 130    & 93.7   & 113
    ``std::ranlux48``         & 223    & 159    & 243    & 224    & 159    & 230
    ``std::knuth_b``          & 43.7   & 49.6   & 30.5   & 37.8   & 48.8   & 30.4

.. _tab-Performance of RNGs in the Random123 Library:

.. csv-table:: Performance of RNGs in the Random123 Library
    :delim: &
    :header-rows: 1
    :header: ,,Single,,,Batch,

    RNG                       & LLVM   & GCC    & Intel  & LLVM   & GCC    & Intel
    ``r123::AESNI4x32``       & 6.58   & 1.56   & 8.93   & 6.61   & 1.52   & 8.83
    ``r123::ARS4x32``         & 5.13   & 1.18   & 6.39   & 5.19   & 1.18   & 6.30
    ``r123::Philox2x32``      & 10.1   & 3.04   & 12.6   & 10.1   & 3.07   & 12.6
    ``r123::Philox4x32``      & 6.14   & 2.86   & 13.1   & 6.17   & 2.81   & 13.1
    ``r123::Philox2x64``      & 2.19   & 1.77   & 10.3   & 2.15   & 1.56   & 10.2
    ``r123::Philox4x64``      & 2.25   & 1.72   & 12.0   & 2.25   & 1.67   & 11.9
    ``r123::Threefry2x32``    & 11.6   & 4.59   & 10.8   & 11.7   & 4.59   & 10.9
    ``r123::Threefry4x32``    & 6.94   & 3.86   & 7.86   & 6.84   & 3.87   & 7.80
    ``r123::Threefry2x64``    & 3.38   & 2.25   & 3.17   & 3.30   & 2.32   & 3.17
    ``r123::Threefry4x64``    & 2.62   & 1.97   & 6.75   & 2.53   & 1.97   & 6.72

.. _tab-Performance of AESEngine:

.. csv-table:: Performance of ``AESEngine``
    :delim: &
    :header-rows: 1
    :header: ,,Single,,,Batch,

    RNG                       & LLVM   & GCC    & Intel  & LLVM   & GCC    & Intel
    ``AES128``                & 1.53   & 1.90   & 1.87   & 0.64   & 0.63   & 0.63
    ``AES192``                & 2.19   & 2.28   & 2.25   & 0.76   & 0.76   & 0.76
    ``AES256``                & 2.57   & 2.63   & 2.63   & 0.89   & 0.89   & 0.89
    ``ARS``                   & 1.04   & 1.33   & 1.01   & 0.32   & 0.32   & 0.32
    ``AES128_64``             & 1.14   & 1.36   & 1.48   & 0.63   & 0.63   & 0.63
    ``AES192_64``             & 1.62   & 1.62   & 1.78   & 0.76   & 0.76   & 0.76
    ``AES256_64``             & 1.93   & 1.96   & 2.06   & 0.88   & 0.88   & 0.88
    ``ARS_64``                & 0.73   & 0.86   & 0.67   & 0.32   & 0.32   & 0.32

.. _tab-Performance of PhiloxEngine:

.. csv-table:: Performance of ``PhiloxEngine``
    :delim: &
    :header-rows: 1
    :header: ,,Single,,,Batch,

    RNG                       & LLVM   & GCC    & Intel  & LLVM   & GCC    & Intel
    ``Philox2x32``            & 4.71   & 4.52   & 5.57   & 0.61   & 0.61   & 0.61
    ``Philox4x32``            & 3.74   & 6.27   & 4.34   & 0.63   & 0.63   & 0.63
    ``Philox2x64``            & 2.70   & 2.38   & 3.07   & 1.42   & 1.42   & 1.43
    ``Philox4x64``            & 2.79   & 2.67   & 2.48   & 1.45   & 1.45   & 1.45
    ``Philox2x32_64``         & 4.08   & 4.08   & 5.32   & 0.61   & 0.61   & 0.61
    ``Philox4x32_64``         & 3.46   & 6.07   & 4.06   & 0.61   & 0.61   & 0.63
    ``Philox2x64_64``         & 2.09   & 2.08   & 2.82   & 1.42   & 1.42   & 1.42
    ``Philox4x64_64``         & 2.28   & 2.16   & 2.08   & 1.45   & 1.45   & 1.45

.. _tab-Performance of ThreefryEngine:

.. csv-table:: Performance of ``ThreefryEngine``
    :delim: &
    :header-rows: 1
    :header: ,,Single,,,Batch,

    RNG                       & LLVM   & GCC    & Intel  & LLVM   & GCC    & Intel
    ``Threefry2x32``          & 7.34   & 6.61   & 7.16   & 0.99   & 0.99   & 0.94
    ``Threefry4x32``          & 5.33   & 6.81   & 5.32   & 1.01   & 0.99   & 0.98
    ``Threefry2x64``          & 4.02   & 3.33   & 4.28   & 0.92   & 0.92   & 0.89
    ``Threefry4x64``          & 3.27   & 2.89   & 3.52   & 0.95   & 0.91   & 0.86
    ``Threefry8x64``          & 3.04   & 2.19   & 3.13   & 0.89   & 0.86   & 0.86
    ``Threefry16x64``         & 3.99   & 3.46   & 3.60   & 0.91   & 1.02   & 0.86
    ``Threefish256``          & 8.78   & 8.89   & 9.16   & 2.97   & 2.76   & 2.81
    ``Threefish512``          & 6.72   & 6.55   & 6.83   & 2.92   & 2.78   & 2.79
    ``Threefish1024``         & 10.6   & 9.21   & 9.44   & 3.41   & 3.84   & 3.26
    ``Threefry2x32_64``       & 6.53   & 5.50   & 6.59   & 0.98   & 0.98   & 0.94
    ``Threefry4x32_64``       & 4.78   & 6.61   & 4.85   & 1.01   & 0.98   & 0.96
    ``Threefry2x64_64``       & 3.29   & 3.13   & 4.03   & 0.92   & 0.91   & 0.88
    ``Threefry4x64_64``       & 2.57   & 2.62   & 2.95   & 0.94   & 0.89   & 0.85
    ``Threefry8x64_64``       & 1.97   & 1.93   & 2.50   & 0.88   & 0.86   & 0.86
    ``Threefry16x64_64``      & 2.50   & 2.48   & 2.43   & 0.91   & 0.99   & 0.85
    ``Threefish256_64``       & 8.05   & 8.35   & 8.43   & 2.95   & 2.75   & 2.79
    ``Threefish512_64``       & 5.67   & 5.58   & 6.05   & 2.89   & 2.78   & 2.78
    ``Threefish1024_64``      & 8.70   & 8.29   & 8.43   & 3.38   & 3.80   & 3.22

.. _tab-Performance of MKLEngine:

.. csv-table:: Performance of ``MKLEngine``
    :delim: &
    :header-rows: 1
    :header: ,,Single,,,Batch,

    RNG                       & LLVM   & GCC    & Intel  & LLVM   & GCC    & Intel
    ``MKL_ARS5``              & 2.37   & 2.38   & 2.35   & 0.41   & 0.41   & 0.41
    ``MKL_PHILOX4X32X10``     & 2.95   & 2.98   & 2.95   & 0.76   & 0.77   & 0.77
    ``MKL_MCG59``             & 2.09   & 2.13   & 2.10   & 0.44   & 0.44   & 0.44
    ``MKL_MT19937``           & 2.00   & 2.09   & 2.09   & 0.32   & 0.32   & 0.32
    ``MKL_MT2203``            & 1.99   & 2.00   & 1.97   & 0.25   & 0.26   & 0.25
    ``MKL_SFMT19937``         & 1.97   & 2.00   & 2.00   & 0.22   & 0.22   & 0.22
    ``MKL_ARS5_64``           & 1.15   & 1.15   & 1.11   & 0.39   & 0.39   & 0.39
    ``MKL_PHILOX4X32X10_64``  & 1.58   & 1.58   & 1.52   & 0.75   & 0.76   & 0.76
    ``MKL_MCG59_64``          & 0.98   & 0.98   & 0.94   & 0.42   & 0.42   & 0.42
    ``MKL_MT19937_64``        & 0.92   & 0.92   & 0.89   & 0.32   & 0.32   & 0.32
    ``MKL_MT2203_64``         & 0.88   & 0.83   & 0.80   & 0.25   & 0.25   & 0.25
    ``MKL_SFMT19937_64``      & 0.89   & 0.91   & 0.85   & 0.20   & 0.20   & 0.20
