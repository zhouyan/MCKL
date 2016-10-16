.PHONY : all run \
	random_testu01_bigcrush_aes128 \
	random_testu01_bigcrush_aes192_64 \
	random_testu01_bigcrush_ars \
	random_testu01_bigcrush_ars_64 \
	random_testu01_bigcrush_mkl_ars5 \
	random_testu01_bigcrush_mkl_mcg59 \
	random_testu01_bigcrush_mkl_mcg59_64 \
	random_testu01_bigcrush_mkl_mt19937 \
	random_testu01_bigcrush_mkl_mt19937_64 \
	random_testu01_bigcrush_mkl_mt2203 \
	random_testu01_bigcrush_mkl_mt2203_64 \
	random_testu01_bigcrush_mkl_philox4x32x10 \
	random_testu01_bigcrush_mkl_sfmt19937 \
	random_testu01_bigcrush_mkl_sfmt19937_64 \
	random_testu01_bigcrush_mt19937 \
	random_testu01_bigcrush_philox2x64 \
	random_testu01_bigcrush_philox2x64_64 \
	random_testu01_bigcrush_philox4x32 \
	random_testu01_bigcrush_philox4x32_64 \
	random_testu01_bigcrush_philox4x64 \
	random_testu01_bigcrush_philox4x64_64 \
	random_testu01_bigcrush_ranlux24_base \
	random_testu01_bigcrush_ranlux48_base \
	random_testu01_bigcrush_rdrand32 \
	random_testu01_bigcrush_rdrand64 \
	random_testu01_bigcrush_threefish1024 \
	random_testu01_bigcrush_threefish1024_64 \
	random_testu01_bigcrush_threefish256 \
	random_testu01_bigcrush_threefish256_64 \
	random_testu01_bigcrush_threefish512 \
	random_testu01_bigcrush_threefish512_64 \
	random_testu01_bigcrush_threefry16x64_64 \
	random_testu01_bigcrush_threefry2x32 \
	random_testu01_bigcrush_threefry2x32_64 \
	random_testu01_bigcrush_threefry2x64 \
	random_testu01_bigcrush_threefry2x64_64 \
	random_testu01_bigcrush_threefry4x32 \
	random_testu01_bigcrush_threefry4x64 \
	random_testu01_bigcrush_threefry4x64_64 \
	random_testu01_bigcrush_threefry8x64
all :

run : \
	random_testu01_bigcrush_aes128 \
	random_testu01_bigcrush_aes192_64 \
	random_testu01_bigcrush_ars \
	random_testu01_bigcrush_ars_64 \
	random_testu01_bigcrush_mkl_ars5 \
	random_testu01_bigcrush_mkl_mcg59 \
	random_testu01_bigcrush_mkl_mcg59_64 \
	random_testu01_bigcrush_mkl_mt19937 \
	random_testu01_bigcrush_mkl_mt19937_64 \
	random_testu01_bigcrush_mkl_mt2203 \
	random_testu01_bigcrush_mkl_mt2203_64 \
	random_testu01_bigcrush_mkl_philox4x32x10 \
	random_testu01_bigcrush_mkl_sfmt19937 \
	random_testu01_bigcrush_mkl_sfmt19937_64 \
	random_testu01_bigcrush_mt19937 \
	random_testu01_bigcrush_philox2x64 \
	random_testu01_bigcrush_philox2x64_64 \
	random_testu01_bigcrush_philox4x32 \
	random_testu01_bigcrush_philox4x32_64 \
	random_testu01_bigcrush_philox4x64 \
	random_testu01_bigcrush_philox4x64_64 \
	random_testu01_bigcrush_ranlux24_base \
	random_testu01_bigcrush_ranlux48_base \
	random_testu01_bigcrush_rdrand32 \
	random_testu01_bigcrush_rdrand64 \
	random_testu01_bigcrush_threefish1024 \
	random_testu01_bigcrush_threefish1024_64 \
	random_testu01_bigcrush_threefish256 \
	random_testu01_bigcrush_threefish256_64 \
	random_testu01_bigcrush_threefish512 \
	random_testu01_bigcrush_threefish512_64 \
	random_testu01_bigcrush_threefry16x64_64 \
	random_testu01_bigcrush_threefry2x32 \
	random_testu01_bigcrush_threefry2x32_64 \
	random_testu01_bigcrush_threefry2x64 \
	random_testu01_bigcrush_threefry2x64_64 \
	random_testu01_bigcrush_threefry4x32 \
	random_testu01_bigcrush_threefry4x64 \
	random_testu01_bigcrush_threefry4x64_64 \
	random_testu01_bigcrush_threefry8x64

random_testu01_bigcrush_aes128 :
	ninja -C ../.. random_testu01_bigcrush_aes128
	./random_testu01_bigcrush_aes128 U01 78
	./random_testu01_bigcrush_aes128 U01CC 11
	./random_testu01_bigcrush_aes128 U01CO 44

random_testu01_bigcrush_aes192_64 :
	ninja -C ../.. random_testu01_bigcrush_aes192_64
	./random_testu01_bigcrush_aes192_64 U01OC 17 48 49
	./random_testu01_bigcrush_aes192_64 U01OO 61

random_testu01_bigcrush_ars :
	ninja -C ../.. random_testu01_bigcrush_ars
	./random_testu01_bigcrush_ars U01 12

random_testu01_bigcrush_ars_64 :
	ninja -C ../.. random_testu01_bigcrush_ars_64
	./random_testu01_bigcrush_ars_64 U01 12

random_testu01_bigcrush_mkl_ars5 :
	ninja -C ../.. random_testu01_bigcrush_mkl_ars5
	./random_testu01_bigcrush_mkl_ars5 STD 77
	./random_testu01_bigcrush_mkl_ars5 U01CC 62
	./random_testu01_bigcrush_mkl_ars5 U01CO 61

random_testu01_bigcrush_mkl_mcg59 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mcg59
	./random_testu01_bigcrush_mkl_mcg59 STD 76 85 96
	./random_testu01_bigcrush_mkl_mcg59 U01 35
	./random_testu01_bigcrush_mkl_mcg59 U01CC 64
	./random_testu01_bigcrush_mkl_mcg59 U01CO 23 27 29 6
	./random_testu01_bigcrush_mkl_mcg59 U01OC 12
	./random_testu01_bigcrush_mkl_mcg59 U01OO 6 85

random_testu01_bigcrush_mkl_mcg59_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mcg59_64
	./random_testu01_bigcrush_mkl_mcg59_64 STD 76 85 96
	./random_testu01_bigcrush_mkl_mcg59_64 U01 35
	./random_testu01_bigcrush_mkl_mcg59_64 U01CC 58
	./random_testu01_bigcrush_mkl_mcg59_64 U01CO 32 55 77
	./random_testu01_bigcrush_mkl_mcg59_64 U01OC 35 85

random_testu01_bigcrush_mkl_mt19937 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mt19937
	./random_testu01_bigcrush_mkl_mt19937 STD 26 59
	./random_testu01_bigcrush_mkl_mt19937 U01 22
	./random_testu01_bigcrush_mkl_mt19937 U01CO 43

random_testu01_bigcrush_mkl_mt19937_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mt19937_64
	./random_testu01_bigcrush_mkl_mt19937_64 STD 26 59
	./random_testu01_bigcrush_mkl_mt19937_64 U01 22
	./random_testu01_bigcrush_mkl_mt19937_64 U01CO 64

random_testu01_bigcrush_mkl_mt2203 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mt2203
	./random_testu01_bigcrush_mkl_mt2203 U01 14 41 74
	./random_testu01_bigcrush_mkl_mt2203 U01CO 11
	./random_testu01_bigcrush_mkl_mt2203 U01OC 79

random_testu01_bigcrush_mkl_mt2203_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mt2203_64
	./random_testu01_bigcrush_mkl_mt2203_64 U01 14 41 74
	./random_testu01_bigcrush_mkl_mt2203_64 U01CO 11
	./random_testu01_bigcrush_mkl_mt2203_64 U01OO 75

random_testu01_bigcrush_mkl_philox4x32x10 :
	ninja -C ../.. random_testu01_bigcrush_mkl_philox4x32x10
	./random_testu01_bigcrush_mkl_philox4x32x10 STD 59
	./random_testu01_bigcrush_mkl_philox4x32x10 U01OC 36

random_testu01_bigcrush_mkl_sfmt19937 :
	ninja -C ../.. random_testu01_bigcrush_mkl_sfmt19937
	./random_testu01_bigcrush_mkl_sfmt19937 U01CO 81
	./random_testu01_bigcrush_mkl_sfmt19937 U01OC 21

random_testu01_bigcrush_mkl_sfmt19937_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_sfmt19937_64
	./random_testu01_bigcrush_mkl_sfmt19937_64 U01CC 50
	./random_testu01_bigcrush_mkl_sfmt19937_64 U01OO 36

random_testu01_bigcrush_mt19937 :
	ninja -C ../.. random_testu01_bigcrush_mt19937
	./random_testu01_bigcrush_mt19937 U01CC 52

random_testu01_bigcrush_philox2x64 :
	ninja -C ../.. random_testu01_bigcrush_philox2x64
	./random_testu01_bigcrush_philox2x64 STD 22
	./random_testu01_bigcrush_philox2x64 U01OC 74

random_testu01_bigcrush_philox2x64_64 :
	ninja -C ../.. random_testu01_bigcrush_philox2x64_64
	./random_testu01_bigcrush_philox2x64_64 STD 22

random_testu01_bigcrush_philox4x32 :
	ninja -C ../.. random_testu01_bigcrush_philox4x32
	./random_testu01_bigcrush_philox4x32 U01 14
	./random_testu01_bigcrush_philox4x32 U01CC 2

random_testu01_bigcrush_philox4x32_64 :
	ninja -C ../.. random_testu01_bigcrush_philox4x32_64
	./random_testu01_bigcrush_philox4x32_64 U01 14
	./random_testu01_bigcrush_philox4x32_64 U01CC 7
	./random_testu01_bigcrush_philox4x32_64 U01CO 12

random_testu01_bigcrush_philox4x64 :
	ninja -C ../.. random_testu01_bigcrush_philox4x64
	./random_testu01_bigcrush_philox4x64 STD 6
	./random_testu01_bigcrush_philox4x64 U01CO 11

random_testu01_bigcrush_philox4x64_64 :
	ninja -C ../.. random_testu01_bigcrush_philox4x64_64
	./random_testu01_bigcrush_philox4x64_64 STD 6
	./random_testu01_bigcrush_philox4x64_64 U01OC 39

random_testu01_bigcrush_ranlux24_base :
	ninja -C ../.. random_testu01_bigcrush_ranlux24_base
	./random_testu01_bigcrush_ranlux24_base U01 25 30 31 32 33 48
	./random_testu01_bigcrush_ranlux24_base U01CC 53 76 77
	./random_testu01_bigcrush_ranlux24_base U01CO 53 54 76
	./random_testu01_bigcrush_ranlux24_base U01OC 53 76 77
	./random_testu01_bigcrush_ranlux24_base U01OO 53 54 76

random_testu01_bigcrush_ranlux48_base :
	ninja -C ../.. random_testu01_bigcrush_ranlux48_base
	./random_testu01_bigcrush_ranlux48_base STD 25 32 33 47 48
	./random_testu01_bigcrush_ranlux48_base U01 30 31 32 33 34 47 48 61 77
	./random_testu01_bigcrush_ranlux48_base U01CC 53 76
	./random_testu01_bigcrush_ranlux48_base U01CO 53 54 76 77
	./random_testu01_bigcrush_ranlux48_base U01OC 53 54 76
	./random_testu01_bigcrush_ranlux48_base U01OO 53 54 74

random_testu01_bigcrush_rdrand32 :
	ninja -C ../.. random_testu01_bigcrush_rdrand32
	./random_testu01_bigcrush_rdrand32 U01CC 105
	./random_testu01_bigcrush_rdrand32 U01OC 77

random_testu01_bigcrush_rdrand64 :
	ninja -C ../.. random_testu01_bigcrush_rdrand64
	./random_testu01_bigcrush_rdrand64 U01OO 11

random_testu01_bigcrush_threefish1024 :
	ninja -C ../.. random_testu01_bigcrush_threefish1024
	./random_testu01_bigcrush_threefish1024 U01CC 94
	./random_testu01_bigcrush_threefish1024 U01OC 11

random_testu01_bigcrush_threefish1024_64 :
	ninja -C ../.. random_testu01_bigcrush_threefish1024_64
	./random_testu01_bigcrush_threefish1024_64 U01CO 64
	./random_testu01_bigcrush_threefish1024_64 U01OC 12 74

random_testu01_bigcrush_threefish256 :
	ninja -C ../.. random_testu01_bigcrush_threefish256
	./random_testu01_bigcrush_threefish256 U01OC 77

random_testu01_bigcrush_threefish256_64 :
	ninja -C ../.. random_testu01_bigcrush_threefish256_64
	./random_testu01_bigcrush_threefish256_64 U01CO 30
	./random_testu01_bigcrush_threefish256_64 U01OC 103
	./random_testu01_bigcrush_threefish256_64 U01OO 77

random_testu01_bigcrush_threefish512 :
	ninja -C ../.. random_testu01_bigcrush_threefish512
	./random_testu01_bigcrush_threefish512 U01 74
	./random_testu01_bigcrush_threefish512 U01CO 13

random_testu01_bigcrush_threefish512_64 :
	ninja -C ../.. random_testu01_bigcrush_threefish512_64
	./random_testu01_bigcrush_threefish512_64 U01 74

random_testu01_bigcrush_threefry16x64_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry16x64_64
	./random_testu01_bigcrush_threefry16x64_64 STD 35
	./random_testu01_bigcrush_threefry16x64_64 U01CC 23
	./random_testu01_bigcrush_threefry16x64_64 U01OO 49 76

random_testu01_bigcrush_threefry2x32 :
	ninja -C ../.. random_testu01_bigcrush_threefry2x32
	./random_testu01_bigcrush_threefry2x32 U01OO 89 97

random_testu01_bigcrush_threefry2x32_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry2x32_64
	./random_testu01_bigcrush_threefry2x32_64 U01OC 12

random_testu01_bigcrush_threefry2x64 :
	ninja -C ../.. random_testu01_bigcrush_threefry2x64
	./random_testu01_bigcrush_threefry2x64 U01 23
	./random_testu01_bigcrush_threefry2x64 U01OC 11 46 79
	./random_testu01_bigcrush_threefry2x64 U01OO 56

random_testu01_bigcrush_threefry2x64_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry2x64_64
	./random_testu01_bigcrush_threefry2x64_64 U01 23
	./random_testu01_bigcrush_threefry2x64_64 U01OC 79
	./random_testu01_bigcrush_threefry2x64_64 U01OO 103

random_testu01_bigcrush_threefry4x32 :
	ninja -C ../.. random_testu01_bigcrush_threefry4x32
	./random_testu01_bigcrush_threefry4x32 U01OC 72

random_testu01_bigcrush_threefry4x64 :
	ninja -C ../.. random_testu01_bigcrush_threefry4x64
	./random_testu01_bigcrush_threefry4x64 U01 72
	./random_testu01_bigcrush_threefry4x64 U01CC 12

random_testu01_bigcrush_threefry4x64_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry4x64_64
	./random_testu01_bigcrush_threefry4x64_64 U01 72
	./random_testu01_bigcrush_threefry4x64_64 U01CC 35 43

random_testu01_bigcrush_threefry8x64 :
	ninja -C ../.. random_testu01_bigcrush_threefry8x64
	./random_testu01_bigcrush_threefry8x64 U01OO 38

# vim:ft=make
