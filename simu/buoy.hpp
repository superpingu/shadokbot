#ifndef BUOY_HPP
#define BUOY_HPP

#include "SFML/Graphics.hpp"

class Buoy {
public:
	Buoy(const char* desc);
	void draw();
private:
	sf::CircleShape mShape;
};

#endif // BUOY_HPP
