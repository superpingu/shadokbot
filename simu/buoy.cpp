#include "buoy.hpp"
#include "screen.hpp"
#include "utils.hpp"

#define RADIUS 36

Buoy::Buoy(const char* desc) :
	mShape()
{
	int x, y;
	char color;
	sscanf(desc, "%i %i %c", &x, &y, &color);
	mShape.setOrigin(MM_TO_PX(RADIUS), MM_TO_PX(RADIUS));
	mShape.setPosition(MM_TO_PX(x), MM_TO_PX(y));
	mShape.setRadius(MM_TO_PX(RADIUS));
	if (color == 'r') {
		mShape.setFillColor(sf::Color::Red);
	} else {
		mShape.setFillColor(sf::Color::Green);
	}
}

void Buoy::draw()
{
	Screen::getInstance()->getWindow().draw(mShape);
}
