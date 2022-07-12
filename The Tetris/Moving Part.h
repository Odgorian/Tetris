#pragma once
#include "Color.h"

#include <vector>

enum class Type
{
	I,
	O,
	L,
	J,
	T,
	Z,
	S,
	MAX_SIZE
};

class MovingPart
{
public:

	int getSize() const;
	int getX() const;
	int getY() const;
	bool getCell(int x, int y) const;
	Color getColor() const;

	void moveLeft();
	void moveRight();
	void moveDown();

	void nextMovingPart();
	void rotate();

	MovingPart();
	MovingPart(Color);
	MovingPart(Type, Color);
	//~MovingPart(); //Нет никаких особенных правил.

	MovingPart& operator =(const MovingPart&);
private:
	int x, y;
	int rotateCounter = 0;
	Color color = Color::BLOCK_COLOR_1;
	std::vector<std::vector<bool>> map;
	Type type;
	std::vector<bool> listOfUsedTypes;
};
