//Player.h

#pragma once

#include <string>
#include <cstring>
#include <cstdlib>  // For malloc and free 

class Player {
private:
	char* name;
	int x;
	int y;

public:
	Player(const char* n, int startX, int startY);
	Player(const Player& other);
	Player& operator=(const Player& other);
	~Player();

	void move(char direction); // Using "W","A","S","D" present direction
	int getX() const;
	int getY() const;
	const char* getName() const { return name; }
	void setX(int X) { x = X; }
	void setY(int Y) { y = Y; }
	void setName(const char* Name);
};