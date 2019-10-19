#include "simu_table.hpp"

Table::Table(float length, float width, sf::RenderWindow* window) :
	mLength(length),
	mWidth(width),
	mWindow(window)
{
	mTable = new sf::RectangleShape(sf::Vector2f(mLength, mWidth));
	mTable->setFillColor(sf::Color::Transparent);
	mTable->setOutlineThickness(1.f);
	mTable->setOutlineColor(sf::Color::Black);
	mTable->setOrigin(mLength/2, mWidth/2);
	sf::Vector2f windowSize(mWindow->getSize());
	mTable->setPosition(windowSize / 2.f);
}

void Table::draw()
{
	mWindow->draw(*mTable);
}
