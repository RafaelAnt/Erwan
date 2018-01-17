#include "TreeNode.h"

TreeNode::TreeNode(){
	this->type = '0';
	width = -1;
	length = -1;
	degree = 1;
	stage = -1;
	nodes = list<TreeNode*>();
	color[0] = -1;
	color[1] = -1;
	color[2] = -1;
	created = currentTimeInNanos();
	father = nullptr;
}

TreeNode::TreeNode(char type, TreeNode* father){
	this->type = type;
	width = 0;
	length = 0;
	degree = 1;
	stage = 1;
	nodes = list<TreeNode*>();
	color[0] = 0;
	color[1] = 1;
	color[2] = 0;
	created = currentTimeInNanos();
	this->father = father;
}


TreeNode::TreeNode(char type, TreeNode * father, int stage){
	this->type = type;
	width = 0;
	length = 0;
	degree = 1;
	this->stage = stage;
	circlePoints = vector<Point3>();
	centralPoint = Point3(0, 0, 0);
	nodes = list<TreeNode*>();
	color[0] = 0;
	color[1] = 1;
	color[2] = 0;
	created = currentTimeInNanos();
	this->father = father;
	this->lastFromStage = false;
	this->lastPointFromStage = Point3(0, 0, 0);


	if (type == '+' || type == '-') length = 1;
	else length = 0;
}

TreeNode::TreeNode(const TreeNode &node) {
	this->type = node.type;
	this->width = node.width;
	this->length = node.length;
	this->degree = node.degree;
	this->stage = node.stage;
	circlePoints = vector<Point3>();
	centralPoint = Point3(0, 0, 0);
	this->nodes = list<TreeNode*>(node.nodes);
	this->color[0] = node.color[0];
	this->color[1] = node.color[1];
	this->color[2] = node.color[2];
	this->created = node.created;
	this->father = node.father;
	this->lastFromStage = node.lastFromStage;
	this->lastPointFromStage = node.lastPointFromStage;
}

int TreeNode::getType(){
	switch (type){
	case 'F':
	case 'X':
		return TREE_NODE_TYPE_BRANCH;
	case '-':
		return TREE_NODE_TYPE_TURN_LEFT;
	case '+':
		return TREE_NODE_TYPE_TURN_RIGHT;
	case '/':
		return TREE_NODE_TYPE_SWITCH_LEFT;
	case '*':
		return TREE_NODE_TYPE_SWITCH_RIGHT;
	default:
		assert("TREE_NODE_TYPE_ERROR: unknown type" && false);
	}

}

float TreeNode::getWidth(){
	return width;
}

int TreeNode::setWidth(float newWidth){
	assert("TREE_NODE_INVALID_VALUE: newWidth" && newWidth >= 0);
	width = newWidth;
	return TREE_NODE_DONE;
}

float TreeNode::getLength(){
	return length;
}

int TreeNode::setLength(float newLength){
	assert("TREE_NODE_INVALID_VALUE: newLength" && newLength >= 0);
	length = newLength;
	return TREE_NODE_DONE;
}

float TreeNode::getDegree(){
	return this->degree;
}

int TreeNode::setDegree(float newDegree){
	this->degree = newDegree;
	return TREE_NODE_DONE;
}

int TreeNode::getStage(){
	return stage;
}

int TreeNode::setStage(int newStage){
	assert("TREE_NODE_INVALID_VALUE: newStage" && newStage <= 0);
	stage = newStage;
	return TREE_NODE_DONE;
}

double TreeNode::getCreated(){
	return created;
}

int TreeNode::setCreated(double time){
	this->created = time;
	return TREE_NODE_DONE;
}

list<TreeNode*> TreeNode::getNodes(){
	return nodes;
}

void TreeNode::addNode(TreeNode* node){
	nodes.push_back(node);
}

Point3 TreeNode::getCentralPoint(){
	return this->centralPoint;
}

int TreeNode::setCentralPoint(Point3 p){
	//if (p == nullptr) return TREE_NODE_NULL_POINT;
	this->centralPoint = p;
	return TREE_NODE_DONE;
}

int TreeNode::setCentralPoint(float x, float y, float z){
	this->centralPoint = Point3(x, y, z);
	return TREE_NODE_DONE;
}

Point3 TreeNode::getGrowthDirection(){
	return growthDirection;
}

int TreeNode::setGrowthDirection(Point3 p){
	growthDirection = p;
	return TREE_NODE_DONE;
}

int TreeNode::setGrowthDirection(float x, float y, float z){
	growthDirection = Point3(x, y, z);
	return TREE_NODE_DONE;
}

vector<Point3> TreeNode::getCirclePoints(){
	return circlePoints;
}

int TreeNode::setCirclePoints(vector<Point3> points){
	this->circlePoints = points;
	return TREE_NODE_DONE;
}

vector<Point3> TreeNode::getNormalCirclePoints(){
	return this->normalCirclePoints;
}

int TreeNode::setNormalCirclePoints(vector<Point3> points){
	this->normalCirclePoints = points;
	return TREE_NODE_DONE;
}


/// <summary>
/// Gets the father of this node. Can be null.
/// </summary>
/// <returns></returns>
TreeNode * TreeNode::getFather() {
	return father;
}

int TreeNode::setFather(TreeNode * newFather){
	assert("TREE_NODE_INVALID_VALUE: newFather" && newFather == nullptr && newFather == NULL);
	this->father = newFather;
	return TREE_NODE_DONE;
}

int TreeNode::grow(list<ProductionRule> prodRule){
	list<ProductionRule>::iterator it;
	list<TreeNode*>::iterator tnIt;
	string::iterator sIt;
	list<TreeNode*> old;
	string result;
	TreeNode *aux;
	TreeNode *current = this;
	TreeNode *goBackTo = this;
	int stageMod = this->stage;

	//printf("Growing %c...\n", type);

	// Do not grow auxiliary nodes, such as - + * / 
	if (this->getType() == TREE_NODE_TYPE_TURN_LEFT 
		|| this->getType() == TREE_NODE_TYPE_TURN_RIGHT
		|| this->getType() == TREE_NODE_TYPE_SWITCH_RIGHT
		|| this->getType() == TREE_NODE_TYPE_SWITCH_LEFT) {

		for (tnIt = this->nodes.begin(); tnIt != this->nodes.end(); tnIt++) {
			aux = *tnIt;
			int r = aux->grow(prodRule);
			if (r != TREE_NODE_DONE) return r;
		}
		return TREE_NODE_DONE;
	}
	else {

		// Save Old Nodes
		if (!this->nodes.empty()) {
			//printf("\tSaving old nodes...\n");
			old = list<TreeNode*>(this->nodes);
			this->nodes.clear();
		}



		//printf("\tFinding Production Rule...\n");
		for (it = prodRule.begin(); it != prodRule.end(); it++) {
			ProductionRule aux = *it;
			if (aux.getTarget() == this->type) {
				result = aux.getResult();
				break;
			}
		}
	
		assert("TREE_NODE_INVALID_PRODUCTION_RULE" && result.size() > 0);

		if (result.at(0) != this->type) {
			//printf("\tChanging my type to %c...\n", result.at(0));
			this->type = result.at(0);
		}

		//printf("\tGrowth started...\n");
		for (sIt = result.begin() + 1; sIt != result.end(); sIt++) {
			char c = *sIt;
			switch (c) {
			case 'X':
				aux = new TreeNode('X', current, stageMod);
				current->addNode(aux);
				current = aux;
				break;

			case 'F':
				aux = new TreeNode('F', current, stageMod);
				current->addNode(aux);
				//printf("\t\tF\t\tNew F Added to Current: angle: %f, stage: %d\n", aux->getAngle(), stageMod);
				current = aux;
				break;

			case '+':
				aux = new TreeNode('+', current, stageMod);
				current->addNode(aux);
				current = aux;
				break;

			case '-':
				aux = new TreeNode('-', current, stageMod);
				current->addNode(aux);
				current = aux;
				break;

			case '*':
				aux = new TreeNode('*', current, stageMod);
				current->addNode(aux);
				current = aux;
				break;

			case '/':
				aux = new TreeNode('/', current, stageMod);
				current->addNode(aux);
				current = aux;
				break;

			case '[':
				//printf("\t\t[\n");
				goBackTo = current;
				stageMod++;
				break;

			case ']':
				//printf("\t\t]\n");
				current = goBackTo;
				stageMod--;
				break;

			default:
				assert("TREE_NODE_UNDIFINED_SYMBOL" && true);
			}
		}


		//printf("\tLoading old nodes...\n");
		for (tnIt = old.begin(); tnIt != old.end(); tnIt++) {
			aux = *tnIt;
			int r = aux->grow(prodRule);
			if (r != TREE_NODE_DONE) return r;
			current->addNode(aux);
		}
		//printf("\tComplete...\n");
	}

	return TREE_NODE_DONE;
}

string TreeNode::getLSystem(){
	list<TreeNode*>::iterator it;
	TreeNode* aux;
	string r;
	r += type;

	if (nodes.size() == 0) {
		return r;
	}

	if (nodes.size() >= 1) {
		for (it = nodes.begin(); it != nodes.end(); it++) {
			aux = *it;
			if (aux->getStage() > this->getStage()) {
				r += '[';
			}
			r += aux->getLSystem();
			if (aux->getStage() > this->getStage()) {
				r += ']';
			}
		}
	}
	return r;
}

int TreeNode::getBranchNumber(){
	list<TreeNode*>::iterator it;
	TreeNode* aux;

	if (this->nodes.size() == 0) return 0;

	int r = 0;

	for (it = nodes.begin(); it != nodes.end(); it++) {
		aux = *it;
		if(aux->getType() == TREE_NODE_TYPE_BRANCH)	r++;
		else r += aux->getBranchNumber();
	}
	return r;
}

int TreeNode::clearPoints(){
	this->centralPoint = { 0,0,0 };
	circlePoints.clear();
	return TREE_NODE_DONE;
}

bool TreeNode::isLastFromStage(){
	return lastFromStage;
}

int TreeNode::setIsLastFromStage(bool is){
	this->lastFromStage = is;
	return TREE_NODE_DONE;
}

bool TreeNode::checkIfIsLastFromStage(){
	list<TreeNode*>::iterator it;
	TreeNode * aux;
	bool r = true;

	if (nodes.size() == 0) r = true;

	for (it = nodes.begin(); it != nodes.end(); it++) {
		aux = *it;
		if (aux->getStage() == stage) {

			if (aux->getType() == TREE_NODE_TYPE_BRANCH && aux->getLength() > 0) {
				r = false;
				break;
			}

			if (aux->getType() == TREE_NODE_TYPE_TURN_LEFT || aux->getType() == TREE_NODE_TYPE_TURN_RIGHT || aux->getType() == TREE_NODE_TYPE_SWITCH_LEFT || aux->getType() == TREE_NODE_TYPE_SWITCH_RIGHT) {
				if (!aux->checkIfIsLastFromStage()) {
					r = false;
					break;
				}
			}
		}
	}

	lastFromStage = r;
	return r;
}

int TreeNode::setLastPointFromStage(Point3 point){
	lastPointFromStage = point;
	return TREE_NODE_DONE;
}

Point3 TreeNode::getLastPointFromStage(){
	return lastPointFromStage;
}

int TreeNode::calculateLastPointFromStage(){
	assert("Is last from stage." && lastFromStage == true);
	this->lastPointFromStage = centralPoint + (Point3(0, this->length, 0) * growthDirection);
	return TREE_NODE_DONE;
}

