* Require C++14

* `Particle::index` renamed to `at` and `operator[]`. The old member function
  is marked `[[deprecated]]` and will be removed in future releases.

* `StateMatrix::particle_index_type` gain a new member `operator[]`.

* `ParticleRange::first` renamed to `ibegin`, `ParticleRange::last` renamed to
  `iend`. The old member functions are marked `[[deprecated]]` and will be
  removed in future releases.
