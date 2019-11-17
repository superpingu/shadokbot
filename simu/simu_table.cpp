#include "simu_table.hpp"

Table::Table(float length, float width, sf::RenderWindow* window) :
	mLength(length),
	mWidth(width),
	mWindow(window),
	mTable(),
	mTexture()
{
	//mTable = new sf::RectangleShape(sf::Vector2f(mLength, mWidth));
	if (!mTexture.loadFromFile("Eurobot2020_vinyl_plateau_v1.png")) {
		printf("Fail to open texture file\n");
	} else {
		mTable.setTexture(mTexture);
	}
	sf::FloatRect rect = mTable.getLocalBounds();
	mTable.setOrigin(rect.width/2, rect.height/2);
	printf("height %f width %f\n", rect.height, rect.width);
	mTable.setScale(width/rect.height,width/rect.height);
	sf::Vector2f windowSize(mWindow->getSize());
	mTable.setPosition(windowSize / 2.f);
	mTable.setRotation(180);
}

void Table::draw()
{
	mWindow->draw(mTable);
}
