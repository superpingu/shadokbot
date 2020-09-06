#include "buoy.hpp"
#include "screen.hpp"
#include "utils.hpp"

Buoy::Buoy(const char* desc) :
	mShape()
{
	char color;
	sscanf(desc, "%i %i %c", &mPos.x, &mPos.y, &color);
	mShape.setOrigin(MM_TO_PX(BUOY_RADIUS), MM_TO_PX(BUOY_RADIUS));
	mShape.setPosition(MM_TO_PX(mPos.x), MM_TO_PX(mPos.y));
	mShape.setRadius(MM_TO_PX(BUOY_RADIUS));
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

struct Position Buoy::getPosition()
{
	return mPos;
}
