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

	mClampList[0] =new Clamp();
	mClampList[1] =new Clamp();
	mClampList[2] =new Clamp();

	mSensorList[0] = new DistanceSensor({91,40,0});
	mSensorList[1] = new DistanceSensor({27,44,-60});
	mSensorList[2] = new DistanceSensor({-80,59,-120});
	mSensorList[3] = new DistanceSensor({-51,0,-180});
	mSensorList[4] = new DistanceSensor({24,-45,60});
	mSensorList[5] = new DistanceSensor({-11,-99,120});

	digitalWrite(START_JACK, LOW);
}

AbsoluteMotion* Robot::getMotion()
{
	return &mMotion;
}

void Robot::toggleDeployed()
{
	mDeployed = !mDeployed;
}

void Robot::draw() {
	if (motion != NULL) {
		int centerX = motion->getX() - 19*cos(DEG_TO_RAD(motion->getHeading()));
		int centerY = motion->getY() - 19*sin(DEG_TO_RAD(motion->getHeading()));
		mClampList[0]->setPosition(MM_TO_PX(centerX + 113*cos(DEG_TO_RAD(motion->getHeading()+60))), MM_TO_PX(centerY + 113*sin(DEG_TO_RAD(motion->getHeading()+60))));
		mClampList[0]->rotate(motion->getHeading()+60);
		mClampList[1]->setPosition(MM_TO_PX(centerX + 113*cos(DEG_TO_RAD(motion->getHeading()+180))), MM_TO_PX(centerY + 113*sin(DEG_TO_RAD(motion->getHeading()+180))));
		mClampList[1]->rotate(motion->getHeading()+180);
		mClampList[2]->setPosition(MM_TO_PX(centerX + 113*cos(DEG_TO_RAD(motion->getHeading()-60))), MM_TO_PX(centerY + 113*sin(DEG_TO_RAD(motion->getHeading()-60))));
		mClampList[2]->rotate(motion->getHeading()-60);

		for (int i = 0; i < 6; i++) {
			mSensorList[i]->setPosition({centerX, centerY, motion->getHeading()});
			mSensorList[i]->getDistance();
		}

		mShape->setPosition(MM_TO_PX(motion->getX()), MM_TO_PX(motion->getY()));
		mShape->setRotation(motion->getHeading());
		sprintf(mTextContent, "ROBOT X=%d Y=%d heading=%d", motion->getX(), motion->getY(), motion->getHeading());
		mText.setString(mTextContent);
	}

	if (mDeployed) {
		mClampList[0]->draw();
		mClampList[1]->draw();
		mClampList[2]->draw();
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

void Robot::onEvent(Event* event)
{
	if (event != NULL && event->type == EVENT_TOGGLE_DEPLOY) {
		toggleDeployed();
	}
}

int Robot::getDistance(int direction)
{
	while(direction < 0) direction += 360;
	int selected_sensor = ((direction+30)/60) % 6;

	return mSensorList[selected_sensor]->getDistance();
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

Clamp::Clamp():
	mShape()
{
	mShape.setPointCount(8);
	mShape.setPoint(0, sf::Vector2f(MM_TO_PX(34), MM_TO_PX(0)));
	mShape.setPoint(1, sf::Vector2f(MM_TO_PX(98), MM_TO_PX(0)));
	mShape.setPoint(2, sf::Vector2f(MM_TO_PX(132), MM_TO_PX(34)));
	mShape.setPoint(3, sf::Vector2f(MM_TO_PX(132), MM_TO_PX(98)));
	mShape.setPoint(4, sf::Vector2f(MM_TO_PX(98), MM_TO_PX(132)));
	mShape.setPoint(5, sf::Vector2f(MM_TO_PX(34), MM_TO_PX(132)));
	mShape.setPoint(6, sf::Vector2f(MM_TO_PX(0), MM_TO_PX(98)));
	mShape.setPoint(7, sf::Vector2f(MM_TO_PX(0), MM_TO_PX(34)));

	mShape.setOrigin(MM_TO_PX(66), MM_TO_PX(66));
}

void Clamp::setPosition(int x, int y)
{
	mShape.setPosition(x, y);
}

void Clamp::rotate(int angle)
{
	mShape.setRotation(angle);
}

void Clamp::draw()
{
	Screen::getInstance()->getWindow().draw(mShape);
}
