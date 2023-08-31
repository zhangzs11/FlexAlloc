#pragma once

//Monster.h

#include <cstring>
#include <cstdlib>
#include "Player.h"

class Monster {
private:
	char* name;
	int x;
	int y;
	int lifespan;
	int currentAge = 0;
	bool isAlive;

public:
	Monster();
	Monster(const char* n, int startX, int startY, int l);
	Monster(const Monster& other);
	Monster& operator=(const Monster& other);
	~Monster();

	void move(char direction); // Using "W","A","S","D" present direction
	void chaseAI(const Player& player);
	void checkDeath();
	int getX() const;
	int getY() const;
	void setX(int X) { x = X; }
	void setY(int Y) { y = Y; }
	void setName(const char* Name);
	void setAlive(bool ifA) { isAlive = ifA; }
	void setLifespan(int ls) { lifespan = ls; }
	const char* getName() const { return name; }
	bool getIsAlive() const { return isAlive; }
	int getlifespan() { return lifespan; }
	int getleftSpan() { return lifespan - currentAge; }
	void reset();
};