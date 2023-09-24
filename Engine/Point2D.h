#pragma once
class Point2D {
private:
	float x, y;
public:
	Point2D() :x(0), y(0) {}
	Point2D(float x, float y) :x(x), y(y) {}
	float getX() const { return x; }
	float getY() const { return y; }
	void setX(float x) { this->x = x; }
	void setY(float y) { this->y = y; }

	Point2D operator+(const Point2D& other) const;
	Point2D operator-(const Point2D& other) const;
	float operator*(const Point2D& other) const;
	Point2D operator*(float scalar) const;
	Point2D operator/(float scalar) const;
	bool operator==(const Point2D& other) const;
	bool operator!=(const Point2D& other) const;
	Point2D& operator+=(const Point2D& other);
	Point2D& operator-=(const Point2D& other);
	Point2D& operator*=(float scalar);
	Point2D& operator/=(float scalar);
	Point2D operator-() const;
	static float distance(const Point2D& a, const Point2D& b);
	void move(const Point2D& velocity, float time);
};