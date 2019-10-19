#include "simu_robot.hpp"
#include "motion/AbsoluteMotion.hpp"
#include "simu_table.hpp"

Robot::Robot(float length, float width, sf::RenderWindow *window):
	mWindow(window),
	mDimensions(length, width)
{
	mShape = new sf::RectangleShape(mDimensions);
	mShape->setOrigin(length/2.f, width/2.f);
	mShape->setOutlineThickness(-2.f);
	mShape->setOutlineColor(sf::Color::Black);
	mShape->setFillColor(sf::Color::Red);
}

void Robot::draw() {
	if (motion != NULL) {
		mShape->setPosition(MM_TO_PX(motion->getX()), MM_TO_PX(motion->getY()));
	}
	mWindow->draw(*mShape);
}
