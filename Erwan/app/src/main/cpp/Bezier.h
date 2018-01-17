#pragma once
#include <list>
#include <vector>

#include "Point.h"

#ifndef BEZIER_H

#define BEZIER_H


using namespace std;
#define NM 3


typedef struct patch {
	vector<vector<Point>> vertices;
	vector<vector<Point>> normais;
}Patch;

vector<Point> catmullromPath(vector<Point> controlPoints, int segmentsNumber);
vector<Point> bezierPath(vector<Point> controlPoints, int segmentsNumber);
Point calculateBezierPoint(Point p0, Point p1, Point p2, Point p3, float t);


Point getBezierPoint(Point pontos_control[][NM + 1], float u, float v);
Point getBezierNormal(Point pontos[][NM + 1], float u, float v);
void makeControlPoints(int patch, Point pontos_control[][NM + 1], vector<Point> vertices, vector<vector<unsigned int>> indices);

#endif 


