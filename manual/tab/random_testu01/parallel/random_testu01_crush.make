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
	random_testu01_crush_ranlux48 \
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
	random_testu01_crush_ranlux48 \
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
	./random_testu01_crush_aes128 U01CC parallel 28
	./random_testu01_crush_aes128 U01OO parallel 70

random_testu01_crush_aes192_64 :
	ninja -C ../.. random_testu01_crush_aes192_64
	./random_testu01_crush_aes192_64 U01CO parallel 10
	./random_testu01_crush_aes192_64 U01OC parallel 4 70
	./random_testu01_crush_aes192_64 U01OO parallel 10

random_testu01_crush_aes256 :
	ninja -C ../.. random_testu01_crush_aes256
	./random_testu01_crush_aes256 U01 parallel 36
	./random_testu01_crush_aes256 U01CO parallel 68
	./random_testu01_crush_aes256 U01OC parallel 40

random_testu01_crush_aes256_64 :
	ninja -C ../.. random_testu01_crush_aes256_64
	./random_testu01_crush_aes256_64 U01 parallel 36
	./random_testu01_crush_aes256_64 U01OC parallel 66

random_testu01_crush_ars :
	ninja -C ../.. random_testu01_crush_ars
	./random_testu01_crush_ars U01 parallel 68
	./random_testu01_crush_ars U01CC parallel 18
	./random_testu01_crush_ars U01OC parallel 19

random_testu01_crush_ars_64 :
	ninja -C ../.. random_testu01_crush_ars_64
	./random_testu01_crush_ars_64 U01 parallel 68
	./random_testu01_crush_ars_64 U01OC parallel 8
	./random_testu01_crush_ars_64 U01OO parallel 19

random_testu01_crush_knuth_b :
	ninja -C ../.. random_testu01_crush_knuth_b
	./random_testu01_crush_knuth_b STD parallel 44 79
	./random_testu01_crush_knuth_b U01OC parallel 76

random_testu01_crush_minstd_rand :
	ninja -C ../.. random_testu01_crush_minstd_rand
	./random_testu01_crush_minstd_rand STD parallel 29 39 85
	./random_testu01_crush_minstd_rand U01 parallel 37 76 91
	./random_testu01_crush_minstd_rand U01CC parallel 10 73 85 9
	./random_testu01_crush_minstd_rand U01CO parallel 10 34 76 9
	./random_testu01_crush_minstd_rand U01OC parallel 10 27 29 33 85 9
	./random_testu01_crush_minstd_rand U01OO parallel 10 73 76

random_testu01_crush_minstd_rand0 :
	ninja -C ../.. random_testu01_crush_minstd_rand0
	./random_testu01_crush_minstd_rand0 STD parallel 27 29 78 9
	./random_testu01_crush_minstd_rand0 U01 parallel 49 9
	./random_testu01_crush_minstd_rand0 U01CC parallel 10 34 39 65 66 9
	./random_testu01_crush_minstd_rand0 U01CO parallel 10 65
	./random_testu01_crush_minstd_rand0 U01OC parallel 10 32 33 65 85
	./random_testu01_crush_minstd_rand0 U01OO parallel 76 9

random_testu01_crush_mkl_ars5 :
	ninja -C ../.. random_testu01_crush_mkl_ars5
	./random_testu01_crush_mkl_ars5 U01OC parallel 96

random_testu01_crush_mkl_ars5_64 :
	ninja -C ../.. random_testu01_crush_mkl_ars5_64
	./random_testu01_crush_mkl_ars5_64 U01CC parallel 20
	./random_testu01_crush_mkl_ars5_64 U01CO parallel 49
	./random_testu01_crush_mkl_ars5_64 U01OO parallel 47

random_testu01_crush_mkl_mcg59 :
	ninja -C ../.. random_testu01_crush_mkl_mcg59
	./random_testu01_crush_mkl_mcg59 STD parallel 30 32
	./random_testu01_crush_mkl_mcg59 U01 parallel 28
	./random_testu01_crush_mkl_mcg59 U01CC parallel 14 18 30
	./random_testu01_crush_mkl_mcg59 U01CO parallel 28 32
	./random_testu01_crush_mkl_mcg59 U01OC parallel 14 28 32
	./random_testu01_crush_mkl_mcg59 U01OO parallel 10

random_testu01_crush_mkl_mcg59_64 :
	ninja -C ../.. random_testu01_crush_mkl_mcg59_64
	./random_testu01_crush_mkl_mcg59_64 STD parallel 30 32
	./random_testu01_crush_mkl_mcg59_64 U01 parallel 28
	./random_testu01_crush_mkl_mcg59_64 U01OO parallel 32 36

random_testu01_crush_mkl_mt19937 :
	ninja -C ../.. random_testu01_crush_mkl_mt19937
	./random_testu01_crush_mkl_mt19937 U01 parallel 81

random_testu01_crush_mkl_mt19937_64 :
	ninja -C ../.. random_testu01_crush_mkl_mt19937_64
	./random_testu01_crush_mkl_mt19937_64 U01 parallel 81

random_testu01_crush_mkl_philox4x32x10 :
	ninja -C ../.. random_testu01_crush_mkl_philox4x32x10
	./random_testu01_crush_mkl_philox4x32x10 U01 parallel 20
	./random_testu01_crush_mkl_philox4x32x10 U01CC parallel 21

random_testu01_crush_mkl_philox4x32x10_64 :
	ninja -C ../.. random_testu01_crush_mkl_philox4x32x10_64
	./random_testu01_crush_mkl_philox4x32x10_64 U01 parallel 20
	./random_testu01_crush_mkl_philox4x32x10_64 U01CC parallel 43

random_testu01_crush_mkl_sfmt19937 :
	ninja -C ../.. random_testu01_crush_mkl_sfmt19937
	./random_testu01_crush_mkl_sfmt19937 U01 parallel 73
	./random_testu01_crush_mkl_sfmt19937 U01OC parallel 95
	./random_testu01_crush_mkl_sfmt19937 U01OO parallel 90

random_testu01_crush_mkl_sfmt19937_64 :
	ninja -C ../.. random_testu01_crush_mkl_sfmt19937_64
	./random_testu01_crush_mkl_sfmt19937_64 U01 parallel 73
	./random_testu01_crush_mkl_sfmt19937_64 U01CO parallel 34
	./random_testu01_crush_mkl_sfmt19937_64 U01OO parallel 14

random_testu01_crush_mt19937 :
	ninja -C ../.. random_testu01_crush_mt19937
	./random_testu01_crush_mt19937 U01OC parallel 34 9

random_testu01_crush_mt19937_64 :
	ninja -C ../.. random_testu01_crush_mt19937_64
	./random_testu01_crush_mt19937_64 STD parallel 9
	./random_testu01_crush_mt19937_64 U01OO parallel 56 79 90

random_testu01_crush_philox2x32_64 :
	ninja -C ../.. random_testu01_crush_philox2x32_64
	./random_testu01_crush_philox2x32_64 U01CO parallel 46

random_testu01_crush_philox2x64 :
	ninja -C ../.. random_testu01_crush_philox2x64
	./random_testu01_crush_philox2x64 U01CO parallel 19
	./random_testu01_crush_philox2x64 U01OC parallel 65
	./random_testu01_crush_philox2x64 U01OO parallel 65

random_testu01_crush_philox2x64_64 :
	ninja -C ../.. random_testu01_crush_philox2x64_64
	./random_testu01_crush_philox2x64_64 U01CC parallel 73

random_testu01_crush_philox4x32_64 :
	ninja -C ../.. random_testu01_crush_philox4x32_64
	./random_testu01_crush_philox4x32_64 U01CC parallel 81

random_testu01_crush_philox4x64 :
	ninja -C ../.. random_testu01_crush_philox4x64
	./random_testu01_crush_philox4x64 U01CO parallel 68

random_testu01_crush_philox4x64_64 :
	ninja -C ../.. random_testu01_crush_philox4x64_64
	./random_testu01_crush_philox4x64_64 U01CC parallel 67
	./random_testu01_crush_philox4x64_64 U01OC parallel 70

random_testu01_crush_ranlux24_base :
	ninja -C ../.. random_testu01_crush_ranlux24_base
	./random_testu01_crush_ranlux24_base STD parallel 9
	./random_testu01_crush_ranlux24_base U01 parallel 27 46 53
	./random_testu01_crush_ranlux24_base U01CO parallel 22
	./random_testu01_crush_ranlux24_base U01OC parallel 67
	./random_testu01_crush_ranlux24_base U01OO parallel 7

random_testu01_crush_ranlux48 :
	ninja -C ../.. random_testu01_crush_ranlux48
	./random_testu01_crush_ranlux48 U01CC parallel 89
	./random_testu01_crush_ranlux48 U01OC parallel 67

random_testu01_crush_ranlux48_base :
	ninja -C ../.. random_testu01_crush_ranlux48_base
	./random_testu01_crush_ranlux48_base STD parallel 27 28 46
	./random_testu01_crush_ranlux48_base U01 parallel 28 46
	./random_testu01_crush_ranlux48_base U01CC parallel 67 79

random_testu01_crush_threefish1024 :
	ninja -C ../.. random_testu01_crush_threefish1024
	./random_testu01_crush_threefish1024 STD parallel 37
	./random_testu01_crush_threefish1024 U01CC parallel 29

random_testu01_crush_threefish1024_64 :
	ninja -C ../.. random_testu01_crush_threefish1024_64
	./random_testu01_crush_threefish1024_64 STD parallel 37
	./random_testu01_crush_threefish1024_64 U01CO parallel 65
	./random_testu01_crush_threefish1024_64 U01OO parallel 32 85

random_testu01_crush_threefish256 :
	ninja -C ../.. random_testu01_crush_threefish256
	./random_testu01_crush_threefish256 U01CC parallel 67
	./random_testu01_crush_threefish256 U01OC parallel 62

random_testu01_crush_threefish512 :
	ninja -C ../.. random_testu01_crush_threefish512
	./random_testu01_crush_threefish512 U01 parallel 18
	./random_testu01_crush_threefish512 U01CC parallel 87

random_testu01_crush_threefish512_64 :
	ninja -C ../.. random_testu01_crush_threefish512_64
	./random_testu01_crush_threefish512_64 U01 parallel 18
	./random_testu01_crush_threefish512_64 U01CC parallel 66
	./random_testu01_crush_threefish512_64 U01CO parallel 21

random_testu01_crush_threefry16x64 :
	ninja -C ../.. random_testu01_crush_threefry16x64
	./random_testu01_crush_threefry16x64 U01CO parallel 39 69

random_testu01_crush_threefry16x64_64 :
	ninja -C ../.. random_testu01_crush_threefry16x64_64
	./random_testu01_crush_threefry16x64_64 U01OC parallel 69

random_testu01_crush_threefry2x32 :
	ninja -C ../.. random_testu01_crush_threefry2x32
	./random_testu01_crush_threefry2x32 U01OC parallel 66

random_testu01_crush_threefry2x64 :
	ninja -C ../.. random_testu01_crush_threefry2x64
	./random_testu01_crush_threefry2x64 U01 parallel 77 81
	./random_testu01_crush_threefry2x64 U01OO parallel 10

random_testu01_crush_threefry2x64_64 :
	ninja -C ../.. random_testu01_crush_threefry2x64_64
	./random_testu01_crush_threefry2x64_64 U01 parallel 77 81
	./random_testu01_crush_threefry2x64_64 U01CC parallel 31
	./random_testu01_crush_threefry2x64_64 U01OC parallel 75

random_testu01_crush_threefry4x32 :
	ninja -C ../.. random_testu01_crush_threefry4x32
	./random_testu01_crush_threefry4x32 U01 parallel 19

random_testu01_crush_threefry4x32_64 :
	ninja -C ../.. random_testu01_crush_threefry4x32_64
	./random_testu01_crush_threefry4x32_64 U01 parallel 19
	./random_testu01_crush_threefry4x32_64 U01CC parallel 10

random_testu01_crush_threefry4x64 :
	ninja -C ../.. random_testu01_crush_threefry4x64
	./random_testu01_crush_threefry4x64 U01 parallel 75
	./random_testu01_crush_threefry4x64 U01CC parallel 38
	./random_testu01_crush_threefry4x64 U01OO parallel 27

random_testu01_crush_threefry4x64_64 :
	ninja -C ../.. random_testu01_crush_threefry4x64_64
	./random_testu01_crush_threefry4x64_64 U01 parallel 75
	./random_testu01_crush_threefry4x64_64 U01CC parallel 3
	./random_testu01_crush_threefry4x64_64 U01CO parallel 10
	./random_testu01_crush_threefry4x64_64 U01OO parallel 63

random_testu01_crush_threefry8x64 :
	ninja -C ../.. random_testu01_crush_threefry8x64
	./random_testu01_crush_threefry8x64 STD parallel 68
	./random_testu01_crush_threefry8x64 U01CC parallel 66

random_testu01_crush_threefry8x64_64 :
	ninja -C ../.. random_testu01_crush_threefry8x64_64
	./random_testu01_crush_threefry8x64_64 STD parallel 68

# vim:ft=make
