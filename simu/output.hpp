#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "eventHandler.hpp"
#include <iostream>
#include <fstream>

class Output : public EventHandler
{
public:
	Output(std::fstream* file);
	void onEvent(Event* event);
private:
	std::fstream* mFile;
};

#endif /* OUTPUT_HPP */
