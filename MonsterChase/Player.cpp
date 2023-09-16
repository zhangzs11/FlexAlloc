//Play.cpp

#include "Player.h"
const int MAP_WIDTH = 30;
const int MAP_HEIGHT = 10;
Player::Player(const char* n, int startX, int startY)
	: x(startX), y(startY) {
	name = (char*)malloc(strlen(n) + 1);  // allocate memory for the name
	if (name != nullptr) {
		strcpy_s(name, strlen(n) + 1, n);
	}
}

Player::Player(const Player& other)
	: x(other.x), y(other.y) {
	name = (char*)malloc(strlen(other.name) + 1);
	if (name != nullptr) {
		strcpy_s(name, strlen(other.name) + 1, other.name);
	}
}

Player& Player::operator=(const Player& other) {
	if (this != &other) {
		free(name);
		
		name = (char*)malloc(strlen(other.name) + 1);
		if (name != nullptr) {
			strcpy_s(name, strlen(other.name) + 1, other.name);
		}

		x = other.x;
		y = other.y;
	}
	return *this;
}

Player::~Player() {
	free(name);
}

void Player::setName(const char* Name) {
	free(name);
	name = (char*)malloc(strlen(Name) + 1);
	if (name != nullptr) {
		strcpy_s(name, strlen(Name) + 1, Name);
	}
}

void Player::move(char direction) {
	switch (direction) {
		case'W':	//Up
			if (y > 0) { y--; }
			break;
		case'A':	//Left
			if (x > 0) { x--; }
			break;
		case'S':	//Down
			if (y < MAP_HEIGHT - 1) { y++; }
			break;
		case'D':	//Right
			if (x < MAP_WIDTH - 1) { x++; }
			break;
		default:
			break;
	}
}
int Player::getX() const {
	return x;
}
int Player::getY() const {
	return y;
}

