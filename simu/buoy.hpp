#ifndef BUOY_HPP
#define BUOY_HPP

#include "SFML/Graphics.hpp"
#include "eventHandler.hpp"

#define BUOY_RADIUS 36 // in mm

class Buoy {
public:
	Buoy(const char* desc);
	void draw();
	/**
	 * Get the current position of the buoy.
	 * @return Cartesian coordinates of the center of the buoy.
	 */
	struct Position getPosition();
private:
	sf::CircleShape mShape;
	struct Position mPos;
};

#endif // BUOY_HPP
