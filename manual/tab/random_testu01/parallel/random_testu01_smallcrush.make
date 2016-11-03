.PHONY : all run \
	random_testu01_smallcrush_ars \
	random_testu01_smallcrush_ars_64 \
	random_testu01_smallcrush_minstd_rand0 \
	random_testu01_smallcrush_mkl_mcg59 \
	random_testu01_smallcrush_mkl_mcg59_64 \
	random_testu01_smallcrush_philox2x64 \
	random_testu01_smallcrush_ranlux24_base \
	random_testu01_smallcrush_ranlux48_base \
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
	random_testu01_smallcrush_threefish512 \
	random_testu01_smallcrush_threefry2x64_64

random_testu01_smallcrush_ars :
	ninja -C ../.. random_testu01_smallcrush_ars
	./random_testu01_smallcrush_ars U01 parallel 2

random_testu01_smallcrush_ars_64 :
	ninja -C ../.. random_testu01_smallcrush_ars_64
	./random_testu01_smallcrush_ars_64 U01 parallel 2

random_testu01_smallcrush_minstd_rand0 :
	ninja -C ../.. random_testu01_smallcrush_minstd_rand0
	./random_testu01_smallcrush_minstd_rand0 U01CC parallel 3
	./random_testu01_smallcrush_minstd_rand0 U01OO parallel 8

random_testu01_smallcrush_mkl_mcg59 :
	ninja -C ../.. random_testu01_smallcrush_mkl_mcg59
	./random_testu01_smallcrush_mkl_mcg59 U01 parallel 1

random_testu01_smallcrush_mkl_mcg59_64 :
	ninja -C ../.. random_testu01_smallcrush_mkl_mcg59_64
	./random_testu01_smallcrush_mkl_mcg59_64 U01 parallel 1
	./random_testu01_smallcrush_mkl_mcg59_64 U01CO parallel 1

random_testu01_smallcrush_philox2x64 :
	ninja -C ../.. random_testu01_smallcrush_philox2x64
	./random_testu01_smallcrush_philox2x64 U01CC parallel 7

random_testu01_smallcrush_ranlux24_base :
	ninja -C ../.. random_testu01_smallcrush_ranlux24_base
	./random_testu01_smallcrush_ranlux24_base U01OC parallel 5 9
	./random_testu01_smallcrush_ranlux24_base U01OO parallel 3 5

random_testu01_smallcrush_ranlux48_base :
	ninja -C ../.. random_testu01_smallcrush_ranlux48_base
	./random_testu01_smallcrush_ranlux48_base STD parallel 10
	./random_testu01_smallcrush_ranlux48_base U01CC parallel 9
	./random_testu01_smallcrush_ranlux48_base U01CO parallel 3

random_testu01_smallcrush_threefish512 :
	ninja -C ../.. random_testu01_smallcrush_threefish512
	./random_testu01_smallcrush_threefish512 U01CO parallel 10

random_testu01_smallcrush_threefry2x64_64 :
	ninja -C ../.. random_testu01_smallcrush_threefry2x64_64
	./random_testu01_smallcrush_threefry2x64_64 U01CO parallel 7

# vim:ft=make
