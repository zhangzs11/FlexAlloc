#pragma once
#include <cstring>
#include <cstdlib>
#include "Point2D.h"
class GameEntity {
protected:
	char* name;
	Point2D position;

public:
	GameEntity(const char* n, int startX, int startY) : name(_strdup(n)), position(startX, startY) {}
	virtual ~GameEntity() { free(name); }
	virtual void move(char direction) = 0;

	int getX() const { return position.getX(); }
	int getY() const { return position.getY(); }
	const char* getName() const { return name; }

	void setX(int X) { position.setX(X); }
	void setY(int Y) { position.setY(Y); }
	void setName(const char* Name) {
		free(name);
		name = (char*)malloc(strlen(Name) + 1);
		if (name != nullptr) {
			strcpy_s(name, strlen(Name) + 1, Name);
		}
	}
	GameEntity& operator=(const GameEntity& other) {
		if (this != &other) {
			free(name);

			name = _strdup(other.name);
			position = other.position;
		}
		return *this;
	}
};