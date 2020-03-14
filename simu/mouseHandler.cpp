#include "mouseHandler.hpp"
#include <math.h>
#include "event.h"
#define RAD_TO_DEG(x) (((x) * 180) / M_PI)
extern sf::Font font;

MouseHandler::MouseHandler() :
	mWindow(NULL),
	mEventX(0),
	mEventY(0),
	mPressed(false)
{
	mText.setFont(font);
	mText.setPosition(0, 30);
	mText.setFillColor(sf::Color::Magenta);
}

void MouseHandler::onEvent(sf::Event *event)
{
	switch (event->type) {
	case sf::Event::MouseButtonPressed:
		if (event->mouseButton.button == sf::Mouse::Left) {
			mPressed = true;
			mEventX = event->mouseButton.x;
			mEventY = event->mouseButton.y;
		}
		break;
	case sf::Event::MouseButtonReleased:
		mPressed = false;
		int angle = RAD_TO_DEG(atan2(event->mouseButton.y-mEventY,event->mouseButton.x - mEventX));
		while (angle >= 360)
			angle -= 360;
		while (angle < 0)
			angle += 360;
		newTarget(mEventX*2, mEventY*2, angle);
		break;
	}
}

void MouseHandler::draw()
{
	char angle[400];
	if (mPressed) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(*mWindow);
		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(mEventX, mEventY)),
			sf::Vertex(sf::Vector2f((float)mousePos.x, (float)mousePos.y))
		};
		sprintf(angle, "%f degree", RAD_TO_DEG(atan2(mousePos.y-mEventY,mousePos.x - mEventX)));
		mText.setString(angle);
		mWindow->draw(mText);
		mWindow->draw(line, 2, sf::Lines);
	}
}

void MouseHandler::setWindow(sf::RenderWindow *window)
{
	mWindow = window;
}

void MouseHandler::onNewTarget(int x, int y, int angle) {}
