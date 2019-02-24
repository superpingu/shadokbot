#ifndef LIDAR_H
#define LIDAR_H

#include <vector>
#include <tuple>

int clean_data(std::vector<float>& data);

std::tuple<int,float> closest_obstacle(std::vector<float> scan);

#endif // LIDAR_H
