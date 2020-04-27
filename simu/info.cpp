#include "info.hpp"
#include "screen.hpp"
#include "utils.hpp"

Info::Info()
{}

void Info::onEvent(Event* event)
{
	if (event == NULL)
		return;

	if (event->type == EVENT_NEW_TARGET) {
		Line line;
		if (mPath.empty()) {
			line.endPoints[0] = sf::Vertex(sf::Vector2f((float)MM_TO_PX(event->targetEvent.target.x), (float)MM_TO_PX(event->targetEvent.target.y)));
		} else {
			line.endPoints[0] = mPath.back().endPoints[1];
		}
		line.endPoints[1] = sf::Vertex(sf::Vector2f((float)MM_TO_PX(event->targetEvent.target.x), (float)MM_TO_PX(event->targetEvent.target.y)));
		mPath.push_back(line);
	}

	if (event->type == EVENT_RESTART)
		mPath.clear();
}

void Info::draw()
{
	for (auto &line : mPath) {
		Screen::getInstance()->getWindow().draw(line.endPoints, 2, sf::Lines);
	}
}
