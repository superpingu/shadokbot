#include "obstacle.h"
#include <math.h>

Obstacle::Obstacle(float a, float b, float c, float x_min, float x_max) {
  this->a = a;
  this->b = b;
  this->c = c;
  this->x_min = x_min;
  this->x_max = x_max;
}

float Obstacle::distance(float xA, float yA) {
  float xB = x_min;
  float yB;
  float xv = -b;
  float yv = a;
  if (b == 0) {
    yB = 0;
  } else {
    yB = -(a * x_min + c) / b;
  }

  float xI = xB + ((xA - xB)*xv + (yA - yB)*yv) * xv / (xv*xv + yv*yv);
  if ((xI >= x_min) && (xI <= x_max)) { // orthogonal projection is inside the segment
    return ((std::abs(a*xA + b*yA + c)) / (sqrt(a*a + b*b)));
  } else { // closest point is one of the extremities
    xB = x_min;
    if (b == 0) {
      yB = 0;
    } else {
      yB = -(a * xB + c) / b;
    }
    float dist_x_min = std::sqrt((xB-xA)*(xB-xA) + (yB-yA)*(yB-yA));

    xB = x_max;
    if (b == 0) {
      yB = 0;
    } else {
      yB = -(a * xB + c) / b;
    }
    float dist_x_max = std::sqrt((xB-xA)*(xB-xA) + (yB-yA)*(yB-yA));

    if (dist_x_min < dist_x_max) {
      return dist_x_min;
    } else {
      return dist_x_max;
    }
  }
}
