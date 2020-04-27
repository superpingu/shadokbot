#ifndef INFO_HPP
#define INFO_HPP

#include "eventHandler.hpp"
#include "SFML/Graphics.hpp"
#include <list>

class Line {
public:
	sf::Vertex endPoints[2];
};

class Info : public EventHandler {
public:
	Info();
	void onEvent(Event* event);
	void draw();
private:
	std::list<Line> mPath;
};

#endif // INFO_HPP
