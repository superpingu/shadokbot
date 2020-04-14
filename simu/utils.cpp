#include "utils.hpp"

float Utils::normalizeAngle(float angle)
{
	while (angle > 180)
		angle -= 360;
	while (angle <= -180)
		angle += 360;

	return angle;
}
