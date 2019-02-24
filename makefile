LDFLAGS = -lm -lstdc++

test_lidar: test/lidar.cpp test/mockup_CYdLidar.o src/lidar/lidar.o src/lidar/obstacle.o
	$(CC) $^ -o $@ -I test/ -I sdk/include -I src/lidar $(LDFLAGS)

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

sdk_clean:
	rm -rf sdk/

dir_clean: clean sdk_clean
	rm sdk.zip

sdk: sdk.zip
	unzip sdk.zip
	unzip SDK/SDK╟¤╢п░№/sdk_v1.3.0.zip
	rm -rf SDK/
	cd sdk/ && cmake CMakeLists.txt && make

sdk.zip:
	wget http://www.ydlidar.com/en/download/11
	mv 11 sdk.zip
