#include "simu_robot.hpp"
#include "utils.hpp"
#include "mockup/arduino_pin.hpp"
#include "board.h"
#include "screen.hpp"

#define LED_SIZE MM_TO_PX(50)

Robot::Robot(sf::RenderWindow *table, sf::RenderWindow *roof):
	mMotion(),
	mWindow(table),
	mRoof(roof),
	mRedLed(RED_LED, sf::Color::Red, MM_TO_PX(100), MM_TO_PX(150)),
	mGreenLed(GREEN_LED, sf::Color::Green, MM_TO_PX(300), MM_TO_PX(150)),
	mYellowLed(YELLOW_LED, sf::Color::Yellow, MM_TO_PX(500), MM_TO_PX(150))
{
	mShape = new sf::ConvexShape();
	mShape->setPointCount(6);
	mShape->setPoint(0, sf::Vector2f(MM_TO_PX(34), MM_TO_PX(0)));
	mShape->setPoint(1, sf::Vector2f(MM_TO_PX(214), MM_TO_PX(93)));
	mShape->setPoint(2, sf::Vector2f(MM_TO_PX(214), MM_TO_PX(153)));
	mShape->setPoint(3, sf::Vector2f(MM_TO_PX(34), MM_TO_PX(246)));
	mShape->setPoint(4, sf::Vector2f(MM_TO_PX(0), MM_TO_PX(216)));
	mShape->setPoint(5, sf::Vector2f(MM_TO_PX(0), MM_TO_PX(30)));
	mShape->setOrigin(MM_TO_PX(107), MM_TO_PX(123));
	mShape->setOutlineThickness(-2.f);
	mShape->setOutlineColor(sf::Color::Black);
	mShape->setFillColor(sf::Color::Red);

	mText.setFont(Screen::getInstance()->getFont());
	mText.setPosition(0, 0);
	mText.setFillColor(sf::Color::Magenta);

	digitalWrite(START_JACK, LOW);
}

AbsoluteMotion* Robot::getMotion()
{
	return &mMotion;
}

void Robot::draw() {
	if (motion != NULL) {
		mShape->setPosition(MM_TO_PX(motion->getX()), MM_TO_PX(motion->getY()));
		mShape->setRotation(motion->getHeading());
		sprintf(mTextContent, "ROBOT X=%d Y=%d heading=%d", (int)MM_TO_PX(motion->getX()), (int)MM_TO_PX(motion->getY()), motion->getHeading());
		mText.setString(mTextContent);
	}
	mWindow->draw(*mShape);
	mWindow->draw(mText);

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
