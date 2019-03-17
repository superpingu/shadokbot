LDFLAGS = -lm -lstdc++ -lcppunit
CPPFLAGS = -I test/ -I sdk/include -I src/lidar

test_lidar: test/lidar.cpp test/mockup_CYdLidar.o src/lidar/lidar.o src/lidar/obstacle.o test/obstacleTest.o
	$(CC) $^ -o $@  $(CPPFLAGS) $(LDFLAGS)

test/mockup_CYdLidar.o: test/mockup_CYdLidar.cpp
	cp sdk/include/CYdLidar.h test/
	touch test/utils.h
	$(CC) -c test/mockup_CYdLidar.cpp -o test/mockup_CYdLidar.o -I test/

target:
	$(CC) $< -o $@ -lydlidar_driver -L ./sdk/lib/ -I ./sdk/include/

clean:
	rm -f test_lidar
	rm -f test/utils.h
	rm -f test/*.o
	rm -f src/lidar/*.o
	rm -f test/CYdLidar.h
	rm -f test/utils.h
	rm -f sdk/CMakeCache.txt
	rm -rf sdk/CMakeFiles/
	rm -f sdk/Makefile
	rm -f sdk/cmake_install.cmake
	rm -rf sdk/lib/
	rm -rf sdk/samples/CMakeFiles/
	rm -f sdk/samples/Makefile
	rm -f sdk/samples/cmake_install.cmake
	rm -f sdk/samples/ydlidar_test

sdk_clean:
	rm -rf sdk/

dir_clean: clean sdk_clean
	rm sdk.zip

lib: sdk
	cd sdk/ && cmake CMakeLists.txt -DARDUINO:BOOL=ON && make

sdk: sdk.zip
	unzip sdk.zip
	unzip SDK/SDK╟¤╢п░№/sdk_v1.3.0.zip
	rm -rf SDK/

sdk.zip:
	wget http://www.ydlidar.com/en/download/11
	mv 11 sdk.zip
