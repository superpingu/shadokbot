#ifndef SIMU_ROBOT_HPP
#define SIMU_ROBOT_HPP

#include "SFML/Graphics.hpp"

class Robot {
public:
	Robot(float length, float width, sf::RenderWindow* window);
	void draw();
private:
	sf::RenderWindow *mWindow;
	sf::RectangleShape* mShape;
	sf::Vector2f mDimensions;
};

#endif /* SIMU_ROBOT_HPP */
