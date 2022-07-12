#pragma once

enum class Buttons
{
	LEFT,
	DOWN,
	RIGHT,
	LEFT_DOWN,
	RIGHT_DOWN,
	ROTATE,
	NOTHING
};

struct Keys
{
	bool* left, * down, * right, * rotate, *keyRed;
	Keys() {
		left = new bool;
		*left = false;
		down = new bool;
		*down = false;
		right = new bool;
		*right = false;
		rotate = new bool;
		*rotate = false;
		keyRed = new bool;
		*keyRed = false;
	}
};

Buttons buttonPressed();
void playerInputs(const Keys&, bool* stop);
Buttons getInput(const Keys&);