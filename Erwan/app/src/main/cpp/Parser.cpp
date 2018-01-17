
#include "Parser.h"

#define LINE_MAX 500
#define SAVE_TO_AXIOM 200
#define SAVE_TO_DEGREE 201
#define SAVE_TO_PRODUCTION_RULES 202
#define SAVE_TO_END 203

#define TYPE_AXIOM 204
#define TYPE_DEGREE 205
#define TYPE_PRODUCTION 206


Parser::Parser() {
	file = "";
	f = NULL;
	nProductionRules = 0;
	//productionRules = list<ProductionRule>();
	parsed = false;
	degree = 0;
	axiom = (char *) malloc(LINE_MAX);
}

Parser::Parser(string file) {
	Parser::setFile(file);
	nProductionRules = 0;
	parsed = false;
	degree = 0;
	axiom = (char *)malloc(LINE_MAX);
}

char * Parser::getAxiom(){
	if (!parsed) return NULL;
	char * r = (char*) malloc(sizeof(char) * strlen(axiom) + 1);
	strcpy(r, axiom);
	return r;
}

float Parser::getDegree(){
	if (!parsed) return NULL;
	return degree;
}

list<ProductionRule> Parser::getProductionRules(){
	if (!parsed) return list<ProductionRule>();
	return list<ProductionRule>(productionRules);
}

int Parser::getNProductionRules(){
	if (!parsed) return NULL;
	return nProductionRules;
}

int Parser::setFile(string file) {
	this->file = file;
	f = fopen(file.c_str(), "r");
	if (f == NULL) {
		//assert("PARSER_FILE_NOT_FOUND" && f != NULL);
		return PARSER_FILE_NOT_FOUND;
	}
	return PARSER_DONE;
}

int Parser::separate(char* prodRule, char * target, char** result) {
	int i = 0;
	char* esq, *dir = NULL;
	int equals = 0;

	while (prodRule[i] != NULL && prodRule[i] != '\0') {
		if (prodRule[i] == '=') equals++;
		i++;
	}
	if (equals != 1) return PARSER_PRODUCTION_RULE_EQUALS_ERROR;

	esq = strtok(prodRule, "=");
    strcpy(prodRule,dir);
    LOGD("esq: %s,   dir: %s\n",esq,dir);

	
	//printf("Sizeof esq: %d\n", strlen(esq));
	if (strlen(esq) > 1) return PARSER_PRODUCTION_RULE_MORE_THAN_1_LEFT;
	if (!isalpha(esq[0])) return PARSER_PRODUCTION_RULE_INVALID_CHARACTERS;

	//printf("Esq: %s\n", esq);
	//printf("Dir: %s\n", dir);

	*target = esq[0];
	*result = dir;
	//printf("Result: %s\n", *result);

	return PARSER_DONE;
}

int Parser::addProdutionRule(char target, char* result) {
	//printf("inicio\n");
	ProductionRule pr(target, result);
	
	//printf("%c         %s\n", pr.getTarget(), pr.getResult());
	productionRules.push_back(pr);
	//printf("%c         %s\n", productionRules.front().getTarget(), productionRules.front().getResult());
	nProductionRules++;
	//printf("fim\n");
	return PARSER_DONE;
}

int Parser::parse() {
	int saveTo = -1;
	//int i = 0;

	char buf[LINE_MAX];
	char * token = NULL;
	//char * nextToken = NULL;
	char * separators = (char *) " \n\t";
	bool justTag = false;
	char c;
	char *s2 = NULL;
	int r;

	if (f == NULL) return PARSER_FILE_NOT_FOUND;
	
	//TODO: Check new strtok!!!
	while (fgets(buf, sizeof buf, f) != NULL) {
		//printf("FRASE ORIGINAL: %s\n", buf);
		token = strtok(buf, separators);
		//printf("primeiro token: %s\n", token);
		//printf("primeiro char: %c\n", token[0]);
		while (token != NULL){
			//printf("ENTREI AQUI\n");
			if (token[0] == '%') {
				//printf("COMMENT DETECTED\n");
				break;
			}
			if (strcmp(token, "#AXIOM") == 0) {
				saveTo = SAVE_TO_AXIOM;
				//printf("AXIOMA DETETADO\n");
				break;
			}
			if (strcmp(token, "#DEGREES") == 0) {
				saveTo = SAVE_TO_DEGREE;
				//printf("DEGREE DETETADO\n");
				break;
			}
			if (strcmp(token, "#PRODUCTION") == 0) {
				saveTo = SAVE_TO_PRODUCTION_RULES;
				//printf("PRODUTION DETETADO\n");
				break;
			}
			if (strcmp(token, "#END") == 0) {
				saveTo = SAVE_TO_END;
				//printf("FIM DETETADO\n");
				break;
			}
			
			switch (saveTo) {
			case(-1):
				return PARSER_SYNTAX_ERROR;

			case(SAVE_TO_AXIOM):
				if (hasInvalidChar(TYPE_AXIOM, token)) {
					return PARSER_AXIOM_INVALID_CHARACTERS;
				}
				//printf("AXIOMA: %d\n", strlen(token));
				strcpy(axiom, token);
				//printf("AXIOMA: %s\n", axiom);
				break;

			case(SAVE_TO_DEGREE):
				if (hasInvalidChar(TYPE_DEGREE, token)) {
					return PARSER_DEGREE_INVALID_CHARACTERS;
				}
				degree = (float) atof(token);
				//printf("GRAU: %f\n", degree);
				break;
				
			case(SAVE_TO_PRODUCTION_RULES):
				

				if (hasInvalidChar(TYPE_PRODUCTION, token)) {
					return PARSER_PRODUCTION_RULE_INVALID_CHARACTERS;
				}
				
				r = separate(token, &c, &s2);
				if (r == PARSER_DONE) {
					//printf("------------PRODUTION RULE: %c = %s\n", c, s2);
					addProdutionRule(c, s2);
					//printf("------------Added %c\n",c);
				}
				else {
					return r;
				}
				
				break;

			default:
				return PARSER_UNKNOWN_ERROR;
			}
			
			token = strtok(NULL, separators);
		}

		if (saveTo == SAVE_TO_END) break;


	}


	parsed = true;
	return PARSER_DONE;
}

int Parser::clean() {
	if (f != NULL) {
		fclose(f);
		f = NULL;
	}
	file = "";
	axiom = "";
	productionRules.clear();
	nProductionRules = 0;
	parsed = false;
	degree = 0;

	return PARSER_DONE;
}

void Parser::printGrammar(){
	if (!parsed) return;

	list<ProductionRule>::iterator it;

	printf("AXIOM: %s\n", getAxiom());
	printf("DEGREE: %f\n", getDegree());


	for (it = productionRules.begin(); it != productionRules.end(); it++) {
		ProductionRule aux = *it;
		printf("PRODUTION RULE:\n\tTarget: %c\n\tResult: %s\n", aux.getTarget(), aux.getResult().data());
	}
}

string Parser::expand(int n){
	if (!parsed) return nullptr;
	
	string result=getAxiom();
	char ch;
	list<ProductionRule>::iterator it;
	

	if (n < 1) return result;
	
	//printf("A expandir %s\n", getAxiom());
	//printf("A expandir %s\n", result.data());

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < result.length(); j++) {
			ch = result.at(j);
			//printf("\tA procurar char %c\n",ch);
			for (it = productionRules.begin(); it != productionRules.end(); it++) {
				ProductionRule aux = *it;
				if (aux.getTarget() == ch) {
					//printf("\t\tEncontrado!\n");
					result.replace(j, 1, aux.getResult());
					//printf("\t\tSubstituido por %s\n", aux.getResult());
					//printf("\t\t\tTamanho: %d\n\t\t\tAte agora esta: %s\n", result.length(), result.data());
					j = j + (aux.getResult().length() - 1);
					break;
				}
			}
		}
		//printf("i:  %d\nString final: %s\n", i, result.data());
	}

	return result;
}

bool Parser::hasInvalidChar(int type, char* string) {
	int i = 0;
	char axiomValidChars[] = "QWERTYUIOPASDFGHJKLZXCVBNM-+";
	char productionValidChars[] = "QWERTYUIOPASDFGHJKLZXCVBNM-+[]=*/";

	//printf("%s\n", &string[0]);

	switch (type){
	case(TYPE_AXIOM):
		while (string[i] != NULL && string[i] != '\0') {
			if (strchr(axiomValidChars, string[i]) == NULL) return true;
			i++;
		}
		break;

	case(TYPE_DEGREE):
		while (string[i] != NULL && string[i]!='\0') {
			if (!isdigit(string[i])) return true;
			i++;
		}
		break;

	case(TYPE_PRODUCTION):
		bool hasEquals = false;
		while (string[i] != NULL && string[i] != '\0') {
			if (strchr(productionValidChars, string[i]) == NULL) return true;
			i++;
		}
		break;
	}

	return false;
}


