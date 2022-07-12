#include "Moving Part.h"

//Так как двигающаяся часть всегда описана квадратным массивом, нет нужды вводить функцию для выяснения размеров массива.
//There is no need for function that would return amount of columns — moving part's map is always square.
int MovingPart::getSize() const
{
	return map.size();
}

int MovingPart::getX() const
{
	return this->x;
}

int MovingPart::getY() const
{
	return this->y;
}

bool MovingPart::getCell(int x, int y) const
{
	return this->map[y][x];
}

Color MovingPart::getColor() const
{
	return this->color;
}

void MovingPart::moveLeft()
{
	x--;
}

void MovingPart::moveRight()
{
	x++;
}

void MovingPart::moveDown()
{
	y++;
}

//Генерация следующей двигающейся части. Использует алгоритм "мешок" — в мешке лежит по одному экземпляру каждой фигурки. При генерации новой фигурки эту фигурку из мешка "достают".
//Мешок обновляется только после того, как он будет опустошён.
//Uses default algorithm for generating next part — there is "bag" that contains one instance of each type of figures. I "take" one of them to choose the type of the next moving part.
//After the bag was cleared, it is filled up again.
void MovingPart::nextMovingPart()
{
	bool flag = false;
	for (int i = 0; i < listOfUsedTypes.size(); i++) {
		if (listOfUsedTypes[i] == true) {
			flag = true;
			break;
		}
	}

	if (flag == false)
		for (int i = 0; i < listOfUsedTypes.size(); i++)
			listOfUsedTypes[i] = true;

	int r = rand() % listOfUsedTypes.size();
	while (listOfUsedTypes[r] != true) {
		r = rand() % listOfUsedTypes.size();
	}

	listOfUsedTypes[r] = false;

	*this = MovingPart(static_cast<Type>(r), this->color);
}

//Вращение двигающейся части. К сожалению, реализовано как огромная череда switch'ей. Надо посмотреть, как его реализовывали более умные люди.
//Rotating of moving part. Sadly, was implemented through 200+ lines of switches.
//TODO: look at normal implementations.
void MovingPart::rotate()
{
	for (int y = 0; y < this->getSize(); y++)
		for (int x = 0; x < this->getSize(); x++)
			map[y][x] = false;
	switch (type)
	{
	case Type::I:
		switch (rotateCounter)
		{
		case 0:
			map[2][0] = true;
			map[2][1] = true;
			map[2][2] = true;
			map[2][3] = true;

			rotateCounter = 1;
			break;
		case 1:
			map[0][1] = true;
			map[1][1] = true;
			map[2][1] = true;
			map[3][1] = true;

			rotateCounter = 2;
			break;
		case 2:
			map[1][0] = true;
			map[1][1] = true;
			map[1][2] = true;
			map[1][3] = true;

			rotateCounter = 3;
			break;
		case 3:
			map[0][2] = true;
			map[1][2] = true;
			map[2][2] = true;
			map[3][2] = true;

			rotateCounter = 0;
			break;
		default:
			break;
		}
		break;

	case Type::O:
		map[0][0] = true;
		map[0][1] = true;
		map[1][0] = true;
		map[1][1] = true;
		break;
		
	case Type::L:
		switch (rotateCounter)
		{
		case 0:
			map[1][0] = true;
			map[1][1] = true;
			map[1][2] = true;
			map[2][0] = true;

			rotateCounter = 1;
			break;
		case 1:
			map[0][1] = true;
			map[1][1] = true;
			map[2][1] = true;
			map[0][0] = true;

			rotateCounter = 2;
			break;
		case 2:
			map[1][0] = true;
			map[1][1] = true;
			map[1][2] = true;
			map[0][2] = true;

			rotateCounter = 3;
			break;
		case 3:
			map[0][1] = true;
			map[1][1] = true;
			map[2][1] = true;
			map[2][2] = true;

			rotateCounter = 0;
			break;
		default:
			break;
		}
		break;
	case Type::J:
		switch (rotateCounter)
		{
		case 0:
			map[1][0] = true;
			map[1][1] = true;
			map[1][2] = true;
			map[0][0] = true;

			rotateCounter = 1;
			break;
		case 1:
			map[0][1] = true;
			map[1][1] = true;
			map[2][1] = true;
			map[0][2] = true;

			rotateCounter = 2;
			break;
		case 2:
			map[1][0] = true;
			map[1][1] = true;
			map[1][2] = true;
			map[2][2] = true;

			rotateCounter = 3;
			break;
		case 3:
			map[0][1] = true;
			map[1][1] = true;
			map[2][1] = true;
			map[2][0] = true;

			rotateCounter = 0;
			break;
		default:
			break;
		}
		break;
	case Type::T:
		switch (rotateCounter)
		{
		case 0:
			map[0][1] = true;
			map[1][1] = true;
			map[2][1] = true;
			map[1][2] = true;

			rotateCounter = 1;
			break;
		case 1:
			map[1][0] = true;
			map[1][1] = true;
			map[1][2] = true;
			map[2][1] = true;

			rotateCounter = 2;
			break;
		case 2:
			map[0][1] = true;
			map[1][1] = true;
			map[2][1] = true;
			map[1][0] = true;

			rotateCounter = 3;
			break;
		case 3:
			map[0][1] = true;
			map[1][0] = true;
			map[1][1] = true;
			map[1][2] = true;

			rotateCounter = 0;
			break;
		default:
			break;
		}
		break;

	case Type::Z:
		switch (rotateCounter)
		{
		case 0:
			map[0][1] = true;
			map[1][0] = true;
			map[1][1] = true;
			map[2][0] = true;

			rotateCounter = 1;
			break;
		case 1:
			map[0][0] = true;
			map[0][1] = true;
			map[1][1] = true;
			map[1][2] = true;

			rotateCounter = 2;
			break;
		case 2:
			map[0][2] = true;
			map[1][1] = true;
			map[1][2] = true;
			map[2][1] = true;

			rotateCounter = 3;
			break;
		case 3:
			map[1][0] = true;
			map[1][1] = true;
			map[2][1] = true;
			map[2][2] = true;

			rotateCounter = 0;
			break;
		default:
			break;
		}
		break;

	case Type::S:
		switch (rotateCounter)
		{
		case 0:
			map[0][0] = true;
			map[1][0] = true;
			map[1][1] = true;
			map[2][1] = true;

			rotateCounter = 1;
			break;
		case 1:
			map[0][1] = true;
			map[0][2] = true;
			map[1][0] = true;
			map[1][1] = true;

			rotateCounter = 2;
			break;
		case 2:
			map[0][1] = true;
			map[1][1] = true;
			map[1][2] = true;
			map[2][2] = true;

			rotateCounter = 3;
			break;
		case 3:
			map[1][1] = true;
			map[1][2] = true;
			map[2][0] = true;
			map[2][1] = true;

			rotateCounter = 0;
			break;
		default:
			break;
		}
	default:
		break;
	}
}

//Определяет стартовую позицию фигуры по оси Y с помощью нахождения нижайшей точки фигуры.
//Calculates starting Y-coordinate through finding the lowest point of the figure.
int findTheLowestPart(const MovingPart& mp) {
	for (int y = mp.getSize() - 1; y >= 0; y--)
		for (int x = 0; x < mp.getSize(); x++) {
			if (mp.getCell(x, y) == true)
				return 3 - y;
		}
	return 0; //Досюда не должно доходить, но на всякий случай.
	//Useless return.
}

//Генерация первой фигурки. Можно было бы сделать лучше, но будет моим подарком игроку — первая фигурка всегда является палочкой.
//Generation of the first moving part. Always generates I. Artefact of the previous version, there as a tribute to it.
MovingPart::MovingPart()
{
	type = Type::I;
	int length = 4;
	x = 4;
	y = 0;
	map.resize(length);
	for (int i = 0; i < map.size(); i++) {
		map[i].resize(length, false);
	}
	listOfUsedTypes.resize(static_cast<int>(Type::MAX_SIZE), true);

	//Фигура I.
	//Figure I.
	map[0][1] = true;
	map[1][1] = true;
	map[2][1] = true;
	map[3][1] = true;
}

//Генерация фигурки выбранного типа. Принимает цвет предыдущей фигурки и выбранный тип.
//Generation of the next moving part. Inputs are the type and the previous color.
MovingPart::MovingPart(Type t, Color prev)
{
	type = t;
	x = 4;

	int length = 3;
	switch (t)
	{
	case Type::I:
		length = 4;
		break;
	case Type::O:
		length = 2;
		break;
	//case Type::L:
	//case Type::J:
	//case Type::T:
	//case Type::Z:
	//case Type::S:
	//	length = 3;
	//	break;
	//case Type::MAX_SIZE:
	//	break;
	//default:
	//	break;
	}
	map.resize(length);
	for (int i = 0; i < map.size(); i++) {
		map[i].resize(length, false);
	}

	//Generates the map, chooses random rotation position, calls rotate() function.
	int clr = static_cast<int>(prev) + 1;
	if (clr == 9)
		clr = 2;
	color = static_cast<Color>(clr);

	rotateCounter = rand() % 4;
	this->rotate();
	y = findTheLowestPart(*this);
}

MovingPart& MovingPart::operator =(const MovingPart& mp)
{
	//Намеренно не включает в себя приравнивание списка использованных типов.
	//Does not modifies "bag of figures"!
	this->x = mp.x;
	this->y = mp.y;
	this->color = mp.color;
	this->rotateCounter = mp.rotateCounter;
	this->type = mp.type;

	this->map.resize(mp.map.size());
	for (int y = 0; y < map.size(); y++) {
		this->map[y].resize(mp.map.size());
		for (int x = 0; x < map.size(); x++)
			this->map[y][x] = mp.map[y][x];
	}

	return *this;
}