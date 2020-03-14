#include "Arduino.h"
#include <stdio.h>
#include "simu_robot.hpp"
#include "simu_table.hpp"
#include "SFML/Graphics.hpp"
#include "simu_time.hpp"
#include "board.h"
#include "simu_sequence.hpp"
#include "simu_obstacle.hpp"
#include "motion/AbsoluteMotion.hpp"
#include "actions/sequence.hpp"
#include "mouseHandler.hpp"
#include "event.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <list>
using namespace std;
#include "output.hpp"

#define LOOP_DURATION 1 // in ms
#define LOOP_PERIOD_US 5000 // duration of each loop iteration
#define MOUSE_POS_STR_LENGTH 50
char mousePosStr[MOUSE_POS_STR_LENGTH];
sf::Font font;
list<Obstacle*> obstaclesList;
list<EventHandler*> handlersList;
Sequence *sequence = NULL;
void initPin()
{
	digitalWrite(START_JACK, LOW);
}

void loadObstacles(sf::RenderWindow *window, const char* fileName)
{
	string line;
	printf("Loading obstacles from file %s\n", fileName);
	ifstream file(fileName);
	if (file.is_open()) {
		while (getline(file, line)) {
			obstaclesList.push_back(new Obstacle(window, line.c_str()));
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
	if ((argc % 2) == 0) { // Even number of parameters
		printf("Invalid parameters\nUsage: %s [-i <inputFile>] [-o <outputFile>] [-b <obstacleFile>]\n", argv[0]);
		return -1;
	}
	for (int i = 1; i < argc; i = i+2) {
		if(strcmp(argv[i], "-b") == 0) {
			printf("Obstacle file: %s\n", argv[i+1]);
			//loadObstacles(&window, argv[1]);
		} else if (strcmp(argv[i], "-i") == 0) {
			printf("Input file: %s\n", argv[i+1]);
			sequence = new Sequence(argv[i+1]);
		} else if (strcmp(argv[i], "-o") == 0) {
			printf("Output file: %s\n", argv[i+1]);
			handlersList.push_back(new Output(argv[i+1]));
		} else {
			printf("Invalid parameter %s\nUsage: %s [-i <inputFile>] [-o <outputFile>] [-b <obstacleFile>]\n", argv[i], argv[0]);
			return -1;
		}
	}

	return 0;
}

#define DEG_TO_RAD(x) (((x) * M_PI) / 180)
#define RAD_TO_DEG(x) (((x) * 180) / M_PI)

int main(int argc, const char* argv[])
{
	if (!font.loadFromFile("data/JetBrainsMono-Italic.ttf")) {
		printf("Fail to load font");
		return -1;
	}
    sf::RenderWindow window(sf::VideoMode(MM_TO_PX(3000), MM_TO_PX(2000)), "Table");
    sf::RenderWindow roof(sf::VideoMode(MM_TO_PX(600), MM_TO_PX(300)), "Toit");
    Table table(MM_TO_PX(3000.f), MM_TO_PX(2000.f), &window);
    Robot robot(MM_TO_PX(250), MM_TO_PX(250), &window, &roof);
	MouseHandler* mouseHandler = new MouseHandler();
	mouseHandler->setWindow(&window);
	handlersList.push_back(mouseHandler);
	Time::reset();
	initPin();
	motion = new AbsoluteMotion();
	motion->enable(false);
	sf::Text mousePos;
	mousePos.setFont(font);
	mousePos.setCharacterSize(18);
	mousePos.setFillColor(sf::Color::Magenta);

	if (parseOptions(argc, argv) != 0)
		return -1;
	if (sequence == NULL)
		sequence = new Sequence(NULL);
	handlersList.push_back(sequence);

	// Main loop
    while (window.isOpen()) {
		Time::increaseTime(LOOP_DURATION);
		unsigned long loopStart = micros();
		motion->update();
		sequence->update();
		unsigned long loopTime = micros() - loopStart;
		delayMicroseconds(loopTime > LOOP_PERIOD_US ? 0 : LOOP_PERIOD_US - loopTime);

		// Event handling
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				digitalWrite(START_JACK, HIGH);
				break;
			case sf::Event::MouseMoved:
				snprintf(mousePosStr, MOUSE_POS_STR_LENGTH, "MOUSE X=%d Y=%d", (int)PX_TO_MM(event.mouseMove.x), (int)PX_TO_MM(event.mouseMove.y));
				mousePos.setString(mousePosStr);
			default:
				break;
			}
			for (auto &handler :handlersList)
				handler->onEvent(&event);
		}

		// Display
		window.clear(sf::Color::White);
		roof.clear(sf::Color::Black);
		table.draw();
		robot.draw();
		roof.draw(mousePos);
		for (auto &obstacle :obstaclesList)
			obstacle->draw();
		mouseHandler->draw();
		window.display();
		roof.display();
	}
	return 0;
}

void newTarget(int x, int y, int angle)
{
	printf("Event onNewTarget\n");
	for (auto &handler :handlersList)
		handler->onNewTarget(x, y, angle);
}
