.PHONY : all run \
	random_testu01_smallcrush_minstd_rand \
	random_testu01_smallcrush_philox2x32 \
	random_testu01_smallcrush_ranlux24_base \
	random_testu01_smallcrush_ranlux48_base \
	random_testu01_smallcrush_threefish256 \
	random_testu01_smallcrush_threefish256_64 \
	random_testu01_smallcrush_threefish512_64 \
	random_testu01_smallcrush_threefry16x64 \
	random_testu01_smallcrush_threefry4x32 \
	random_testu01_smallcrush_threefry4x32_64 \
	random_testu01_smallcrush_threefry4x64 \
	random_testu01_smallcrush_threefry4x64_64
all :

run : \
	random_testu01_smallcrush_minstd_rand \
	random_testu01_smallcrush_philox2x32 \
	random_testu01_smallcrush_ranlux24_base \
	random_testu01_smallcrush_ranlux48_base \
	random_testu01_smallcrush_threefish256 \
	random_testu01_smallcrush_threefish256_64 \
	random_testu01_smallcrush_threefish512_64 \
	random_testu01_smallcrush_threefry16x64 \
	random_testu01_smallcrush_threefry4x32 \
	random_testu01_smallcrush_threefry4x32_64 \
	random_testu01_smallcrush_threefry4x64 \
	random_testu01_smallcrush_threefry4x64_64

random_testu01_smallcrush_minstd_rand :
	ninja -C ../.. random_testu01_smallcrush_minstd_rand
	./random_testu01_smallcrush_minstd_rand U01CC 4

random_testu01_smallcrush_philox2x32 :
	ninja -C ../.. random_testu01_smallcrush_philox2x32
	./random_testu01_smallcrush_philox2x32 U01OC 2

random_testu01_smallcrush_ranlux24_base :
	ninja -C ../.. random_testu01_smallcrush_ranlux24_base
	./random_testu01_smallcrush_ranlux24_base U01OO 3

random_testu01_smallcrush_ranlux48_base :
	ninja -C ../.. random_testu01_smallcrush_ranlux48_base
	./random_testu01_smallcrush_ranlux48_base U01CC 5
	./random_testu01_smallcrush_ranlux48_base U01CO 3 5
	./random_testu01_smallcrush_ranlux48_base U01OO 3

random_testu01_smallcrush_threefish256 :
	ninja -C ../.. random_testu01_smallcrush_threefish256
	./random_testu01_smallcrush_threefish256 STD 10

random_testu01_smallcrush_threefish256_64 :
	ninja -C ../.. random_testu01_smallcrush_threefish256_64
	./random_testu01_smallcrush_threefish256_64 STD 10

random_testu01_smallcrush_threefish512_64 :
	ninja -C ../.. random_testu01_smallcrush_threefish512_64
	./random_testu01_smallcrush_threefish512_64 U01CC 3

random_testu01_smallcrush_threefry16x64 :
	ninja -C ../.. random_testu01_smallcrush_threefry16x64
	./random_testu01_smallcrush_threefry16x64 U01OC 3

random_testu01_smallcrush_threefry4x32 :
	ninja -C ../.. random_testu01_smallcrush_threefry4x32
	./random_testu01_smallcrush_threefry4x32 U01 10

random_testu01_smallcrush_threefry4x32_64 :
	ninja -C ../.. random_testu01_smallcrush_threefry4x32_64
	./random_testu01_smallcrush_threefry4x32_64 U01 10

random_testu01_smallcrush_threefry4x64 :
	ninja -C ../.. random_testu01_smallcrush_threefry4x64
	./random_testu01_smallcrush_threefry4x64 STD 10

random_testu01_smallcrush_threefry4x64_64 :
	ninja -C ../.. random_testu01_smallcrush_threefry4x64_64
	./random_testu01_smallcrush_threefry4x64_64 STD 10

# vim:ft=make
