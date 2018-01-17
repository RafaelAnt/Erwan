#pragma once
#include <iostream>
#include <stdio.h>

using namespace std;

class ProductionRule {
	char target;
	string result;

public:
	ProductionRule(char target, char* result);
	char getTarget();
	string getResult();
	//int separateProdRule(char * rule);
};