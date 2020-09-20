/* extra functions for simulation only */
#ifndef SIMU_SEQUENCE_H
#define SIMU_SEQUENCE_H

#include "eventHandler.hpp"
#include "simu_robot.hpp"
#include <iostream>
#include <fstream>
#include <queue>
#include "actions/path_type.h"

class Sequence : public EventHandler
{
public:
	Sequence(std::fstream* file);
	void init();
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
	std::queue<SequenceElement> path;
};

#endif // SIMU_SEQUENCE_H
