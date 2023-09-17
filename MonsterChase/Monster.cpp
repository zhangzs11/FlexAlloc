//Monster.cpp

#include "Monster.h"
#include <iostream>

Monster& Monster::operator=(const Monster& other) {
	if (this != &other) {
		GameEntity::operator=(other);
		lifespan = other.lifespan;
		currentAge = other.currentAge;
		isAlive = other.isAlive;
	}
	return *this;
}
void Monster::move(char direction) {
	switch (direction) {
	case'W':	//Up
		y--;
		break;
	case'A':	//Left
		x--;
		break;
	case'S':	//Down
		y++;
		break;
	case'D':	//Right
		x++;
		break;
	default:
		break;
	}
	currentAge++;
	checkDeath();
}
void Monster::chaseAI(const GameEntity& player) {
	int dx = player.getX() - x;
	int dy = player.getY() - y;
	if (dx > dy) {
		if (dx > 0) {
			move('D');
		} else {
			move('A');
		}
	} else {
		if (dy > 0) {
			move('S');
		} else {
			move('W');
		}
	}

}
void Monster::checkDeath() {
	if (currentAge >= lifespan) {
		std::cout << name << " already die. " << std::endl;
		isAlive = false;
	}
}
void Monster::reset() {
	const char* defaultStr = "defaultName";

	if (name != nullptr) {
		free(name);
	}

	name = (char*)malloc(strlen(defaultStr) + 1);
	if (name != nullptr) {
		strcpy_s(name, strlen(defaultStr) + 1, defaultStr);
	}
	x = 0;
	y = 0;
	lifespan = 5;
	currentAge = 0;
	isAlive = 0;
}
