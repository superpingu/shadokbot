#ifndef MOUSE_HANDLER_HPP
#define MOUSE_HANDLER_HPP

#include "eventHandler.hpp"

class MouseHandler : public EventHandler {
public:
	MouseHandler();
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

#endif // MOUSE_HANDLER_HPP
