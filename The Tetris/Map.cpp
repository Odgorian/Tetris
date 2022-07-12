#include "Map.h"
#include "Moving Part.h"

int Map::getX() const
{
	return this->xSize;
}

int Map::getY() const
{
	return this->ySize;
}

Color Map::getCell(int x, int y) const
{
	return this->map[y][x];
}

void Map::setCell(int x, int y, Color cell)
{
	this->map[y][x] = cell;
}

Map::Map(int x, int y)
{
	this->xSize = x;
	this->ySize = y + 3; //Фигура I занимает три клетки за экраном на момент спавна.
	//+3 is here to accomodate figure I.
	map.resize(this->ySize);
	for (unsigned int i = 0; i < map.size(); i++)
		map[i].resize(this->xSize, Color::BLOCK_FREE);
}

//Поиск и удаление полных строк.
//Finds and deletes full lines. Gives points for them.
void Map::checkFullRows(unsigned int& score, unsigned int scoreMulti)
{
	int counter = 0;
	for (int y = 3; y < this->ySize; y++) {
		bool flag = true;
		for (int x = 0; x < this->xSize; x++) {
			if (map[y][x] == Color::BLOCK_FREE) { //Поиск полной строки.
				flag = false;
				break;
			}
		}
		if (flag) { //Удаление полной строки, сдвиг всех вышестоящих.
			counter++;
			for (int y2 = y; y2 >= 3; y2--)
				for (int x = 0; x < this->xSize; x++) {
					map[y2][x] = map[y2 - 1][x];
				}
			for (int x = 0; x < this->xSize; x++)
				map[3][x] = Color::BLOCK_FREE;
		}
	}

	//Очки за удаление строк.
	switch (counter)
	{
	case 1:
		score += 1 * scoreMulti;
		break;
	case 2:
		score += 3 * scoreMulti;
		break;
	case 3:
		score += 7 * scoreMulti;
		break;
	case 4:
		score += 15 * scoreMulti;
		break;
	default:
		break;
	}
}

Map& Map::operator=(const Map& map)
{
	this->xSize = map.getX();
	this->ySize = map.getY();
	this->map.resize(this->ySize);
	for (int y = 0; y < this->ySize; y++) {
		this->map[y].resize(this->xSize);
		for (int x = 0; x < this->xSize; x++)
			this->map[y][x] = map.getCell(x, y);
	}
	return *this;
}

//Проверка возможности предложенного положения двигающейся части.
//Checks if moving part's position is valid.
bool Map::checkMovingPartValidity(const MovingPart& mp) const
{
	for (int y = 0; y < mp.getSize(); y++) {
		for (int x = 0; x < mp.getSize(); x++) {
			if (mp.getCell(x, y) == true
				&& ((y + mp.getY() >= this->ySize || x + mp.getX() < 0 || x + mp.getX() >= this->xSize)
					|| this->map[y + mp.getY()][x + mp.getX()] != Color::BLOCK_FREE))
				return false;
		}
	}
	return true;
}

//Добавляет двигающуюся часть к карте (применяется тогда, когда она перестаёт быть двигающейся).
//Adds moving part to the map (used when moving part hits the bottom).
void Map::addMovingPart(const MovingPart& mp)
{
	for (int y = 0; y < mp.getSize(); y++)
		for (int x = 0; x < mp.getSize(); x++) {
			if (mp.getCell(x, y) == true)
				this->map[y + mp.getY()][x + mp.getX()] = mp.getColor();
		}
}

//Сдвиг двигающейся части вниз. Может быть вызван как игроком, так и таймером игры. В случае, если этот сдвиг невозможен, производится добавление двигающейся части к карте и генерация новой двигающейся.
//Moves the moving part down. Can be called by both player and engine.
//If moving it down is impossible, calls addMovingPart() and nextMovingPart() for obvious reasons.
void Map::movePartDown(MovingPart& mp, MovingPart& nextMP, bool& flagNewMovingPartGenerated)
{
	MovingPart tempMP = mp;
	tempMP.moveDown();
	if (this->checkMovingPartValidity(tempMP) == true) {
		mp = tempMP;
		return;
	}
	this->addMovingPart(mp);
	mp = nextMP;
	nextMP.nextMovingPart();
	flagNewMovingPartGenerated = true;
}

//Проверка, проиграл ли игрок.
//Checks if the game can continue.
bool Map::checkLosing()
{
	for (int i = 0; i < map[4].size(); i++)
		if (map[4][i] != Color::BLOCK_FREE)
			return true;
	return false;
}
