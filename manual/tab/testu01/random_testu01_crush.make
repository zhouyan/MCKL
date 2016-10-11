.PHONY : all run \
	random_testu01_crush_aes128 \
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
	random_testu01_crush_mkl_mt19937 \
	random_testu01_crush_mkl_mt19937_64 \
	random_testu01_crush_mkl_nondeterm \
	random_testu01_crush_mkl_nondeterm_64 \
	random_testu01_crush_mkl_philox4x32x10 \
	random_testu01_crush_mkl_philox4x32x10_64 \
	random_testu01_crush_mkl_sfmt19937 \
	random_testu01_crush_mkl_sfmt19937_64 \
	random_testu01_crush_mt19937 \
	random_testu01_crush_mt19937_64 \
	random_testu01_crush_philox2x32_64 \
	random_testu01_crush_philox2x64 \
	random_testu01_crush_philox2x64_64 \
	random_testu01_crush_philox4x32_64 \
	random_testu01_crush_philox4x64 \
	random_testu01_crush_philox4x64_64 \
	random_testu01_crush_ranlux24_base \
	random_testu01_crush_ranlux48_base \
	random_testu01_crush_threefish1024 \
	random_testu01_crush_threefish1024_64 \
	random_testu01_crush_threefish256 \
	random_testu01_crush_threefish512 \
	random_testu01_crush_threefish512_64 \
	random_testu01_crush_threefry16x64 \
	random_testu01_crush_threefry16x64_64 \
	random_testu01_crush_threefry2x32 \
	random_testu01_crush_threefry2x64 \
	random_testu01_crush_threefry2x64_64 \
	random_testu01_crush_threefry4x32 \
	random_testu01_crush_threefry4x32_64 \
	random_testu01_crush_threefry4x64 \
	random_testu01_crush_threefry4x64_64 \
	random_testu01_crush_threefry8x64 \
	random_testu01_crush_threefry8x64_64
all :

run : \
	random_testu01_crush_aes128 \
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
	random_testu01_crush_mkl_mt19937 \
	random_testu01_crush_mkl_mt19937_64 \
	random_testu01_crush_mkl_nondeterm \
	random_testu01_crush_mkl_nondeterm_64 \
	random_testu01_crush_mkl_philox4x32x10 \
	random_testu01_crush_mkl_philox4x32x10_64 \
	random_testu01_crush_mkl_sfmt19937 \
	random_testu01_crush_mkl_sfmt19937_64 \
	random_testu01_crush_mt19937 \
	random_testu01_crush_mt19937_64 \
	random_testu01_crush_philox2x32_64 \
	random_testu01_crush_philox2x64 \
	random_testu01_crush_philox2x64_64 \
	random_testu01_crush_philox4x32_64 \
	random_testu01_crush_philox4x64 \
	random_testu01_crush_philox4x64_64 \
	random_testu01_crush_ranlux24_base \
	random_testu01_crush_ranlux48_base \
	random_testu01_crush_threefish1024 \
	random_testu01_crush_threefish1024_64 \
	random_testu01_crush_threefish256 \
	random_testu01_crush_threefish512 \
	random_testu01_crush_threefish512_64 \
	random_testu01_crush_threefry16x64 \
	random_testu01_crush_threefry16x64_64 \
	random_testu01_crush_threefry2x32 \
	random_testu01_crush_threefry2x64 \
	random_testu01_crush_threefry2x64_64 \
	random_testu01_crush_threefry4x32 \
	random_testu01_crush_threefry4x32_64 \
	random_testu01_crush_threefry4x64 \
	random_testu01_crush_threefry4x64_64 \
	random_testu01_crush_threefry8x64 \
	random_testu01_crush_threefry8x64_64

random_testu01_crush_aes128 :
	ninja -C ../.. random_testu01_crush_aes128
	./random_testu01_crush_aes128 U01CC 28
	./random_testu01_crush_aes128 U01OO 70

random_testu01_crush_aes192_64 :
	ninja -C ../.. random_testu01_crush_aes192_64
	./random_testu01_crush_aes192_64 U01CO 10
	./random_testu01_crush_aes192_64 U01OC 4 70
	./random_testu01_crush_aes192_64 U01OO 10

random_testu01_crush_aes256 :
	ninja -C ../.. random_testu01_crush_aes256
	./random_testu01_crush_aes256 U01 36
	./random_testu01_crush_aes256 U01CO 68
	./random_testu01_crush_aes256 U01OC 40

random_testu01_crush_aes256_64 :
	ninja -C ../.. random_testu01_crush_aes256_64
	./random_testu01_crush_aes256_64 U01 36
	./random_testu01_crush_aes256_64 U01OC 66

random_testu01_crush_ars :
	ninja -C ../.. random_testu01_crush_ars
	./random_testu01_crush_ars U01 68
	./random_testu01_crush_ars U01CC 18
	./random_testu01_crush_ars U01OC 19

random_testu01_crush_ars_64 :
	ninja -C ../.. random_testu01_crush_ars_64
	./random_testu01_crush_ars_64 U01 68
	./random_testu01_crush_ars_64 U01OC 8
	./random_testu01_crush_ars_64 U01OO 19

random_testu01_crush_knuth_b :
	ninja -C ../.. random_testu01_crush_knuth_b
	./random_testu01_crush_knuth_b STD 44 79
	./random_testu01_crush_knuth_b U01OC 76

random_testu01_crush_minstd_rand :
	ninja -C ../.. random_testu01_crush_minstd_rand
	./random_testu01_crush_minstd_rand STD 29 39 85
	./random_testu01_crush_minstd_rand U01 37 76 91
	./random_testu01_crush_minstd_rand U01CC 10 73 85 9
	./random_testu01_crush_minstd_rand U01CO 10 34 76 9
	./random_testu01_crush_minstd_rand U01OC 10 27 29 33 85 9
	./random_testu01_crush_minstd_rand U01OO 10 73 76

random_testu01_crush_minstd_rand0 :
	ninja -C ../.. random_testu01_crush_minstd_rand0
	./random_testu01_crush_minstd_rand0 STD 27 29 78 9
	./random_testu01_crush_minstd_rand0 U01 49 9
	./random_testu01_crush_minstd_rand0 U01CC 10 34 39 65 66 9
	./random_testu01_crush_minstd_rand0 U01CO 10 65
	./random_testu01_crush_minstd_rand0 U01OC 10 32 33 65 85
	./random_testu01_crush_minstd_rand0 U01OO 76 9

random_testu01_crush_mkl_ars5 :
	ninja -C ../.. random_testu01_crush_mkl_ars5
	./random_testu01_crush_mkl_ars5 U01OC 96

random_testu01_crush_mkl_ars5_64 :
	ninja -C ../.. random_testu01_crush_mkl_ars5_64
	./random_testu01_crush_mkl_ars5_64 U01CC 20
	./random_testu01_crush_mkl_ars5_64 U01CO 49
	./random_testu01_crush_mkl_ars5_64 U01OO 47

random_testu01_crush_mkl_mcg59 :
	ninja -C ../.. random_testu01_crush_mkl_mcg59
	./random_testu01_crush_mkl_mcg59 STD 30 32
	./random_testu01_crush_mkl_mcg59 U01 28
	./random_testu01_crush_mkl_mcg59 U01CC 14 18 30
	./random_testu01_crush_mkl_mcg59 U01CO 28 32
	./random_testu01_crush_mkl_mcg59 U01OC 14 28 32
	./random_testu01_crush_mkl_mcg59 U01OO 10

random_testu01_crush_mkl_mcg59_64 :
	ninja -C ../.. random_testu01_crush_mkl_mcg59_64
	./random_testu01_crush_mkl_mcg59_64 STD 30 32
	./random_testu01_crush_mkl_mcg59_64 U01 28
	./random_testu01_crush_mkl_mcg59_64 U01OO 32 36

random_testu01_crush_mkl_mt19937 :
	ninja -C ../.. random_testu01_crush_mkl_mt19937
	./random_testu01_crush_mkl_mt19937 U01 81

random_testu01_crush_mkl_mt19937_64 :
	ninja -C ../.. random_testu01_crush_mkl_mt19937_64
	./random_testu01_crush_mkl_mt19937_64 U01 81

random_testu01_crush_mkl_nondeterm :
	ninja -C ../.. random_testu01_crush_mkl_nondeterm
	./random_testu01_crush_mkl_nondeterm U01CC 69
	./random_testu01_crush_mkl_nondeterm U01CO 67

random_testu01_crush_mkl_nondeterm_64 :
	ninja -C ../.. random_testu01_crush_mkl_nondeterm_64
	./random_testu01_crush_mkl_nondeterm_64 U01CO 65

random_testu01_crush_mkl_philox4x32x10 :
	ninja -C ../.. random_testu01_crush_mkl_philox4x32x10
	./random_testu01_crush_mkl_philox4x32x10 U01 20
	./random_testu01_crush_mkl_philox4x32x10 U01CC 21

random_testu01_crush_mkl_philox4x32x10_64 :
	ninja -C ../.. random_testu01_crush_mkl_philox4x32x10_64
	./random_testu01_crush_mkl_philox4x32x10_64 U01 20
	./random_testu01_crush_mkl_philox4x32x10_64 U01CC 43

random_testu01_crush_mkl_sfmt19937 :
	ninja -C ../.. random_testu01_crush_mkl_sfmt19937
	./random_testu01_crush_mkl_sfmt19937 U01 73
	./random_testu01_crush_mkl_sfmt19937 U01OC 95
	./random_testu01_crush_mkl_sfmt19937 U01OO 90

random_testu01_crush_mkl_sfmt19937_64 :
	ninja -C ../.. random_testu01_crush_mkl_sfmt19937_64
	./random_testu01_crush_mkl_sfmt19937_64 U01 73
	./random_testu01_crush_mkl_sfmt19937_64 U01CO 34
	./random_testu01_crush_mkl_sfmt19937_64 U01OO 14

random_testu01_crush_mt19937 :
	ninja -C ../.. random_testu01_crush_mt19937
	./random_testu01_crush_mt19937 U01OC 34 9

random_testu01_crush_mt19937_64 :
	ninja -C ../.. random_testu01_crush_mt19937_64
	./random_testu01_crush_mt19937_64 STD 9
	./random_testu01_crush_mt19937_64 U01OO 56 79 90

random_testu01_crush_philox2x32_64 :
	ninja -C ../.. random_testu01_crush_philox2x32_64
	./random_testu01_crush_philox2x32_64 U01CO 46

random_testu01_crush_philox2x64 :
	ninja -C ../.. random_testu01_crush_philox2x64
	./random_testu01_crush_philox2x64 U01CO 19
	./random_testu01_crush_philox2x64 U01OC 65
	./random_testu01_crush_philox2x64 U01OO 65

random_testu01_crush_philox2x64_64 :
	ninja -C ../.. random_testu01_crush_philox2x64_64
	./random_testu01_crush_philox2x64_64 U01CC 73

random_testu01_crush_philox4x32_64 :
	ninja -C ../.. random_testu01_crush_philox4x32_64
	./random_testu01_crush_philox4x32_64 U01CC 81

random_testu01_crush_philox4x64 :
	ninja -C ../.. random_testu01_crush_philox4x64
	./random_testu01_crush_philox4x64 U01CO 68

random_testu01_crush_philox4x64_64 :
	ninja -C ../.. random_testu01_crush_philox4x64_64
	./random_testu01_crush_philox4x64_64 U01CC 67
	./random_testu01_crush_philox4x64_64 U01OC 70

random_testu01_crush_ranlux24_base :
	ninja -C ../.. random_testu01_crush_ranlux24_base
	./random_testu01_crush_ranlux24_base STD 9
	./random_testu01_crush_ranlux24_base U01 27 46 53
	./random_testu01_crush_ranlux24_base U01CO 22
	./random_testu01_crush_ranlux24_base U01OC 67
	./random_testu01_crush_ranlux24_base U01OO 7

random_testu01_crush_ranlux48_base :
	ninja -C ../.. random_testu01_crush_ranlux48_base
	./random_testu01_crush_ranlux48_base STD 27 28 46
	./random_testu01_crush_ranlux48_base U01 28 46
	./random_testu01_crush_ranlux48_base U01CC 67 79

random_testu01_crush_threefish1024 :
	ninja -C ../.. random_testu01_crush_threefish1024
	./random_testu01_crush_threefish1024 STD 37
	./random_testu01_crush_threefish1024 U01CC 29

random_testu01_crush_threefish1024_64 :
	ninja -C ../.. random_testu01_crush_threefish1024_64
	./random_testu01_crush_threefish1024_64 STD 37
	./random_testu01_crush_threefish1024_64 U01CO 65
	./random_testu01_crush_threefish1024_64 U01OO 32 85

random_testu01_crush_threefish256 :
	ninja -C ../.. random_testu01_crush_threefish256
	./random_testu01_crush_threefish256 U01CC 67
	./random_testu01_crush_threefish256 U01OC 62

random_testu01_crush_threefish512 :
	ninja -C ../.. random_testu01_crush_threefish512
	./random_testu01_crush_threefish512 U01 18
	./random_testu01_crush_threefish512 U01CC 87

random_testu01_crush_threefish512_64 :
	ninja -C ../.. random_testu01_crush_threefish512_64
	./random_testu01_crush_threefish512_64 U01 18
	./random_testu01_crush_threefish512_64 U01CC 66
	./random_testu01_crush_threefish512_64 U01CO 21

random_testu01_crush_threefry16x64 :
	ninja -C ../.. random_testu01_crush_threefry16x64
	./random_testu01_crush_threefry16x64 U01CO 39 69

random_testu01_crush_threefry16x64_64 :
	ninja -C ../.. random_testu01_crush_threefry16x64_64
	./random_testu01_crush_threefry16x64_64 U01OC 69

random_testu01_crush_threefry2x32 :
	ninja -C ../.. random_testu01_crush_threefry2x32
	./random_testu01_crush_threefry2x32 U01OC 66

random_testu01_crush_threefry2x64 :
	ninja -C ../.. random_testu01_crush_threefry2x64
	./random_testu01_crush_threefry2x64 U01 77 81
	./random_testu01_crush_threefry2x64 U01OO 10

random_testu01_crush_threefry2x64_64 :
	ninja -C ../.. random_testu01_crush_threefry2x64_64
	./random_testu01_crush_threefry2x64_64 U01 77 81
	./random_testu01_crush_threefry2x64_64 U01CC 31
	./random_testu01_crush_threefry2x64_64 U01OC 75

random_testu01_crush_threefry4x32 :
	ninja -C ../.. random_testu01_crush_threefry4x32
	./random_testu01_crush_threefry4x32 U01 19

random_testu01_crush_threefry4x32_64 :
	ninja -C ../.. random_testu01_crush_threefry4x32_64
	./random_testu01_crush_threefry4x32_64 U01 19
	./random_testu01_crush_threefry4x32_64 U01CC 10

random_testu01_crush_threefry4x64 :
	ninja -C ../.. random_testu01_crush_threefry4x64
	./random_testu01_crush_threefry4x64 U01 75
	./random_testu01_crush_threefry4x64 U01CC 38
	./random_testu01_crush_threefry4x64 U01OO 27

random_testu01_crush_threefry4x64_64 :
	ninja -C ../.. random_testu01_crush_threefry4x64_64
	./random_testu01_crush_threefry4x64_64 U01 75
	./random_testu01_crush_threefry4x64_64 U01CC 3
	./random_testu01_crush_threefry4x64_64 U01CO 10
	./random_testu01_crush_threefry4x64_64 U01OO 63

random_testu01_crush_threefry8x64 :
	ninja -C ../.. random_testu01_crush_threefry8x64
	./random_testu01_crush_threefry8x64 STD 68
	./random_testu01_crush_threefry8x64 U01CC 66

random_testu01_crush_threefry8x64_64 :
	ninja -C ../.. random_testu01_crush_threefry8x64_64
	./random_testu01_crush_threefry8x64_64 STD 68

# vim:ft=make
