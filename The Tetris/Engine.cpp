#include "Engine.h"

#include "Interface.h"
#include "Player.h"

#include <chrono>
#include <thread>

//Основной модуль игры.
//Main module.
void game(const Settings& settings) {
	system("cls");
	Map map(10, 20);
	MovingPart nextMP, mp;
	mp = nextMP;
	nextMP.nextMovingPart();

	int framesPerSecond, scoreMulti;
	switch (settings.speed)
	{
	case Speed::HALF:
		framesPerSecond = 4;
		scoreMulti = 1;
		break;
	case Speed::NORMAL:
		framesPerSecond = 8;
		scoreMulti = 2;
		break;
	case Speed::ONEANDAHALF:
		framesPerSecond = 12;
		scoreMulti = 4;
		break;
	case Speed::DOUBLE:
		framesPerSecond = 16;
		scoreMulti = 8;
		break;
	default:
		framesPerSecond = 8;
		scoreMulti = 2;
		break;
	}
	unsigned int score = 0, tickCounter = 0;;
	bool flagGameEnded = false;
	Keys keys;

	printBorders(map);
	printScore(score);
	printNextMovingPart(nextMP);
	playerInputs(keys, &flagGameEnded);

	while (flagGameEnded == false) {
		unsigned int oldScore = score;
		MovingPart oldMP = mp;
		Map oldMap = map;
		bool flagNewMovingPartGenerated = false;
		playerInput(map, mp, nextMP, flagNewMovingPartGenerated,
			keys);
		tickCounter++;
		if (tickCounter == 2) {
			map.movePartDown(mp, nextMP, flagNewMovingPartGenerated);
			tickCounter = 0;
		}
		map.checkFullRows(score, scoreMulti);
		if (score != oldScore)
			printScore(score);
		printMovingPart(mp, oldMP);
		printMap(map, oldMap);
		if (flagNewMovingPartGenerated == true) {
			flagGameEnded = map.checkLosing();
			printNextMovingPart(nextMP);
			flagNewMovingPartGenerated = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / framesPerSecond));
	}
	congratulations(score);
}

void main()
{
	srand(time(NULL));
	menu();

}