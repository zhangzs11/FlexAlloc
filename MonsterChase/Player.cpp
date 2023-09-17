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
			if (position.getY() > 0) { position.setY(position.getY() - 1); }
			break;
		case'A':	//Left
			if (position.getX() > 0) { position.setX(position.getX() - 1); }
			break;
		case'S':	//Down
			if (position.getY() < MAP_HEIGHT - 1) { position.setY(position.getY() + 1); }
			break;
		case'D':	//Right
			if (position.getX() < MAP_WIDTH - 1) { position.setX(position.getX() + 1); }
			break;
		default:
			break;
	}
}

