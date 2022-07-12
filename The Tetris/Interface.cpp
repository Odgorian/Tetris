#include "Interface.h"
#include "Map.h"
#include "Moving Part.h"
#include "Player.h"
#include "Engine.h"

#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>

//Вспомогательная функция, производит перевод перечислителя-цвета в используемый консолью цвет.
//Utility function, translates enumerator to console color.
int cellToColorNum(Color cell) {
	switch (cell)
	{
	case Color::WALL:
		return 192; //Красный.
		break;
	case Color::BLOCK_FREE:
		return 0; //Чёрный.
		break;
	case Color::BLOCK_COLOR_1:
		return 16; //Синий.
		break;
	case Color::BLOCK_COLOR_2:
		return 32; //Зелёный.
		break;
	case Color::BLOCK_COLOR_3:
		return 48; //Голубой.
		break;
	case Color::BLOCK_COLOR_4:
		return 64; //Другой красный.
		break;
	case Color::BLOCK_COLOR_5:
		return 80; //Фиолетовый.
		break;
	case Color::BLOCK_COLOR_6:
		return 96; //Жёлтый.
		break;
	case Color::BLOCK_COLOR_7:
		return 112; //Серый.
		break;
	case Color::BLOCK_COLOR_FULL_LINE:
		return 240; //Белый.
		break;
	case Color::TEXT_COLOR:
		return 7; //Дефолтный цвет консоли.
		break;
	case Color::MAX_SIZE:
		return -1;
		break;
	default:
		return -1;
		break;
	}
	return -1;
}

//Вспомогательная функция для вычисления координат курсора в консоли по координатам карты.
//Utility function, translates Map's X and Y coordinates to console's X and Y coordinates.
COORD coordToCOORD(int x, int y)
{
	COORD coord;
	coord.X = 20 + x;
	coord.Y = y - 3;
	return coord;
}

//Рисование карты.
//Printing of map. 
//Compares each cell of previous map to the current, if they aren't equal — prints cell of the current map.
void printMap(const Map& newMap, const Map& oldMap)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//Производится сравнение предыдущей карты с текущей, если у них есть несовпадающие участки — эти участки перекрашиваются в соответствующие цвета.
	try
	{
		for (int x = 0; x < newMap.getX(); x++) {
			for (int y = 3; y < newMap.getY(); y++) {
				if (oldMap.getCell(x, y) != newMap.getCell(x, y)) {
					COORD coord = coordToCOORD(x, y);
					SetConsoleCursorPosition(hConsole, coord);
					int color = cellToColorNum(newMap.getCell(x, y));
					if (color == -1)
						throw std::runtime_error("Invalid color.");
					SetConsoleTextAttribute(hConsole, color);
					std::cout << " ";
				}
			}
		}

	}
	//Mostly useless try-catch, was used at the testing phase.
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(hConsole, cellToColorNum(Color::TEXT_COLOR));
		system("cls");
		std::cout << "Error: " << e.what() << std::endl
			<< "Some weird shit happened. Sorry, this application is closing." << std::endl;
		system("pause");
		exit(0);
	}
	SetConsoleTextAttribute(hConsole, cellToColorNum(Color::TEXT_COLOR));
	COORD coord;
	coord.X = 0;
	coord.Y = newMap.getY() - 3 + 1;
	SetConsoleCursorPosition(hConsole, coord);
}

//Рисование движущейся части.
//Print of the moving part.
//Clears the previous image of the moving part, then prints new.
//Both images can have crossing cells, but finding them would spend more time then just coloring twice.
void printMovingPart(const MovingPart& newMP, const MovingPart& oldMP)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//Вначале полностью очищается предыдущий рисунок движущейся части, потом рисуется новый на новой позиции. 
	//Они могут пересекаться и, соответственно, вначале очищать клетку, а потом красить её заново в тот же цвет — но поиск этих ситуаций займёт больше времени по моим тестам.
	SetConsoleTextAttribute(hConsole, cellToColorNum(Color::BLOCK_FREE));
	for (int x = 0; x < oldMP.getSize(); x++)
		for (int y = 0; y < oldMP.getSize(); y++) {
			if (oldMP.getCell(x, y) == true) {
				COORD coord = coordToCOORD(oldMP.getX() + x, oldMP.getY() + y);
				if (coord.Y >= 0) {
					SetConsoleCursorPosition(hConsole, coord);
					std::cout << " ";
				}
			}
		}

	SetConsoleTextAttribute(hConsole, cellToColorNum(newMP.getColor()));
	for (int x = 0; x < newMP.getSize(); x++)
		for (int y = 0; y < newMP.getSize(); y++) {
			if (newMP.getCell(x, y) == true) {
				COORD coord = coordToCOORD(newMP.getX() + x, newMP.getY() + y);
				if (coord.Y >= 0) {
					SetConsoleCursorPosition(hConsole, coord);
					std::cout << " ";
				}
			}
		}

	SetConsoleTextAttribute(hConsole, cellToColorNum(Color::TEXT_COLOR));
}

//Printing the next moving part in the special rectangle.
void printNextMovingPart(const MovingPart& mp)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//Очистка от предыдущего рисунка.
	SetConsoleTextAttribute(hConsole, cellToColorNum(Color::BLOCK_FREE));
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++) {
			COORD coord;
			coord.X = 32 + x;
			coord.Y = 1 + y;
			SetConsoleCursorPosition(hConsole, coord);
			std::cout << " ";

		}

	//Создание следующего рисунка.
	SetConsoleTextAttribute(hConsole, cellToColorNum(mp.getColor()));
	for (int y = 0; y < mp.getSize(); y++)
		for (int x = 0; x < mp.getSize(); x++) {
			if (mp.getCell(x, y) == true) {
				COORD coord;
				coord.X = 32 + x;
				coord.Y = 1 + y;
				SetConsoleCursorPosition(hConsole, coord);
				std::cout << " ";
			}
		}

	COORD coord;
	coord.X = 0;
	coord.Y = 24 - 3 + 1;
	SetConsoleCursorPosition(hConsole, coord);
	SetConsoleTextAttribute(hConsole, cellToColorNum(Color::TEXT_COLOR));
}

//Рисование границ поля. Вызывается единожды.
//Printing map's borders. Would be called only once per game.
void printBorders(const Map& map)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, cellToColorNum(Color::WALL));

	//Main map.
	COORD coord;
	coord.X = 19; //Левая граница.
	for (int i = 0; i < map.getY() - 3; i++) {
		coord.Y = i;
		SetConsoleCursorPosition(hConsole, coord);
		std::cout << " ";
	}

	coord.X += map.getX() + 1; //Правая граница.
	for (int i = 0; i < map.getY() - 3; i++) {
		coord.Y = i;
		SetConsoleCursorPosition(hConsole, coord);
		std::cout << " ";
	}

	coord.X = 19; //Нижняя граница.
	coord.Y++;
	SetConsoleCursorPosition(hConsole, coord);
	for (int i = 0; i < map.getX() + 2; i++)
		std::cout << " ";

	//Next moving part's rectangle.
	coord.X = 21 + map.getX(); //Нижняя граница окна следующей фигуры.
	coord.Y = 6;
	SetConsoleCursorPosition(hConsole, coord);
	for (int i = 0; i < 7; i++)
		std::cout << " ";

	coord.X = 27 + map.getX(); //Правая граница окна следующей фигуры.
	for (int i = 0; i < 6; i++) {
		coord.Y--;
		SetConsoleCursorPosition(hConsole, coord);
		std::cout << " ";
	}

	SetConsoleTextAttribute(hConsole, cellToColorNum(Color::TEXT_COLOR));
}

//Анализ нажатых игроком кнопок, исполнение его команд, если они возможны.
//Gets player's inputs, tries to execute them.
void playerInput(Map& map, MovingPart& mp, MovingPart& nextMP, bool& flagNewMovingPartGenerated,
	const Keys& k) {
	Buttons input = getInput(k);
	MovingPart tempMP = mp;
	if (input == Buttons::LEFT || input == Buttons::LEFT_DOWN)
		tempMP.moveLeft();
	if (input == Buttons::RIGHT || input == Buttons::RIGHT_DOWN)
		tempMP.moveRight();
	if (map.checkMovingPartValidity(tempMP))
		mp = tempMP;
	if (input == Buttons::DOWN || input == Buttons::LEFT_DOWN || input == Buttons::RIGHT_DOWN)
		map.movePartDown(mp, nextMP, flagNewMovingPartGenerated);
	if (input == Buttons::ROTATE)
	{
		tempMP.rotate();
		if (map.checkMovingPartValidity(tempMP))
			mp = tempMP;
	}
}

//Печатает счёт поверх предыдущего. Так как счёт может только расти, ситуация, где часть старого счёта остаётся, невозможна.
//Prints score over previous. As the score can only go up, we can ignore the previous score — it will always be painted over.
void printScore(unsigned int score)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = 33;
	coord.Y = 7;
	SetConsoleCursorPosition(hConsole, coord);
	std::cout << score;
}

//Скрин конца игры.
//TODO: добавить рекорды.
//Game over screen.
//TODO: add records.
void congratulations(unsigned int score)
{
	system("cls");
	std::cout << "Congratulations! You lost!\nYour score: " << score << std::endl;
	system("pause");
}

//Меню. Позволяет изменить скорость и запустить игру.
//TODO: рекорды.
//Menu screen. Allows player to change the speed, read the rules and start the game.
//TODO: records? Maybe. Not sure.
void menu()
{
	Settings s;
	s.speed = Speed::NORMAL;
	while (true) {
		system("cls");
		std::cout << "1. Start the game.\n"
			<< "2. Settings.\n"
			<< "3. Rules.\n"
			<< "4. Exit\n";
		char input = '0';
		while (input < '1' || input > '4')
		{
			std::cout << "Choose option.\n";
			std::cin >> input;
			std::cin.ignore(INT64_MAX, '\n');
		}
		switch (input)
		{
		case '1':
			game(s);
			break;
		case '2':
			std::cout << "Current speed is: ";
			switch (s.speed)
			{
			case Speed::HALF:
				std::cout << "half (4 actions per second).\nScore multiplier is 1.\n";
				break;
			case Speed::NORMAL:
				std::cout << "normal (8 actions per second).\nScore multiplier is 2.\n";
				break;
			case Speed::ONEANDAHALF:
				std::cout << "one and a half (12 actions per second).\nScore multiplier is 4.\n";
				break;
			case Speed::DOUBLE:
				std::cout << "double (16 actions per second).\nScore multiplier is 8.\n";
				break;
			default:
				break;
			}
			std::cout << "If you want to change speed, input 1. Else input 0.\n";
			std::cin >> input;
			std::cin.ignore(INT64_MAX, '\n');
			if (input == '1') {
				std::cout << "Choose your speed:\n"
					<< "1. Half (4 actions per second, score multiplier is 1).\n"
					<< "2. Normal (8 actions per second, score multiplier is 2).\n"
					<< "3. One and a half (12 actions per second, score multiplier is 4).\n"
					<< "4. Double (16 actions per second, score multiplier is 8). WARNING: extremely hard.\n";
				std::cin >> input;
				std::cin.ignore(INT64_MAX, '\n');
				if (input >= '1' && input <= '4') {
					int t = input - '0';
					t--;
					s.speed = static_cast<Speed>(t);
				}
			}
			break;
		case '3':
			rules();
			break;
		case '4':
			std::cout << "Thanks for playing!\n";
			system("pause");
			exit(0);
			break;
		default:
			//Бесполезная ветка, так как сюда не может зайти.
			//Useless branch. There is no way to get here.
			std::cout << "Invalid input\n";
			system("pause");
			break;
		}
	}
}

//Print rules.
void rules() {
	system("cls");
	std::cout << "Tetris.\n"
		<< "Random figures fall down from the top. Player can move figures to the left/right, rotate them clockwise and accelerate\ntheir descent.\n"
		<< "If the figures form a complete horizontal line, the line disappears, increasing player's score.\n"
		<< "Removal of 1 line gives the player 1 point, 2 lines at once - 3 points, 3 lines - 7 points and 4 lines - 15 points.\n"
		<< "Game finishes when figures reach the top.\n"
		<< "Keys: press \'A\' to move a figure to the left, \'D\' - to the right, \'R\' - to rotate clockwise, \'S\' - to speed up\ndescension.\n"
		<< "Good luck!\n";
	system("pause");
}