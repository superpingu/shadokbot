#include "simu_robot.hpp"
#include "motion/AbsoluteMotion.hpp"
#include "simu_table.hpp"
#include "mockup/arduino_pin.hpp"
#include "board.h"

#define LED_SIZE MM_TO_PX(50)

Robot::Robot(float length, float width, sf::RenderWindow *table, sf::RenderWindow *roof):
	mWindow(table),
	mRoof(roof),
	mDimensions(length, width),
	mRedLed(RED_LED, sf::Color::Red, MM_TO_PX(100), MM_TO_PX(150)),
	mGreenLed(GREEN_LED, sf::Color::Green, MM_TO_PX(300), MM_TO_PX(150)),
	mYellowLed(YELLOW_LED, sf::Color::Yellow, MM_TO_PX(500), MM_TO_PX(150))
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

	mRedLed.update();
	mGreenLed.update();
	mYellowLed.update();

	mRoof->draw(*mRedLed.getShape());
	mRoof->draw(*mGreenLed.getShape());
	mRoof->draw(*mYellowLed.getShape());
}

Led::Led(int pin, sf::Color color, int x, int y)
{
	mColor = color;
	mPin = pin;

	mShape = new sf::CircleShape(LED_SIZE);
	mShape->setOrigin(LED_SIZE, LED_SIZE);
	mShape->setPosition(x, y);
	mShape->setOutlineThickness(2.f);
	mShape->setOutlineColor(mColor);
}

void Led::update()
{
	if (digitalRead(mPin) == HIGH)
		mShape->setFillColor(mColor);
	else
		mShape->setFillColor(sf::Color::Transparent);
}

const sf::CircleShape* Led::getShape()
{
	return mShape;
}
