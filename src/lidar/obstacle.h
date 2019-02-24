#ifndef OBSTACLE_H
#define OBSTACLE_H

/**
 * An obstacle is a segment defined as 0 = a*x + b*y + c between x_min and x_max
 */
class Obstacle {
public:
  Obstacle(float a, float b, float c, float x_min, float x_max, bool vertical = false);
  float distance(float x, float y);
private:
  float a;
  float b;
  float c;
  float x_min;
  float x_max;
  bool vertical;
};

#endif // OBSTACLE_H
