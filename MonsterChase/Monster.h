#pragma once

//Monster.h

#include <cstring>
#include <cstdlib>
#include "GameEntity.h"
#include "Player.h"

class Monster : public GameEntity{
private:
	int lifespan;
	int currentAge = 0;
	bool isAlive;

public:
	Monster()
		:GameEntity("defaultName", 0, 0), lifespan(0), isAlive(false) {}
	Monster(const char* n, int startX, int startY, int l)
		:GameEntity(n, startX, startY), lifespan(l), isAlive(true) {}
	Monster(const Monster& other)
		:GameEntity(other.name, other.getX(), other.getY()),
		lifespan(other.lifespan), currentAge(other.currentAge), isAlive(other.isAlive) {}
	Monster& operator=(const Monster& other);
	~Monster() {}

	void move(char direction) override; // Using "W","A","S","D" present direction
	void chaseAI(const GameEntity& player);
	void checkDeath();

	void setAlive(bool ifA) { isAlive = ifA; }
	void setLifespan(int ls) { lifespan = ls; }
	const char* getName() const { return name; }
	bool getIsAlive() const { return isAlive; }
	int getlifespan() { return lifespan; }
	int getleftSpan() { return lifespan - currentAge; }
	void reset();
};