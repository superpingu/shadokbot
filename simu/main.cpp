#include "Arduino.h"
#include <stdio.h>
#include "simu_robot.hpp"
#include "simu_table.hpp"
#include "SFML/Graphics.hpp"
#include "simu_time.hpp"
#include "board.h"

#define LOOP_DURATION 1 // in ms

void initPin()
{
	digitalWrite(START_JACK, LOW);
}

#define DEG_TO_RAD(x) (((x) * M_PI) / 180)
#define RAD_TO_DEG(x) (((x) * 180) / M_PI)
int main()
{
    sf::RenderWindow window(sf::VideoMode(MM_TO_PX(3000), MM_TO_PX(2000)), "Table");
    sf::RenderWindow roof(sf::VideoMode(MM_TO_PX(600), MM_TO_PX(300)), "Toit");
    Table table(MM_TO_PX(3000.f), MM_TO_PX(2000.f), &window);
    Robot robot(MM_TO_PX(250), MM_TO_PX(250), &window, &roof);
	Time::reset();
	initPin();
    setup();
    while (window.isOpen()) {
		Time::increaseTime(LOOP_DURATION);
        loop();
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
			} else if (event.type == sf::Event::MouseButtonPressed) {
				digitalWrite(START_JACK, HIGH);
			}
        }

        window.clear(sf::Color::White);
		roof.clear(sf::Color::Black);
        table.draw();
		robot.draw();
        window.display();
		roof.display();
    }
    return 0;
}
