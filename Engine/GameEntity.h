#pragma once
#include <cstring>
#include <cstdlib>
class GameEntity {
protected:
	char* name;
	int x, y;

public:
	GameEntity(const char* n, int startX, int startY) : name(_strdup(n)), x(startX), y(startY) {}
	virtual ~GameEntity() { free(name); }
	virtual void move(char direction) = 0;

	int getX() const { return x; }
	int getY() const { return y; }
	const char* getName() const { return name; }

	void setX(int X) { x = X; }
	void setY(int Y) { y = Y; }
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
			x = other.x;
			y = other.y;
		}
		return *this;
	}
};