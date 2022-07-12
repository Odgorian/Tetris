#pragma once
#include "Color.h"
#include "Moving Part.h"

#include <vector>

class Map
{
public:

	int getX() const;
	int getY() const;

	Color getCell(int, int) const;
	void setCell(int, int, Color);

	void checkFullRows(unsigned int& score, unsigned int scoreMulti);

	Map& operator =(const Map&);

	bool checkMovingPartValidity(const MovingPart&) const;

	void addMovingPart(const MovingPart&);

	void movePartDown(MovingPart&, MovingPart&, bool&);

	bool checkLosing();

	Map(int, int);
	//~Map(); //Ќет нужды в специализированном деструкторе.
private:
	std::vector<std::vector<Color>> map;
	int xSize, ySize;
};
