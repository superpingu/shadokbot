#include "obstacle.h"
#include <math.h>

Line::Line(float a, float b, float c) {
  this->a = a;
  this->b = b;
  this->c = c;
}

bool Line::isValid() {
  return ((a != 0) || (b != 0));
}

float Line::getY(float x) {
  if (this->b != 0) {
    return -(this->a * x + this->c) / this->b;
  } else {
    return 0;
  }
}

Obstacle::Obstacle(float a, float b, float c, float min, float max) {
  line = Line(a, b, c);
  this->min = min;
  this->max = max;
}

Obstacle::Obstacle(Line line, float min, float max) {
  this->line = line;
  this->min = min;
  this->max = max;
}

float Obstacle::distance(float xA, float yA) {
  float xB = min;
  float yB;
  float xv = -line.b;
  float yv = line.a;
  if (line.b == 0) {
    yB = 0;
  } else {
    yB = -(line.a * min + line.c) / line.b;
  }

  float xI = xB + ((xA - xB)*xv + (yA - yB)*yv) * xv / (xv*xv + yv*yv);
  if ((xI >= min) && (xI <= max)) { // orthogonal projection is inside the segment
    return ((std::abs(line.a*xA + line.b*yA + line.c)) / (sqrt(line.a*line.a + line.b*line.b)));
  } else { // closest point is one of the extremities
    xB = min;
    if (line.b == 0) {
      yB = 0;
    } else {
      yB = -(line.a * xB + line.c) / line.b;
    }
    float dist_min = std::sqrt((xB-xA)*(xB-xA) + (yB-yA)*(yB-yA));

    xB = max;
    if (line.b == 0) {
      yB = 0;
    } else {
      yB = -(line.a * xB + line.c) / line.b;
    }
    float dist_max = std::sqrt((xB-xA)*(xB-xA) + (yB-yA)*(yB-yA));

    if (dist_min < dist_max) {
      return dist_min;
    } else {
      return dist_max;
    }
  }
}

bool Obstacle::intersect(Line &ext_line) {
  float x;
  float y;
  if (this->line.b != 0) {
    if (ext_line.a == ext_line.b * this->line.a / this->line.b) {
      // Lines are parallel or equals
      if (ext_line.c == ext_line.b * this->line.c / this->line.b) {
        // Lines are equal
        return true;
      } else {
        // Lines are parallel and distinct
        return false;
      }
    } else {
      x = (ext_line.c - (ext_line.b * this->line.c / this->line.b)) / (ext_line.a - (ext_line.b * this->line.a / this->line.b));
      if ((x >= this->min) && (x <= this->max)) {
        return true;
      } else {
        return false;
      }
    }
  } else {
    if (ext_line.b != 0) {
      y = (ext_line.a * this->line.c / this->line.a * ext_line.b) - (ext_line.c / ext_line.b);
      if (y >= this->min && y <= this->max) {
        return true;
      } else {
        return false;
      }
    } else {
      if (this->line.c / this->line.a == ext_line.c / ext_line.a) {
        // Lines are equals
        return true;
      } else {
        // Lines are parallel and distinct
        return false;
      }
    }
  }
}
