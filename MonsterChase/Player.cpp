//Play.cpp

#include "Player.h"
const int MAP_WIDTH = 30;
const int MAP_HEIGHT = 10;
Player& Player::operator=(const Player& other) {
	if (this != &other) {
		GameEntity::operator=(other);
	}
	return *this;
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

