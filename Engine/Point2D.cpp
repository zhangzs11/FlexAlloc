#include <stdexcept>
#include "Point2D.h"

Point2D Point2D::operator+(const Point2D& other) const {
	return Point2D(x + other.x, y + other.y);
}
Point2D Point2D::operator-(const Point2D& other) const {
	return Point2D(x - other.x, y - other.y);
}
float Point2D::operator*(const Point2D& other) const {
	return x * other.x + y * other.y;
}
Point2D Point2D::operator*(float scalar) const {
	return Point2D(x * scalar, y * scalar);
}
Point2D Point2D::operator/(float scalar) const {
	if (scalar == 0.0f) {
		throw std::runtime_error("Division by zero!");
	}
	return Point2D(x / scalar, y / scalar);
}
bool Point2D::operator==(const Point2D& other) const {
	return x == other.x && y == other.y;
}
bool Point2D::operator!=(const Point2D& other) const {
	return !(*this == other);
}
float Point2D::distance(const Point2D& a, const Point2D& b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrt(dx * dx + dy * dy);
}
void Point2D::move(const Point2D& velocity, float time) {
	x += velocity.x * time;
	y += velocity.y * time;
}
Point2D& Point2D::operator+=(const Point2D& other) {
	x += other.x;
	y += other.y;
	return *this;
}
Point2D& Point2D::operator-=(const Point2D& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}
Point2D& Point2D::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}
Point2D& Point2D::operator/=(float scalar) {
	if (scalar == 0.0f) {
		throw std::runtime_error("Division by zero!");
	}
	x /= scalar;
	y /= scalar;
	return *this;
}
Point2D Point2D::operator-() const {
	return Point2D(-x, -y);
}

Point2D operator*(float scalar, const Point2D& point) {
	return Point2D(scalar * point.getX(), scalar * point.getY());
}