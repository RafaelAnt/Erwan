#pragma once
#include <jni.h>
#include <android/log.h>
#include <iostream>
#include <list>
#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <string.h>

#include "ProductionRule.h"

static const char* parserTAG = "RafaDebugParserCPP";

#define LOGD(...) \
  ((void)__android_log_print(ANDROID_LOG_DEBUG, parserTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, parserTAG, __VA_ARGS__))

#define PARSER_DONE 100
#define PARSER_FILE_NOT_FOUND 101
#define PARSER_SYNTAX_ERROR 102
#define PARSER_UNKNOWN_ERROR 103
#define PARSER_AXIOM_INVALID_CHARACTERS 104
#define PARSER_DEGREE_INVALID_CHARACTERS 105
#define PARSER_PRODUCTION_RULE_INVALID_CHARACTERS 106
#define PARSER_PRODUCTION_RULE_EQUALS_ERROR 107
#define PARSER_PRODUCTION_RULE_MORE_THAN_1_LEFT 108


using namespace std;



class Parser {
	string file;
	FILE *f;
	char * axiom;
	float degree;
	list<ProductionRule> productionRules;
	int nProductionRules;
	bool parsed;

public:
	Parser();
	Parser(string file);
	char* getAxiom();
	float getDegree();
	list<ProductionRule> getProductionRules();
	int getNProductionRules();
	int setFile(string file);
	int parse();
	int clean();
	void printGrammar();
	string expand(int n);
private:
	bool hasInvalidChar(int type, char* string);
	int addProdutionRule(char target, char* result);
	int separate(char* prodRule, char * target, char** result);
};
