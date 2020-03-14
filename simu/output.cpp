#include "output.hpp"

Output::Output(const char* fileName)
{
	file.open(fileName);
	if (!file.is_open()) {
		printf("Fail to open file %s\n", fileName);
	}
}

void Output::onEvent(sf::Event* event)
{
	if (event->type == sf::Event::KeyPressed && event->key.code == sf::Keyboard::Key::Q) {
		file.close();
		printf("File closed\n");
	}
}

void Output::onNewTarget(int x, int y, int angle)
{
	if (!file.is_open()) {
		return;
	}

	file << x << " " << y << " " << angle << " 200 MOVE_TURN\n";
}
