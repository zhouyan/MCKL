.PHONY : all run \
	random_testu01_bigcrush_aes128 \
	random_testu01_bigcrush_aes128_64 \
	random_testu01_bigcrush_aes192_64 \
	random_testu01_bigcrush_ars \
	random_testu01_bigcrush_ars_64 \
	random_testu01_bigcrush_minstd_rand \
	random_testu01_bigcrush_minstd_rand0 \
	random_testu01_bigcrush_mkl_ars5 \
	random_testu01_bigcrush_mkl_ars5_64 \
	random_testu01_bigcrush_mkl_mcg59 \
	random_testu01_bigcrush_mkl_mcg59_64 \
	random_testu01_bigcrush_mkl_mt19937 \
	random_testu01_bigcrush_mkl_mt19937_64 \
	random_testu01_bigcrush_mkl_mt2203 \
	random_testu01_bigcrush_mkl_mt2203_64 \
	random_testu01_bigcrush_mkl_philox4x32x10 \
	random_testu01_bigcrush_mkl_philox4x32x10_64 \
	random_testu01_bigcrush_mkl_sfmt19937 \
	random_testu01_bigcrush_mkl_sfmt19937_64 \
	random_testu01_bigcrush_mt19937 \
	random_testu01_bigcrush_philox2x32_64 \
	random_testu01_bigcrush_philox2x64 \
	random_testu01_bigcrush_philox2x64_64 \
	random_testu01_bigcrush_philox4x32 \
	random_testu01_bigcrush_philox4x32_64 \
	random_testu01_bigcrush_philox4x64 \
	random_testu01_bigcrush_philox4x64_64 \
	random_testu01_bigcrush_ranlux24 \
	random_testu01_bigcrush_ranlux24_base \
	random_testu01_bigcrush_ranlux48 \
	random_testu01_bigcrush_ranlux48_base \
	random_testu01_bigcrush_threefish1024 \
	random_testu01_bigcrush_threefish1024_64 \
	random_testu01_bigcrush_threefish256 \
	random_testu01_bigcrush_threefish256_64 \
	random_testu01_bigcrush_threefish512 \
	random_testu01_bigcrush_threefish512_64 \
	random_testu01_bigcrush_threefry16x64 \
	random_testu01_bigcrush_threefry16x64_64 \
	random_testu01_bigcrush_threefry2x32 \
	random_testu01_bigcrush_threefry2x32_64 \
	random_testu01_bigcrush_threefry2x64 \
	random_testu01_bigcrush_threefry2x64_64 \
	random_testu01_bigcrush_threefry4x32 \
	random_testu01_bigcrush_threefry4x32_64 \
	random_testu01_bigcrush_threefry4x64 \
	random_testu01_bigcrush_threefry4x64_64 \
	random_testu01_bigcrush_threefry8x64
all :

run : \
	random_testu01_bigcrush_aes128 \
	random_testu01_bigcrush_aes128_64 \
	random_testu01_bigcrush_aes192_64 \
	random_testu01_bigcrush_ars \
	random_testu01_bigcrush_ars_64 \
	random_testu01_bigcrush_minstd_rand \
	random_testu01_bigcrush_minstd_rand0 \
	random_testu01_bigcrush_mkl_ars5 \
	random_testu01_bigcrush_mkl_ars5_64 \
	random_testu01_bigcrush_mkl_mcg59 \
	random_testu01_bigcrush_mkl_mcg59_64 \
	random_testu01_bigcrush_mkl_mt19937 \
	random_testu01_bigcrush_mkl_mt19937_64 \
	random_testu01_bigcrush_mkl_mt2203 \
	random_testu01_bigcrush_mkl_mt2203_64 \
	random_testu01_bigcrush_mkl_philox4x32x10 \
	random_testu01_bigcrush_mkl_philox4x32x10_64 \
	random_testu01_bigcrush_mkl_sfmt19937 \
	random_testu01_bigcrush_mkl_sfmt19937_64 \
	random_testu01_bigcrush_mt19937 \
	random_testu01_bigcrush_philox2x32_64 \
	random_testu01_bigcrush_philox2x64 \
	random_testu01_bigcrush_philox2x64_64 \
	random_testu01_bigcrush_philox4x32 \
	random_testu01_bigcrush_philox4x32_64 \
	random_testu01_bigcrush_philox4x64 \
	random_testu01_bigcrush_philox4x64_64 \
	random_testu01_bigcrush_ranlux24 \
	random_testu01_bigcrush_ranlux24_base \
	random_testu01_bigcrush_ranlux48 \
	random_testu01_bigcrush_ranlux48_base \
	random_testu01_bigcrush_threefish1024 \
	random_testu01_bigcrush_threefish1024_64 \
	random_testu01_bigcrush_threefish256 \
	random_testu01_bigcrush_threefish256_64 \
	random_testu01_bigcrush_threefish512 \
	random_testu01_bigcrush_threefish512_64 \
	random_testu01_bigcrush_threefry16x64 \
	random_testu01_bigcrush_threefry16x64_64 \
	random_testu01_bigcrush_threefry2x32 \
	random_testu01_bigcrush_threefry2x32_64 \
	random_testu01_bigcrush_threefry2x64 \
	random_testu01_bigcrush_threefry2x64_64 \
	random_testu01_bigcrush_threefry4x32 \
	random_testu01_bigcrush_threefry4x32_64 \
	random_testu01_bigcrush_threefry4x64 \
	random_testu01_bigcrush_threefry4x64_64 \
	random_testu01_bigcrush_threefry8x64

random_testu01_bigcrush_aes128 :
	ninja -C ../.. random_testu01_bigcrush_aes128
	./random_testu01_bigcrush_aes128 U01 parallel 78
	./random_testu01_bigcrush_aes128 U01CC parallel 11
	./random_testu01_bigcrush_aes128 U01CO parallel 44

random_testu01_bigcrush_aes128_64 :
	ninja -C ../.. random_testu01_bigcrush_aes128_64
	./random_testu01_bigcrush_aes128_64 U01 parallel 78

random_testu01_bigcrush_aes192_64 :
	ninja -C ../.. random_testu01_bigcrush_aes192_64
	./random_testu01_bigcrush_aes192_64 U01OC parallel 17 48 49
	./random_testu01_bigcrush_aes192_64 U01OO parallel 61

random_testu01_bigcrush_ars :
	ninja -C ../.. random_testu01_bigcrush_ars
	./random_testu01_bigcrush_ars U01 parallel 12

random_testu01_bigcrush_ars_64 :
	ninja -C ../.. random_testu01_bigcrush_ars_64
	./random_testu01_bigcrush_ars_64 U01 parallel 12

random_testu01_bigcrush_minstd_rand :
	ninja -C ../.. random_testu01_bigcrush_minstd_rand
	./random_testu01_bigcrush_minstd_rand STD parallel 102 30 41 74
	./random_testu01_bigcrush_minstd_rand U01 parallel 65
	./random_testu01_bigcrush_minstd_rand U01CC parallel 29 32 49
	./random_testu01_bigcrush_minstd_rand U01OC parallel 102 42 44 49 65
	./random_testu01_bigcrush_minstd_rand U01OO parallel 30 32 68 87 88 89

random_testu01_bigcrush_minstd_rand0 :
	ninja -C ../.. random_testu01_bigcrush_minstd_rand0
	./random_testu01_bigcrush_minstd_rand0 STD parallel 12 88 89
	./random_testu01_bigcrush_minstd_rand0 U01 parallel 60 61 90 91
	./random_testu01_bigcrush_minstd_rand0 U01CC parallel 41 88 89
	./random_testu01_bigcrush_minstd_rand0 U01CO parallel 38
	./random_testu01_bigcrush_minstd_rand0 U01OC parallel 45
	./random_testu01_bigcrush_minstd_rand0 U01OO parallel 101 47

random_testu01_bigcrush_mkl_ars5 :
	ninja -C ../.. random_testu01_bigcrush_mkl_ars5
	./random_testu01_bigcrush_mkl_ars5 STD parallel 77
	./random_testu01_bigcrush_mkl_ars5 U01CC parallel 62
	./random_testu01_bigcrush_mkl_ars5 U01CO parallel 61

random_testu01_bigcrush_mkl_ars5_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_ars5_64
	./random_testu01_bigcrush_mkl_ars5_64 STD parallel 77
	./random_testu01_bigcrush_mkl_ars5_64 U01CO parallel 77

random_testu01_bigcrush_mkl_mcg59 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mcg59
	./random_testu01_bigcrush_mkl_mcg59 STD parallel 76 85 96
	./random_testu01_bigcrush_mkl_mcg59 U01 parallel 35
	./random_testu01_bigcrush_mkl_mcg59 U01CC parallel 64
	./random_testu01_bigcrush_mkl_mcg59 U01CO parallel 23 27 29 6
	./random_testu01_bigcrush_mkl_mcg59 U01OC parallel 12
	./random_testu01_bigcrush_mkl_mcg59 U01OO parallel 6 85

random_testu01_bigcrush_mkl_mcg59_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mcg59_64
	./random_testu01_bigcrush_mkl_mcg59_64 STD parallel 76 85 96
	./random_testu01_bigcrush_mkl_mcg59_64 U01 parallel 35
	./random_testu01_bigcrush_mkl_mcg59_64 U01CC parallel 58
	./random_testu01_bigcrush_mkl_mcg59_64 U01CO parallel 32 55 77
	./random_testu01_bigcrush_mkl_mcg59_64 U01OC parallel 35 85

random_testu01_bigcrush_mkl_mt19937 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mt19937
	./random_testu01_bigcrush_mkl_mt19937 STD parallel 26 59
	./random_testu01_bigcrush_mkl_mt19937 U01 parallel 22
	./random_testu01_bigcrush_mkl_mt19937 U01CO parallel 43

random_testu01_bigcrush_mkl_mt19937_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mt19937_64
	./random_testu01_bigcrush_mkl_mt19937_64 STD parallel 26 59
	./random_testu01_bigcrush_mkl_mt19937_64 U01 parallel 22
	./random_testu01_bigcrush_mkl_mt19937_64 U01CO parallel 64

random_testu01_bigcrush_mkl_mt2203 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mt2203
	./random_testu01_bigcrush_mkl_mt2203 U01 parallel 14 41 74
	./random_testu01_bigcrush_mkl_mt2203 U01CO parallel 11
	./random_testu01_bigcrush_mkl_mt2203 U01OC parallel 79

random_testu01_bigcrush_mkl_mt2203_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mt2203_64
	./random_testu01_bigcrush_mkl_mt2203_64 U01 parallel 14 41 74
	./random_testu01_bigcrush_mkl_mt2203_64 U01CO parallel 11
	./random_testu01_bigcrush_mkl_mt2203_64 U01OO parallel 75

random_testu01_bigcrush_mkl_philox4x32x10 :
	ninja -C ../.. random_testu01_bigcrush_mkl_philox4x32x10
	./random_testu01_bigcrush_mkl_philox4x32x10 STD parallel 59
	./random_testu01_bigcrush_mkl_philox4x32x10 U01OC parallel 36

random_testu01_bigcrush_mkl_philox4x32x10_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_philox4x32x10_64
	./random_testu01_bigcrush_mkl_philox4x32x10_64 STD parallel 59
	./random_testu01_bigcrush_mkl_philox4x32x10_64 U01CC parallel 20

random_testu01_bigcrush_mkl_sfmt19937 :
	ninja -C ../.. random_testu01_bigcrush_mkl_sfmt19937
	./random_testu01_bigcrush_mkl_sfmt19937 U01CO parallel 81
	./random_testu01_bigcrush_mkl_sfmt19937 U01OC parallel 21

random_testu01_bigcrush_mkl_sfmt19937_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_sfmt19937_64
	./random_testu01_bigcrush_mkl_sfmt19937_64 U01CC parallel 50
	./random_testu01_bigcrush_mkl_sfmt19937_64 U01OO parallel 36

random_testu01_bigcrush_mt19937 :
	ninja -C ../.. random_testu01_bigcrush_mt19937
	./random_testu01_bigcrush_mt19937 U01CC parallel 52

random_testu01_bigcrush_philox2x32_64 :
	ninja -C ../.. random_testu01_bigcrush_philox2x32_64
	./random_testu01_bigcrush_philox2x32_64 U01CO parallel 17

random_testu01_bigcrush_philox2x64 :
	ninja -C ../.. random_testu01_bigcrush_philox2x64
	./random_testu01_bigcrush_philox2x64 STD parallel 22
	./random_testu01_bigcrush_philox2x64 U01OC parallel 74

random_testu01_bigcrush_philox2x64_64 :
	ninja -C ../.. random_testu01_bigcrush_philox2x64_64
	./random_testu01_bigcrush_philox2x64_64 STD parallel 22

random_testu01_bigcrush_philox4x32 :
	ninja -C ../.. random_testu01_bigcrush_philox4x32
	./random_testu01_bigcrush_philox4x32 U01 parallel 14
	./random_testu01_bigcrush_philox4x32 U01CC parallel 2

random_testu01_bigcrush_philox4x32_64 :
	ninja -C ../.. random_testu01_bigcrush_philox4x32_64
	./random_testu01_bigcrush_philox4x32_64 U01 parallel 14
	./random_testu01_bigcrush_philox4x32_64 U01CC parallel 7
	./random_testu01_bigcrush_philox4x32_64 U01CO parallel 12

random_testu01_bigcrush_philox4x64 :
	ninja -C ../.. random_testu01_bigcrush_philox4x64
	./random_testu01_bigcrush_philox4x64 STD parallel 6
	./random_testu01_bigcrush_philox4x64 U01CO parallel 11

random_testu01_bigcrush_philox4x64_64 :
	ninja -C ../.. random_testu01_bigcrush_philox4x64_64
	./random_testu01_bigcrush_philox4x64_64 STD parallel 6
	./random_testu01_bigcrush_philox4x64_64 U01OC parallel 39

random_testu01_bigcrush_ranlux24 :
	ninja -C ../.. random_testu01_bigcrush_ranlux24
	./random_testu01_bigcrush_ranlux24 U01OO parallel 58

random_testu01_bigcrush_ranlux24_base :
	ninja -C ../.. random_testu01_bigcrush_ranlux24_base
	./random_testu01_bigcrush_ranlux24_base U01 parallel 25 30 31 32 33 48
	./random_testu01_bigcrush_ranlux24_base U01CC parallel 53 76 77
	./random_testu01_bigcrush_ranlux24_base U01CO parallel 53 54 76
	./random_testu01_bigcrush_ranlux24_base U01OC parallel 53 76 77
	./random_testu01_bigcrush_ranlux24_base U01OO parallel 53 54 76

random_testu01_bigcrush_ranlux48 :
	ninja -C ../.. random_testu01_bigcrush_ranlux48
	./random_testu01_bigcrush_ranlux48 U01CC parallel 32

random_testu01_bigcrush_ranlux48_base :
	ninja -C ../.. random_testu01_bigcrush_ranlux48_base
	./random_testu01_bigcrush_ranlux48_base STD parallel 25 32 33 47 48
	./random_testu01_bigcrush_ranlux48_base U01 parallel 30 31 32 33 34 47 48 61 77
	./random_testu01_bigcrush_ranlux48_base U01CC parallel 53 76
	./random_testu01_bigcrush_ranlux48_base U01CO parallel 53 54 76 77
	./random_testu01_bigcrush_ranlux48_base U01OC parallel 53 54 76
	./random_testu01_bigcrush_ranlux48_base U01OO parallel 53 54 74

random_testu01_bigcrush_threefish1024 :
	ninja -C ../.. random_testu01_bigcrush_threefish1024
	./random_testu01_bigcrush_threefish1024 U01CC parallel 94
	./random_testu01_bigcrush_threefish1024 U01OC parallel 11

random_testu01_bigcrush_threefish1024_64 :
	ninja -C ../.. random_testu01_bigcrush_threefish1024_64
	./random_testu01_bigcrush_threefish1024_64 U01CO parallel 64
	./random_testu01_bigcrush_threefish1024_64 U01OC parallel 12 74

random_testu01_bigcrush_threefish256 :
	ninja -C ../.. random_testu01_bigcrush_threefish256
	./random_testu01_bigcrush_threefish256 U01OC parallel 77

random_testu01_bigcrush_threefish256_64 :
	ninja -C ../.. random_testu01_bigcrush_threefish256_64
	./random_testu01_bigcrush_threefish256_64 U01CO parallel 30
	./random_testu01_bigcrush_threefish256_64 U01OC parallel 103
	./random_testu01_bigcrush_threefish256_64 U01OO parallel 77

random_testu01_bigcrush_threefish512 :
	ninja -C ../.. random_testu01_bigcrush_threefish512
	./random_testu01_bigcrush_threefish512 U01 parallel 74
	./random_testu01_bigcrush_threefish512 U01CO parallel 13

random_testu01_bigcrush_threefish512_64 :
	ninja -C ../.. random_testu01_bigcrush_threefish512_64
	./random_testu01_bigcrush_threefish512_64 U01 parallel 74

random_testu01_bigcrush_threefry16x64 :
	ninja -C ../.. random_testu01_bigcrush_threefry16x64
	./random_testu01_bigcrush_threefry16x64 STD parallel 35
	./random_testu01_bigcrush_threefry16x64 U01CC parallel 23

random_testu01_bigcrush_threefry16x64_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry16x64_64
	./random_testu01_bigcrush_threefry16x64_64 STD parallel 35
	./random_testu01_bigcrush_threefry16x64_64 U01CC parallel 23
	./random_testu01_bigcrush_threefry16x64_64 U01OO parallel 49 76

random_testu01_bigcrush_threefry2x32 :
	ninja -C ../.. random_testu01_bigcrush_threefry2x32
	./random_testu01_bigcrush_threefry2x32 U01OO parallel 89 97

random_testu01_bigcrush_threefry2x32_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry2x32_64
	./random_testu01_bigcrush_threefry2x32_64 U01OC parallel 12

random_testu01_bigcrush_threefry2x64 :
	ninja -C ../.. random_testu01_bigcrush_threefry2x64
	./random_testu01_bigcrush_threefry2x64 U01 parallel 23
	./random_testu01_bigcrush_threefry2x64 U01OC parallel 11 46 79
	./random_testu01_bigcrush_threefry2x64 U01OO parallel 56

random_testu01_bigcrush_threefry2x64_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry2x64_64
	./random_testu01_bigcrush_threefry2x64_64 U01 parallel 23
	./random_testu01_bigcrush_threefry2x64_64 U01OC parallel 79
	./random_testu01_bigcrush_threefry2x64_64 U01OO parallel 103

random_testu01_bigcrush_threefry4x32 :
	ninja -C ../.. random_testu01_bigcrush_threefry4x32
	./random_testu01_bigcrush_threefry4x32 U01OC parallel 72

random_testu01_bigcrush_threefry4x32_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry4x32_64
	./random_testu01_bigcrush_threefry4x32_64 U01CO parallel 97

random_testu01_bigcrush_threefry4x64 :
	ninja -C ../.. random_testu01_bigcrush_threefry4x64
	./random_testu01_bigcrush_threefry4x64 U01 parallel 72
	./random_testu01_bigcrush_threefry4x64 U01CC parallel 12

random_testu01_bigcrush_threefry4x64_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry4x64_64
	./random_testu01_bigcrush_threefry4x64_64 U01 parallel 72
	./random_testu01_bigcrush_threefry4x64_64 U01CC parallel 35 43

random_testu01_bigcrush_threefry8x64 :
	ninja -C ../.. random_testu01_bigcrush_threefry8x64
	./random_testu01_bigcrush_threefry8x64 U01OO parallel 38

# vim:ft=make
