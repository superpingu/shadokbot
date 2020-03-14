#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "eventHandler.hpp"
#include <iostream>
#include <fstream>

class Output : public EventHandler
{
public:
	Output(const char* fileName);
	void onEvent(sf::Event* event);
	void onNewTarget(int x , int y, int angle);
private:
	std::ofstream file;
};

#endif /* OUTPUT_HPP */
