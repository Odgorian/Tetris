#include "Player.h"

#include <Windows.h>
#include <thread>
#include <chrono>

//Ввод игрока. Не принимает ввод в течении 50 мс после установки двигающейся части на поле.
//Работает с указателями, а не ссылками, так как используется дополнительный поток.
//Player's input. Ignores inputs for 50 ms after new frame.
//Working through pointers (and not references), cause uses additional thread.
void playerInputs(const Keys& k, bool* stop) {
	std::thread([=]() {
		while (*stop == false) {
			if (*k.keyRed == false) {
				if (GetKeyState('A') & 0b1000'0000'0000'0000)
					*k.left = true;
				if (GetKeyState('S') & 0x8000)
					*k.down = true;
				if (GetKeyState('D') & 0x8000)
					*k.right = true;
				if (GetKeyState('R') & 0x8000)
					*k.rotate = true;
			}
			else {
				*k.keyRed = false;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		}).detach();
}

//Очищение ввода.
//Clear inputs.
void nullInputs(const Keys& k) {
	*k.left = false;
	*k.down = false;
	*k.right = false;
	*k.rotate = false;
}

//Анализ нажатых игроком кнопок, их очистка, отправка соответствующей команды.
//Analyze inputs, clear them, return corresponding enumerator.
Buttons getInput(const Keys& k) {
	*k.keyRed = true;
	if (*k.rotate) {
		nullInputs(k);
		return Buttons::ROTATE;
	}
	if (*k.left && *k.right && *k.down) {
		nullInputs(k);
		return Buttons::DOWN;
	}
	if (*k.left && *k.down) {
		nullInputs(k);
		return Buttons::LEFT_DOWN;
	}
	if (*k.right && *k.down) {
		nullInputs(k);
		return Buttons::RIGHT_DOWN;
	}
	if (*k.left && *k.right) {
		nullInputs(k);
		return Buttons::NOTHING;
	}
	if (*k.left) {
		nullInputs(k);
		return Buttons::LEFT;
	}
	if (*k.down) {
		nullInputs(k);
		return Buttons::DOWN;
	}
	if (*k.right) {
		nullInputs(k);
		return Buttons::RIGHT;
	}
	return Buttons::NOTHING;
}