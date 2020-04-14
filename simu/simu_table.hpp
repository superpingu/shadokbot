#ifndef SIMU_TABLE_HPP
#define SIMU_TABLE_HPP
#include "SFML/Graphics.hpp"

class Table {
public:
	Table(float length, float width, sf::RenderWindow* window);
	void draw();

private:
	float mLength;
	float mWidth;
	sf::RenderWindow* mWindow;
	sf::Sprite mTable;
	sf::Texture mTexture;
};

#endif /* SIMU_TABLE_HPP */
