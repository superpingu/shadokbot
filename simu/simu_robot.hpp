#ifndef SIMU_ROBOT_HPP
#define SIMU_ROBOT_HPP

#include "SFML/Graphics.hpp"
#include "motion/AbsoluteMotion.hpp"
#include "eventHandler.hpp"

class Led {
public:
	Led(int pin, sf::Color color, int x, int y);
	const sf::CircleShape* getShape();
	void update();

private:
	sf::CircleShape *mShape;
	sf::Color mColor;
	int mPin;
};

class Clamp {
public:
	Clamp();
	void setPosition(int x, int y);
	void rotate(int angle);
	void draw();
private:
	sf::ConvexShape mShape;
};

class Robot : public EventHandler {
public:
	Robot(sf::RenderWindow* table, sf::RenderWindow *roof);
	AbsoluteMotion* getMotion();
	void toggleDeployed();
	void draw();
	void onEvent(Event* event);
private:
	AbsoluteMotion mMotion;
	bool mDeployed = false;

	sf::RenderWindow *mWindow;
	sf::RenderWindow *mRoof;
	sf::ConvexShape* mShape;
	Led mRedLed;
	Led mGreenLed;
	Led mYellowLed;
	sf::Text mText;
	char mTextContent[30];
	Clamp* mClampList[3];
};

#endif /* SIMU_ROBOT_HPP */
