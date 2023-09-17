//Player.h

#pragma once

#include <string>
#include <cstring>
#include <cstdlib>  // For malloc and free 
#include "GameEntity.h"

class Player : public GameEntity {
public:
	Player()
		: GameEntity("defaultName", 0, 0) {}
	Player(const char* n, int startX, int startY)
		: GameEntity(n, startX, startY) {}
	Player(const Player& other)
		: GameEntity(other.name, other.x, other.y) {}
	Player& operator=(const Player& other);
	~Player() {}

	void move(char direction) override; // Using "W","A","S","D" present direction
};