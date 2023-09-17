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
		position.setY(position.getY() - 1);
		break;
	case'A':	//Left
		position.setX(position.getX() - 1);
		break;
	case'S':	//Down
		position.setY(position.getY() + 1);
		break;
	case'D':	//Right
		position.setX(position.getX() + 1);
		break;
	default:
		break;
	}
	currentAge++;
	checkDeath();
}
void Monster::chaseAI(const GameEntity& player) {
	int dx = player.getX() - this->getX();
	int dy = player.getY() - this->getY();
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
	position.setX(0);
	position.setY(0);
	lifespan = 5;
	currentAge = 0;
	isAlive = 0;
}
