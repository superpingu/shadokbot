#ifndef SIMU_OBSTACLE_H
#define SIMU_OBSTACLE_H

#include "SFML/Graphics.hpp"

class Obstacle {
public:
	Obstacle(sf::RenderWindow* window, const char* desc = NULL);
	void setPosition(int x, int y);
	void setSize(int length, int width);
	void setRotation(float angle);
	void draw();
private:
	sf::RenderWindow *mWindow;
	sf::RectangleShape mShape;
};

#endif // SIMU_OBSTACLE_H
