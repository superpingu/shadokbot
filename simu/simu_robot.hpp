#ifndef SIMU_ROBOT_HPP
#define SIMU_ROBOT_HPP

#include "SFML/Graphics.hpp"
#include "motion/AbsoluteMotion.hpp"

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

class Robot {
public:
	Robot(float length, float width, sf::RenderWindow* table, sf::RenderWindow *roof);
	AbsoluteMotion* getMotion();
	void draw();
private:
	AbsoluteMotion mMotion;

	sf::RenderWindow *mWindow;
	sf::RenderWindow *mRoof;
	sf::RectangleShape* mShape;
	sf::CircleShape* mFront;
	sf::Vector2f mDimensions;
	Led mRedLed;
	Led mGreenLed;
	Led mYellowLed;
	sf::Text mText;
	char mTextContent[30];
};

#endif /* SIMU_ROBOT_HPP */
