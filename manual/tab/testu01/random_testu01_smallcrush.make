.PHONY : all run \
	random_testu01_smallcrush_ars \
	random_testu01_smallcrush_ars_64 \
	random_testu01_smallcrush_minstd_rand0 \
	random_testu01_smallcrush_mkl_mcg59 \
	random_testu01_smallcrush_mkl_mcg59_64 \
	random_testu01_smallcrush_philox2x64 \
	random_testu01_smallcrush_ranlux24_base \
	random_testu01_smallcrush_ranlux48_base \
	random_testu01_smallcrush_rdrand64 \
	random_testu01_smallcrush_threefish512 \
	random_testu01_smallcrush_threefry2x64_64
all :

run : \
	random_testu01_smallcrush_ars \
	random_testu01_smallcrush_ars_64 \
	random_testu01_smallcrush_minstd_rand0 \
	random_testu01_smallcrush_mkl_mcg59 \
	random_testu01_smallcrush_mkl_mcg59_64 \
	random_testu01_smallcrush_philox2x64 \
	random_testu01_smallcrush_ranlux24_base \
	random_testu01_smallcrush_ranlux48_base \
	random_testu01_smallcrush_rdrand64 \
	random_testu01_smallcrush_threefish512 \
	random_testu01_smallcrush_threefry2x64_64

random_testu01_smallcrush_ars :
	ninja -C ../.. random_testu01_smallcrush_ars
	./random_testu01_smallcrush_ars U01 2

random_testu01_smallcrush_ars_64 :
	ninja -C ../.. random_testu01_smallcrush_ars_64
	./random_testu01_smallcrush_ars_64 U01 2

random_testu01_smallcrush_minstd_rand0 :
	ninja -C ../.. random_testu01_smallcrush_minstd_rand0
	./random_testu01_smallcrush_minstd_rand0 U01CC 3
	./random_testu01_smallcrush_minstd_rand0 U01OO 8

random_testu01_smallcrush_mkl_mcg59 :
	ninja -C ../.. random_testu01_smallcrush_mkl_mcg59
	./random_testu01_smallcrush_mkl_mcg59 U01 1

random_testu01_smallcrush_mkl_mcg59_64 :
	ninja -C ../.. random_testu01_smallcrush_mkl_mcg59_64
	./random_testu01_smallcrush_mkl_mcg59_64 U01 1
	./random_testu01_smallcrush_mkl_mcg59_64 U01CO 1

random_testu01_smallcrush_philox2x64 :
	ninja -C ../.. random_testu01_smallcrush_philox2x64
	./random_testu01_smallcrush_philox2x64 U01CC 7

random_testu01_smallcrush_ranlux24_base :
	ninja -C ../.. random_testu01_smallcrush_ranlux24_base
	./random_testu01_smallcrush_ranlux24_base U01OC 5 9
	./random_testu01_smallcrush_ranlux24_base U01OO 3 5

random_testu01_smallcrush_ranlux48_base :
	ninja -C ../.. random_testu01_smallcrush_ranlux48_base
	./random_testu01_smallcrush_ranlux48_base STD 10
	./random_testu01_smallcrush_ranlux48_base U01CC 9
	./random_testu01_smallcrush_ranlux48_base U01CO 3

random_testu01_smallcrush_rdrand64 :
	ninja -C ../.. random_testu01_smallcrush_rdrand64
	./random_testu01_smallcrush_rdrand64 U01OC 3

random_testu01_smallcrush_threefish512 :
	ninja -C ../.. random_testu01_smallcrush_threefish512
	./random_testu01_smallcrush_threefish512 U01CO 10

random_testu01_smallcrush_threefry2x64_64 :
	ninja -C ../.. random_testu01_smallcrush_threefry2x64_64
	./random_testu01_smallcrush_threefry2x64_64 U01CO 7

# vim:ft=make
