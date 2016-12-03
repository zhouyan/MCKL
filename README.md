# Introduction

The MCKL library provides a framework for implementing Monte Carlo algorithms.
It was originally developed for the purpose of parallel implementation of SMC
algorithms. But it now supports other algorithms as well. The library provides
basic building blocks for implementing parallelizable Monte Carlo algorithms.

# Installation

This is a header only template C++ library. To install the library just move
the contents of the `include` directory into a proper place, e.g.,
`/usr/local/include` on Unix-alike systems.

# Documentation

[Doxygen][doxygen] generated reference manuals for the
[master][MCKLDoxygenMaster] and [develop][MCKLDoxygenDevelop] branches, as well
as for individual releases can be found online (see [release
notes][MCKLReleases] for links). A [Manual][MCKLManualMaster] is also provided.

# Third-party dependencies

This library requires a working BLAS implementation. Some of the library's
functionalities can only be used if a optional dependencies are present.
Notably, [HDF5][hdf5], [TBB][tbb], [OpenMP][omp] and [MKL][mkl]. One can tell
the library that these optional features are available by defining
configuration macros such as `-DMCKL_HAS_HDF5=1` during compilation.

# Optional runtime library

In addition to the header-only library, one can optionally build and use a
runtime library. To build the library, one need a recent version of
[NASM][NASM] assembler and the [CMake][CMake] program. The library is only
supported on x86-64 CPUs and 64-bit Unix-alike systems.

To build and install the library,
```
cd /path/to/mckl/source
mkdir -p build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/prefix/to/install/directory
make lib
make install
```
To use and link to the library in a user program, say `prog.cpp`,
```
c++ -DMCKL_USE_ASM_LIB=1 -o prog prog.cpp -lmckl
```

The runtime library provides optimized implementation of selected RNGs
Performance of the header-only implementation might vary considerably depending
on the compiler. Though even the worst case, the performance is still better
than most alternatives, the best performance of some RNGs can provide best
performance regardless of the choice of compiler.

Further, one can define a configuration macro `MCKL_USE_ASM_VMF` to a non-zero
value, to use an assembly implementation of selected vectorized math functions
when MKL VML is unavailable. This feature is highly experimental. These
functions are fast, but have slightly lower accuracy for some values of input
than the standard library or MKL VML in high accuracy mode.

# Compiler support

This library makes heavy use of some template metaprogramming techniques. It
requires a C++14 standard conforming compiler.

This library has been regularly tested with recent [Clang][clang], [GCC][gcc]
and [Intel C++ Compiler][icpc] in C++14 mode.

Other compilers might work but are not tested. Complete C++14 implementation is
required with thread-local storage as an exception.

# Examples

Examples are in the `example` subdirectory, to build them,
```
export CXXFLAGS="-std=c++11"
cd /path/to/mckl/source
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make example
```
Some examples may only be built if optional dependencies are present.

# License

The MCKL library is distributed with a 2-clause BSD license which can be found
in the `LICENSE` file distributed with the source.

[clang]: http://clang.llvm.org
[CMake]: http://www.cmake.org
[doxygen]: http://www.stack.nl/~dimitri/doxygen
[gcc]: http://gcc.gnu.org
[NASM]: http://nasm.us
[hdf5]: http://www.hdfgroup.org
[icpc]: http://software.intel.com/en-us/intel-compilers
[mkl]: https://software.intel.com/en-us/intel-mkl
[omp]: http://www.openmp.org
[tbb]: http://threadingbuildingblocks.org
[MCKLDoxygenDevelop]: http://zhouyan.github.io/MCKLDoc/develop
[MCKLDoxygenMaster]: http://zhouyan.github.io/MCKLDoc/master
[MCKLManualDevelop]: http://zhouyan.github.io/MCKLDoc/develop/manual.pdf
[MCKLManualMaster]: http://zhouyan.github.io/MCKLDoc/master/manual.pdf
[MCKLReleases]: https://github.com/zhouyan/MCKL/releases
