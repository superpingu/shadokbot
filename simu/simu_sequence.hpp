/* extra functions for simulation only */
#ifndef SIMU_SEQUENCE_H
#define SIMU_SEQUENCE_H

#include "eventHandler.hpp"
#include "simu_robot.hpp"
#include <iostream>
#include <fstream>
#include <queue>

class Sequence : public EventHandler
{
public:
	Sequence(std::fstream* file);
	void onEvent(Event* event);
	void update();
	void setRobot(Robot* robot);
	void setStartingPos(Position startingPos);
	void read();
private:
	std::fstream* mFile;
	Robot* mRobot;
	Position mStartingPos;
	MotionElement* buildPath(int targetX, int targetY, int angle);
	std::queue<Position> path;
};

#endif // SIMU_SEQUENCE_H
