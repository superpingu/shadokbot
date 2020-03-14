/* extra functions for simulation only */
#ifndef SIMU_SEQUENCE_H
#define SIMU_SEQUENCE_H

#include "SFML/Graphics.hpp"
#include "eventHandler.hpp"
#include <iostream>
#include <fstream>

class Sequence : public EventHandler
{
public:
	Sequence(const char *fileName);
	void onEvent(sf::Event* event);
	void onNewTarget(int x, int y, int angle);
	void update();
private:
	std::ifstream file;
};

#endif // SIMU_SEQUENCE_H
