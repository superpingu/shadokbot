#ifndef SIMU_TABLE_HPP
#define SIMU_TABLE_HPP
#include "SFML/Graphics.hpp"

#define MM_TO_PX(x) (0.5f * x)

class Table {
public:
	Table(float length, float width, sf::RenderWindow* window);
	void draw();

private:
	float mLength;
	float mWidth;
	sf::RenderWindow* mWindow;
	sf::RectangleShape* mTable;
};

#endif /* SIMU_TABLE_HPP */
