* Require C++14

* `Particle::index` renamed to `at` and `operator[]`. The old member function
  is marked `[[deprecated]]` and will be removed in the next major releases.

* `StateMatrix::particle_index_type` gain a new member `operator[]`.

* `ParticleRange::first` renamed to `ibegin`, `ParticleRange::last` renamed to
  `iend`. The old member functions are marked `[[deprecated]]` and will be
  removed in the next major releases.

* New assembly implementation of RNGs: `ARS`, `AES128`, `AES192`, `AES256`,
  `Philox2x32` and `Philox4x32`, and their 64-bit versions. They provide
  compiler independent performance. See README for information of how to build
  and use the runtime library

* New assembly implementation of vectorized `exp`, `exp2`, `expm1`, `log`,
  `log2`, `log10`, `log1p`, `sin`, `cos`, `sincos` and `tan. These are
  experimental, with lower accuracy. They are suitable for vectorized random
  number generating and situations where a couple ULP errors are non-critical.
  See README for information of how to use them.

* New `muladd`, `fmadd` and related vector math functions. The former only does
  fused operation when FMA3 is supported and the assembly library is used. The
  later always does fused operation. The old `fma` functions are marked
  `[[deprecated]]` and will be removed in the next major release.
