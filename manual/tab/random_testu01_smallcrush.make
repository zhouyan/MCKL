.PHONY : all run

all :

random_testu01_smallcrush_ranlux48_base_std.txt :
	ninja -C ../.. random_testu01_smallcrush_ranlux48_base
	./random_testu01_smallcrush_ranlux48_base STD stdout \
		10 \
		>random_testu01_smallcrush_ranlux48_base_std.txt

random_testu01_smallcrush_ars_u01.txt :
	ninja -C ../.. random_testu01_smallcrush_ars
	./random_testu01_smallcrush_ars U01 stdout \
		2 \
		>random_testu01_smallcrush_ars_u01.txt

random_testu01_smallcrush_ars_64_u01.txt :
	ninja -C ../.. random_testu01_smallcrush_ars_64
	./random_testu01_smallcrush_ars_64 U01 stdout \
		2 \
		>random_testu01_smallcrush_ars_64_u01.txt

random_testu01_smallcrush_mkl_mcg59_u01.txt :
	ninja -C ../.. random_testu01_smallcrush_mkl_mcg59
	./random_testu01_smallcrush_mkl_mcg59 U01 stdout \
		1 \
		>random_testu01_smallcrush_mkl_mcg59_u01.txt

random_testu01_smallcrush_mkl_mcg59_64_u01.txt :
	ninja -C ../.. random_testu01_smallcrush_mkl_mcg59_64
	./random_testu01_smallcrush_mkl_mcg59_64 U01 stdout \
		1 \
		>random_testu01_smallcrush_mkl_mcg59_64_u01.txt

random_testu01_smallcrush_minstd_rand0_u01cc.txt :
	ninja -C ../.. random_testu01_smallcrush_minstd_rand0
	./random_testu01_smallcrush_minstd_rand0 U01CC stdout \
		3 \
		>random_testu01_smallcrush_minstd_rand0_u01cc.txt

random_testu01_smallcrush_ranlux48_base_u01cc.txt :
	ninja -C ../.. random_testu01_smallcrush_ranlux48_base
	./random_testu01_smallcrush_ranlux48_base U01CC stdout \
		9 \
		>random_testu01_smallcrush_ranlux48_base_u01cc.txt

random_testu01_smallcrush_philox2x64_u01cc.txt :
	ninja -C ../.. random_testu01_smallcrush_philox2x64
	./random_testu01_smallcrush_philox2x64 U01CC stdout \
		7 \
		>random_testu01_smallcrush_philox2x64_u01cc.txt

random_testu01_smallcrush_ranlux48_base_u01co.txt :
	ninja -C ../.. random_testu01_smallcrush_ranlux48_base
	./random_testu01_smallcrush_ranlux48_base U01CO stdout \
		3 \
		>random_testu01_smallcrush_ranlux48_base_u01co.txt

random_testu01_smallcrush_threefry2x64_64_u01co.txt :
	ninja -C ../.. random_testu01_smallcrush_threefry2x64_64
	./random_testu01_smallcrush_threefry2x64_64 U01CO stdout \
		7 \
		>random_testu01_smallcrush_threefry2x64_64_u01co.txt

random_testu01_smallcrush_threefish512_u01co.txt :
	ninja -C ../.. random_testu01_smallcrush_threefish512
	./random_testu01_smallcrush_threefish512 U01CO stdout \
		10 \
		>random_testu01_smallcrush_threefish512_u01co.txt

random_testu01_smallcrush_mkl_mcg59_64_u01co.txt :
	ninja -C ../.. random_testu01_smallcrush_mkl_mcg59_64
	./random_testu01_smallcrush_mkl_mcg59_64 U01CO stdout \
		1 \
		>random_testu01_smallcrush_mkl_mcg59_64_u01co.txt

random_testu01_smallcrush_ranlux24_base_u01oc.txt :
	ninja -C ../.. random_testu01_smallcrush_ranlux24_base
	./random_testu01_smallcrush_ranlux24_base U01OC stdout \
		5 9 \
		>random_testu01_smallcrush_ranlux24_base_u01oc.txt

random_testu01_smallcrush_minstd_rand0_u01oo.txt :
	ninja -C ../.. random_testu01_smallcrush_minstd_rand0
	./random_testu01_smallcrush_minstd_rand0 U01OO stdout \
		8 \
		>random_testu01_smallcrush_minstd_rand0_u01oo.txt

random_testu01_smallcrush_ranlux24_base_u01oo.txt :
	ninja -C ../.. random_testu01_smallcrush_ranlux24_base
	./random_testu01_smallcrush_ranlux24_base U01OO stdout \
		3 5 \
		>random_testu01_smallcrush_ranlux24_base_u01oo.txt

run : \
	random_testu01_smallcrush_ranlux48_base_std.txt \
	random_testu01_smallcrush_ars_u01.txt \
	random_testu01_smallcrush_ars_64_u01.txt \
	random_testu01_smallcrush_mkl_mcg59_u01.txt \
	random_testu01_smallcrush_mkl_mcg59_64_u01.txt \
	random_testu01_smallcrush_minstd_rand0_u01cc.txt \
	random_testu01_smallcrush_ranlux48_base_u01cc.txt \
	random_testu01_smallcrush_philox2x64_u01cc.txt \
	random_testu01_smallcrush_ranlux48_base_u01co.txt \
	random_testu01_smallcrush_threefry2x64_64_u01co.txt \
	random_testu01_smallcrush_threefish512_u01co.txt \
	random_testu01_smallcrush_mkl_mcg59_64_u01co.txt \
	random_testu01_smallcrush_ranlux24_base_u01oc.txt \
	random_testu01_smallcrush_minstd_rand0_u01oo.txt \
	random_testu01_smallcrush_ranlux24_base_u01oo.txt \


.PHONY : \
	random_testu01_smallcrush_ranlux48_base_std.txt \
	random_testu01_smallcrush_ars_u01.txt \
	random_testu01_smallcrush_ars_64_u01.txt \
	random_testu01_smallcrush_mkl_mcg59_u01.txt \
	random_testu01_smallcrush_mkl_mcg59_64_u01.txt \
	random_testu01_smallcrush_minstd_rand0_u01cc.txt \
	random_testu01_smallcrush_ranlux48_base_u01cc.txt \
	random_testu01_smallcrush_philox2x64_u01cc.txt \
	random_testu01_smallcrush_ranlux48_base_u01co.txt \
	random_testu01_smallcrush_threefry2x64_64_u01co.txt \
	random_testu01_smallcrush_threefish512_u01co.txt \
	random_testu01_smallcrush_mkl_mcg59_64_u01co.txt \
	random_testu01_smallcrush_ranlux24_base_u01oc.txt \
	random_testu01_smallcrush_minstd_rand0_u01oo.txt \
	random_testu01_smallcrush_ranlux24_base_u01oo.txt \

# vim:ft=make
