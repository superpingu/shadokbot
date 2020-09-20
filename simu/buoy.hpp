#ifndef BUOY_HPP
#define BUOY_HPP

#include "SFML/Graphics.hpp"
#include "eventHandler.hpp"

#define BUOY_RADIUS 36 // in mm

class Buoy {
public:
	/**
	 * Constructor. All buoys have the same shape (circle) and radius (BUOY_RADIUS).
	 * @param desc String with the following format: "<x> <y> <c>"
	 *                                  <x> x position in mm (center of the buoy)
	 *                                  <y> y position in mm (center of the buoy)
	 *                                  <c> Letter indicating the buoy color: 'r' for red and 'g' for green
	 */
	Buoy(const char* desc);

	/**
	 * Display the buoy on the window.
	 */
	void draw();

	/**
	 * Get the current position of the buoy.
	 * @return Cartesian coordinates of the center of the buoy.
	 */
	struct Position getPosition();
private:
	sf::CircleShape mShape;
	/**
	 * Cartesian coordinates of the center of the buoy
	 */
	struct Position mPos;
};

#endif // BUOY_HPP
