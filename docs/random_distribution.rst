.. ============================================================================
..  MCKL/docs/random_distribution.rst
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

.. _chap-Performance of Distributions:

****************************
Performance of Distributions
****************************

The performance is measured on an MacBook Pro with an Intel Core i7-4960HQ CPU
running macOS Sierra (version 10.12.4).

Three compilers are tested, LLVM clang (version Apple 8.1.0), GNU GCC (version
6.3.0), and Intel C++ compiler (version 17.0.2). Results of the LLVM clang
compiler is shown here. The results with other compilers are similar.

Two usage cases of RNGs are considered. The first is generating random integers
within a loop, each iteration generate a single random integer,

Four usage cases of the distributions are considered. First, if the
distribution is available in the standard library, we measure the following
case,

.. code-block:: cpp

    std::normal_distribution<double> rnorm_std(0, 1);
    for (size_t i = 0; i = n; ++i)
        r[i] = rand(rng, rnorm_std);

Second, we measure the performance of the library's implementation,

.. code-block:: cpp

    ::mckl::NormalDistribution<double> rnorm_mckl(0, 1);
    for (size_t i = 0; i = n; ++i)
        r[i] = ::mckl::rand(rng, rnorm_mckl);

The third and the fourth are the vectorized performance,

.. code-block:: cpp

    ::mckl::rand(rng, rnorm_mckl, n, r.data());

See :ref:`sec-Vectorized Random Number Generating`. The difference is that we
first test the performance without the `Intel MKL`_ library and then with it.
When `Intel MKL`_ is not used, the assembly implementation of selected
functions are used. For all the four above, the `ARS` RNG is used (see
:ref:`sub-AES Round Function Based Random Number Generators`). The last is when
there are \mkl routines for generating random numbers from the distribution,
either directly or indirectly. In this case, we use the ``MKL_ARS5`` RNG (see
:ref:`sec-MKL Random Number Generators`),

.. code-block:: cpp

    ::mckl::MKL_ARS5 rng_mkl;
    ::mckl::rand(rng_mkl, rnorm_mckl, n, r.data());

In all cases, we repeat the simulations 100 times, each time with :math:`n`
chosen randomly between 5,000 and 10,000. The total number of cycles of the 100
simulations are recorded, and then divided by the total number of elements
generated. This gives the performance measurement in cpE. This experiment is
repeated ten times, and the best results are shown. The five cases are labeled
“STD”, “MCKL”, “VMF”, “VML” and “MKL”, respectively, in tables below.

.. _Intel MKL:
    https://software.intel.com/en-us/intel-mkl/

.. _tab-Performance of Standard Uniform Distributions:

.. csv-table:: Performance of Standard Uniform Distributions
    :delim: &
    :header: Distribution, STD, MCKL, VMF, VML, MKL

    ``U01Canonical``               & 16.6   & 16.1   & 4.02   & 4.02   & —
    ``U01CC``                      & —      & 10.9   & 2.54   & 2.53   & —
    ``U01CO``                      & 16.6   & 7.86   & 2.53   & 2.54   & 3.46
    ``U01OC``                      & —      & 8.59   & 2.54   & 2.54   & —
    ``U01OO``                      & —      & 8.58   & 2.54   & 2.51   & —

.. _tab-Performance of Distributions using the Inverse Method:

.. csv-table:: Performance of Distributions using the Inverse Method
    :delim: &
    :header: Distribution, STD, MCKL, VMF, VML, MKL

    ``Arcsine(0,1)``               & —      & 64.5   & 12.9   & 12.8   & —
    ``Cauchy(0,1)``                & 97.2   & 71.9   & 15.9   & 19.6   & 11.7
    ``Exponential(1)``             & 68.6   & 48.4   & 12.2   & 9.94   & 9.34
    ``ExtremeValue(0,1)``          & 130    & 103    & 22.0   & 17.8   & 17.2
    ``Laplace(0,1)``               & —      & 72.5   & 15.3   & 13.2   & 15.3
    ``Logistic(0,1)``              & —      & 63.1   & 20.6   & 18.7   & 19.7
    ``Pareto(1,1)``                & —      & 107    & 18.3   & 16.1   & —
    ``Rayleigh(1)``                & —      & 51.1   & 19.9   & 14.7   & 14.3
    ``UniformReal(-1.3,1.3)``      & 17.6   & 9.85   & 3.00   & 3.00   & 2.86
    ``Weibull(1,1)``               & 187    & 47.7   & 12.2   & 10.2   & 10.3

.. _tab-Performance of Beta Distribution:

.. csv-table:: Performance of Beta Distribution
    :delim: &
    :header: Distribution, STD, MCKL, VMF, VML, MKL

    ``Beta(0.3,0.3)``              & —      & 244    & 167    & 71.7   & 51.6
    ``Beta(0.5,0.5)``              & —      & 77.8   & 12.8   & 13.8   & 57.0
    ``Beta(0.5,1)``                & —      & 98.7   & 18.1   & 15.8   & 14.7
    ``Beta(0.5,1.5)``              & —      & 291    & 273    & 271    & 86.8
    ``Beta(0.9,0.9)``              & —      & 249    & 231    & 231    & 74.0
    ``Beta(1,0.5)``                & —      & 104    & 18.3   & 16.1   & 14.7
    ``Beta(1,1)``                  & —      & 21.4   & 2.59   & 2.59   & 14.7
    ``Beta(1,1.5)``                & —      & 104    & 18.3   & 16.1   & 14.7
    ``Beta(1.5,0.5)``              & —      & 292    & 273    & 273    & 87.0
    ``Beta(1.5,1)``                & —      & 98.7   & 18.1   & 15.8   & 14.7
    ``Beta(1.5,1.5)``              & —      & 257    & 67.7   & 62.8   & 60.3

.. _tab-Performance of chi-Squared-Distribution:

.. csv-table:: Performance of :math:`\chi^2`-Distribution
    :delim: &
    :header: Distribution, STD, MCKL, VMF, VML, MKL

    ``ChiSquared(0.2)``            & 198    & 180    & 45.8   & 42.2   & 47.3
    ``ChiSquared(1)``              & 258    & 235    & 81.5   & 77.8   & 68.5
    ``ChiSquared(1.4)``            & 271    & 263    & 67.4   & 60.2   & 50.7
    ``ChiSquared(1.8)``            & 266    & 220    & 51.0   & 45.6   & 39.1
    ``ChiSquared(2)``              & 89.6   & 66.6   & 12.3   & 10.1   & 10.4
    ``ChiSquared(3)``              & 315    & 235    & 44.2   & 45.0   & 39.4
    ``ChiSquared(30)``             & 286    & 239    & 39.5   & 41.8   & 36.3

.. _tab-Performance of Gamma Distribution:

.. csv-table:: Performance of Gamma Distribution
    :delim: &
    :header: Distribution, STD, MCKL, VMF, VML, MKL

    ``Gamma(0.1,1)``               & 198    & 179    & 45.7   & 42.3   & 47.2
    ``Gamma(0.5,1)``               & 257    & 234    & 81.0   & 77.7   & 68.4
    ``Gamma(0.7,1)``               & 270    & 261    & 67.4   & 60.1   & 50.7
    ``Gamma(0.9,1)``               & 265    & 218    & 50.8   & 44.8   & 39.2
    ``Gamma(1,1)``                 & 88.2   & 65.2   & 12.2   & 10.2   & 10.4
    ``Gamma(1.5,1)``               & 310    & 234    & 43.9   & 45.2   & 39.5
    ``Gamma(15,1)``                & 285    & 236    & 40.4   & 41.8   & 36.3

.. _tab-Performance of Fisher’s F-Distribution:

.. csv-table:: Performance of Fisher’s *F*-Distribution
    :delim: &
    :header: Distribution, STD, MCKL, VMF, VML, MKL

    ``FisherF(0.5,0.5)``           & 443    & 402    & 168    & 155    & 140
    ``FisherF(0.5,1)``             & 482    & 431    & 193    & 179    & 153
    ``FisherF(0.5,1.5)``           & 493    & 454    & 187    & 173    & 130
    ``FisherF(0.5,3)``             & 543    & 454    & 140    & 134    & 118
    ``FisherF(0.5,30)``            & 512    & 455    & 131    & 125    & 114
    ``FisherF(1,0.5)``             & 482    & 432    & 195    & 180    & 153
    ``FisherF(1,1)``               & 520    & 461    & 217    & 201    & 164
    ``FisherF(1,1.5)``             & 530    & 483    & 211    & 195    & 142
    ``FisherF(1,3)``               & 577    & 485    & 163    & 156    & 130
    ``FisherF(1,30)``              & 544    & 486    & 154    & 147    & 126
    ``FisherF(1.5,0.5)``           & 495    & 445    & 187    & 173    & 130
    ``FisherF(1.5,1)``             & 529    & 471    & 211    & 196    & 142
    ``FisherF(1.5,1.5)``           & 540    & 492    & 204    & 190    & 117
    ``FisherF(1.5,3)``             & 594    & 491    & 156    & 149    & 108
    ``FisherF(1.5,30)``            & 555    & 493    & 148    & 142    & 103
    ``FisherF(3,0.5)``             & 551    & 438    & 139    & 134    & 119
    ``FisherF(3,1)``               & 564    & 464    & 163    & 155    & 130
    ``FisherF(3,1.5)``             & 579    & 481    & 157    & 150    & 107
    ``FisherF(3,3)``               & 615    & 477    & 107    & 109    & 93.7
    ``FisherF(3,30)``              & 601    & 478    & 99.2   & 101    & 89.4
    ``FisherF(30,0.5)``            & 508    & 437    & 131    & 124    & 114
    ``FisherF(30,1)``              & 549    & 465    & 155    & 148    & 126
    ``FisherF(30,1.5)``            & 561    & 480    & 149    & 144    & 103
    ``FisherF(30,3)``              & 595    & 475    & 99.3   & 101    & 89.6
    ``FisherF(30,30)``             & 555    & 474    & 91.3   & 92.5   & 84.9

.. _tab-Performance of Normal and Related Distributions:

.. csv-table:: Performance of Normal and Related Distributions
    :delim: &
    :header: Distribution, STD, MCKL, VMF, VML, MKL

    ``Normal(0,1)``                & 87.8   & 77.3   & 17.6   & 18.4   & 15.4
    ``Lognormal(0,1)``             & 143    & 119    & 23.6   & 24.2   & 19.9
    ``Levy(0,1)``                  & —      & 83.3   & 29.2   & 30.9   & 27.6

.. _tab-Performance of Stable Distribution:

.. csv-table:: Performance of Stable Distribution
    :delim: &
    :header: Distribution, STD, MCKL, VMF, VML, MKL

    ``Stable(0.5,1,0,1)``          & —      & 387    & 202    & 67.0   & —
    ``Stable(1,0,0,1)``            & —      & 194    & 49.0   & 51.5   & —
    ``Stable(2,0,0,1)``            & —      & 388    & 202    & 75.2   & —

.. _tab-Performance of Student’s t-Distribution:

.. csv-table:: Performance of Student’s *t*-Distribution
    :delim: &
    :header: Distribution, STD, MCKL, VMF, VML, MKL

    ``StudentT(0.2)``              & 298    & 283    & 91.4   & 87.4   & 87.4
    ``StudentT(1)``                & 362    & 335    & 137    & 134    & 112
    ``StudentT(1.4)``              & 374    & 353    & 146    & 139    & 91.7
    ``StudentT(1.8)``              & 369    & 315    & 104    & 97.2   & 76.8
    ``StudentT(2)``                & 187    & 178    & 49.1   & 45.1   & 44.7
    ``StudentT(3)``                & 413    & 329    & 87.3   & 86.8   & 77.9
    ``StudentT(30)``               & 374    & 330    & 78.5   & 78.5   & 72.3

.. _tab-Performance of Discrete Distributions:

.. csv-table:: Performance of Discrete Distributions
    :delim: &
    :header: Distribution, STD, MCKL, VMF, VML, MKL

    ``Geometric(0.5)``             & 119    & 50.8   & 15.7   & 14.0   & 14.7
    ``UniformInt(-10,10)``         & 142    & 26.8   & 6.40   & 6.83   & 7.34
