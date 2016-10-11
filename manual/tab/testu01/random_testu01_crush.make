.PHONY : all run \
	random_testu01_crush_philox2x32_64 \
	random_testu01_crush_philox2x64 \
	random_testu01_crush_philox2x64_64 \
	random_testu01_crush_philox4x32_64 \
	random_testu01_crush_philox4x64 \
	random_testu01_crush_philox4x64_64 \
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
	random_testu01_crush_philox2x32_64 \
	random_testu01_crush_philox2x64 \
	random_testu01_crush_philox2x64_64 \
	random_testu01_crush_philox4x32_64 \
	random_testu01_crush_philox4x64 \
	random_testu01_crush_philox4x64_64 \
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
