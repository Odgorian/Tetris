#pragma once
#include "Map.h"
#include "Moving Part.h"
#include "Player.h"

void printMap(const Map& newMap, const Map& oldMap);
void printBorders(const Map&);
void printMovingPart(const MovingPart& newMP, const MovingPart& oldMP);
void printNextMovingPart(const MovingPart&);
void playerInput(Map&, MovingPart&, MovingPart&, bool&, const Keys&);
void game(Map&, MovingPart&, MovingPart&);
void printScore(unsigned int);
void congratulations(unsigned int);
void menu();
void rules();

enum class Speed {
	HALF,
	NORMAL,
	ONEANDAHALF,
	DOUBLE
};
struct Settings
{
	Speed speed;
};