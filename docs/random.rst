.. ============================================================================
..  MCKL/docs/random.rst
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

.. _chap-Random Number Generating:

************************
Random Number Generating
************************

.. _sec-Vectorized Random Number Generating:

Vectorized Random Number Generating
===================================

The generic function ``rand`` provides vectorized random number generating.
There are two main variants.

The first operates on RNG engines and generates unsigned random integers,

.. code-block:: cpp

    namespace mckl {

    template <typename RNGType>
    void rand(RNGType &rng, size_t n, typename RNGType::result_type *r);

    } // namespace mckl

For example,

.. code-block:: cpp

    ::mckl::RNG rng;
    constexpr size_t n = 1024;
    ::mckl::RNG::result_type r[n];
    ::mckl::rand(rng, n, r);

The effect of the function call above is equivalent to the following loop,

.. code-block:: cpp

    for (size_t i = 0; i != n; ++i)
        r[i] = rng();

The results are always the same unless a :ref:`sec-Non-Deterministic Random
Number Generators` is used. For some RNGs implemented in MCKL, the vectorized
function may have considerable performance advantage.

The second variant of ``rand`` is for generating distribution random variates,

.. code-block:: cpp

    namespace mckl {

    template <typename RNGType, typename DistributionType>
    void rand(RNGType &rng, DistributionType &distribution, size_t n,
        typename DistributionType::result_type *r);

    } // namespace mckl

For example,

.. code-block:: cpp

    ::mckl::RNG rng;
    ::mckl::NormalDistribution<double> normal(mean, sd);
    constexpr size_t n = 1024;
    double r[n];
    ::mckl::rand(rng, normal, n, r);

This is similar to the following loop,

.. code-block:: cpp

    for (size_t i = 0; i != n; ++i)
        r[i] = normal(rng);

Depending on the type of RNG and the distribution (including its parameters),
the vectorized function may have superior performance. However, the results are
not always the same as using a loop. The :ref:`sec-Uniform Bits Distribution`
and the :ref:`sec-Standard Uniform Distribution` will always produce exactly
the same results as using a loop. For some distributions, such as those using
the inverse method, the difference is up to rounding errors. For others,
completely different sequences of random numbers might be generated.

.. _sec-Counter-Based Random Number Generators:

Counter-Based Random Number Generators
======================================

The development by [Salmon2011]_ made high performance parallel RNGs much more
accessible than before. The RNGs introduced in the paper use bijection
:math:`f_k`, such that, for a sequence :math:`\{c_i = i\}_{i\ge0}`, the
sequence :math:`\{y_i = f_k(c_i)\}_{i\ge0}` appears random. In addition, for
:math:`k_1 \ne k_2`, :math:`f_{k_1}` and :math:`f_{k_2}` generate two sequences
that appear statistically independent. Compared to more conventional RNGs which
use recursions :math:`y_i = f_k(y_{i - 1})`, these counter-based RNGs are much
easier to use in a parallelized environment. If :math:`c`, the counter, is an
unsigned integer with :math:`b` bits, and :math:`k`, the key, is an unsigned
integer with :math:`d` bits. Then for each :math:`k`, the RNG has a period
:math:`2^b`. And there can be at most :math:`2^d` independent streams. MCKL
defines the following class template as the interface,

.. code-block:: cpp

    namespace mckl {

    template <typename ResultType, typename Generator>
    class CounterEngine;

    } // namespace mckl

where ``ResultType`` shall be an unsigned integer type and it is the output
type of the RNG engine. An instance of this class template is compatible with
standard library RNG engines, and can be used as a drop-in replacement of
classes such as ``std::mt19937``. A few classes that can be used as the
``Generator`` template argument are implemented in MCKL and discussed briefly
in this section. See [Salmon2011]_ for details of each algorithm.

.. _sub-AES Round Function Based Random Number Generators:

AES Round Function Based Random Number Generators
-------------------------------------------------

`AES <https://en.wikipedia.org/wiki/Advanced_Encryption_Standard>`_ round
function based RNGs in [Salmon2011]_ are implemented in the following
generator.

.. code-block:: cpp

    namespace mckl {

    template <typename KeySeqType>
    class AESGenerator;

    } // namespace mckl

The corresponding RNG engine is,

.. code-block:: cpp

    namespace mckl {

    template <typename ResultType, typename KeySeqType>
    using AESEngine = CounterEngine<ResultType, AESGenerator<KeySeqType>;

    } // namespace mckl

where ``KeySeqType`` is the class used to generate the sequences of round keys.
When `AESNI <https://en.wikipedia.org/wiki/AES_instruction_set>`_ instructions
are available, they are used for performance boost. Without going into details,
there are four types of sequences of round keys implemented by MCKL,

.. code-block:: cpp

    namespace mckl {

    template <size_t Rounds = MCKL_AES128_ROUNDS>
    class AES128KeySeq;

    template <size_t Rounds = MCKL_AES192_ROUNDS>
    class AES192KeySeq;

    template <size_t Rounds = MCKL_AES256_ROUNDS>
    class AES256KeySeq;

    template <size_t Rounds = MCKL_ARS_ROUNDS,
        typename Constants = ARSConstants>
    class ARSKeySeq;

    } // namespace mckl

The default rounds of the first three are 10, 12 and 14, respectively. And thus
they are equivalent to the AES-128, AES-192, and AES-256 block ciphers,
respectively. The last one is the ARS algorithm in [Salmon2011]_. The default
rounds is 5, instead of 7 as in the paper, but the same as its `Intel MKL`_
implementation.

The trait class ``Constants`` defines the Weyl’s sequence constants. The only
restriction on this trait class is that the following expressions are valid,

.. code-block:: cpp

    constexpr uint64_t w0 = Constants::weyl::value[0];
    constexpr uint64_t w1 = Constants::weyl::value[1];

The member data ``value`` will not be ODR used. The default constants are taken
from the paper. Correspondingly, there are four RNG engines,

.. code-block:: cpp

    namespace mckl {

    template <typename ResultType, size_t Rounds = MCKL_AES128_ROUNDS>
    using AES128Engine = AESEngine<ResultType, AES128KeySeq<Rounds>>;

    template <typename ResultType, size_t Rounds = MCKL_AES192_ROUNDS>
    using AES192Engine = AESEngine<ResultType, AES192KeySeq<Rounds>>;

    template <typename ResultType, size_t Rounds = MCKL_AES256_ROUNDS>
    using AES256Engine = AESEngine<ResultType, AES256KeySeq<Rounds>>;

    template <typename ResultType, size_t Rounds = MCKL_ARS_ROUNDS,
        typename Constants = ARSConstants>
    using ARSEngine = AESEngine<ResultType, ARSKeySeq<Rounds, Constants>>;

    } // namespace mckl

A few type aliases are defined for convenience.

.. code-block:: cpp

    namespace mckl {

    using AES128 = AES128Engine<uint32_t>;
    using AES192 = AES192Engine<uint32_t>;
    using AES256 = AES256Engine<uint32_t>;
    using ARS    = ARSEngine<uint32_t>;

    using AES128_64 = AES128Engine<uint64_t>;
    using AES192_64 = AES192Engine<uint64_t>;
    using AES256_64 = AES256Engine<uint64_t>;
    using ARS_64    = ARSEngine<uint64_t>;

    } // namespace mckl

.. _sub-Philox:

Philox
------

The Philox algorithm in [Salmon2011]_ is implemented in the following
generator,

.. code-block:: cpp

    namespace mckl {

    template <typename T, size_t K, size_t Rounds = MCKL_PHILOX_ROUNDS,
        typename Constants = PhiloxConstants<T, K>>
    class PhiloxGenerator;

    } // namespace mckl

The corresponding RNG engine is,

.. code-block:: cpp

    namespace mckl {

    template <typename ResultType, typename T, size_t K,
        size_t Rounds = MCKL_PHILOX_ROUNDS,
        typename Constants = PhiloxConstants<T, K>>
    using PhiloxEngine =
        CounterEngine<ResultType, PhiloxGenerator<T, K, Rounds, Constants>>;

    } // namespace mckl

The template parameter ``Constants`` is a trait class that defines the Weyl’s
sequence constants and the multipliers. The only restriction on this trait
class is that the following expressions are valid,

.. code-block:: cpp

    // i is a compile time constant expression
    constexpr T w = Constants::weyl::value[i];       // i = 0, … , K / 2 - 1
    constexpr T m = Constants::multiplier::value[i]; // i = 0, … , K / 2 - 1

The member data ``value`` will not be ODR used. The defaults are taken
from [Salmon2011]_. Four engines are defined in MCKL,

.. code-block:: cpp

    namespace mckl {

    template <typename ResultType>
    using Philox2x32Engine = PhiloxEngine<ResultType, uint32_t, 2>;

    template <typename ResultType>
    using Philox4x32Engine = PhiloxEngine<ResultType, uint32_t, 4>;

    template <typename ResultType>
    using Philox2x64Engine = PhiloxEngine<ResultType, uint64_t, 2>;

    template <typename ResultType>
    using Philox4x64Engine = PhiloxEngine<ResultType, uint64_t, 4>;

    } // namespace mckl

A few type aliases are defined for convenience,

.. code-block:: cpp

    namespace mckl {

    using Philox2x32 = Philox2x32Engine<uint32_t>;
    using Philox4x32 = Philox4x32Engine<uint32_t>;
    using Philox2x64 = Philox2x64Engine<uint32_t>;
    using Philox4x64 = Philox4x64Engine<uint32_t>;

    using Philox2x32_64 = Philox2x32Engine<uint64_t>;
    using Philox4x32_64 = Philox4x32Engine<uint64_t>;
    using Philox2x64_64 = Philox2x64Engine<uint64_t>;
    using Philox4x64_64 = Philox4x64Engine<uint64_t>;

    } // namespace mckl

.. _sub-Threefry:

Threefry
--------

The Threefry algorithm in [Salmon2011]_ is implemented in the following
generator,

.. code-block:: cpp

    namespace mckl {

    template <typename T, size_t K, size_t Rounds = MCKL_THREEFRY_ROUNDS,
        typename Constants = ThreefryConstants<T, K>>
    class ThreefryGenerator;

    } // namespace mckl

The corresponding RNG engine is,

.. code-block:: cpp

    namespace mckl {

    template <typename ResultType, typename T, size_t K,
        size_t Rounds = MCKL_THREEFRY_ROUNDS,
        typename Constants = ThreefryConstants<T, K>>
    using ThreefryEngine =
        CounterEngine<ResultType, ThreefryGenerator<T, K, Rounds, Constants>>;

    } // namespace mckl

The template parameter ``Constants`` is a trait class that defines the parity
constants, the rotation constants, and the permutation. The only restriction on
this trait class is that the following expressions are valid,

.. code-block:: cpp

    // i, j are compile time constant expression
    constexpr T k = Constants::parity::value;
    constexpr int r = Constants::rotate::value[i][j];  // i = 0, … , K / 2 - 1
                                                       // j = 1, … , 8
    constexpr size_t p = Constants::permute::value[i]; // i = 0, … , K - 1

The member data ``value`` will not be ODR used. The defaults are taken from the
`skein <http://www.skein-hash.info>`_ hash function and [Salmon2011]_. Six
engines are defined in MCKL,

.. code-block:: cpp

    namespace mckl {

    template <typename ResultType>
    using Threefry2x32Engine = ThreefryEngine<ResultType, uint32_t, 2>;

    template <typename ResultType>
    using Threefry4x32Engine = ThreefryEngine<ResultType, uint32_t, 4>;

    template <typename ResultType>
    using Threefry2x64Engine = ThreefryEngine<ResultType, uint64_t, 2>;

    template <typename ResultType>
    using Threefry4x64Engine = ThreefryEngine<ResultType, uint64_t, 4>;

    template <typename ResultType>
    using Threefry8x64Engine = ThreefryEngine<ResultType, uint64_t, 8>;

    template <typename ResultType>
    using Threefry16x64Engine = ThreefryEngine<ResultType, uint64_t, 16>;

    } // namespace mckl

In addition, three engines that are equivalent to Threefish-256, Threefish-512
and Threefish-1024 block ciphers, respectively, are also defined,

.. code-block:: cpp

    namespace mckl {

    template <typename ResultType>
    using Threefish256Engine = ThreefryEngine<ResultType, uint64_t, 4, 72>;

    template <typename ResultType>
    using Threefish512Engine = ThreefryEngine<ResultType, uint64_t, 8, 72>;

    template <typename ResultType>
    using Threefish1024Engine = ThreefryEngine<ResultType, uint64_t, 16, 80>;

    } // namespace mckl

A few type aliases are defined for convenience,

.. code-block:: cpp

    namespace mckl {

    using Threefry2x32  = Threefry2x32Engine<uint32_t>;
    using Threefry4x32  = Threefry4x32Engine<uint32_t>;
    using Threefry2x64  = Threefry2x64Engine<uint32_t>;
    using Threefry4x64  = Threefry4x64Engine<uint32_t>;
    using Threefry8x64  = Threefry8x64Engine<uint32_t>;
    using Threefry16x64 = Threefry16x64Engine<uint32_t>;

    using Threefry2x32_64  = Threefry2x32Engine<uint64_t>;
    using Threefry4x32_64  = Threefry4x32Engine<uint64_t>;
    using Threefry2x64_64  = Threefry2x64Engine<uint64_t>;
    using Threefry4x64_64  = Threefry4x64Engine<uint64_t>;
    using Threefry8x64_64  = Threefry8x64Engine<uint64_t>;
    using Threefry16x64_64 = Threefry16x64Engine<uint64_t>;

    using Threefish256  = Threefish256Engine<uint32_t>;
    using Threefish512  = Threefish512Engine<uint32_t>;
    using Threefish1024 = Threefish1024Engine<uint32_t>;

    using Threefish256_64  = Threefish256Engine<uint64_t>;
    using Threefish512_64  = Threefish512Engine<uint64_t>;
    using Threefish1024_64 = Threefish1024Engine<uint64_t>;

    } // namespace mckl

.. _sec-MKL Random Number Generators:

MKL Random Number Generators
============================

`Intel MKL`_ provides some high performance RNGs. MCKL implements a wrapper
class

.. code-block:: cpp

    namespace mckl {

    template <MKL_INT BRNG, int Bits>
    class MKLEngine;

    } // namespace mckl

that makes them accessible as C++11 engines. The output is either 32- or 64-bit
unsigned integers. This is determined by the template parameter ``Bits``, which
can only take one of these two values. The template parameter ``BRNG`` can be
any `Intel MKL`_ basic RNG that supports ``viRngUniformBits32`` (``Bits`` is
32) or ``viRngUniformBits64`` (``Bits`` is 64). Type aliases are listed below,

.. code-block:: cpp

    namespace mckl {

    using MKL_ARS5             = MKLEngine<VSL_BRNG_ARS5, 32>;
    using MKL_ARS5_64          = MKLEngine<VSL_BRNG_ARS5, 64>;
    using MKL_PHILOX4X32X10    = MKLEngine<VSL_BRNG_PHILOX4X32X10, 32>;
    using MKL_PHILOX4X32X10_64 = MKLEngine<VSL_BRNG_PHILOX4X32X10, 64>;
    using MKL_MCG59            = MKLEngine<VSL_BRNG_MCG59, 32>;
    using MKL_MCG59_64         = MKLEngine<VSL_BRNG_MCG59, 64>;
    using MKL_MT19937          = MKLEngine<VSL_BRNG_MT19937, 32>;
    using MKL_MT19937_64       = MKLEngine<VSL_BRNG_MT19937, 64>;
    using MKL_MT2203           = MKLEngine<VSL_BRNG_MT2203, 32>;
    using MKL_MT2203_64        = MKLEngine<VSL_BRNG_MT2203, 64>;
    using MKL_SFMT19937        = MKLEngine<VSL_BRNG_SFMT19937, 32>;
    using MKL_SFMT19937_64     = MKLEngine<VSL_BRNG_SFMT19937, 64>;
    using MKL_NONDETERM        = MKLEngine<VSL_BRNG_NONDETERM, 32>;
    using MKL_NONDETERM_64     = MKLEngine<VSL_BRNG_NONDETERM, 64>;

    } // namespace mckl

Note that, `Intel MKL`_ RNGs perform the best when they are used to generate
vectors of random numbers. These wrappers use a buffer to store such vectors.
And thus they have much larger state space than usual RNGs. When there are
`Intel MKL`_ routines for generating distribution random variates for one of
the distributions discussed later in :ref:`sub-Continuous Distribution` and
:ref:`sub-Discrete Distribution`, MCKL automatically uses these routines for
vectorized random number generating if the RNG is one of that listed above. For
example,

.. code-block:: cpp

    ::mckl::MKL_MT2203 rng;
    ::mckl::NormalDistribution<double> normal;
    normal(rng, n, r);               // MKL rountines used
    ::mckl::rand(rng, normal, n, r); // MKL rountines used

Note that, this is applicable when the distribution is a class in MCKL. It does
not work with classes such as ``std::normal_distribution``. This is also
applicable when the distribution is not directly supported by `Intel MKL`_, but
can be easily generated using other distributions, e.g., the Student’s
*t*-distribution. In addition, it is also applicable if a distribution is a
special case of one of the distributions supported by `Intel MKL`_, e.g., the
:math:`\chi^2`-distribution.

.. _sec-Non-Deterministic Random Number Generators:

Non-Deterministic Random Number Generators
==========================================

If `RDRAND <https://en.wikipedia.org/wiki/RdRand>`_ instructions are supported,
MCKL also implements three non-deterministic RNGs, ``RDRAND16``, ``RDRAND32``
and ``RDRAND64``. They output 16-, 32-, and 64-bit random integers,
respectively. RDRAND instructions may not return a random integer at all. The
RNG engine keeps trying until it succeeds. One can limit the maximum number of
trials by defining the configuration macro ``MCKL_RDRAND_NTRIAL_MAX``. A value
of zero, the default, means that the number of trials is unlimited. If it is a
positive number, and if after the specified number of trials no random integer
is returned by RDRAND instructions, zero is returned.

.. _sec-Seeding Random Number Generators:

Seeding Random Number Generators
================================

The following singleton class template,

.. code-block:: cpp

    namespace mckl {

    template <typename ResultType,
        typename ID = std::integral_constant<size_t, sizeof(ResultType)>,
        bool Randomize = true, bool Atomic = true>
    class SeedGenerator;

    } // namespace mckl

can be used to generate distinctive seeds. The method ``instance`` returns a
reference to the singleton. For example,

.. code-block:: cpp

    // Generate integers
    auto &seed = SeedGenerator<unsigned>::instance();

    // Generate keys for ARS
    auto &keys = SeedGenerator<ARS::key_type>::instance();

Different combinations of the template parameters will create different
instances of the singleton. To generate new seeds,

.. code-block:: cpp

    ::mckl::RNG rng1(seed.get()); // Construct with a random seed
    ::mckl::RNG rng2(seed.get()); // Construct with a different random seed
    ::mckl::ARS ars1(keys.get()); // Construct with a random key
    ::mckl::ARS ars2(keys.get()); // Construct with a different random key

The procedure for generating the seeds is described here. Let :math:`N` be the
total number of bits of ``ResultType``, that is,

.. code-block:: cpp

    constexpr int N = sizeof(ResultType) * CHAR_BIT;

If :math:`N` is not a multiple of 32, then a compile time error will be raised.
Otherwise, let :math:`S = \min\{N,64\}` and :math:`M = N / S`. The generator
keeps an :math:`S`-bit unsigned integer as its internal counter, say :math:`c`.
Use the ``set`` method to set the value of this internal integer. For example,

.. code-block:: cpp

    seed.set(101);

Each time a new seed is requested, this counter is incremented. If the template
parameter ``Atomic`` is true, then this increment is atomic and thread-safe.
That is, let the old value be :math:`s`,

.. math::

    s &\leftarrow c,\\
    c &\leftarrow c + 1.

The generator first calculates an :math:`N`-bit unsigned integer :math:`t` as
the following.

.. math::

    t = \begin{cases}
      (s \bmod m)p + r & \text{if } M = 1 \\
      s + 2^{N - S}r   & \text{otherwise}
    \end{cases},

where :math:`m = (2^S - 1 - r) / p + 1`, :math:`p = 1` and :math:`r = 0` by
default. To change their values, use the ``partition`` method. For example,

.. code-block:: cpp

    seed.partition(10, 3); // p = 10, r = 3

Last, if ``Randomize`` is false, then :math:`t` is returned as the requested
seed, with possible reordering of bytes on big-endian platforms such that the
results are exactly the same as on little-endian platforms. Otherwise, i.e.,
``Randomize`` is true, then :math:`t` is transformed through a randomize
function. If :math:`N = 32`, then it is transformed using a 32-bit `Speck block
cipher <https://en.wikipedia.org/wiki/Speck_(cipher)>`_ with a zero key. If
:math:`N = 2^W`, :math:`W = 5,\dots,10`, then it is transformed with the
bijection of ``Threefry2x32``, ``Threefry2x64``, ``Threefry4x64``,
``Threefry8x64``, and ``Threefry16x64`` RNGs with zero keys, respectively.
Otherwise, it is transformed with the Skein-512 hash function.

It is clear that, if :math:`N = 2^W`, :math:`W = 5,\dots,10`, or ``Randomize``
is false, then the seeds generated are always distinctive if the values of the
internal counter :math:`c` are distinctive. Therefore, the seed generator has a
period of :math:`\lfloor 2^S / p \rfloor` if :math:`M = 1` and :math:`2^S` if
:math:`M > 1`. Moreover, the values of :math:`t` belongs to the equivalent
class :math:`t \equiv r \pmod{p}` if :math:`M = 1` and :math:`\lfloor t / 2^{N
- S} \rfloor \equiv r` if :math:`M > 1`. Therefore, using the ``partition``
method, one can generate distinctive seeds across multiple computing nodes or
multiple programs.

One can save and restore the seed generator using standard library streams. For
example,

.. code-block:: cpp

    std::ifstream is("seed.txt");
    if (is)
        is >> ::mckl::Seed<RNG>::instance();    // Read seed from a file
    if (!is)
        ::mckl::Seed<RNG>::instance().set(101); // Set it manually
    is.close();
    // Using Seed
    std::ofstream os("seed.txt");
    os << Seed<RNG>::instance();        // Write the seed to a file
    os.close();

This is useful when one need to run a simulation program multiple times, but
need a different set of seeds for each run.

Last, the following class is defined for convenience,

.. code-block:: cpp

    template <typename RNGType>
    class Seed;

which is a derived class of the following generator,

.. code-block:: cpp

    ::mckl::SeedGenerator<typename SeedType<RNGType>>;

where ``SeedType`` is an alias to ``SeedTrait<RNGType>::type``,

.. code-block:: cpp

    namespace mckl {

    template <typename RNGType>
    class SeedTrait
    {
        public:
        using type = unsigned;
    };

    template <typename ResultType, typename Generator>
    class SeedTrait<CounterEngine<ResultType, Generator>>
    {
        public:
        using type = typename CounterEngine<ResultType, Generator>::key_type;
    };

    } // namespace mckl

Therefore, for most RNGs, the unsigned integers are generated as seeds and they
share the same internal counter :math:`c`. For counter-based RNGs implemented
in MCKL, keys will be generated as seeds and RNGs with the same key width will
share the same internal counter :math:`c`.

.. _sec-Using Multiple Random Number Generators:

Using Multiple Random Number Generators
=======================================

The class template ``RNGSet`` can be used to manage multiple RNG instances
within a parallel program. Three of them are implemented in MCKL. They all
have the same interface,

.. code-block:: cpp

    ::mckl::RNGSet<RNG> rng_set(N); // A set of N RNGs
    rng_set.resize(n);              // Change the size of the set
    rng_set[i];                     // Get a reference to the i-th RNG
    rng_set.reset();                // Re-seed each RNG in the set

The ``reset`` method use ``Seed<RNG>`` discussed earlier to generate new seeds.

The first implementation is ``RNGSetScalar``. As its name suggests, it is only
a wrapper of a single RNG. All calls to ``rng_set[i]`` returns a reference to
the same RNG. It is only useful when an ``RNGSet`` interface is required while
the thread-safety and other issues are not important. The second implementation
is ``RNGSetVector``. It is an array of RNGs with length :math:`N`. It has
memory cost :math:`O(N)`. Many of the :ref:`sec-Counter-Based Random Number
Generators` have small state sizes and thus for moderate :math:`N`, this cost
is not significant. The method calls ``rng_set[i]`` and ``rng_set[j]`` return
independent RNGs if :math:`i \ne j`. If `Intel TBB`_ is available, there is a
third implementation, ``RNGSetTBB``, which uses thread-local storage (TLS). It
has much smaller memory footprint than ``RNGSetVector`` while maintains better
thread-safety. The type alias ``RNGSet`` is defined to be ``RNGSetTBB`` if \tbb
is available. Otherwise it is defined to be ``RNGSetVector``.

.. _sec-Uniform Bits Distribution:

Uniform Bits Distribution
=========================

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename UIntType>
    class UniformBitsDistribution;

    } // namespace mckl

is similar to the standard library’s ``std::independent_bits_engine``, except
that it always generates full size random integers and ``UIntType`` must have a
size of at least of 16 bits. That is, let :math:`W` be the number of bits of
``UIntType``, then the output is uniform on the set :math:`\{0,\dots,2^W -
1\}`. For example,

.. code-block:: cpp

    ::mckl::UniformBitsDistribution<uint32_t> ubits;
    ubits(rng); // Return 32-bit random integers

Let :math:`r_{\mathrm{min}}` and :math:`r_{\mathrm{max}}` be the minimum and
maximum of the random integers generated by ``rng``. Let :math:`R =
r_{\mathrm{max}} - r_{\mathrm{min}} + 1`. Let :math:`r_i` be consecutive output
of ``rng()``. If there exists an integer :math:`V > 0` such that :math:`R =
2^V`, then the result is,

.. math::

    U = \sum_{k = 0}^{K - 1} (r_k - r_{\mathrm{min}}) 2^{kV} \bmod 2^W

where :math:`K = \lceil W / V \rceil`. Unlike ``std::independent_bits_engine``,
the calculation can be vectorized, which leads to better performance. Note
that, all constants in the algorithm are computed at compile-time and the
summation is fully unrolled. There is no runtime overhead. In the case
:math:`r_{\mathrm{min}} = 0` and :math:`V = W`, most optimizing compilers shall
be able to generate instructions such that the distribution does exactly
nothing and returns the results of ``rng()`` directly. If there does not exist
an integer :math:`V > 0` such that :math:`R = 2^V`, then
``std::indepdent_bits_engine`` is used.

.. _sec-Standard Uniform Distribution:

Standard Uniform Distribution
=============================

MCKL provides five standard uniform distributions. They are all class templates
with a single template type parameter ``RealType``. The random integers
produced by RNGs are transferred to 32- or 64-bit random integers through the
class ``UniformBitsDistribution`` before they are mapped to floating point
numbers within the interval :math:`[0, 1]`. The integer type depends on
``RealType`` and the range of the RNG, :math:`R`. If :math:`\log_2 R \ge 64` or
``RealType`` is ``long double``, then the integer type is ``uint64_t``. If
``MCKL_U01_USE_64BITS_DOUBLE`` is true and ``RealType`` is ``double``, then the
integer type is also ``uint64_t``. Otherwise, the integer type is ``uint32_t``.

In the following, let :math:`W` be the number of bits of the integer type, and
:math:`M` be the number of significant bits (including the implicit one) of
``RealType``. We also denote the input random integers as :math:`U` and the
output random real numbers as :math:`X`. The type ``U01Distribution`` is
aliased to ``U01CanonicalDistribution`` if the ``MCKL_U01_USE_FIXED_POINT`` is
set to false (the default). Otherwise it is aliased to ``U01CODistribution``.

.. _sub-Canonical Form:

Canonical Form
--------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class U01CanonicalDistribution;

    } // namespace mckl

implements the uniform distribution on :math:`[0, 1)`. It is implemented
through the mapping,

.. math::

    P & = \lfloor (W + M - 1) / W \rfloor,\\
    K & = \max\{1, P\},\\
    X & = \sum_{k=0}^{K - 1} U_k 2^{-(K - k)W}

This is equivalent to the standard library ``std::generate_canonical``. The
minimum and maximum are :math:`0` and :math:`1 - 2^{-KW}`, respectively.

.. _sub-Closed-Closed Interval:

Closed-Closed Interval
----------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class U01CCDistribution;

    } // namespace mckl

implements the uniform distribution on :math:`[0, 1]` through the mapping,

.. math::

    P &= \min\{W - 1, M\},\\
    V &= \begin{cases}
      U & \text{if } P + 1 < W \\
      \lfloor (U \bmod 2^{W - 1}) / 2^{W - P -2} \rfloor & \text{otherwise}
    \end{cases},\\
    Z &= (V \bmod 2) + V,\\
    X &= 2^{-(P + 1)} Z.

The minimum and maximum are :math:`0` and :math:`1`, respectively.

.. _sub-Closed-Open Interval:

Closed-Open Interval
--------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class U01CODistribution;

    } // namespace mckl

implements the uniform distribution on :math:`[0, 1)` through the mapping,

.. math::

    P &= \min\{W, M\},\\
    V &= \lfloor U / 2^{W - P} \rfloor,\\
    X &= 2^{-P} V.

The minimum and maximum are :math:`0` and :math:`1 - 2^{-P}`, respectively.

.. _sub-Open-Closed Interval:

Open-Closed Interval
--------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class U01OCDistribution;

    } // namespace mckl

implements the uniform distribution on :math:`(0, 1]` through the mapping,

.. math::

    P &= \min\{W, M\},\\
    V &= \lfloor U / 2^{W - P} \rfloor,\\
    X &= 2^{-P} V + 2^{-P}.

The minimum and maximum are :math:`2^{-P}` and :math:`1`, respectively.

.. _sub-Open-Open Interval:

Open-Open Interval
------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class U01CODistribution;

    } // namespace mckl

implements the uniform distribution on :math:`(0, 1)` through the mapping,

.. math::

    P &= \min\{W + 1, M\},\\
    V &= \lfloor U / 2^{W + 1 - P} \rfloor,\\
    X &= 2^{-(P - 1)} V + 2^{-P}.

The minimum and maximum are :math:`2^{-P}` and :math:`1 - 2^{-P}`,
respectively.

.. _sub-Continuous Distribution:

Continuous Distribution
=======================

.. _sub-Arcsine Distribution:

Arcsine Distribution
--------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class ArcsineDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;\alpha,\beta) = \frac{1}{\pi\sqrt{(x - \alpha)(\beta - x)}},\\
    & x \in [a, b],\quad
      a \in (0, \infty),\quad
      b \in (0, \infty),

using the inverse method.

.. _sub-Beta Distribution:

Beta Distribution
-----------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class BetaDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;\alpha,\beta) =
      \frac{\Gamma(\alpha + \beta)}{\Gamma(\alpha)\Gamma(\beta)}
      x^{\alpha - 1}(1 - x)^{\beta - 1},\\
    & x \in (0, 1),\quad
      \alpha \in (0, \infty),\quad
      \beta \in (0, \infty).

The specific algorithm used depends on the parameters. If :math:`\alpha = 1/2`
and :math:`\beta = 1/2`, or :math:`\alpha = 1` or :math:`\beta = 1`, then the
inverse method is used. If :math:`\alpha > 1` and :math:`\beta > 1`, the method
in [Cheng1978]_ is used. Otherwise, let :math:`K = 0.852`, :math:`C = -0.956`,
and :math:`D = \beta + K\alpha^2 + C`. If :math:`\alpha < 1`, :math:`\beta < 1`
and :math:`D \le 0`, then Jöhnk’s method [Devroye1986]_ (sec. 3.5) is used. In
all other cases, one of the switching algorithms in [Atkinson1979]_ is used.

.. _sub-Cauchy Distribution:

Cauchy Distribution
-------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class CauchyDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;a,b) =
      \frac{1}{\pi b\Bigl(1 + \Bigl(\frac{x - a}{b}\Bigr)^2\Bigr)},\\
    & x \in \mathbb{R},\quad
      a \in \mathbb{R},\quad
      b \in (0,\infty),

using the inverse method.

.. _sub-_chi-Squared-Distribution:

:math:`\chi^2`-Distribution
---------------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class ChiSquaredDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;n) = \frac{x^{n/2 - 1}\mathrm{e}^{-x/2}}{2^{n/2}\Gamma(n/2)},\\
    & x \in (0, \infty),\quad
      n \in (0, \infty).

The implementation uses the fact that if :math:`X` is a Gamma random variable
with shape :math:`n / 2` and scale :math:`2`, then :math:`X` is also
:math:`\chi^2`-distributed with degree of freedom :math:`n`.

.. _sub-Exponential Distribution:

Exponential Distribution
------------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class ExponentialDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;\lambda) = \lambda\mathrm{e}^{-\lambda x},\\
    & x \in [0, \infty),\quad
      \lambda \in (0, \infty),

using the inverse method.

.. _sub-Extreme Value Distribution:

Extreme Value Distribution
--------------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class ExtremeValueDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;a,b) = \frac{1}{b}
    \exp\Bigl\{\frac{a - x}{b} - \exp\Bigl\{\frac{a - x}{b}\}\Bigr\},\\
    & x \in \mathbb{R},\quad
      a \in \mathbb{R},\quad
      b \in (0, \infty),

using the inverse method.

.. _sub-Fisher’s F-distribution:

Fisher’s *F*-distribution
-------------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class FisherFDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;m,n) =
      \frac{\Gamma\Bigl(\frac{m + n}{2}\Bigr)}
           {\Gamma\Bigl(\frac{m}{2}\Bigr)\Gamma\Bigl(\frac{n}{2}\Bigr)}
      \Bigl(\frac{m}{n}\Bigr)^{m/2} x^{m / 2 - 1}
      \Bigl(1 + \frac{m}{n}x\Bigr)^{-(m + n) / 2} \\
    & x \in [0, \infty),\quad
      m \in (0, \infty),\quad
      n \in (0, \infty).

The implementation uses the fact that if :math:`U` and :math:`V` are
:math:`\chi^2`-distributed random variables with degrees of freedom :math:`m`
and :math:`n`, respectively, and they are independent, then :math:`X = (U /
V)(m / n)` is a Fisher’s *F*-distributed random variable with the respective
degrees of freedom.

.. _sub-Gamma Distribution:

Gamma Distribution
------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class GammaDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;\alpha,\beta) =
      \frac{\mathrm{e}^{-x/\beta}}{\Gamma(\alpha)}
      \beta^{-\alpha}x^{\alpha-1},\\
    & x \in (0, \infty),\quad
      \alpha \in (0, \infty),\quad
      \beta \in (0, \infty).

The specific algorithm used depends on the parameters. If :math:`\alpha = 1`,
it becomes the exponential distribution. If :math:`0 < \alpha < 0.6`, it is
generated through transformation of exponential power distribution
[Devroye1986]_ (sec 2.6). If :math:`0.6\le\alpha<1`, then rejection method from
the Weibull distribution is used [Devroye1986]_ (sec. 3.4). If :math:`\alpha >
1`, then the method in [Marsaglia2000vq] is used.

.. _sub-Laplace Distribution:

Laplace Distribution
--------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class LaplaceDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;a,b) =
      \frac{1}{2b}\exp\Bigl\{-\frac{\lvert{x - a}\rvert}{b}\Bigr\},\\
    & x \in \mathbb{R},\quad
      b \in (0, \infty),

using the inverse method.

.. _sub-Levy Distribution:

Levy Distribution
-----------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class LevyDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;a,b) =
      \sqrt{\frac{b}{2\pi}}
      \frac{\exp\Bigl\{-\frac{b}{2(x - a)}\Bigr\}}{(x - a)^{3/2}},\\
    & x \in [a, \infty),\quad
      a \in \mathbb{R},\quad
      b \in (0, \infty).

The implementation uses the fact that if :math:`Z` is a standard Normal random
variable, then :math:`X = a + b / Z^2` is Levy distributed with location
:math:`a` and scale :math:`b`.

.. _sub-Logistic Distribution:

Logistic Distribution
---------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class LaplaceDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;a,b) = \frac{1}{4b}\mathrm{sech}^2\Bigl(\frac{x - a}{2b}\Bigr),\\
    & x \in \mathbb{R},\quad
      a \in \mathbb{R},\quad
      b \in (0, \infty),

using the inverse method.

.. _sub-Log-Normal Distribution:

Log-Normal Distribution
-----------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class LognormalDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;m,s) =
      \frac{1}{x\sigma\sqrt{2\pi}}
      \exp\Bigl\{-\frac{(\ln x - m)^2}{2\sigma^2}\Bigr\},\\
    & x \in (0, \infty),\quad
      m \in \mathbb{R},\quad
      s \in (0, \infty).

The implementation uses the fact that if :math:`Z` is a standard Normal random
variable, then :math:`X = \exp\{m + sZ\}` is Log-normal distributed with
location :math:`m` and scale :math:`s`.

.. _sub-Normal Distribution:

Normal Distribution
-------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class NormalDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;\mu,\sigma) =
      \frac{1}{\sqrt{2\pi\sigma^2}}
      \exp\Bigl\{-\frac{(x-\mu)^2}{2\sigma^2}\Bigr\},\\
    & x \in \mathbb{R},\quad
      \mu \in \mathbb{R},\quad
      \sigma \in (0, \infty),

using the Box-Muller method [Box1958]_.

.. _sub-Pareto Distribution:

Pareto Distribution
-------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class ParetoDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;a,b) = \frac{a b^a}{x^{a + 1}},\\
    & x \in [b, \infty),\quad
      a \in [0, \infty),\quad
      b \in [0, \infty),

using the inverse method.

.. _sub-Rayleigh Distribution:

Rayleigh Distribution
---------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class RayleighDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;\sigma) =
      \frac{x}{\sigma^2}\exp\Bigl\{-\frac{x^2}{2\sigma^2}\Bigr\},\\
    & x \in [0, \infty),\quad
      \sigma \in (0, \infty),

using the inverse method.

.. _sub-Stable Distribution:

Stable Distribution
-------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class StableDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;\alpha,\beta,a,b) =
      \frac{1}{2\pi}\int_{\infty}^{\infty}
      \varphi(t;\alpha,\beta,a,b)\mathrm{e}^{-ixt} \,dt\\
    & \varphi(t;\alpha,\beta,a,b) =
      \exp\{
        ita - \lvert{bt}\rvert^{\alpha}
        (1 - i\beta\mathrm{sgn}(t)\Phi(t;\alpha))\} \\
    & \Phi(t;\alpha) = \begin{cases}
        \tan\Bigl(\frac{\pi}{2}\alpha\Bigr) & \alpha \ne 1 \\
        -\frac{2}{\pi}\log\lvert{t}\rvert   & \alpha = 1
      \end{cases}, \\
    & x \in \mathbb{R},\quad
      \alpha \in (0, 2],\quad \beta \in [-1, 1],\quad
      a \in \mathbb{R},\quad b \in (0, \infty).

The implementation uses the method in [Chambers1976]_.

.. _sub-Student’s t-Distribution:

Student’s *t*-Distribution
--------------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class StudentTDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;n) =
      \frac{\Gamma\Bigl(\frac{n + 1}{2}\Bigr)}
      {\sqrt{n\pi}\Gamma\Bigl(\frac{n}{2}\Bigr)}
      \Bigl(1 + \frac{x^2}{n}\Bigr)^{-(n + 1)/2},\\
    & x \in \mathbb{R},\quad n \in (0, \infty).

The implementation uses the fact that if :math:`Z` is a standard Normal random
variable, :math:`V` is a :math:`\chi^2`-distributed random variable with degree
of freedom :math:`n`, and they are independent, then :math:`X = Z/\sqrt{V / n}`
is Student’s *t*-distributed with the respective degree of freedom.

.. _sub-Uniform Real Distribution:

Uniform Real Distribution
-------------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class UniformRealDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;a,b) = \frac{1}{b - a},\\
    & x \in [a, b),\quad a \in \mathbb{R},\quad b \in (a, \infty),

using the inverse method.

.. _sub-Weibull Distribution:

Weibull Distribution
--------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double>
    class WeibullDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x;a,b) = \frac{a}{b}\Bigl(\frac{x}{b}\Bigr)^{a - 1}
      \exp\Bigl\{-\Bigl(\frac{x}{b}\Bigr)^a\Bigr\},\\
    & x \in [0, \infty),\quad a \in (0, \infty),\quad b \in (0, \infty),

using the inverse method.

.. _sub-Discrete Distribution:

Discrete Distribution
=====================

.. _sub-Bernoulli Distribution:

Bernoulli Distribution
----------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename IntType = bool>
    class BernoulliDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & \mathbb{P}(X = k;p) = kp + (1 - k)(1 - p),\\
    & k \in \{0, 1\},\quad p \in [0, 1].

Unlike other discrete distributions, the Bernoulli distribution supports any
integer type, while others require an integer type with size larger than 16
bits. The implementation uses the simple fact that if :math:`U` is a standard
uniform random variable, than :math:`\mathbb{I}_{[0,p)}(U)` is Bernoulli
distributed with success probability :math:`p`. This is not a drop-in
replacement for ``std::bernoulli_distribution``, which is not a class template.

.. _sub-Geometric Distribution:

Geometric Distribution
----------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename IntType = int>
    class GeometricDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & \mathbb{P}(X = k;p) = p(1-p)^k,\\
    & k \in \mathrm{N},\quad
      p \in (0, 1].

The implementation uses the fact that if :math:`U` is a standard uniform random
variable, then :math:`X = \lfloor{\ln U / \ln(1-p)}\rfloor` is a Geometric
random variable with success probability :math:`p`.

.. _sub-Uniform Integer Distribution:

Uniform Integer Distribution
----------------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename IntType = int>
    class UniformIntDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & \mathbb{P}(X = k;a,b) = \frac{1}{b - a + 1},\\
    & k \in \{a,\dots,b\},\quad
      a \in \mathbb{Z},\quad
      b \in \{x \in \mathbb{Z} \mid x \ge a\}.

The specific algorithm used depends on the parameters. If :math:`a = b`, then
it simply returns :math:`a`. If :math:`b - a + 1 = 2^W`, where :math:`W` is the
number of bits of ``IntType``, then ``UniformBitsDistribution`` is used (see
:ref:`sec-Uniform Bits Distribution`). If :math:`\max\{\lvert{a}\rvert, b\} <
2^{32}`, then it uses the fact that if :math:`U` is a standard uniform random
variable, then :math:`X = \lfloor{a + (b - a + 1) U}\rfloor` is uniform on the
set :math:`\{a,\dots,b\}`. Otherwise the standard library is used.

.. _sub-Multivariate Distribution:

Multivariate Distribution
=========================

.. _sub-Dirichlet Distribution:

Dirichlet Distribution
----------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double, size_t Dim = Dynamic>
    class DirichletDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x_{1:d};\alpha_{1:d}) =
      \frac{\Gamma\Bigl(\sum_{i=1}^d\alpha_i\Bigr)}
           {\prod_{i=1}^d\Gamma(\alpha_i)}
      \prod_{i=1}^d x_i^{\alpha_i - 1},\\
    & \sum_{i=1}^d x_i = 1,\quad
      x_{1:d}\in(0,1)^d,\quad
      \alpha_{1:d}\in(0,\infty)^d.

The template parameter ``Dim`` is the dimension of the distribution. If it is
positive, then the dimension is fixed. The distribution generator can be
constructed by,

.. code-block:: cpp

    ::mckl::DirichletDistribution<double, Dim> dirichlet(alpha);

and if it is zero (recall that ``Dynamic`` is just an enumerator with value
zero), then the dimension has to be specified at runtime. The distribution
generator can be constructed by,

.. code-block:: cpp

    ::mckl::DirichletDistribution<double> dirichlet(dim, alpha);

The parameter ``alpha`` can be either a pointer to a :math:`d`-vector or a
scalar. If it is a scalar, say :math:`\alpha`, then :math:`\alpha_i = \alpha`
for :math:`i = 1,\dots,d`. To generate one random variate,

.. code-block:: cpp

    dirichlet(rng, r);
    ::mckl::rand(rng, dirichlet, r);

where the output parameter ``r`` is a pointer to a :math:`d`-vector. Vectorized
generating is also possible,

.. code-block:: cpp

    dirichlet(rng, n, r);
    ::mckl::rand(rng, dirichlet, n, r);

where the output parameter ``r`` is a pointer to an :math:`n \times d` matrix
of row major order.

.. _sub-Multivariate Normal Distribution:

Multivariate Normal Distribution
--------------------------------

The class template,

.. code-block:: cpp

    namespace mckl {

    template <typename RealType = double, size_t Dim = Dynamic>
    class NormalMVDistribution;

    } // namespace mckl

implements the distribution with PDF,

.. math::

    & f(x_{1:d};\mu_{1:d},\Sigma) = \frac{1}{\sqrt{(2\pi)^d\lvert{\Sigma}\rvert}}
    \exp\Bigl\{-\frac{1}{2}(x_{1:d} -
      \mu_{1:d})^{\mathrm{T}}\Sigma^{-1}(x_{1:d} - \mu_{1:d})\Bigr\},\\
    & x_{1:d}\in\mathbb{R}^d,\quad
      \mu_{1:d}\in\mathbb{R}^d,\quad
      \Sigma\in
      \{\text{positive semi-definite }d \times d\text{ matrix}\}.

At the time of writing, only ``float`` and ``double`` are supported types for
the template parameter ``RealType``. The second template parameter ``Dim``
specify the dimension of the distribution, :math:`d`. If ``Dim`` is positive,
then the dimension is fixed. The distribution generator can be constructed by,

.. code-block:: cpp

    ::mckl::NormalMVDistribution<double, Dim> normal_mv(mean, chol);

Otherwise, if ``Dim`` is zero, the dimension has to be specified at runtime.
The distribution generator can be constructed by,

.. code-block:: cpp

    ::mckl::NormalMVDistribution<double> normal_mv(d, mean, chol);

In either case, the parameter ``mean`` is a pointer to the mean vector of
length :math:`d`, and ``chol`` is a pointer to the lower triangular of the
Cholesky decomposition of the covariance matrix packed row by row. For those
unfamiliar with matrix storage schemes, this means that, ``chol`` is a vector
of length :math:`d(d + 1) / 2`. Let :math:`L` be the lower triangular of the
Cholesky decomposition, that is :math:`LL^{\mathrm{T}} = \Sigma`, then the
vector is :math:`(L_{1,1},L_{2,1},L_{2,2},\dots,L_{d,d})^{\mathrm{T}}`.
Further, both ``mean`` and ``chol`` can also be scalars instead of pointers to
vectors. If ``mean`` is a scalar, say :math:`\mu`, then the mean vector is
assumed to be a :math:`d`-vector with all elements equal to :math:`\mu`. If
``chol`` is a scalar, say :math:`\sigma`, then it is assumed that :math:`L =
\sigma I_d` and thus the covariance matrix is :math:`\Sigma = \sigma^2 I_d`,
where :math:`I_d` is the identity matrix. To generate a single multivariate
Normal random number,

.. code-block:: cpp

    normal_mv(rng, r);
    ::mckl::rand(rng, normal_mv, r);

where the output parameter ``r`` is a pointer to a :math:`d`-vector. Vectorized
generating is also possible,

.. code-block:: cpp

    normal_mv(rng, n, r);
    ::mckl::rand(rng, normal_mv, n, r);

where the output parameter ``r`` is a pointer to an :math:`n \times d` matrix
of row major order.

.. _Intel TBB:
    https://www.threadingbuildingblocks.org

.. _Intel MKL:
    https://software.intel.com/en-us/intel-mkl/

.. [Atkinson1979]
    Atkinson, A.C. (1979). “A family of switching algorithms for the computer
    generation of beta random variables.” Biometrika, 66(1), 141–145.

.. [Box1958]
    Box, G.E.P., & Muller, M.E. (1958). “A note on the generation of random
    Normal deviates.” The Annals of Mathematical Statistics, 29(2), 610–611.

.. [Chambers1976]
    Chambers, J.M., Mallows, C.L., & Stuck, B.W. (1976). “A method for
    simulating stable random variables.” Journal of the American Statistical
    Association, 71(354), 340–344.

.. [Cheng1978]
    Cheng, R.C.H. (1978). “Generating Beta variates with nonintegral shape
    parameters.” Communications of the ACM, 21(4), 317–322.

.. [Devroye1986]
    Devroye, L. (1986). Non-Uniform Random Variate Generation. New York, NY:
    Springer New York.

.. [Salmon2011]
    Salmon, J.K., Moraes, M.A., Dror, R.O., & Shaw, D.E. (2011). “Parallel
    random numbers: As easy as 1, 2, 3.” Proceedings of 2011 International
    Conference for High Performance Computing, Networking, Storage and
    Analysis, 1–12.
