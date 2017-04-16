**********************
Mathematical Functions
**********************

Constants
=========

MCKL defines some mathematical constants in the form of constant expression
functions. For example, to get the value of :math:`\pi` with a desired
precision, one can use the following,

.. code-block:: cpp

    constexpr float       pi_f = const_pi<float>();
    constexpr double      pi_d = const_pi<double>();
    constexpr long double pi_l = const_pi<long double>();

The compiler evaluates these values at compile-time and thus there is no
performance difference from hard-coding the constants in the program, while the
readability is improved. All defined constants are listed in the table below.

.. _tab-mathematical-constants:

.. csv-table:: Mathematical constants
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

Vectorized Functions
====================

MCKL provides a set of vectorized elementary mathematical functions. For
example, to perform additions of two vectors,

.. code-block:: cpp

    size_t n = 1000;
    mckl::Vector<double> a(n), b(n), y(n);
    // Fill vectors a and b
    mckl::add(n, a.data(), b.data(), y.data());

This is equivalent to,

.. code-block:: cpp

    for (size_t i = 0; i != n; ++i)
        y[i] = a[i] + b[i];

The functions defined are listed in tables below. For each function, the first
parameter is always the length of the vector, and the last is a pointer to the
output vector (except for ``sincos`` and ``modf`` which have two output
vecrors). The output parameters are always vectors. Some of the input
parameters may be scalars. For example, in the function call,

.. code-block:: cpp

    mckl::muladd(n, a, b, c, y);

in :ref:`tab-arithmetic-functions`, the input parameters are ``a``, ``b``, and
``c``. Some of them, (but not all), can be scalars instead of pointers to
vectors. The output parameter ``y`` has to be a pointer to a vector. Therefore,
there are seven versions of this function for each type of the parameters.

The input of these functions can be either real numbers (floating point types),
or complex numbers (``std::complex<double>``, etc.), or both. The supported
data types are also listed in the tables. In most cases, output data type is
the same as the input. There are a few exceptions. The ``abs`` and ``arg``
functions always have real numbers as output. The ``cis`` function takes real
numbers as input and complex numbers as output. Note that, mixed precision is
not allowed. For example,

.. code-block:: cpp

    mckl::Vector<double> a(n);
    mckl::Vector<double> b(n);
    mckl::Vector<double> y(n);
    mckl::muladd(n, a.data(), b.data(), 2, y.data());

causes compile-time error because the fourth argument is an integer while the
others are floating point types. The correct call shall be,

.. code-block:: cpp

    mckl::muladd(n, a.data(), b.data(), 2.0, y.data());

The same principle applies to mixed types functions (``abs``, ``arg`` and
``cis``).

With only the standard library, these functions do not provide performance
advantage compared to simple loops. When MCL VML is available, some functions
can have substantial performance improvement when all input arguments are
vectors of types single or double precision floating point types, or their
complex counterparts.
.. The performance of vectorized random number generating
.. (see section~\ref{sec:Vectorized Random Number Generating}) heavily depends
.. on these functions.

The input and output pointers are allowed to alias to each other in the sense
that they might pointing to the same memory locations. However, if they point
different locations but the vectors overlap, the behavior is undefined.

.. _tab-arithmetic-functions:

.. csv-table:: Arithmetic Functions
    :delim: &
    :header: Function, Operation, Data Type

    ``add``       & :math:`y = a + b`           & Real, Complex
    ``sub``       & :math:`y = a - b`           & Real, Complex
    ``sqr``       & :math:`y = a^2`             & Real
    ``mul``       & :math:`y = ab`              & Real, Complex
    ``mulbyconj`` & :math:`y = a\bar{b}`        & Complex
    ``conj``      & :math:`y = \bar{a}`         & Complex
    ``abs``       & :math:`y = \lvert a\rvert`  & Real, Complex
    ``arg``       & :math:`y = \mathrm{arg}(a)` & Complex
    ``muladd``    & :math:`y = ab + c`          & Real, Complex
    ``mulsub``    & :math:`y = ab - c`          & Real, Complex
    ``nmuladd``   & :math:`y = -ab + c`         & Real, Complex
    ``nmulsub``   & :math:`y = -ab - c`         & Real, Complex
    ``fmadd``     & :math:`y = ab + c` (fused)  & Real
    ``fmsub``     & :math:`y = ab - c` (fused)  & Real
    ``fnmadd``    & :math:`y = -ab + c` (fused) & Real
    ``fnmsub``    & :math:`y = -ab - c` (fused) & Real

.. _tab-power-and-root-functions:

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

.. _tab-exponential-and-logarithm-functions:

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

.. _tab-trigonometric-functions:

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

.. _tab-hyperbolic-functions:

.. csv-table:: Hyperbolic Functions
    :delim: &
    :header: Function, Operation, Data Type

    ``cosh``  & :math:`y = \cosh(a)`             & Real, Complex
    ``sinh``  & :math:`y = \sinh(a)`             & Real, Complex
    ``tanh``  & :math:`y = \tanh(a)`             & Real, Complex
    ``acosh`` & :math:`y = \mathrm{arc}\cosh(a)` & Real, Complex
    ``asinh`` & :math:`y = \mathrm{arc}\sinh(a)` & Real, Complex
    ``atanh`` & :math:`y = \mathrm{arc}\tanh(a)` & Real, Complex

.. _tab-special-functions:

.. csv-table:: Special Functions
    :delim: &
    :header: Function, Operation, Data Type

    ``erf``     & :math:`y = \mathrm{erf}(a)`                      & Real
    ``erfc``    & :math:`y = \mathrm{erfc}(a)`                     & Real
    ``cdfnorm`` & :math:`y = (1 + \mathrm{erf}(a / \sqrt{2})) / 2` & Real
    ``lgamma``  & :math:`y = \ln\Gamma(a)`                         & Real
    ``tgamma``  & :math:`y = \Gamma(a)`                            & Real

.. _tab-rounding-functions:

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

Fused Multiplication and Addition
=================================

The ``muladd`` and ``fmadd`` functions in :ref:`tab-arithmetic-functions`
differs in that ``fmadd`` always does fused multiplication and addition by
``std::fma`` while ``muladd`` use one multiplication and one addition. However,
the compiler may or may not be able to make use the platform FMA support. And
it may not vectorize the loop in ``fmadd`` etc., as most modern C++ compilers
would do for simpler operations such as addition and multiplication.

When software implementation of ``std::fma`` is used, it will be much slower
than using one multiplication and one addition. In this case, there are
assembly implementations that take advantage of the platform support for single
and double precision. To enable this feature, one need to build and link to the
:ref:`optional runtime library <optional-runtime-library>`. One also need to
set the configuration macro ``MCKL_USE_ASM_LIB`` to true. In addition, this
feature is only enabled for platforms with FMA3 instruction set support.

The library detects the availability of these instructions using compiler
macros. If this mechanism is not adequate, one can manually enable or disable
them using the configuration macro ``MCKL_HAS_FMA``. Note that, when the
compiler is able to generate vectorized loop for ``std::fma``, the assembly
library may or may not outperform the compiler generated binary. If the
compiler generated binary is preferred but the runtime library is also enabled
for its other features, then one can define the configuration macro
``MCKL_USE_ASM_FMA`` to false to disable this feature.

The same principles also applies to other related functions such as ``mulsub``
and ``fmsub``. Last but not least, when the runtime library is used, it also
replace single and double precisions of ``muladd``, etc., with their
corresponding fused version.
