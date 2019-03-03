#include "obstacle.h"
#include <math.h>

Line::Line(float a, float b, float c) {
  this->a = a;
  this->b = b;
  this->c = c;
}

Line::isValid() {
  return ((a != 0) || (b != 0));
}

Obstacle::Obstacle(float a, float b, float c, float x_min, float x_max) {
  line = Line(a, b, c);
  this->x_min = x_min;
  this->x_max = x_max;
}

Obstacle::Obstacle(Line line, float x_min, float x_max) {
  this->line = line;
  this->x_min = x_min;
  this->x_max = x_max;
}

float Obstacle::distance(float xA, float yA) {
  float xB = x_min;
  float yB;
  float xv = -line.b;
  float yv = line.a;
  if (line.b == 0) {
    yB = 0;
  } else {
    yB = -(line.a * x_min + line.c) / line.b;
  }

  float xI = xB + ((xA - xB)*xv + (yA - yB)*yv) * xv / (xv*xv + yv*yv);
  if ((xI >= x_min) && (xI <= x_max)) { // orthogonal projection is inside the segment
    return ((std::abs(line.a*xA + line.b*yA + line.c)) / (sqrt(line.a*line.a + line.b*line.b)));
  } else { // closest point is one of the extremities
    xB = x_min;
    if (line.b == 0) {
      yB = 0;
    } else {
      yB = -(line.a * xB + line.c) / line.b;
    }
    float dist_x_min = std::sqrt((xB-xA)*(xB-xA) + (yB-yA)*(yB-yA));

    xB = x_max;
    if (line.b == 0) {
      yB = 0;
    } else {
      yB = -(line.a * xB + line.c) / line.b;
    }
    float dist_x_max = std::sqrt((xB-xA)*(xB-xA) + (yB-yA)*(yB-yA));

    if (dist_x_min < dist_x_max) {
      return dist_x_min;
    } else {
      return dist_x_max;
    }
  }
}

bool Obstacle::intersect(Line ext_line) {
  return true;
}
