.PHONY : all run \
	random_testu01_smallcrush_ars_64_U01 \
	random_testu01_smallcrush_ars_U01 \
	random_testu01_smallcrush_minstd_rand0_U01CC \
	random_testu01_smallcrush_minstd_rand0_U01OO \
	random_testu01_smallcrush_mkl_mcg59_64_U01 \
	random_testu01_smallcrush_mkl_mcg59_64_U01CO \
	random_testu01_smallcrush_mkl_mcg59_U01 \
	random_testu01_smallcrush_philox2x64_U01CC \
	random_testu01_smallcrush_ranlux24_base_U01OC \
	random_testu01_smallcrush_ranlux24_base_U01OO \
	random_testu01_smallcrush_ranlux48_base_STD \
	random_testu01_smallcrush_ranlux48_base_U01CC \
	random_testu01_smallcrush_ranlux48_base_U01CO \
	random_testu01_smallcrush_rdrand64_U01OC \
	random_testu01_smallcrush_threefish512_U01CO \
	random_testu01_smallcrush_threefry2x64_64_U01CO
all :

run : \
	random_testu01_smallcrush_ars_64_U01 \
	random_testu01_smallcrush_ars_U01 \
	random_testu01_smallcrush_minstd_rand0_U01CC \
	random_testu01_smallcrush_minstd_rand0_U01OO \
	random_testu01_smallcrush_mkl_mcg59_64_U01 \
	random_testu01_smallcrush_mkl_mcg59_64_U01CO \
	random_testu01_smallcrush_mkl_mcg59_U01 \
	random_testu01_smallcrush_philox2x64_U01CC \
	random_testu01_smallcrush_ranlux24_base_U01OC \
	random_testu01_smallcrush_ranlux24_base_U01OO \
	random_testu01_smallcrush_ranlux48_base_STD \
	random_testu01_smallcrush_ranlux48_base_U01CC \
	random_testu01_smallcrush_ranlux48_base_U01CO \
	random_testu01_smallcrush_rdrand64_U01OC \
	random_testu01_smallcrush_threefish512_U01CO \
	random_testu01_smallcrush_threefry2x64_64_U01CO

random_testu01_smallcrush_ars_64_U01 :
	ninja -C ../.. random_testu01_smallcrush_ars_64
	./random_testu01_smallcrush_ars_64 U01 2

random_testu01_smallcrush_ars_U01 :
	ninja -C ../.. random_testu01_smallcrush_ars
	./random_testu01_smallcrush_ars U01 2

random_testu01_smallcrush_minstd_rand0_U01CC :
	ninja -C ../.. random_testu01_smallcrush_minstd_rand0
	./random_testu01_smallcrush_minstd_rand0 U01CC 3

random_testu01_smallcrush_minstd_rand0_U01OO :
	ninja -C ../.. random_testu01_smallcrush_minstd_rand0
	./random_testu01_smallcrush_minstd_rand0 U01OO 8

random_testu01_smallcrush_mkl_mcg59_64_U01 :
	ninja -C ../.. random_testu01_smallcrush_mkl_mcg59_64
	./random_testu01_smallcrush_mkl_mcg59_64 U01 1

random_testu01_smallcrush_mkl_mcg59_64_U01CO :
	ninja -C ../.. random_testu01_smallcrush_mkl_mcg59_64
	./random_testu01_smallcrush_mkl_mcg59_64 U01CO 1

random_testu01_smallcrush_mkl_mcg59_U01 :
	ninja -C ../.. random_testu01_smallcrush_mkl_mcg59
	./random_testu01_smallcrush_mkl_mcg59 U01 1

random_testu01_smallcrush_philox2x64_U01CC :
	ninja -C ../.. random_testu01_smallcrush_philox2x64
	./random_testu01_smallcrush_philox2x64 U01CC 7

random_testu01_smallcrush_ranlux24_base_U01OC :
	ninja -C ../.. random_testu01_smallcrush_ranlux24_base
	./random_testu01_smallcrush_ranlux24_base U01OC 9 5

random_testu01_smallcrush_ranlux24_base_U01OO :
	ninja -C ../.. random_testu01_smallcrush_ranlux24_base
	./random_testu01_smallcrush_ranlux24_base U01OO 5 3

random_testu01_smallcrush_ranlux48_base_STD :
	ninja -C ../.. random_testu01_smallcrush_ranlux48_base
	./random_testu01_smallcrush_ranlux48_base STD 10

random_testu01_smallcrush_ranlux48_base_U01CC :
	ninja -C ../.. random_testu01_smallcrush_ranlux48_base
	./random_testu01_smallcrush_ranlux48_base U01CC 9

random_testu01_smallcrush_ranlux48_base_U01CO :
	ninja -C ../.. random_testu01_smallcrush_ranlux48_base
	./random_testu01_smallcrush_ranlux48_base U01CO 3

random_testu01_smallcrush_rdrand64_U01OC :
	ninja -C ../.. random_testu01_smallcrush_rdrand64
	./random_testu01_smallcrush_rdrand64 U01OC 3

random_testu01_smallcrush_threefish512_U01CO :
	ninja -C ../.. random_testu01_smallcrush_threefish512
	./random_testu01_smallcrush_threefish512 U01CO 10

random_testu01_smallcrush_threefry2x64_64_U01CO :
	ninja -C ../.. random_testu01_smallcrush_threefry2x64_64
	./random_testu01_smallcrush_threefry2x64_64 U01CO 7

# vim:ft=make
