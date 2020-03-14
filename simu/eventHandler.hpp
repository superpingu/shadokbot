#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include "SFML/Graphics.hpp"

class EventHandler {
public:
	/**
	 * Handler for SFML events, to call whenever an event is triggered
	 * See implementation to know the list of handled events.
	 * @param event The event to handle
	 */
	virtual void onEvent(sf::Event* event) = 0;

	virtual void onNewTarget(int x, int y, int angle) = 0;
};

#endif /* EVENT_HANDLER_HPP */
