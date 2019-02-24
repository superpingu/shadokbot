#include "lidar.h"
#include <stdio.h>
#include <limits>

#define ABS(x) ((x) > 0 ? (x) : -(x))

template <typename T> typename std::vector<T>::iterator circular_prev(std::vector<T>& vec, typename std::vector<T>::iterator it) {
  if (it == vec.begin()) {
    return std::prev(vec.end());
  } else {
    return std::prev(it);
  }
}

template <typename T> typename std::vector<T>::iterator circular_next(std::vector<T>& vec, typename std::vector<T>::iterator it) {
  if (std::next(it) == vec.end()) {
    return vec.begin();
  } else {
    return std::next(it);
  }
}

int clean_data(std::vector<float>& data) {
  int invalid_data_count = 0;
  bool warning = false;

  for (std::vector<float>::iterator it = data.begin(); it != data.end(); ++it) {
    if (ABS(*it - *circular_prev(data, it)) > 0.5) {
      if (warning) {
        printf("value %f %f\n", *it, *circular_prev(data, it));
        *circular_prev(data, it) = 0;
        invalid_data_count++;
      }
      warning = true;
    } else {
      warning = false;
    }
  }

  return invalid_data_count;
}

std::tuple<int,float> closest_obstacle(std::vector<float> scan) {
  std::tuple<int,float> closest (0,std::numeric_limits<float>::max());
  int index = 0;

  for (std::vector<float>::iterator it = scan.begin(); it != scan.end(); ++it) {
    if ((*it > 0.01) && (*it < std::get<1>(closest))) {
      std::get<0>(closest) = index;
      std::get<1>(closest) = *it;
    }
    index++;
  }

  return closest;
}
