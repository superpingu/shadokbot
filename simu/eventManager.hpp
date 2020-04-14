#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include "SFML/Graphics.hpp"
#include "eventHandler.hpp"

class EventManager {
public:
	EventManager();
	void setWindow(sf::RenderWindow *window);
	void onEvent(sf::Event *event);
	void onNewTarget(int x, int y, int angle);
	void draw();
private:
	sf::RenderWindow *mWindow;
	int mEventX;
	int mEventY;
	bool mPressed;
	sf::Text mText;
};

#endif // EVENT_MANAGER_HPP
