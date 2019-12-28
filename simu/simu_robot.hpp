#ifndef SIMU_ROBOT_HPP
#define SIMU_ROBOT_HPP

#include "SFML/Graphics.hpp"

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
	void draw();
private:
	sf::RenderWindow *mWindow;
	sf::RenderWindow *mRoof;
	sf::RectangleShape* mShape;
	sf::CircleShape* mFront;
	sf::Vector2f mDimensions;
	Led mRedLed;
	Led mGreenLed;
	Led mYellowLed;
};

#endif /* SIMU_ROBOT_HPP */
