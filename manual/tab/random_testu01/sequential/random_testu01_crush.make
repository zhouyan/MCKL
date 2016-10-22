.PHONY : all run \
	random_testu01_crush_aes128 \
	random_testu01_crush_aes192 \
	random_testu01_crush_aes192_64 \
	random_testu01_crush_aes256 \
	random_testu01_crush_aes256_64 \
	random_testu01_crush_ars \
	random_testu01_crush_ars_64 \
	random_testu01_crush_knuth_b \
	random_testu01_crush_minstd_rand \
	random_testu01_crush_minstd_rand0 \
	random_testu01_crush_mkl_ars5 \
	random_testu01_crush_mkl_ars5_64 \
	random_testu01_crush_mkl_mcg59 \
	random_testu01_crush_mkl_mcg59_64 \
	random_testu01_crush_mkl_mt19937_64 \
	random_testu01_crush_mkl_mt2203 \
	random_testu01_crush_mkl_nondeterm \
	random_testu01_crush_mkl_philox4x32x10 \
	random_testu01_crush_mkl_philox4x32x10_64 \
	random_testu01_crush_mkl_sfmt19937 \
	random_testu01_crush_mkl_sfmt19937_64 \
	random_testu01_crush_mt19937 \
	random_testu01_crush_mt19937_64 \
	random_testu01_crush_philox2x32 \
	random_testu01_crush_philox2x32_64 \
	random_testu01_crush_philox2x64 \
	random_testu01_crush_philox2x64_64 \
	random_testu01_crush_philox4x32_64 \
	random_testu01_crush_ranlux24 \
	random_testu01_crush_ranlux24_base \
	random_testu01_crush_ranlux48_base \
	random_testu01_crush_rdrand16 \
	random_testu01_crush_rdrand32 \
	random_testu01_crush_rdrand64 \
	random_testu01_crush_threefish1024 \
	random_testu01_crush_threefish256 \
	random_testu01_crush_threefish512 \
	random_testu01_crush_threefish512_64 \
	random_testu01_crush_threefry16x64 \
	random_testu01_crush_threefry16x64_64 \
	random_testu01_crush_threefry2x32_64 \
	random_testu01_crush_threefry2x64 \
	random_testu01_crush_threefry2x64_64 \
	random_testu01_crush_threefry4x32 \
	random_testu01_crush_threefry4x32_64 \
	random_testu01_crush_threefry4x64 \
	random_testu01_crush_threefry4x64_64 \
	random_testu01_crush_threefry8x64
all :

run : \
	random_testu01_crush_aes128 \
	random_testu01_crush_aes192 \
	random_testu01_crush_aes192_64 \
	random_testu01_crush_aes256 \
	random_testu01_crush_aes256_64 \
	random_testu01_crush_ars \
	random_testu01_crush_ars_64 \
	random_testu01_crush_knuth_b \
	random_testu01_crush_minstd_rand \
	random_testu01_crush_minstd_rand0 \
	random_testu01_crush_mkl_ars5 \
	random_testu01_crush_mkl_ars5_64 \
	random_testu01_crush_mkl_mcg59 \
	random_testu01_crush_mkl_mcg59_64 \
	random_testu01_crush_mkl_mt19937_64 \
	random_testu01_crush_mkl_mt2203 \
	random_testu01_crush_mkl_nondeterm \
	random_testu01_crush_mkl_philox4x32x10 \
	random_testu01_crush_mkl_philox4x32x10_64 \
	random_testu01_crush_mkl_sfmt19937 \
	random_testu01_crush_mkl_sfmt19937_64 \
	random_testu01_crush_mt19937 \
	random_testu01_crush_mt19937_64 \
	random_testu01_crush_philox2x32 \
	random_testu01_crush_philox2x32_64 \
	random_testu01_crush_philox2x64 \
	random_testu01_crush_philox2x64_64 \
	random_testu01_crush_philox4x32_64 \
	random_testu01_crush_ranlux24 \
	random_testu01_crush_ranlux24_base \
	random_testu01_crush_ranlux48_base \
	random_testu01_crush_rdrand16 \
	random_testu01_crush_rdrand32 \
	random_testu01_crush_rdrand64 \
	random_testu01_crush_threefish1024 \
	random_testu01_crush_threefish256 \
	random_testu01_crush_threefish512 \
	random_testu01_crush_threefish512_64 \
	random_testu01_crush_threefry16x64 \
	random_testu01_crush_threefry16x64_64 \
	random_testu01_crush_threefry2x32_64 \
	random_testu01_crush_threefry2x64 \
	random_testu01_crush_threefry2x64_64 \
	random_testu01_crush_threefry4x32 \
	random_testu01_crush_threefry4x32_64 \
	random_testu01_crush_threefry4x64 \
	random_testu01_crush_threefry4x64_64 \
	random_testu01_crush_threefry8x64

random_testu01_crush_aes128 :
	ninja -C ../.. random_testu01_crush_aes128
	./random_testu01_crush_aes128 U01CO 10 41
	./random_testu01_crush_aes128 U01OO 9

random_testu01_crush_aes192 :
	ninja -C ../.. random_testu01_crush_aes192
	./random_testu01_crush_aes192 STD 19
	./random_testu01_crush_aes192 U01CC 19
	./random_testu01_crush_aes192 U01CO 96

random_testu01_crush_aes192_64 :
	ninja -C ../.. random_testu01_crush_aes192_64
	./random_testu01_crush_aes192_64 STD 19
	./random_testu01_crush_aes192_64 U01CO 48 63

random_testu01_crush_aes256 :
	ninja -C ../.. random_testu01_crush_aes256
	./random_testu01_crush_aes256 U01 35

random_testu01_crush_aes256_64 :
	ninja -C ../.. random_testu01_crush_aes256_64
	./random_testu01_crush_aes256_64 U01 35

random_testu01_crush_ars :
	ninja -C ../.. random_testu01_crush_ars
	./random_testu01_crush_ars U01CO 41
	./random_testu01_crush_ars U01OC 12

random_testu01_crush_ars_64 :
	ninja -C ../.. random_testu01_crush_ars_64
	./random_testu01_crush_ars_64 U01OC 75

random_testu01_crush_knuth_b :
	ninja -C ../.. random_testu01_crush_knuth_b
	./random_testu01_crush_knuth_b STD 79
	./random_testu01_crush_knuth_b U01CO 41 91
	./random_testu01_crush_knuth_b U01OC 80

random_testu01_crush_minstd_rand :
	ninja -C ../.. random_testu01_crush_minstd_rand
	./random_testu01_crush_minstd_rand STD 39 40 65 78 9
	./random_testu01_crush_minstd_rand U01 32 37 76
	./random_testu01_crush_minstd_rand U01CC 39 78
	./random_testu01_crush_minstd_rand U01CO 10 39 76
	./random_testu01_crush_minstd_rand U01OC 10 76 9
	./random_testu01_crush_minstd_rand U01OO 10 39 41 73 76 78 91

random_testu01_crush_minstd_rand0 :
	ninja -C ../.. random_testu01_crush_minstd_rand0
	./random_testu01_crush_minstd_rand0 STD 75 78 9
	./random_testu01_crush_minstd_rand0 U01 76 80 85
	./random_testu01_crush_minstd_rand0 U01CC 10 65 66 74
	./random_testu01_crush_minstd_rand0 U01CO 10 29 75 9
	./random_testu01_crush_minstd_rand0 U01OC 39 65 74 9
	./random_testu01_crush_minstd_rand0 U01OO 74 9

random_testu01_crush_mkl_ars5 :
	ninja -C ../.. random_testu01_crush_mkl_ars5
	./random_testu01_crush_mkl_ars5 U01CC 20
	./random_testu01_crush_mkl_ars5 U01CO 19

random_testu01_crush_mkl_ars5_64 :
	ninja -C ../.. random_testu01_crush_mkl_ars5_64
	./random_testu01_crush_mkl_ars5_64 U01OC 29

random_testu01_crush_mkl_mcg59 :
	ninja -C ../.. random_testu01_crush_mkl_mcg59
	./random_testu01_crush_mkl_mcg59 U01 10
	./random_testu01_crush_mkl_mcg59 U01CC 24
	./random_testu01_crush_mkl_mcg59 U01CO 24
	./random_testu01_crush_mkl_mcg59 U01OO 10 70 9

random_testu01_crush_mkl_mcg59_64 :
	ninja -C ../.. random_testu01_crush_mkl_mcg59_64
	./random_testu01_crush_mkl_mcg59_64 U01 10
	./random_testu01_crush_mkl_mcg59_64 U01CO 86
	./random_testu01_crush_mkl_mcg59_64 U01OC 84
	./random_testu01_crush_mkl_mcg59_64 U01OO 46 87

random_testu01_crush_mkl_mt19937_64 :
	ninja -C ../.. random_testu01_crush_mkl_mt19937_64
	./random_testu01_crush_mkl_mt19937_64 U01OC 9

random_testu01_crush_mkl_mt2203 :
	ninja -C ../.. random_testu01_crush_mkl_mt2203
	./random_testu01_crush_mkl_mt2203 U01OO 9

random_testu01_crush_mkl_nondeterm :
	ninja -C ../.. random_testu01_crush_mkl_nondeterm
	./random_testu01_crush_mkl_nondeterm U01OC 10 67

random_testu01_crush_mkl_philox4x32x10 :
	ninja -C ../.. random_testu01_crush_mkl_philox4x32x10
	./random_testu01_crush_mkl_philox4x32x10 STD 27 56
	./random_testu01_crush_mkl_philox4x32x10 U01 21
	./random_testu01_crush_mkl_philox4x32x10 U01CC 44
	./random_testu01_crush_mkl_philox4x32x10 U01CO 32 65
	./random_testu01_crush_mkl_philox4x32x10 U01OC 66
	./random_testu01_crush_mkl_philox4x32x10 U01OO 25

random_testu01_crush_mkl_philox4x32x10_64 :
	ninja -C ../.. random_testu01_crush_mkl_philox4x32x10_64
	./random_testu01_crush_mkl_philox4x32x10_64 STD 27 56
	./random_testu01_crush_mkl_philox4x32x10_64 U01 21
	./random_testu01_crush_mkl_philox4x32x10_64 U01OC 65

random_testu01_crush_mkl_sfmt19937 :
	ninja -C ../.. random_testu01_crush_mkl_sfmt19937
	./random_testu01_crush_mkl_sfmt19937 U01 62

random_testu01_crush_mkl_sfmt19937_64 :
	ninja -C ../.. random_testu01_crush_mkl_sfmt19937_64
	./random_testu01_crush_mkl_sfmt19937_64 U01 62
	./random_testu01_crush_mkl_sfmt19937_64 U01CO 48
	./random_testu01_crush_mkl_sfmt19937_64 U01OC 18
	./random_testu01_crush_mkl_sfmt19937_64 U01OO 86

random_testu01_crush_mt19937 :
	ninja -C ../.. random_testu01_crush_mt19937
	./random_testu01_crush_mt19937 STD 9

random_testu01_crush_mt19937_64 :
	ninja -C ../.. random_testu01_crush_mt19937_64
	./random_testu01_crush_mt19937_64 U01 11

random_testu01_crush_philox2x32 :
	ninja -C ../.. random_testu01_crush_philox2x32
	./random_testu01_crush_philox2x32 STD 88

random_testu01_crush_philox2x32_64 :
	ninja -C ../.. random_testu01_crush_philox2x32_64
	./random_testu01_crush_philox2x32_64 STD 88

random_testu01_crush_philox2x64 :
	ninja -C ../.. random_testu01_crush_philox2x64
	./random_testu01_crush_philox2x64 STD 66

random_testu01_crush_philox2x64_64 :
	ninja -C ../.. random_testu01_crush_philox2x64_64
	./random_testu01_crush_philox2x64_64 STD 66
	./random_testu01_crush_philox2x64_64 U01CC 14 22

random_testu01_crush_philox4x32_64 :
	ninja -C ../.. random_testu01_crush_philox4x32_64
	./random_testu01_crush_philox4x32_64 U01CO 89 91

random_testu01_crush_ranlux24 :
	ninja -C ../.. random_testu01_crush_ranlux24
	./random_testu01_crush_ranlux24 U01 48
	./random_testu01_crush_ranlux24 U01OO 18 20

random_testu01_crush_ranlux24_base :
	ninja -C ../.. random_testu01_crush_ranlux24_base
	./random_testu01_crush_ranlux24_base U01 27 28 46
	./random_testu01_crush_ranlux24_base U01OC 38 68

random_testu01_crush_ranlux48_base :
	ninja -C ../.. random_testu01_crush_ranlux48_base
	./random_testu01_crush_ranlux48_base U01 27 46 49
	./random_testu01_crush_ranlux48_base U01CO 68
	./random_testu01_crush_ranlux48_base U01OC 67 68
	./random_testu01_crush_ranlux48_base U01OO 81

random_testu01_crush_rdrand16 :
	ninja -C ../.. random_testu01_crush_rdrand16
	./random_testu01_crush_rdrand16 U01CO 58
	./random_testu01_crush_rdrand16 U01OC 50
	./random_testu01_crush_rdrand16 U01OO 1

random_testu01_crush_rdrand32 :
	ninja -C ../.. random_testu01_crush_rdrand32
	./random_testu01_crush_rdrand32 U01OC 95

random_testu01_crush_rdrand64 :
	ninja -C ../.. random_testu01_crush_rdrand64
	./random_testu01_crush_rdrand64 STD 9

random_testu01_crush_threefish1024 :
	ninja -C ../.. random_testu01_crush_threefish1024
	./random_testu01_crush_threefish1024 U01CO 67

random_testu01_crush_threefish256 :
	ninja -C ../.. random_testu01_crush_threefish256
	./random_testu01_crush_threefish256 U01OC 77

random_testu01_crush_threefish512 :
	ninja -C ../.. random_testu01_crush_threefish512
	./random_testu01_crush_threefish512 STD 13

random_testu01_crush_threefish512_64 :
	ninja -C ../.. random_testu01_crush_threefish512_64
	./random_testu01_crush_threefish512_64 STD 13
	./random_testu01_crush_threefish512_64 U01CO 68
	./random_testu01_crush_threefish512_64 U01OC 65

random_testu01_crush_threefry16x64 :
	ninja -C ../.. random_testu01_crush_threefry16x64
	./random_testu01_crush_threefry16x64 U01CO 63
	./random_testu01_crush_threefry16x64 U01OO 24

random_testu01_crush_threefry16x64_64 :
	ninja -C ../.. random_testu01_crush_threefry16x64_64
	./random_testu01_crush_threefry16x64_64 U01OO 1

random_testu01_crush_threefry2x32_64 :
	ninja -C ../.. random_testu01_crush_threefry2x32_64
	./random_testu01_crush_threefry2x32_64 U01OO 54

random_testu01_crush_threefry2x64 :
	ninja -C ../.. random_testu01_crush_threefry2x64
	./random_testu01_crush_threefry2x64 U01 68 86

random_testu01_crush_threefry2x64_64 :
	ninja -C ../.. random_testu01_crush_threefry2x64_64
	./random_testu01_crush_threefry2x64_64 U01 68 86
	./random_testu01_crush_threefry2x64_64 U01OC 66

random_testu01_crush_threefry4x32 :
	ninja -C ../.. random_testu01_crush_threefry4x32
	./random_testu01_crush_threefry4x32 U01 68
	./random_testu01_crush_threefry4x32 U01CC 15

random_testu01_crush_threefry4x32_64 :
	ninja -C ../.. random_testu01_crush_threefry4x32_64
	./random_testu01_crush_threefry4x32_64 U01 68
	./random_testu01_crush_threefry4x32_64 U01CO 61

random_testu01_crush_threefry4x64 :
	ninja -C ../.. random_testu01_crush_threefry4x64
	./random_testu01_crush_threefry4x64 U01CC 71
	./random_testu01_crush_threefry4x64 U01OC 82

random_testu01_crush_threefry4x64_64 :
	ninja -C ../.. random_testu01_crush_threefry4x64_64
	./random_testu01_crush_threefry4x64_64 U01CC 81
	./random_testu01_crush_threefry4x64_64 U01OC 71

random_testu01_crush_threefry8x64 :
	ninja -C ../.. random_testu01_crush_threefry8x64
	./random_testu01_crush_threefry8x64 U01OO 16

# vim:ft=make
