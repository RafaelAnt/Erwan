#include "Point.h"

Point::Point() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Point::Point(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Point::Point(const Point3 & b){
	this->x = b.x;
	this->y = b.y;
	this->z = b.z;
}

Point::~Point() {
}

float * Point::toVec3f(){
	float vec[3] = { x, y, z };
	return vec;
}

Point3 Point::createPoint3(){
	return Point3(x,y,z);
}

Point& Point::operator=(const Point& src) {
	x = src.x;
	y = src.y;
	z = src.z;
	return *this;
}
Point Point::operator*(const Point& alt) {
	Point p(x * alt.x, y * alt.y, z * alt.z);
	return p;
}
Point Point::operator/(const Point& alt) {
	Point p(x / alt.x, y / alt.y, z / alt.z);
	return p;
}
Point Point::operator-(const Point& alt) {
	Point p(x - alt.x, y - alt.y, z - alt.z);
	return p;
}
Point Point::operator+(const Point& alt) {
	Point p(x + alt.x, y + alt.y, z + alt.z);
	return p;
}
Point Point::operator*(float num) {
	Point p(x * num, y * num, z * num);
	return p;
}
Point Point::operator/(float num) {
	Point p(x / num, y / num, z / num);
	return p;
}
Point Point::operator-(float num) {
	Point p(x - num, y - num, z - num);
	return p;
}
Point Point::operator+(float num) {
	Point p(x + num, y + num, z + num);
	return p;
}
Point Point::operator*=(const Point& alt) {
	x *= alt.x;
	y *= alt.y;
	z *= alt.z;
	return *this;
}
Point Point::operator/=(const Point& alt) {
	x /= alt.x;
	y /= alt.y;
	z /= alt.z;
	return *this;
}
Point Point::operator-=(const Point& alt) {
	x -= alt.x;
	y -= alt.y;
	z -= alt.z;
	return *this;
}
Point Point::operator+=(const Point& alt) {
	x += alt.x;
	y += alt.y;
	z += alt.z;
	return *this;
}
Point Point::operator*=(float num) {
	x *= num;
	y *= num;
	z *= num;
	return *this;
}
Point Point::operator/=(float num) {
	x /= num;
	y /= num;
	z /= num;
	return *this;
}
Point Point::operator-=(float num) {
	x -= num;
	y -= num;
	z -= num;
	return *this;
}
Point Point::operator+=(float num) {
	x += num;
	y += num;
	z += num;
	return *this;
}