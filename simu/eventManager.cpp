#include "eventManager.hpp"
#include <math.h>
#include "event.h"
#include "utils.hpp"
#include "screen.hpp"

EventManager::EventManager() :
	mWindow(NULL),
	mEventX(0),
	mEventY(0),
	mPressed(false)
{
	mText.setFont(Screen::getInstance()->getFont());
	mText.setPosition(0, 30);
	mText.setFillColor(sf::Color::Magenta);
}

void EventManager::onEvent(sf::Event *event)
{
	Event internalEvent;
	switch (event->type) {
	case sf::Event::Closed:
		internalEvent.type = EVENT_CLOSE;
		dispatchEvent(&internalEvent);
		Screen::getInstance()->getWindow().close();
		break;
	case sf::Event::MouseButtonPressed:
		if (event->mouseButton.button == sf::Mouse::Left) {
			mPressed = true;
			mEventX = event->mouseButton.x;
			mEventY = event->mouseButton.y;
		}
		break;
	case sf::Event::MouseButtonReleased:
		internalEvent.type = EVENT_NEW_TARGET;
		mPressed = false;
		internalEvent.targetEvent.angle = RAD_TO_DEG(atan2(event->mouseButton.y-mEventY,event->mouseButton.x - mEventX));
		internalEvent.targetEvent.angle = Utils::normalizeAngle(internalEvent.targetEvent.angle);
		internalEvent.targetEvent.x = mEventX*2;
		internalEvent.targetEvent.y = mEventY*2;
		dispatchEvent(&internalEvent);
		break;
	case sf::Event::KeyPressed:
		switch (event->key.code) {
		case sf::Keyboard::Key::Q:
			internalEvent.type = EVENT_CLOSE;
			dispatchEvent(&internalEvent);
			break;
		case sf::Keyboard::Key::R:
			internalEvent.type = EVENT_RESTART;
			dispatchEvent(&internalEvent);
			break;
		case sf::Keyboard::Key::D:
			internalEvent.type = EVENT_TOGGLE_DEPLOY;
			dispatchEvent(&internalEvent);
			break;
		}
		break;
	}
}

void EventManager::draw()
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

void EventManager::setWindow(sf::RenderWindow *window)
{
	mWindow = window;
}

void EventManager::onNewTarget(int x, int y, int angle) {}
