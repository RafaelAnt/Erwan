#pragma once
#include <iostream>
#include <list>
#include <stdio.h>
#include <stack> 
#include <deque>

#include <vslibs.h>

#include "ProductionRule.h"
#include "TreeNode.h"
#include "Point.h"
#include "Bezier.h"



#define PI 3.1415

#define TREE_DONE 200
#define TREE_INVALID_VALUE 201
#define TREE_MAX_LENGTH_REACHED 202
#define TREE_MAX_WIDTH_REACHED 203
#define TREE_MAX_ANGLE_REACHED 204
#define TREE_NOT_ENOUGH_POINTS_FOR_CATMULLROM 205
#define TREE_ERROR 206


#define TREE_BRANCH_POINTS 8
using namespace std;

class Tree {
	list<ProductionRule> productionRules;
	TreeNode start;
	float maxLength;
	float maxWidth;
	float lengthGrowthRate;
	float widthGrowthRate;
	float angleGrowthRate;
	float angle;
	VSMathLib *vsml;

	float * indices;
	float * points;

	VSSurfRevLib leaf;
	VSModelLib bark;
	VSPolyLine pp;

public:
	Tree();
	Tree(string axiom, list<ProductionRule> prods, float maxLength, float maxWidth, float lengthGrowthRate, float widthGrowthRate, float angleGrowthRate, float angle);

	TreeNode getStart();
	int setStart(TreeNode start);
	float getMaxLength();
	int setMaxLength(float length);
	float getMaxWidth();
	int setMaxWidth(float width);
	float getLenghGrowthRate();
	int setLengthGrowthRate(float rate);
	float getAngleGrowthRate();
	int setAngleGrowthRate(float rate);
	float getWidthGrowthRate();
	int setWidthGrowthRate(float rate);

	int grow(int number);
	int draw();
	string getLSystem();
	int animate(double time);
	int reset();
	int setDevelopment(int percent);
	int drawStaticTree();
	int init();

private:
	
	void rotL(TreeNode* node);
	void rotR(TreeNode* node);
	void switchL(TreeNode* node);
	void switchR(TreeNode* node);
	int incrementLength(TreeNode *current);
	int incrementWidth(TreeNode *current);
	int incrementDegree(TreeNode *current);

	// Point Building / Gathering

	int getBarkPoints();
	int buildpoints(TreeNode* node);
	int buildCentralPoints(TreeNode *current);
	int buildCirclePoints(TreeNode *current);

	// Drawing

	int drawLeaves(TreeNode * current);
	int drawLine(TreeNode *node);
	int drawIntersection(TreeNode* node);
	int drawBark();
		
	
}; 
