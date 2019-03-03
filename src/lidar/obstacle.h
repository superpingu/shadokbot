#ifndef OBSTACLE_H
#define OBSTACLE_H

class Line {
public:
  Line(float a = 0, float b = 0, float c = 0);
  float a;
  float b;
  float c;
  bool isValid();
};

/**
 * An obstacle is a segment defined as 0 = a*x + b*y + c between x_min and x_max
 */
class Obstacle {
public:
  Obstacle(float a, float b, float c, float x_min, float x_max);
  Obstacle(Line line, float x_min, float x_max);
  float distance(float x, float y);
  bool intersect(Line ext_line);
private:
  Line line;
  float x_min;
  float x_max;
};

#endif // OBSTACLE_H
