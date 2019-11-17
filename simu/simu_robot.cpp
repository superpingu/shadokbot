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

	mFront = new sf::CircleShape(length/2,3);
	mFront->setOrigin(length/2,width/2);
	mFront->setFillColor(sf::Color::Black);
}

void Robot::draw() {
	if (motion != NULL) {
		mShape->setPosition(MM_TO_PX(motion->getX()), MM_TO_PX(motion->getY()));
		mShape->setRotation(motion->getHeading());
		mFront->setPosition(MM_TO_PX(motion->getX()), MM_TO_PX(motion->getY()));
		mFront->setRotation(motion->getHeading()+90);
	}
	mWindow->draw(*mShape);
	mWindow->draw(*mFront);
}
