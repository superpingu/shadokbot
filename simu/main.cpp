#include "Arduino.h"
#include <stdio.h>
#include "simu_robot.hpp"
#include "simu_table.hpp"
#include "simu_time.hpp"
#include "board.h"
#include "simu_sequence.hpp"
#include "simu_obstacle.hpp"
#include "buoy.hpp"
#include "actions/sequence.hpp"
#include "eventManager.hpp"
#include "event.h"
#include "utils.hpp"
#include "screen.hpp"
#include <iostream>
#include <fstream>
#include <string.h>
#include <list>
using namespace std;
#include "output.hpp"

#define LOOP_DURATION 1 // in ms
#define WINDOW_REFRESH_PERIOD 6 // number of iterations of the simulated Arduino loop for 1 refresh of the GUI
#define LOOP_PERIOD_US 5000 // duration of each loop iteration
#define MOUSE_POS_STR_LENGTH 50
char mousePosStr[MOUSE_POS_STR_LENGTH];
list<Buoy*> obstaclesList;
list<EventHandler*> handlersList;
Sequence *sequence = NULL;

void loadObstacles(const char* fileName)
{
	string line;
	printf("Loading obstacles from file %s\n", fileName);
	ifstream file(fileName);
	if (file.is_open()) {
		while (getline(file, line)) {
			obstaclesList.push_back(new Buoy(line.c_str()));
		}
		file.close();
	} else {
		printf("Fail to open file\n");
	}
}

/**
 * Get the options provided by the user and set the appropriate internal variables.
 * @param  argc Number of parameters (including program name)
 * @param  argv Array containing the options (including program name in first position)
 * @return      0 if provided options are valid
 *                    -1 otherwise
 */
static int parseOptions(int argc, const char* argv[])
{
	static fstream file;
	if ((argc % 2) == 0) { // Even number of parameters
		printf("Invalid parameters\nUsage: %s [-f <file>] [-b <obstacleFile>]\n", argv[0]);
		return -1;
	}
	for (int i = 1; i < argc; i = i+2) {
		if(strcmp(argv[i], "-b") == 0) {
			printf("Obstacle file: %s\n", argv[i+1]);
			loadObstacles(argv[i+1]);
		} else if (strcmp(argv[i], "-f") == 0) {
			printf("Input/Output file: %s\n", argv[i+1]);
			file.open(argv[i+1], ios::in | ios::out | ios::app);
			handlersList.push_back(new Output(&file));
			sequence = new Sequence(&file);
		} else {
			printf("Invalid parameter %s\nUsage: %s [-f <file>] [-b <obstacleFile>]\n", argv[i], argv[0]);
			return -1;
		}
	}

	return 0;
}

int main(int argc, const char* argv[])
{
    Table table(MM_TO_PX(3000.f), MM_TO_PX(2000.f), &Screen::getInstance()->getWindow());
    Robot robot(&Screen::getInstance()->getWindow(), &Screen::getInstance()->getRoof());
	handlersList.push_back(&robot);
	motion = robot.getMotion(); // Temporary Workaround
	EventManager* eventManager = new EventManager();
	eventManager->setWindow(&Screen::getInstance()->getWindow());
	Time::reset();
	robot.getMotion()->enable(false);
	sf::Text mousePos;
	mousePos.setFont(Screen::getInstance()->getFont());
	mousePos.setCharacterSize(18);
	mousePos.setFillColor(sf::Color::Magenta);

	if (parseOptions(argc, argv) != 0)
		return -1;
	if (sequence == NULL) {
		sequence = new Sequence(NULL);
	}

	handlersList.push_back(sequence);
	sequence->setRobot(&robot);

	uint32_t iteration_counter = 0;
	// Main loop
	while (Screen::getInstance()->getWindow().isOpen()) {
		Time::increaseTime(LOOP_DURATION);
		unsigned long loopStart = micros();
		robot.getMotion()->update();
		sequence->update();
		unsigned long loopTime = micros() - loopStart;
		delayMicroseconds(loopTime > LOOP_PERIOD_US ? 0 : LOOP_PERIOD_US - loopTime);

		// skip GUI refresh most of the time to avoid overloading old computers ;)
		if(iteration_counter++ % WINDOW_REFRESH_PERIOD) continue;

		// Event handling
		sf::Event event;
		while (Screen::getInstance()->getWindow().pollEvent(event)) {
			if (event.type == sf::Event::MouseMoved) {
				snprintf(mousePosStr, MOUSE_POS_STR_LENGTH, "MOUSE X=%d Y=%d", (int)PX_TO_MM(event.mouseMove.x), (int)PX_TO_MM(event.mouseMove.y));
				mousePos.setString(mousePosStr);
			}
			eventManager->onEvent(&event);
		}

		// Display
		Screen::getInstance()->clear();
		table.draw();
		robot.draw();
		Screen::getInstance()->getRoof().draw(mousePos);
		for (auto &obstacle :obstaclesList)
			obstacle->draw();
		eventManager->draw();
		Screen::getInstance()->display();

	}
	return 0;
}

void dispatchEvent(Event *event)
{
	printf("Dispatch Event\n");
	for (auto &handler :handlersList)
		handler->onEvent(event);
}
