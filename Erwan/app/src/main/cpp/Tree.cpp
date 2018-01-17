#include "Tree.h"


vector<Point3> currentPoints;
Point3 last;

Tree::Tree(){
	start = TreeNode();
	maxLength = -1;
	maxWidth = -1;
	lengthGrowthRate = -1;
	widthGrowthRate = -1;
	angleGrowthRate = -1;
}

Tree::Tree(string axiom, list<ProductionRule> prods,float maxLength, float maxWidth, float lengthGrowthRate, float widthGrowthRate, float angleGrowthRate, float angle){
	productionRules = list<ProductionRule>(prods);
	this->maxLength = maxLength;
	this->maxWidth = maxWidth;
	this->lengthGrowthRate = lengthGrowthRate;
	this->widthGrowthRate = widthGrowthRate;
	this->angleGrowthRate = angleGrowthRate;
	this->angle = angle;
	TreeNode *aux;
	this->vsml = VSMathLib::getInstance();
	//this->leaf = VSSurfRevLib();

	char ch = axiom.at(0);
	start = TreeNode(ch, nullptr);

	TreeNode *last = &start;
	for (unsigned int j = 1; j < axiom.length(); j++) {
		ch = axiom.at(j);
		aux = new TreeNode (ch, last);
		last->addNode(aux);
		last = aux;
	}
}

TreeNode Tree::getStart(){
	return start;
}

int Tree::setStart(TreeNode start){
	//if(start==nullptr) return TREE_INVALID_VALUE;
	this->start = start;
	return TREE_DONE;
}

float Tree::getMaxLength(){
	return maxLength;
}

int Tree::setMaxLength(float length){
	if (length <= 0) return TREE_INVALID_VALUE;
	this->maxLength = length;
	return TREE_DONE;
}

float Tree::getMaxWidth(){
	return maxWidth;
}

int Tree::setMaxWidth(float width){
	if (width <= 0) return TREE_INVALID_VALUE;
	this->maxWidth = width;
	return TREE_DONE;
}

float Tree::getLenghGrowthRate(){
	return lengthGrowthRate;
}

int Tree::setLengthGrowthRate(float rate){
	if (rate <= 0) return TREE_INVALID_VALUE;
	this->lengthGrowthRate = rate;
	return TREE_DONE;
}

float Tree::getAngleGrowthRate(){
	return angleGrowthRate;
}

int Tree::setAngleGrowthRate(float rate){
	if (rate <= 0) return TREE_INVALID_VALUE;
	this->angleGrowthRate = rate;
	return TREE_DONE;
}

float Tree::getWidthGrowthRate(){
	return widthGrowthRate;
}

int Tree::setWidthGrowthRate(float rate){
	if (rate <= 0) return TREE_INVALID_VALUE;
	this->widthGrowthRate = rate;
	return TREE_DONE;
}

int Tree::grow(int number){
	int r;
	for (int i = 0; i < number; i++) {
		r=start.grow(productionRules);
		if (r != TREE_NODE_DONE) {
			return r;
		}
	}

	return TREE_DONE;
}

int Tree::drawLine(TreeNode * node) {
	assert("TreeNode Is Null" && node != NULL);
	
	vector<Point3> points={
		Point3(0,0,0),
		Point3(0,node->getLength(),0)
	};
	pp.set(points);
	pp.render();

	/* DEBUG - Complete
	
	if (node->isLastFromStage()) {
		vsml->pushMatrix(VSMathLib::MODEL);
		vsml->translate(0, node->getLength(), 0);
		VSSurfRevLib vssrl;
		vssrl.setColor(VSResourceLib::EMISSIVE, 1, 0, 0, 0);
		vssrl.createSphere(0.1, 10);
		vssrl.render();
		vsml->popMatrix(VSMathLib::MODEL);
	}*/

	return TREE_DONE;
}

int drawBark(TreeNode * node) {
	assert("TreeNode Is Null" && node != NULL);

	VSSurfRevLib bark;
	TreeNode *father = node->getFather();

	if (father == NULL) return TREE_DONE;

	while (father->getType() != TREE_NODE_TYPE_BRANCH) {
		father = father->getFather();
		if (father == NULL) return TREE_DONE;
	}
	
	float points[8];
	points[0] = 0;
	points[1] = 0;

	points[2] = father->getWidth();
	points[3] = 0;

	points[4] = node->getWidth();
	points[5] = node->getLength();

	points[6] = 0;
	points[7] = node->getLength();

	bark.create(points, 4, 10, 1, 1.0f);
	bark.setColor(VSResourceLib::EMISSIVE, 0, 0, 1, 0);
	bark.render();

	return TREE_DONE;
}

int Tree::drawIntersection(TreeNode * node){
	VSSurfRevLib vssrl;
	
	if (node->getBranchNumber() > 1) {
		vssrl.createSphere(node->getWidth(), 7);
		vssrl.setColor(VSResourceLib::EMISSIVE, 0, 1, 0, 1);
		vssrl.render();
	}
	return TREE_DONE;
}

/// <summary>
/// Rotates the tree node "node->getDegree()" degrees in relation to the Y axis. Controls the height or beta angle on spheric coordinates.
/// </summary>
/// <param name="node">The Tree Node to be rotated.</param>
void Tree::rotL(TreeNode* node) {
	vsml->rotate(node->getDegree(), 0, 1, 0);
}

/// <summary>
/// Rotates the tree node "-node->getDegree()" degrees in relation to the Y axis.  Controls the height or beta angle on spheric coordinates.
/// </summary>
/// <param name="node">The Tree Node to be rotated.</param>
void Tree::rotR(TreeNode* node) {
	vsml->rotate(-node->getDegree(), 0, 1, 0);
}

/// <summary>
/// Rotates the tree node "node->getDegree()" degrees in relation to the X axis.  Controls the direction or alpha angle on spheric coordinates.
/// </summary>
/// <param name="node">The Tree Node to be rotated.</param>
void Tree::switchL(TreeNode* node) {
	vsml->rotate(node->getDegree(), 1, 0, 0);
}

/// <summary>
/// Rotates the tree node "-node->getDegree()" degrees in relation to the X axis.  Controls the direction or alpha angle on spheric coordinates.
/// </summary>
/// <param name="node">The Tree Node to be rotated.</param>
void Tree::switchR(TreeNode* node) {
	vsml->rotate(-node->getDegree(), 1, 0, 0);
}




int Tree::incrementLength(TreeNode *current){
	float l = current->getLength();
	if (l >= maxLength) return TREE_MAX_LENGTH_REACHED;

	l += lengthGrowthRate / current->getStage();

	current->setLength(l);

	return TREE_DONE;
}

int Tree::incrementWidth(TreeNode *current){
	float w = current->getWidth();
	if (w >= (maxWidth - (0.05*current->getStage()))) return TREE_MAX_WIDTH_REACHED;

	w += widthGrowthRate / current->getStage();

	current->setWidth(w);
	
	return TREE_DONE;
}

int Tree::incrementDegree(TreeNode * current){
	float a = current->getDegree();
	if (a >= (angle - (0.05*current->getStage()))) return TREE_MAX_ANGLE_REACHED;

	a += angleGrowthRate / current->getStage();

	current->setDegree(a);

	return TREE_DONE;
}


bool isLastFromStage(TreeNode * current) {
	int stage = current->getStage();
	list<TreeNode*> nodes = current->getNodes();
	list<TreeNode*>::iterator it;
	TreeNode * aux;

	if (nodes.size() == 0) return true;

	for (it = nodes.begin(); it != nodes.end(); it++) {
		aux = *it;
		if (aux->getStage() == stage) {
			if (aux->getType() == TREE_NODE_TYPE_BRANCH && aux->getLength() > 0) return false;
			if (aux->getType() == TREE_NODE_TYPE_TURN_LEFT || aux->getType() == TREE_NODE_TYPE_TURN_RIGHT){
				if (!isLastFromStage(aux)) return false;
			}
		}
	}

	return true;
}


int Tree::buildCentralPoints(TreeNode * current){
	
	float* modelMatrix;
	modelMatrix = vsml->get(VSMathLib::VIEW_MODEL);

	current->setCentralPoint(modelMatrix[12],modelMatrix[13], modelMatrix[14]);
	current->setGrowthDirection(modelMatrix[4], modelMatrix[5], modelMatrix[6]);

	// In order to draw the last one:
	bool isLast = current->checkIfIsLastFromStage();
	if (isLast == true) {
		current->calculateLastPointFromStage();
	}
	/*if (isLastFromStage(current) && current->getStage() == 1) {
		vsml->pushMatrix(VSMathLib::MODEL);
		vsml->translate(0, current->getLength(), 0);
		modelMatrix = vsml->get(VSMathLib::VIEW_MODEL);
		last = Point3(modelMatrix[12], modelMatrix[13], modelMatrix[14]);
		vsml->popMatrix(VSMathLib::MODEL);
	}*/
	
	return TREE_DONE;
}

static void MMult1x3By3x3(float a[3], float b[3][3], float res[3]) {
	for (int i = 0; i < 3; i++) {
		float sumAux = 0;
		for (int j = 0; j < 3; j++) {
			sumAux += a[j] * b[j][i];

		}
		res[i] = sumAux;
	}
}

int Tree::buildCirclePoints(TreeNode * current) { // TODO: Check
	// Draw the circle
	const int slices = 10;
	float spaceBetweenPoints = (2 * 3.1415) / slices;
	vector<Point3> circlePoints;
	vector<Point3> normalPoints;
	float random[3] = { 0.0f, -1.0f, 0.0f }; 
	float res[3];
	Point3 normal;

	// Get a perpendicular vector to the growing direction vector.
	vsml->crossProduct(current->getGrowthDirection().toVec3f(), random, res);
	vsml->normalize(res);
	normal = Point3(res);
	
	// Some Variables for the rotation matrix
	float cs = cos(spaceBetweenPoints);
	float sn = sin(spaceBetweenPoints);
	float u[3] = { current->getGrowthDirection().x, current->getGrowthDirection().y, current->getGrowthDirection().z };
	float uu[3] = { u[0] * u[0] , u[1] * u[1] , u[2] * u[2] };
	float R[3][3];

	// Rotation Matrix
	R[0][0] = cs + uu[0] * (1 - cs);
	R[0][1] = u[0] * u[1] * (1 - cs) - u[2] * sn;
	R[0][2] = u[0] * u[1] * (1 - cs) + u[1] * sn;

	R[1][0] = u[1] * u[0] * (1 - cs) + u[2] * sn;
	R[1][1] = cs + uu[1] * (1 - cs);
	R[1][2] = u[1] * u[2] * (1 - cs) - u[0] * sn;

	R[2][0] = u[2] * u[0] * (1 - cs) - u[1] * sn;
	R[2][1] = u[2] * u[1] * (1 - cs) + u[0] * sn;
	R[2][2] = cs + uu[2] * (1 - cs);
	
	for (float i = 0; i < slices; i += 1) {
		//rotate normal by i * spaceBetweenPoints;
		
		normalPoints.push_back(normal);
		circlePoints.push_back(current->getCentralPoint() + (normal * current->getWidth()));
			
		MMult1x3By3x3(normal.toVec3f(), R, res);
		normal = Point3(res);
			
	}

	current->setCirclePoints(circlePoints);
	current->setNormalCirclePoints(normalPoints);

	return TREE_DONE;
}

int Tree::drawLeaves(TreeNode * current) {
	float angle, length;
	const float currentLength = current->getLength();
	
	// initialize random seed
	srand(5);
	if (current->getStage() >= 2) {
		int nLeaves = rand() % 5 + 5;		
		for (int i = 0; i < nLeaves; i++) {
			
			// generate number between 0 and 359:
			angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360));
			// generate number between 0 and 
			length = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / currentLength));

			vsml->pushMatrix(VSMathLib::MODEL);
			vsml->translate(0, length,0);
			vsml->rotate(angle, 0, 1, 0);
			vsml->rotate(90, 1, 0, 0);
			
			this->leaf.render();
			vsml->popMatrix(VSMathLib::MODEL);
		}
	}
	return TREE_NODE_DONE;
}

int Tree::buildpoints(TreeNode* node) {
	list<TreeNode*>::iterator it;
	list<TreeNode*> nodes = node->getNodes();
	TreeNode* aux;
	int r;

	switch (node->getType()){
	case TREE_NODE_TYPE_BRANCH:
		if (node->getLength() > 0) {
			buildCentralPoints(node);
			buildCirclePoints(node);

			drawLine(node);
			//drawBark(node);
			vsml->translate(0, node->getLength(), 0);
			//drawIntersection(node);

			//drawLeaves(node);

		}
		break;
	case TREE_NODE_TYPE_TURN_RIGHT:
		rotR(node);
		break;
	case TREE_NODE_TYPE_TURN_LEFT:
		rotL(node);
		break;
	case TREE_NODE_TYPE_SWITCH_RIGHT:
		switchR(node);
		break;
	case TREE_NODE_TYPE_SWITCH_LEFT:
		switchL(node);
		break;
	default:
		assert("Unexpected Tree Node Type!" && false);
		break;
	}

	if (nodes.size() == 0) {
		return TREE_DONE;
	}

	if (nodes.size() >= 1) {

		for (it = nodes.begin(); it != nodes.end(); it++) {
			aux = *it;
			if (aux->getStage() > node->getStage()) {
				vsml->pushMatrix(VSMathLib::MODEL);
			}
			r = buildpoints(aux);
			if (r != TREE_DONE) return r;
			if (aux->getStage() > node->getStage()) {
				vsml->popMatrix(VSMathLib::MODEL);
			}
		}
	}

	return TREE_DONE;
}

//float points[];

int gatherPoints(TreeNode* current) {
	list<TreeNode*>::iterator it;
	list<TreeNode*> nodes;

	// main stage so far...
	if (current->getStage() == 1 && current->getLength()>0) {
		if (current->getType() == TREE_NODE_TYPE_BRANCH && current->getBranchNumber()>2) {
			currentPoints.push_back(current->getCentralPoint());
		}
		
		nodes = current->getNodes();

		for (it = nodes.begin(); it != nodes.end(); it++) {
			gatherPoints(*it);
		}
		//Add imaginary last point.
		//currentPoints.push_back(last);
		if (current->isLastFromStage()) currentPoints.push_back(current->getLastPointFromStage());
	}

	return TREE_DONE;
}



int Tree::getBarkPoints() {

	vector<Point3> points = start.getCirclePoints();

	for (int i = 0; i < points.size(); i++) {
		vsml->pushMatrix(VSMathLib::MODEL);
		vsml->translate(points.at(i).x, points.at(i).y, points.at(i).z);
		VSSurfRevLib vssrl;
		vssrl.setColor(VSResourceLib::EMISSIVE, 1, 0, 0, 0);
		vssrl.createSphere(0.05, 3);
		vssrl.render();
		vsml->popMatrix(VSMathLib::MODEL);
	}
	
	return TREE_DONE;
}

int Tree::drawBark() {
	VSModelLib bark;

	getBarkPoints();
	
	//bark.addMesh(, , , NULL, NULL, NULL, , );
	//bark.render();

	return TREE_DONE;
}

int Tree::draw(){
	/*VSCubicCurve cc;
	currentPoints.clear();*/


	pp.setColor(VSResourceLib::EMISSIVE, 0, 1, 0, 1);

	int r= buildpoints(&start);

	drawBark();


	//add imaginary first point;
	/*currentPoints.push_back(Point3(0, -1, 0));
	gatherPoints(&start);

	//drawBark();


	vsml->loadIdentity(VSMathLib::MODEL);
	vsml->loadIdentity(VSMathLib::VIEW);

	if (currentPoints.size() > 4) {
		cc.setType(VSCubicCurve::CATMULL_ROM);
		cc.set(currentPoints, 8, false);
		cc.setColor(VSResourceLib::EMISSIVE, 1, 0, 0, 1);
		cc.render();
	}
	else return TREE_NOT_ENOUGH_POINTS_FOR_CATMULLROM; */

	

	//if(currentPoints.size() > 0) printf("Last Point is: %f %f %f\n", currentPoints.back()->x, currentPoints.back()->y, currentPoints.back()->z);

	return r;
}

string Tree::getLSystem(){
	return start.getLSystem();
}

int Tree::animate(double time){
	TreeNode *current;
	list<TreeNode*> TNlist;
	list<TreeNode*>::reverse_iterator it;
	stack<TreeNode*> q;

	q.push(&start);

	// TODO: Change the angle to make it blow in the wind

	while (!q.empty()) {
		current = q.top();
		q.pop();

		incrementLength(current);
		incrementWidth(current);
		incrementDegree(current);

		if(current->getLength()>0.3){
			if (current->getNodes().size() != 0) {
				TNlist = current->getNodes();
				for (it = TNlist.rbegin(); it != TNlist.rend(); it++)
					q.push(*it);
			}
		}
		
	}

	return TREE_DONE;
}

/// <summary>
/// Resets the tree to its basic form.
/// </summary>
/// <returns></returns>
int Tree::reset() {
	TreeNode *current;
	list<TreeNode*> TNlist;
	list<TreeNode*>::iterator it;
	stack<TreeNode*> q;

	q.push(&start);

	while (!q.empty()) {
		current = q.top();
		q.pop();

		current->setLength(0);
		current->setWidth(1);

		if (current->getNodes().size() != 0) {
			TNlist = current->getNodes();
			for (it = TNlist.begin(); it != TNlist.end(); it++)
				q.push(*it);
		}
	}

	return TREE_DONE;
}

int numberOfNodesFromStage(TreeNode * current) {
	assert("Current TreeNode is NULL" && current != NULL);
	int r = 1;
	list<TreeNode*> nodes;
	list<TreeNode*>::iterator it;
	TreeNode* aux;

	nodes = current->getNodes();
	if (nodes.size() == 0) return 0;

	for (it = nodes.begin(); it != nodes.end(); it++) {
		aux = *it;
		if (aux->getStage() == current->getStage()) {
			if (aux->getType() == TREE_NODE_TYPE_BRANCH) {
				r++;
			}
			r += numberOfNodesFromStage(aux);
		}
		
	}

	return r;
}

int Tree::setDevelopment(int percent){
	if (percent < 0 || percent>100) return TREE_INVALID_VALUE;
	
	numberOfNodesFromStage(&start);
	return TREE_DONE;
}

int Tree::drawStaticTree(){
	return TREE_DONE;
}

int Tree::init(){
	this->leaf.createCone(0.15, 0.1, 3);
	this->leaf.setColor(VSResourceLib::EMISSIVE, 0.85, 0.56, 0.01, 0);
	
	return TREE_DONE;
}
