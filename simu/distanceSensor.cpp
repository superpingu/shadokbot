#include "distanceSensor.hpp"
#include <stdio.h>
#include "screen.hpp"
#include <math.h>
#include "utils.hpp"

DistanceSensor::DistanceSensor(struct Position offset) :
	mOffset(offset)
{
	mDeltaAlpha = atan((double)mOffset.y/(double)mOffset.x);
	if (SIGN(mOffset.x)  == SIGN(-1))
		mDeltaAlpha += M_PI;
	mGap = sqrt(mOffset.x*mOffset.x + mOffset.y*mOffset.y);
}

void DistanceSensor::setPosition(struct Position pos)
{
	mPos.x = pos.x + mGap*cos(DEG_TO_RAD(pos.angle) - mDeltaAlpha);
	mPos.y = pos.y + mGap*sin(DEG_TO_RAD(pos.angle) - mDeltaAlpha);
	mPos.angle = pos.angle + mOffset.angle;
}

int DistanceSensor::getDistance()
{
	int distance = MAX_DISTANCE;
	double alpha, beta, Z;
	bool verticalLine = ABS(cos(DEG_TO_RAD(mPos.angle))) < EPSILON;
	int detX, detY;
	double A, B, C, detectionX, detectionY, detectionX1, detectionY1, detectionX2, detectionY2, det, tmpDistance, tmpDistance1, tmpDistance2;
	bool detection;
	if (verticalLine) { // vertical line
		// line of sight of this sensor is defined as x + Z= 0
		alpha = 1;
		beta = 0;
		Z = -mPos.x;
	} else { // non-vertical line
		// Line of sight of this sensor is defined by alpha*x + beta*y + Z = 0
		alpha = tan(DEG_TO_RAD(mPos.angle));
		beta = -1.;
		Z = mPos.y - mPos.x * tan(DEG_TO_RAD(mPos.angle));
	}
	for (auto &buoy : Screen::getInstance()->obstaclesList) {
		// Each circle is defined by (x - P)^2 + (y - Q)^2 = r^2
		int r = BUOY_RADIUS;
		int P = buoy->getPosition().x;
		int Q = buoy->getPosition().y;
		// Formula https://math.stackexchange.com/questions/228841/how-do-i-calculate-the-intersections-of-a-straight-line-and-a-circle
		if (verticalLine) { // vertical line
			B = -2*Q;
			C = P*P + Q*Q -r*r +2*Z*P + Z*Z;
			det = B*B - 4*C;
			if (det >= 0) { // intersection
				detectionX1 = -Z;
				detectionY1 = (-B + sqrt(det)) / 2;
				detectionX2 = -Z;
				detectionY2 = (-B - sqrt(det)) / 2;
			}
		} else { // non-vertical line
			A = alpha*alpha + 1;
			B = 2 * (alpha*Z - alpha*Q - P);
			C = Q*Q - r*r + P*P - 2*Z*Q + Z*Z;
			det = B*B - 4*A*C;
			if (det >= 0) { // intersection
				detectionX1 = (-B + sqrt(det)) / (2*A);
				detectionY1 = alpha*detectionX1 + Z;
				detectionX2 = (-B - sqrt(det)) / (2*A);
				detectionY2 = alpha*detectionX2 + Z;
			}
		}

		if (det >= 0) { // intersection
			tmpDistance1 = sqrt((detectionY1-mPos.y)*(detectionY1-mPos.y) + (detectionX1-mPos.x)*(detectionX1-mPos.x));
			tmpDistance2 = sqrt((detectionY2-mPos.y)*(detectionY2-mPos.y) + (detectionX2-mPos.x)*(detectionX2-mPos.x));
			if (tmpDistance1 < tmpDistance2) {
				tmpDistance = tmpDistance1;
				detectionX = detectionX1;
				detectionY = detectionY1;
			} else {
				tmpDistance = tmpDistance2;
				detectionX = detectionX2;
				detectionY = detectionY2;
			}

			detection = false;
			if (verticalLine) {
				if (SIGN(sin(DEG_TO_RAD(mPos.angle))) == SIGN(detectionY - mPos.y))
					detection = true;
			} else {
				if (SIGN(cos(DEG_TO_RAD(mPos.angle))) == SIGN(detectionX - mPos.x))
					detection = true;
			}

			if (detection && ((int)tmpDistance < distance)) {
				distance = (int)tmpDistance;
				detX = detectionX;
				detY = detectionY;
			}
		}
	}

	// Detection of the walls
	for (auto &border : Screen::getInstance()->bordersList) {
		detection = false;
		if (ABS(border.a) >= EPSILON) { // border is not vertical
			if (ABS(beta - (alpha * border.b) / border.a) >= EPSILON) { // Lines are not parallel
				detectionY = ((alpha * border.c) / border.a - Z) / (beta - (alpha * border.b) / border.a);
				detectionX = - (border.b * detectionY + border.c) / border.a;
				detection = true;
			}
		} else { // vertical border
			if (ABS(alpha) >= EPSILON) {
				detectionY = - border.c / border.b;
				detectionX = -(beta * detectionY + Z) / alpha;
				detection = true;
			}
		}

		if (detection) {
			if (verticalLine) {
				if (SIGN(sin(DEG_TO_RAD(mPos.angle))) != SIGN(detectionY - mPos.y))
					continue;
			} else {
				if (SIGN(cos(DEG_TO_RAD(mPos.angle))) != SIGN(detectionX - mPos.x))
					continue;
			}
			tmpDistance = sqrt((detectionY-mPos.y)*(detectionY-mPos.y) + (detectionX-mPos.x)*(detectionX-mPos.x));
			if ((int)tmpDistance  < distance) {
				distance = (int)tmpDistance;
				detX = detectionX;
				detY = detectionY;
			}
		}
	}

	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f((float)MM_TO_PX(mPos.x), (float)MM_TO_PX(mPos.y))),
		sf::Vertex(sf::Vector2f((float)MM_TO_PX(detX), (float)MM_TO_PX(detY)))
	};
	line[0].color = sf::Color::Red;
	line[1].color = sf::Color::Red;
	if (distance != MAX_DISTANCE)
		Screen::getInstance()->getWindow().draw(line, 2, sf::Lines);
	return distance;
}
