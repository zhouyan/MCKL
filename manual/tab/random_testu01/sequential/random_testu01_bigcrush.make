.PHONY : all run \
	random_testu01_bigcrush_aes128 \
	random_testu01_bigcrush_aes128_64 \
	random_testu01_bigcrush_aes192 \
	random_testu01_bigcrush_aes192_64 \
	random_testu01_bigcrush_aes256 \
	random_testu01_bigcrush_ars \
	random_testu01_bigcrush_ars_64 \
	random_testu01_bigcrush_knuth_b \
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
	random_testu01_bigcrush_mt19937_64 \
	random_testu01_bigcrush_philox2x32 \
	random_testu01_bigcrush_philox2x32_64 \
	random_testu01_bigcrush_philox2x64 \
	random_testu01_bigcrush_philox2x64_64 \
	random_testu01_bigcrush_philox4x32 \
	random_testu01_bigcrush_philox4x32_64 \
	random_testu01_bigcrush_philox4x64 \
	random_testu01_bigcrush_philox4x64_64 \
	random_testu01_bigcrush_ranlux24_base \
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
	random_testu01_bigcrush_threefry4x32_64 \
	random_testu01_bigcrush_threefry4x64 \
	random_testu01_bigcrush_threefry4x64_64 \
	random_testu01_bigcrush_threefry8x64 \
	random_testu01_bigcrush_threefry8x64_64
all :

run : \
	random_testu01_bigcrush_aes128 \
	random_testu01_bigcrush_aes128_64 \
	random_testu01_bigcrush_aes192 \
	random_testu01_bigcrush_aes192_64 \
	random_testu01_bigcrush_aes256 \
	random_testu01_bigcrush_ars \
	random_testu01_bigcrush_ars_64 \
	random_testu01_bigcrush_knuth_b \
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
	random_testu01_bigcrush_mt19937_64 \
	random_testu01_bigcrush_philox2x32 \
	random_testu01_bigcrush_philox2x32_64 \
	random_testu01_bigcrush_philox2x64 \
	random_testu01_bigcrush_philox2x64_64 \
	random_testu01_bigcrush_philox4x32 \
	random_testu01_bigcrush_philox4x32_64 \
	random_testu01_bigcrush_philox4x64 \
	random_testu01_bigcrush_philox4x64_64 \
	random_testu01_bigcrush_ranlux24_base \
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
	random_testu01_bigcrush_threefry4x32_64 \
	random_testu01_bigcrush_threefry4x64 \
	random_testu01_bigcrush_threefry4x64_64 \
	random_testu01_bigcrush_threefry8x64 \
	random_testu01_bigcrush_threefry8x64_64

random_testu01_bigcrush_aes128 :
	ninja -C ../.. random_testu01_bigcrush_aes128
	./random_testu01_bigcrush_aes128 U01 13

random_testu01_bigcrush_aes128_64 :
	ninja -C ../.. random_testu01_bigcrush_aes128_64
	./random_testu01_bigcrush_aes128_64 U01 13
	./random_testu01_bigcrush_aes128_64 U01CC 13
	./random_testu01_bigcrush_aes128_64 U01CO 19
	./random_testu01_bigcrush_aes128_64 U01OC 6

random_testu01_bigcrush_aes192 :
	ninja -C ../.. random_testu01_bigcrush_aes192
	./random_testu01_bigcrush_aes192 STD 74 76
	./random_testu01_bigcrush_aes192 U01 11
	./random_testu01_bigcrush_aes192 U01CO 63
	./random_testu01_bigcrush_aes192 U01OC 25
	./random_testu01_bigcrush_aes192 U01OO 59

random_testu01_bigcrush_aes192_64 :
	ninja -C ../.. random_testu01_bigcrush_aes192_64
	./random_testu01_bigcrush_aes192_64 STD 74 76
	./random_testu01_bigcrush_aes192_64 U01 11
	./random_testu01_bigcrush_aes192_64 U01CC 4
	./random_testu01_bigcrush_aes192_64 U01CO 11
	./random_testu01_bigcrush_aes192_64 U01OC 10
	./random_testu01_bigcrush_aes192_64 U01OO 43

random_testu01_bigcrush_aes256 :
	ninja -C ../.. random_testu01_bigcrush_aes256
	./random_testu01_bigcrush_aes256 U01OC 1 24
	./random_testu01_bigcrush_aes256 U01OO 28

random_testu01_bigcrush_ars :
	ninja -C ../.. random_testu01_bigcrush_ars
	./random_testu01_bigcrush_ars U01CC 29 74
	./random_testu01_bigcrush_ars U01OO 22 67

random_testu01_bigcrush_ars_64 :
	ninja -C ../.. random_testu01_bigcrush_ars_64
	./random_testu01_bigcrush_ars_64 U01OC 17

random_testu01_bigcrush_knuth_b :
	ninja -C ../.. random_testu01_bigcrush_knuth_b
	./random_testu01_bigcrush_knuth_b STD 88 89
	./random_testu01_bigcrush_knuth_b U01 78 89
	./random_testu01_bigcrush_knuth_b U01CC 47
	./random_testu01_bigcrush_knuth_b U01CO 89
	./random_testu01_bigcrush_knuth_b U01OC 89
	./random_testu01_bigcrush_knuth_b U01OO 89

random_testu01_bigcrush_minstd_rand :
	ninja -C ../.. random_testu01_bigcrush_minstd_rand
	./random_testu01_bigcrush_minstd_rand STD 29 40 47 85 92
	./random_testu01_bigcrush_minstd_rand U01 27 38 49 55 56 87 88 95
	./random_testu01_bigcrush_minstd_rand U01CC 102 39 45 49 51 84 89 94
	./random_testu01_bigcrush_minstd_rand U01CO 102 28 85 87
	./random_testu01_bigcrush_minstd_rand U01OC 102 26 28 29 41 45 52 87 89
	./random_testu01_bigcrush_minstd_rand U01OO 102 26 28 29 84 85

random_testu01_bigcrush_minstd_rand0 :
	ninja -C ../.. random_testu01_bigcrush_minstd_rand0
	./random_testu01_bigcrush_minstd_rand0 STD 39 40 62 63 86 87 92
	./random_testu01_bigcrush_minstd_rand0 U01 39 41 44 45 87 94
	./random_testu01_bigcrush_minstd_rand0 U01CC 26 28 38 44 47
	./random_testu01_bigcrush_minstd_rand0 U01CO 27 28 29 38 41 44 47 51 86 89
	./random_testu01_bigcrush_minstd_rand0 U01OC 27 28 29 38 41 47 57 86
	./random_testu01_bigcrush_minstd_rand0 U01OO 38 41 87

random_testu01_bigcrush_mkl_ars5 :
	ninja -C ../.. random_testu01_bigcrush_mkl_ars5
	./random_testu01_bigcrush_mkl_ars5 U01 24
	./random_testu01_bigcrush_mkl_ars5 U01CO 10
	./random_testu01_bigcrush_mkl_ars5 U01OO 30

random_testu01_bigcrush_mkl_ars5_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_ars5_64
	./random_testu01_bigcrush_mkl_ars5_64 U01 24

random_testu01_bigcrush_mkl_mcg59 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mcg59
	./random_testu01_bigcrush_mkl_mcg59 U01 6 64 87
	./random_testu01_bigcrush_mkl_mcg59 U01CC 27 6 87
	./random_testu01_bigcrush_mkl_mcg59 U01CO 22 87
	./random_testu01_bigcrush_mkl_mcg59 U01OC 37 46 58 87
	./random_testu01_bigcrush_mkl_mcg59 U01OO 27 29 6 87

random_testu01_bigcrush_mkl_mcg59_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mcg59_64
	./random_testu01_bigcrush_mkl_mcg59_64 U01 6 64 87
	./random_testu01_bigcrush_mkl_mcg59_64 U01CC 22 37
	./random_testu01_bigcrush_mkl_mcg59_64 U01OC 22 37 4

random_testu01_bigcrush_mkl_mt19937 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mt19937
	./random_testu01_bigcrush_mkl_mt19937 STD 64
	./random_testu01_bigcrush_mkl_mt19937 U01CC 69
	./random_testu01_bigcrush_mkl_mt19937 U01CO 60

random_testu01_bigcrush_mkl_mt19937_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mt19937_64
	./random_testu01_bigcrush_mkl_mt19937_64 STD 64
	./random_testu01_bigcrush_mkl_mt19937_64 U01CO 43 47

random_testu01_bigcrush_mkl_mt2203 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mt2203
	./random_testu01_bigcrush_mkl_mt2203 U01 9

random_testu01_bigcrush_mkl_mt2203_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_mt2203_64
	./random_testu01_bigcrush_mkl_mt2203_64 U01 9
	./random_testu01_bigcrush_mkl_mt2203_64 U01CC 25
	./random_testu01_bigcrush_mkl_mt2203_64 U01OC 10 76
	./random_testu01_bigcrush_mkl_mt2203_64 U01OO 41

random_testu01_bigcrush_mkl_philox4x32x10 :
	ninja -C ../.. random_testu01_bigcrush_mkl_philox4x32x10
	./random_testu01_bigcrush_mkl_philox4x32x10 STD 25
	./random_testu01_bigcrush_mkl_philox4x32x10 U01 76
	./random_testu01_bigcrush_mkl_philox4x32x10 U01OC 106

random_testu01_bigcrush_mkl_philox4x32x10_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_philox4x32x10_64
	./random_testu01_bigcrush_mkl_philox4x32x10_64 STD 25
	./random_testu01_bigcrush_mkl_philox4x32x10_64 U01 76
	./random_testu01_bigcrush_mkl_philox4x32x10_64 U01CO 86 9
	./random_testu01_bigcrush_mkl_philox4x32x10_64 U01OO 55

random_testu01_bigcrush_mkl_sfmt19937 :
	ninja -C ../.. random_testu01_bigcrush_mkl_sfmt19937
	./random_testu01_bigcrush_mkl_sfmt19937 STD 81
	./random_testu01_bigcrush_mkl_sfmt19937 U01 24
	./random_testu01_bigcrush_mkl_sfmt19937 U01CO 86

random_testu01_bigcrush_mkl_sfmt19937_64 :
	ninja -C ../.. random_testu01_bigcrush_mkl_sfmt19937_64
	./random_testu01_bigcrush_mkl_sfmt19937_64 STD 81
	./random_testu01_bigcrush_mkl_sfmt19937_64 U01 24
	./random_testu01_bigcrush_mkl_sfmt19937_64 U01CO 22

random_testu01_bigcrush_mt19937_64 :
	ninja -C ../.. random_testu01_bigcrush_mt19937_64
	./random_testu01_bigcrush_mt19937_64 U01CC 20
	./random_testu01_bigcrush_mt19937_64 U01CO 77

random_testu01_bigcrush_philox2x32 :
	ninja -C ../.. random_testu01_bigcrush_philox2x32
	./random_testu01_bigcrush_philox2x32 U01 41

random_testu01_bigcrush_philox2x32_64 :
	ninja -C ../.. random_testu01_bigcrush_philox2x32_64
	./random_testu01_bigcrush_philox2x32_64 U01 41

random_testu01_bigcrush_philox2x64 :
	ninja -C ../.. random_testu01_bigcrush_philox2x64
	./random_testu01_bigcrush_philox2x64 U01 85
	./random_testu01_bigcrush_philox2x64 U01CO 76

random_testu01_bigcrush_philox2x64_64 :
	ninja -C ../.. random_testu01_bigcrush_philox2x64_64
	./random_testu01_bigcrush_philox2x64_64 U01 85
	./random_testu01_bigcrush_philox2x64_64 U01CC 82

random_testu01_bigcrush_philox4x32 :
	ninja -C ../.. random_testu01_bigcrush_philox4x32
	./random_testu01_bigcrush_philox4x32 U01CC 22

random_testu01_bigcrush_philox4x32_64 :
	ninja -C ../.. random_testu01_bigcrush_philox4x32_64
	./random_testu01_bigcrush_philox4x32_64 U01CC 78

random_testu01_bigcrush_philox4x64 :
	ninja -C ../.. random_testu01_bigcrush_philox4x64
	./random_testu01_bigcrush_philox4x64 U01 44
	./random_testu01_bigcrush_philox4x64 U01CC 58
	./random_testu01_bigcrush_philox4x64 U01CO 10 11
	./random_testu01_bigcrush_philox4x64 U01OO 80

random_testu01_bigcrush_philox4x64_64 :
	ninja -C ../.. random_testu01_bigcrush_philox4x64_64
	./random_testu01_bigcrush_philox4x64_64 U01 44
	./random_testu01_bigcrush_philox4x64_64 U01CC 10
	./random_testu01_bigcrush_philox4x64_64 U01CO 12

random_testu01_bigcrush_ranlux24_base :
	ninja -C ../.. random_testu01_bigcrush_ranlux24_base
	./random_testu01_bigcrush_ranlux24_base U01 30 33 48 59 60
	./random_testu01_bigcrush_ranlux24_base U01CC 53 54 74 76 77
	./random_testu01_bigcrush_ranlux24_base U01CO 54 76 77
	./random_testu01_bigcrush_ranlux24_base U01OC 53 54 76
	./random_testu01_bigcrush_ranlux24_base U01OO 53 54 76 77

random_testu01_bigcrush_ranlux48_base :
	ninja -C ../.. random_testu01_bigcrush_ranlux48_base
	./random_testu01_bigcrush_ranlux48_base STD 25 31 32 48 59 61
	./random_testu01_bigcrush_ranlux48_base U01 25 34 48 59 60 61
	./random_testu01_bigcrush_ranlux48_base U01CC 53 54 76 77 89
	./random_testu01_bigcrush_ranlux48_base U01CO 53 54 76 77
	./random_testu01_bigcrush_ranlux48_base U01OC 14 53 54 77
	./random_testu01_bigcrush_ranlux48_base U01OO 53 54 76 77

random_testu01_bigcrush_threefish1024 :
	ninja -C ../.. random_testu01_bigcrush_threefish1024
	./random_testu01_bigcrush_threefish1024 STD 56

random_testu01_bigcrush_threefish1024_64 :
	ninja -C ../.. random_testu01_bigcrush_threefish1024_64
	./random_testu01_bigcrush_threefish1024_64 STD 56
	./random_testu01_bigcrush_threefish1024_64 U01OO 81

random_testu01_bigcrush_threefish256 :
	ninja -C ../.. random_testu01_bigcrush_threefish256
	./random_testu01_bigcrush_threefish256 U01CO 79
	./random_testu01_bigcrush_threefish256 U01OC 49

random_testu01_bigcrush_threefish256_64 :
	ninja -C ../.. random_testu01_bigcrush_threefish256_64
	./random_testu01_bigcrush_threefish256_64 U01OC 9

random_testu01_bigcrush_threefish512 :
	ninja -C ../.. random_testu01_bigcrush_threefish512
	./random_testu01_bigcrush_threefish512 U01 82
	./random_testu01_bigcrush_threefish512 U01CO 11 42
	./random_testu01_bigcrush_threefish512 U01OO 39 78

random_testu01_bigcrush_threefish512_64 :
	ninja -C ../.. random_testu01_bigcrush_threefish512_64
	./random_testu01_bigcrush_threefish512_64 U01 82
	./random_testu01_bigcrush_threefish512_64 U01CC 82

random_testu01_bigcrush_threefry16x64 :
	ninja -C ../.. random_testu01_bigcrush_threefry16x64
	./random_testu01_bigcrush_threefry16x64 U01 11 25
	./random_testu01_bigcrush_threefry16x64 U01CC 76
	./random_testu01_bigcrush_threefry16x64 U01OO 1

random_testu01_bigcrush_threefry16x64_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry16x64_64
	./random_testu01_bigcrush_threefry16x64_64 U01 11 25
	./random_testu01_bigcrush_threefry16x64_64 U01OC 77
	./random_testu01_bigcrush_threefry16x64_64 U01OO 10 88

random_testu01_bigcrush_threefry2x32 :
	ninja -C ../.. random_testu01_bigcrush_threefry2x32
	./random_testu01_bigcrush_threefry2x32 U01 8

random_testu01_bigcrush_threefry2x32_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry2x32_64
	./random_testu01_bigcrush_threefry2x32_64 U01 8
	./random_testu01_bigcrush_threefry2x32_64 U01CC 89

random_testu01_bigcrush_threefry2x64 :
	ninja -C ../.. random_testu01_bigcrush_threefry2x64
	./random_testu01_bigcrush_threefry2x64 U01CO 106
	./random_testu01_bigcrush_threefry2x64 U01OC 74

random_testu01_bigcrush_threefry2x64_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry2x64_64
	./random_testu01_bigcrush_threefry2x64_64 U01OC 70

random_testu01_bigcrush_threefry4x32_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry4x32_64
	./random_testu01_bigcrush_threefry4x32_64 U01OC 79

random_testu01_bigcrush_threefry4x64 :
	ninja -C ../.. random_testu01_bigcrush_threefry4x64
	./random_testu01_bigcrush_threefry4x64 U01 75 77
	./random_testu01_bigcrush_threefry4x64 U01CO 49
	./random_testu01_bigcrush_threefry4x64 U01OC 58

random_testu01_bigcrush_threefry4x64_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry4x64_64
	./random_testu01_bigcrush_threefry4x64_64 U01 75 77
	./random_testu01_bigcrush_threefry4x64_64 U01CO 22 3 39
	./random_testu01_bigcrush_threefry4x64_64 U01OC 96
	./random_testu01_bigcrush_threefry4x64_64 U01OO 31 75

random_testu01_bigcrush_threefry8x64 :
	ninja -C ../.. random_testu01_bigcrush_threefry8x64
	./random_testu01_bigcrush_threefry8x64 U01CO 12

random_testu01_bigcrush_threefry8x64_64 :
	ninja -C ../.. random_testu01_bigcrush_threefry8x64_64
	./random_testu01_bigcrush_threefry8x64_64 U01CO 102
	./random_testu01_bigcrush_threefry8x64_64 U01OC 25

# vim:ft=make
