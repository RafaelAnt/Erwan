#pragma once

#include <vslibs.h>

using namespace std;

#define POINT_DONE 400

class Point{
public:
	float x;
	float y;
	float z;

	Point();
	Point(float x, float y, float z);
	Point(const Point3& b);
	~Point();

	
	Point& operator=(const Point& src);
	Point operator*(const Point& alt);
	Point operator/(const Point& alt);
	Point operator-(const Point& alt);
	Point operator+(const Point& alt);
	Point operator*(float num);
	Point operator/(float num);
	Point operator-(float num);
	Point operator+(float num);
	Point operator*=(const Point& alt);
	Point operator/=(const Point& alt);
	Point operator-=(const Point& alt);
	Point operator+=(const Point& alt);
	Point operator*=(float num);
	Point operator/=(float num);
	Point operator-=(float num);
	Point operator+=(float num);

	float * toVec3f();
	Point3 createPoint3();



private:

};



