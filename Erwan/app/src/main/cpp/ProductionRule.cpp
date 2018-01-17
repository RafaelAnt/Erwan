#include "ProductionRule.h"


ProductionRule::ProductionRule(char target, char* result) {
	this->target = target;
	this->result = string(result);
}

char ProductionRule::getTarget() {
	return this->target;
}

string ProductionRule::getResult() {
	return this->result;
}