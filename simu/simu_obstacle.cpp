#include "simu_obstacle.hpp"
#include "utils.hpp"

Obstacle::Obstacle(sf::RenderWindow *window, const char* desc) :
	mShape()
{
	int x, y, length, width;
	float angle;
	mWindow = window;
	if (desc != NULL) {
		printf("Creating obstacle <%s>\n", desc);
		sscanf(desc, "%i %i %i %i %f", &x, &y, &length, &width, &angle);
		setPosition(x, y);
		setSize(length, width);
		setRotation(angle);
	}
}

void Obstacle::setPosition(int x, int y)
{
	mShape.setPosition(MM_TO_PX(x), MM_TO_PX(y));
}

void Obstacle::setSize(int length, int width)
{
	mShape.setSize(sf::Vector2f(MM_TO_PX(length), MM_TO_PX(width)));
	mShape.setOrigin(MM_TO_PX(length)/2.f, MM_TO_PX(width)/2.f);
}

void Obstacle::setRotation(float angle)
{
	mShape.setRotation(angle);
}

void Obstacle::draw()
{
	mWindow->draw(mShape);
}
