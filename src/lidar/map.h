#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#define MAP_SIZE 720
#define MAX_AGE 50

typedef struct Map_Data_s {
    uint32_t distance;
    uint32_t age;
} Map_Data_t;

class Map {
public:
    Map();
    /**
    * @brief Update the measured distance for the given index (angle relative,
    *         i.e. as seen by the lidar).
    * @param[in] index Relative angle
    * @param[in] distance The new measurement
    */
    int setDataPoint(uint32_t index, uint32_t distance);

    /**
    * @brief Get the distance of the first obstacle, absolute angle.
    * @ param absoluteAngle The angle for which to get the distance. It can be
    *                       outside the range [0,MAP_SIZE], in which case the angle
    *                       will be automatically brought back to this range.
    *                       It increases counterclockwise.
    */
    uint32_t getDistance(int32_t absoluteAngle);
    void setRobotAngle(uint32_t newRobotAngle);
    void incrementAge();
    void print();
private:
    /**
    * Indexes correspond to the angles as seen by the lidar. These angles are
    * thus relative to the robot.
    */
    Map_Data_t data[MAP_SIZE];

    /**
    * We need to know the robot orientation to convert relative angles to
    * absolute angles.
    */
    uint32_t robotAngle;

    uint32_t getIndexFromAbsoluteAngle(uint32_t angle);
};

#endif // MAP_H
