//Monster.cpp

#include "Monster.h"
#include <iostream>

Monster::Monster() {
	const char* defaultStr = "defaultName";
	name = (char*)malloc(strlen(defaultStr) + 1);
	if (name != nullptr) {
		strcpy_s(name, strlen(defaultStr) + 1, defaultStr);
	}
	x = 0;
	y = 0;
	lifespan = 5;
	currentAge = 0;
	isAlive = false;
}
Monster::Monster(const char* n, int startX, int startY, int l) 
	: x(startX), y(startY), lifespan(l), currentAge(0), isAlive(true){
	name = (char*)malloc(strlen(n) + 1);
	if (name != nullptr) {
		strcpy_s(name, strlen(n) + 1, n);
	}
}
Monster::Monster(const Monster& other)
	: x(other.x), y(other.y), lifespan(other.lifespan), currentAge(other.currentAge), isAlive(other.isAlive) {
	name = (char*)malloc(strlen(other.name) + 1);
	if (name != nullptr) {
		strcpy_s(name, strlen(other.name) + 1, other.name);
	}
}
Monster& Monster::operator=(const Monster& other) {
	if (this != &other) {
		if (name != nullptr) { // 检查指针是否为nullptr
			free(name);
		}

		name = (char*)malloc(strlen(other.name) + 1);
		if (name != nullptr) {
			strcpy_s(name, strlen(other.name) + 1, other.name);
		}

		x = other.x;//can get x through this way？
		y = other.y;
		lifespan = other.lifespan;
		currentAge = other.currentAge;
		isAlive = other.isAlive;
	}
	return *this;
}
Monster::~Monster() {
	if (name != nullptr) {
		free(name);
		name = nullptr; // 设置为nullptr防止多次释放
	}
}
void Monster::setName(const char* Name) {
	free(name);
	name = (char*)malloc(strlen(Name) + 1);
	if (name != nullptr) {
		strcpy_s(name, strlen(Name) + 1, Name);
	}
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
void Monster::chaseAI(const Player& player) {
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
int Monster::getX() const {
	return x;
}
int Monster::getY() const {
	return y;
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
