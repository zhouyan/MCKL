.. ============================================================================
..  MCKL/docs/math.rst
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

.. _chap-Mathematical Functions:

**********************
Mathematical Functions
**********************

.. _sec-Constants:

Constants
=========

MCKL defines some mathematical constants in the form of constant expression
functions. For example, to get the value of :math:`\pi` with a desired
precision, one can use the following,

.. code-block:: cpp

    constexpr float       pi_f = ::mckl::const_pi<float>();
    constexpr double      pi_d = ::mckl::const_pi<double>();
    constexpr long double pi_l = ::mckl::const_pi<long double>();

The compiler evaluates these values at compile-time and thus there is no
performance difference between the function call and hard-coding the constants
in the program, while the readability is improved. All defined constants are
listed in :ref:`the table below <tab-Mathematical Constants>`.

.. _tab-Mathematical Constants:

.. csv-table:: Mathematical Constants
    :delim: &
    :header: Function, Value

    ``const_inf``          & :math:`\infty`
    ``const_nan``          & silent NaN
    ``const_zero``         & :math:`0`
    ``const_one``          & :math:`1`
    ``const_pi``           & :math:`\pi`
    ``const_pi_2``         & :math:`2\pi`
    ``const_pi_inv``       & :math:`1/\pi`
    ``const_pi_sqr``       & :math:`\pi^2`
    ``const_pi_by2``       & :math:`\pi/2`
    ``const_pi_by3``       & :math:`\pi/3`
    ``const_pi_by4``       & :math:`\pi/4`
    ``const_pi_by6``       & :math:`\pi/6`
    ``const_pi_2by3``      & :math:`2\pi/3`
    ``const_pi_3by4``      & :math:`3\pi/4`
    ``const_pi_4by3``      & :math:`4\pi/3`
    ``const_sqrt_pi``      & :math:`\sqrt{\pi}`
    ``const_sqrt_pi_2``    & :math:`\sqrt{2\pi}`
    ``const_sqrt_pi_inv``  & :math:`\sqrt{1/\pi}`
    ``const_sqrt_pi_by2``  & :math:`\sqrt{\pi/2}`
    ``const_sqrt_pi_by3``  & :math:`\sqrt{\pi/3}`
    ``const_sqrt_pi_by4``  & :math:`\sqrt{\pi/4}`
    ``const_sqrt_pi_by6``  & :math:`\sqrt{\pi/6}`
    ``const_sqrt_pi_2by3`` & :math:`\sqrt{2\pi/3}`
    ``const_sqrt_pi_3by4`` & :math:`\sqrt{3\pi/4}`
    ``const_sqrt_pi_4by3`` & :math:`\sqrt{4\pi/3}`
    ``const_ln_pi``        & :math:`\ln{\pi}`
    ``const_ln_pi_2``      & :math:`\ln{2\pi}`
    ``const_ln_pi_inv``    & :math:`\ln{1/\pi}`
    ``const_ln_pi_by2``    & :math:`\ln{\pi/2}`
    ``const_ln_pi_by3``    & :math:`\ln{\pi/3}`
    ``const_ln_pi_by4``    & :math:`\ln{\pi/4}`
    ``const_ln_pi_by6``    & :math:`\ln{\pi/6}`
    ``const_ln_pi_2by3``   & :math:`\ln{2\pi/3}`
    ``const_ln_pi_3by4``   & :math:`\ln{3\pi/4}`
    ``const_ln_pi_4by3``   & :math:`\ln{4\pi/3}`
    ``const_e``            & :math:`\mathrm{e}`
    ``const_e_inv``        & :math:`1/\mathrm{e}`
    ``const_sqrt_e``       & :math:`\sqrt{\mathrm{e}}`
    ``const_sqrt_e_inv``   & :math:`\sqrt{1/\mathrm{e}}`
    ``const_sqrt_2``       & :math:`\sqrt{2}`
    ``const_sqrt_3``       & :math:`\sqrt{3}`
    ``const_sqrt_5``       & :math:`\sqrt{5}`
    ``const_sqrt_10``      & :math:`\sqrt{10}`
    ``const_sqrt_1by2``    & :math:`\sqrt{1/2}`
    ``const_sqrt_1by3``    & :math:`\sqrt{1/3}`
    ``const_sqrt_1by5``    & :math:`\sqrt{1/5}`
    ``const_sqrt_1by10``   & :math:`\sqrt{1/10}`
    ``const_ln_2``         & :math:`\ln{2}`
    ``const_ln_3``         & :math:`\ln{3}`
    ``const_ln_5``         & :math:`\ln{5}`
    ``const_ln_10``        & :math:`\ln{10}`
    ``const_ln_inv_2``     & :math:`1/\ln{2}`
    ``const_ln_inv_3``     & :math:`1/\ln{3}`
    ``const_ln_inv_5``     & :math:`1/\ln{5}`
    ``const_ln_inv_10``    & :math:`1/\ln{10}`
    ``const_ln_ln_2``      & :math:`\ln\ln{2}`

.. _sec-Vectorized Functions:

Vectorized Functions
====================

MCKL provides a set of vectorized elementary mathematical functions. For
example, to perform additions of two vectors,

.. code-block:: cpp

    size_t n = 1000;
    ::mckl::Vector<double> a(n), b(n), y(n);
    // Fill vectors a and b
    ::mckl::add(n, a.data(), b.data(), y.data());

This is equivalent to,

.. code-block:: cpp

    for (size_t i = 0; i != n; ++i)
        y[i] = a[i] + b[i];

.. _sub-Types of Function Parameters:

Conventions of Parameters
-------------------------

For each function, the first parameter is always the length of the vectors, and
the last is a pointer to the output vector (except for ``sincos`` and ``modf``
which have two output vectors). The output parameters are always vectors. Some
of the input parameters may be scalars. For example, the ``muladd`` function
Functions` has the following seven overloads,

.. code-block:: cpp

    namespace mckl {

    template <typename T>
    void muladd(size_t n, const T *a, const T *b, const T *c, T *y);

    template <typename T>
    void muladd(size_t n, const T *a, const T *b, T c, T *y);

    template <typename T>
    void muladd(size_t n, const T *a, T b, const T *c, T *y);

    template <typename T>
    void muladd(size_t n, T a, const T *b, const T *c, T *y);

    template <typename T>
    void muladd(size_t n, T a, T b, const T *c, T *y);

    template <typename T>
    void muladd(size_t n, T a, const T *b, T c, T *y);

    template <typename T>
    void muladd(size_t n, const T *a, T b, T c, T *y);

    } // namespace mckl

The input of these functions can be either real numbers (floating point types),
or complex numbers (``std::complex<double>``, etc.), or both. The supported
data types are also listed in :ref:`the tables <sub-List of Functions>`. In
most cases, output data type is the same as the input. There are a few
exceptions. The ``abs`` and ``arg`` functions always have real numbers as
output. The ``cis`` function takes real numbers as input and complex numbers as
output. Note that, mixed precision is not allowed. For example,

.. code-block:: cpp

    ::mckl::Vector<double> a(n);
    ::mckl::Vector<double> b(n);
    ::mckl::Vector<double> y(n);
    ::mckl::muladd(n, a.data(), b.data(), 2, y.data());

causes compile-time error because the fourth argument is an integer while the
others are floating point types. The correct call shall be,

.. code-block:: cpp

    ::mckl::muladd(n, a.data(), b.data(), 2.0, y.data());

The same principle applies to mixed type functions (``abs``, ``arg`` and
``cis``).

The input and output pointers are allowed to alias to each other in the sense
that they might pointing to the same memory locations. However, if they point
different locations but the vectors overlap, the behavior is undefined.

.. _sub-Vectorized Performance:

Vectorized Performance
----------------------

With only the standard library, these functions do not provide performance
advantage compared to simple loops. When `Intel MKL`_ is available, some
functions can have substantial performance improvement when all input arguments
are vectors of types ``float`` or ``double``, ``std::complex<float>`` or
``std::complex<double>``. The performance of :ref:`sec-Vectorized Random Number
Generating` heavily depends on these functions.

.. _sub-List of Functions:

List of Functions
-----------------

.. _tab-Arithmetic Functions:

.. csv-table:: Arithmetic Functions
    :delim: &
    :header: Function, Operation, Data Type

    ``add``       & :math:`y = a + b`           & Real, Complex
    ``sub``       & :math:`y = a - b`           & Real, Complex
    ``sqr``       & :math:`y = a^2`             & Real
    ``mul``       & :math:`y = ab`              & Real, Complex
    ``mulbyconj`` & :math:`y = a\bar{b}`        & Complex
    ``conj``      & :math:`y = \bar{a}`         & Complex
    ``abs``       & :math:`y = \lvert{a}\rvert` & Real, Complex
    ``arg``       & :math:`y = \mathrm{arg}(a)` & Complex
    ``muladd``    & :math:`y = ab + c`          & Real, Complex
    ``mulsub``    & :math:`y = ab - c`          & Real, Complex
    ``nmuladd``   & :math:`y = -ab + c`         & Real, Complex
    ``nmulsub``   & :math:`y = -ab - c`         & Real, Complex
    ``fmadd``     & :math:`y = ab + c` (fused)  & Real
    ``fmsub``     & :math:`y = ab - c` (fused)  & Real
    ``fnmadd``    & :math:`y = -ab + c` (fused) & Real
    ``fnmsub``    & :math:`y = -ab - c` (fused) & Real

.. _tab-Power and Root Functions:

.. csv-table:: Power and Root Functions
    :delim: &
    :header: Function, Operation, Data Type

    ``inv``     & :math:`y = 1 / a`            & Real
    ``div``     & :math:`y = a / b`            & Real, Complex
    ``sqrt``    & :math:`y = \sqrt{a}`         & Real, Complex
    ``invsqrt`` & :math:`y = 1 / \sqrt{a}`     & Real
    ``cbrt``    & :math:`y = a^{1/3}`          & Real
    ``invcbrt`` & :math:`y = a^{-1/3}`         & Real
    ``pow2o3``  & :math:`y = a^{2/3}`          & Real
    ``pow3o2``  & :math:`y = a^{3/2}`          & Real
    ``pow``     & :math:`y = a^b`              & Real, Complex
    ``hypot``   & :math:`y = \sqrt{a^2 + b^2}` & Real

.. _tab-Exponential and Logarithm Functions:

.. csv-table:: Exponential and Logarithm Functions
    :delim: &
    :header: Function, Operation, Data Type

    ``exp``   & :math:`y = \mathrm{e}^a`     & Real, Complex
    ``exp2``  & :math:`y = 2^a`              & Real
    ``expm1`` & :math:`y = \mathrm{e}^a - 1` & Real
    ``log``   & :math:`y = \ln a`            & Real, Complex
    ``log2``  & :math:`y = \log_2 a`         & Real
    ``log10`` & :math:`y = \log_{10} a`      & Real, Complex
    ``log1p`` & :math:`y = \ln(a + 1)`       & Real

.. _tab-Trigonometric Functions:

.. csv-table:: Trigonometric Functions
    :delim: &
    :header: Function, Operation, Data Type

    ``cos``    & :math:`y = \cos(a)`            & Real, Complex
    ``sin``    & :math:`y = \sin(a)`            & Real, Complex
    ``sincos`` & :math:`z = \cos(a)`            & Real
    ``cis``    & :math:`y = \cos(a) + i\sin(a)` & Complex
    ``tan``    & :math:`y = \tan(a)`            & Real, Complex
    ``acos``   & :math:`y = \arccos(a)`         & Real, Complex
    ``asin``   & :math:`y = \arcsin(a)`         & Real, Complex
    ``atan``   & :math:`y = \arctan(a)`         & Real, Complex
    ``atan2``  & :math:`y = \arctan(a / b)`     & Real

.. _tab-Hyperbolic Functions:

.. csv-table:: Hyperbolic Functions
    :delim: &
    :header: Function, Operation, Data Type

    ``cosh``  & :math:`y = \cosh(a)`             & Real, Complex
    ``sinh``  & :math:`y = \sinh(a)`             & Real, Complex
    ``tanh``  & :math:`y = \tanh(a)`             & Real, Complex
    ``acosh`` & :math:`y = \mathrm{arc}\cosh(a)` & Real, Complex
    ``asinh`` & :math:`y = \mathrm{arc}\sinh(a)` & Real, Complex
    ``atanh`` & :math:`y = \mathrm{arc}\tanh(a)` & Real, Complex

.. _tab-Special Functions:

.. csv-table:: Special Functions
    :delim: &
    :header: Function, Operation, Data Type

    ``erf``     & :math:`y = \mathrm{erf}(a)`                      & Real
    ``erfc``    & :math:`y = \mathrm{erfc}(a)`                     & Real
    ``cdfnorm`` & :math:`y = (1 + \mathrm{erf}(a / \sqrt{2})) / 2` & Real
    ``lgamma``  & :math:`y = \ln\Gamma(a)`                         & Real
    ``tgamma``  & :math:`y = \Gamma(a)`                            & Real

.. _tab-Rounding Functions:

.. csv-table:: Rounding Functions
    :delim: &
    :header: Function, Operation, Data Type

    ``floor``     & :math:`y = \lfloor a \rfloor`                & Real
    ``ceil``      & :math:`y = \lceil a  \rceil`                 & Real
    ``trunc``     & :math:`y = \mathrm{sgn}(a)\lfloor|a|\rfloor` & Real
    ``round``     & :math:`y = \text{nearest integer of }a`      & Real
    ``nearbyint`` & :math:`y = \text{nearest integer of }a`      & Real
    ``rint``      & :math:`y = \text{nearest integer of }a`      & Real
    ``modf``      & :math:`z = a - y`                            & Real

.. _sec-Fused Multiplication and Addition:

Fused Multiplication and Addition
=================================

The ``muladd``, ``fmadd`` and related functions differ in that ``fmadd`` always
does `fused multiply-add
<https://en.wikipedia.org/wiki/Multiply–accumulate_operation#Fused_multiply.E2.80.93add>`_
(FMA) by ``std::fma`` while ``muladd`` use one multiplication and one addition.
However, the compiler may or may not be able to make use the hardware FMA
support. And it may not vectorize the loop in ``fmadd`` etc., as most modern
C++ compilers would do for simpler operations such as addition and
multiplication.

When software implementation of ``std::fma`` is used, it will be much slower
than using one multiplication and one addition. In this case, there are
assembly implementations that take advantage of the platform support for single
and double precision. To enable this feature, one needs to build and link to
the :ref:`sec-Optional Runtime Library`. One also needs to use preprocessor
configuration ``#define MCKL_USE_ASM_LIB 1``. In addition, this feature is only
enabled for platforms with `FMA3
<https://en.wikipedia.org/wiki/FMA_instruction_set#FMA3_instruction_set>`_
instruction set support.

The library detects the availability of FMA3 instructions using compiler
predefined macros. If this mechanism is not adequate, one can manually enable
them using preprocessor configuration ``#define MCKL_HAS_FMA 1``. Note that,
when the compiler is able to generate vectorized loop for ``std::fma``, the
assembly library may or may not outperform the compiler generated binary. If
the compiler generated binary is preferred but the runtime library is also used
for its other features, then one can use the preprocessor configuration
``#define MCKL_USE_ASM_FMA 0``.

.. _Intel MKL: https://software.intel.com/en-us/intel-mkl/
