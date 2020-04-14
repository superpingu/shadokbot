#include "screen.hpp"
#include "utils.hpp"

static Screen* mScreen = NULL;

Screen::Screen() :
mWindow(sf::VideoMode(MM_TO_PX(3000), MM_TO_PX(2000)), "Table"),
mRoof(sf::VideoMode(MM_TO_PX(600), MM_TO_PX(300)), "Toit")
{
	if (!mFont.loadFromFile("data/JetBrainsMono-Italic.ttf")) {
		printf("Fail to load font");
	}
}

Screen* Screen::getInstance()
{
	if (mScreen == NULL)
		mScreen = new Screen();

	return mScreen;
}

void Screen::clear()
{
	mWindow.clear(sf::Color::White);
	mRoof.clear(sf::Color::Black);
}

void Screen::display()
{
	mWindow.display();
	mRoof.display();
}
