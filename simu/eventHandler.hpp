#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

struct Position {
	int x;
	int y;
	int angle;
};

enum EventType {
	EVENT_NEW_TARGET,
	EVENT_CLOSE,
	EVENT_RESTART,
	EVENT_TOGGLE_DEPLOY
};

struct Event {
	EventType type;
	union {
		Position targetEvent;
	};
};

class EventHandler {
public:
	/**
	 * Handler for internal events, to call whenever an event is triggered
	 * See implementation to know the list of handled events.
	 * @param event The event to handle
	 */
	virtual void onEvent(Event* event) = 0;
};

#endif /* EVENT_HANDLER_HPP */
