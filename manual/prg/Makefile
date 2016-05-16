.PHONY : all clean libmckl

CC := clang
CFLAGS := $(CFLAGS) -O3 -DNDEBUG
CFLAGS := $(CFLAGS) -I ../../include
CFLAGS := $(CFLAGS) -L ../../build/clang-Release/lib
CFLAGS := $(CFLAGS) -Wl,-rpath -Wl,../../build/clang-Release/lib

CXX := clang++
CXXFLAGS := $(CXXFLAGS) -std=c++14 -O3 -DNDEBUG
CXXFLAGS := $(CXXFLAGS) -I ../../include
CXXFLAGS := $(CXXFLAGS) -DMCKL_HAS_MKL
CXXFLAGS := $(CXXFLAGS) -DMCKL_HAS_TBB

LDFLAGS := $(LDFLAGS) -lmkl_rt -ltbb -ltbbmalloc -lm

all : gmm pf_seq pf_tbb

libmckl :
	ninja -C ../../build/clang-Release libmckl_shared

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
