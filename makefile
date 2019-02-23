test_lidar: test/lidar.cpp
	$(CC) $< -o $@ -lydlidar_driver -L ./sdk/lib/ -I ./sdk/include/

clean:
	rm test_lidar

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
