# ============================================================================
#  MCKL/manual/prg/Makefile
# ----------------------------------------------------------------------------
#  MCKL: Monte Carlo Kernel Library
# ----------------------------------------------------------------------------
#  Copyright (c) 2013-2016, Yan Zhou
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#    Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
# ============================================================================

CFLAGS := $(CFLAGS) -std=c99 -O3 -DNDEBUG
CFLAGS := $(CFLAGS) -I ../../include
CFLAGS := $(CFLAGS) -L ../../build/llvm-debug-sys/lib
CFLAGS := $(CFLAGS) -Wl,-rpath -Wl,../../build/llvm-debug-sys/lib

CXXFLAGS := $(CXXFLAGS) -std=c++11 -O3 -DNDEBUG
CXXFLAGS := $(CXXFLAGS) -I ../../include
CXXFLAGS := $(CXXFLAGS) -DMCKL_HAS_MKL
CXXFLAGS := $(CXXFLAGS) -DMCKL_HAS_TBB

LDFLAGS := $(LDFLAGS) -lmkl_rt -ltbb -ltbbmalloc -lm

all : gmm pf_seq pf_tbb

libmckl :
	ninja -C ../../build/llvm-debug-sys libmckl_shared

gmm : gmm.cpp
	$(CXX) $(CXXFLAGS) -o gmm gmm.cpp $(LDFLAGS)

pf.pdf : pf_tbb pf.R
	./pf_tbb
	Rscript pf.R

pf_seq : pf_seq.c libmckl
	$(CC) $(CFLAGS) -o pf_seq pf_seq.c -lmckl $(LDFLAGS)

pf_tbb : pf_tbb.c libmckl
	$(CC) $(CFLAGS) -o pf_tbb pf_tbb.c -lmckl $(LDFLAGS)

clean:
	rm -f gmm
	rm -f pf_seq pf_tbb pf.out pf.rout pf.pdf
