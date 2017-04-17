.. ============================================================================
..  MCKL/docs/intro.rst
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

.. _chap-Introduction:

************
Introduction
************

MCKL is a header-only library. All headers files are under the ``mckl``
directory. To include all functionalities,

.. code-block:: cpp

    #include <mckl/mckl.hpp>

There are a few other headers that include a subset of functionalities of MCKL,
each documented in a subsequent chapter in this manual.

.. _sec-Optional Runtime Library:

Optional Runtime Library
========================

In addition to the header-only library, one can optionally build and use a
runtime library. To build the library, one need a recent version of the `NASM
<http://www.nasm.us>`_ assembler and the `CMake <https://cmake.org>`_ program.
The library is only supported on x86-64 CPUs and 64-bit Unix and compatible
systems. To build and install the library,

.. code-block:: sh

    cd /path/to/mckl/source
    mkdir -p build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=/prefix/to/install/directory
    make lib
    make install

To use and link to the library in a user program, say ``prog.cpp``,

.. code-block:: sh

    c++ -DMCKL_USE_ASM_LIB=1 -o prog prog.cpp -lmckl

The runtime library provides optimized implementation of selected vectorized
random number generators (See :ref:`sec-Vectorized Random Number Generating`
and :ref:`sec-Counter-Based Random Number Generators`). Performance of the
header-only implementation might vary considerably depending on the compiler.

Further, one can set the configuration macro ``MCKL_USE_ASM_VMF`` to use an
assembly implementation of selected vectorized math functions when `Intel MKL`_
is unavailable (see :ref:`sec-Vectorized Functions`). This feature is highly
experimental. These functions are fast, but have slightly lower accuracy for
some values of input than the standard library or `Intel MKL`_ in high accuracy
mode.

.. _Intel MKL:
    https://software.intel.com/en-us/intel-mkl/
