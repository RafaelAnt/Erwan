#pragma once
#include <iostream>
#include <stdio.h>
#include <list>
#include <time.h>
#include <vector>
#include <vslibs.h>

#include "ProductionRule.h"
#include "Point.h"

using namespace std;

#define NANOS_IN_SECOND 1000000000
#define TREE_NODE_DONE 300
#define TREE_NODE_TYPE_TURN_RIGHT 301
#define TREE_NODE_TYPE_TURN_LEFT 302
#define TREE_NODE_TYPE_BRANCH 303
#define TREE_NODE_TYPE_SWITCH_LEFT 304
#define TREE_NODE_TYPE_SWITCH_RIGHT 305

class TreeNode {
	char type;
	float width;
	float length;
	float degree;
	int stage;
	list<TreeNode*> nodes;
	Point3 centralPoint;
	Point3 growthDirection;
	vector<Point3> circlePoints;
	vector<Point3> normalCirclePoints;
	TreeNode *father;
	float color[3];
	double created;
	bool lastFromStage;
	Point3 lastPointFromStage;
	

public:
	TreeNode();
	TreeNode(char type, TreeNode* father);
	TreeNode(char type, TreeNode* father, int stage);
	TreeNode(const TreeNode & node);

	int getType();
	float getWidth();
	int setWidth(float newWidth);
	float getLength();
	int setLength(float newLength);
	float getDegree();
	int setDegree(float newDegree);
	int getStage();
	int setStage(int newStage);
	double getCreated();
	int setCreated(double time);
	list<TreeNode*> getNodes();
	void addNode(TreeNode* node);
	TreeNode* getFather();
	int setFather(TreeNode *newFather);

	// Growth Functions
	int grow(list<ProductionRule> prodRule);
	string getLSystem();
	int getBranchNumber();
	
	// Drawing Points
	Point3 getCentralPoint();
	int setCentralPoint(Point3 p);
	int setCentralPoint(float x, float y, float z);
	vector<Point3> getCirclePoints();
	int setCirclePoints(vector<Point3> points);
	vector<Point3> getNormalCirclePoints();
	int setNormalCirclePoints(vector<Point3> points);
	int clearPoints();

	// Growth Direction
	Point3 getGrowthDirection();
	int setGrowthDirection(Point3 p);
	int setGrowthDirection(float x, float y, float z);

	// Last Point Functions
	bool isLastFromStage();
	int setIsLastFromStage(bool is);
	bool checkIfIsLastFromStage();
	int setLastPointFromStage(Point3 point);
	Point3 getLastPointFromStage();
	int calculateLastPointFromStage();

	static long currentTimeInNanos() {

		struct timespec res;
		clock_gettime(CLOCK_MONOTONIC, &res);
		return (res.tv_sec * NANOS_IN_SECOND) + res.tv_nsec;
	}
};