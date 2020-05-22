#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "SFML/Graphics.hpp"
#include "buoy.hpp"
#include <list>

class Screen {
public:
	static Screen *getInstance();
	sf::RenderWindow& getWindow() {return mWindow;};
	sf::RenderWindow& getRoof() {return mRoof;};
	sf::Font& getFont() {return mFont;};
	void clear();
	void display();

	std::list<Buoy*> obstaclesList;
private:
	Screen();
	sf::RenderWindow mWindow;
	sf::RenderWindow mRoof;
	sf::Font mFont;
};

#endif // SCREEN_HPP
