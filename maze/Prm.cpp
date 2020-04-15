#include "Prm.h"
#include <random>
#include <math.h>

//Help function definition

double distFunct(COORD candPosition, COORD newNodePosition);
bool alreadyPresent(int x, int y, std::vector<Node>& set);
bool alreadyPresent(int x, int y, std::vector<Node*>& set);
bool sameCoordinates(COORD coord_1, COORD coord_2);

void Prm::initialize(Node& start, Node& end, int maxIt, int m_ScreenWidth, int m_ScreenHeigth) {

	maxRep = maxIt;
	startNode = start;
	goalNode = end;

	mapWidth = m_ScreenWidth;
	mapHeight= m_ScreenHeigth;

	nodeSet = std::vector<Node>(maxIt);

}

void Prm::findNearestNodes() {

	double nodeDistance;
	Node candNode; 
	Node compNode;

	for (int i = 0; i < maxRep - 1; i++) {
		for (int j = i + 1; j < maxRep; j++) {

			nodeDistance = nodeSet[i].computeDist(nodeSet[j]);

			if (nodeDistance < 5){

				nodeSet[i].addToNodeList(nodeSet[j]); // Both the nodes need to know that they have been connected
				nodeSet[j].addToNodeList(nodeSet[i]); // Both the nodes need to know that they have been connected

			}
		}
	}

	return;
}

bool Prm::findPath() {

	// find a new point from a uniform disrtribution that is not in the same position of the player and is beweeen the screen bounds

	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> distributionX(0, mapWidth - 1);
	std::uniform_int_distribution<int> distributionY(0, mapHeight - 1);

	int xRandom;
	int yRandom;

	COORD startPosition = startNode.getPosition();
	
	int iterations = 0;

	Node newNode;

	while (iterations < maxRep){

		do
		{
			xRandom = distributionX(generator);
			yRandom = distributionY(generator);

		} while (xRandom == startPosition.X && yRandom == startPosition.Y && alreadyPresent(xRandom, yRandom, nodeSet));


		// link it to the nearest node depending by a distance function that we will take as the norm 2 

		// New node initialization
		nodeSet[iterations].initialize(xRandom, yRandom);

		// wait for finishing all the required iterations
		iterations++;
	}

	// Connecting the nodes
	findNearestNodes();

	Node* pNewNode;

	// connect the whole net to start and goal

	//Starting node
	Node bestNode;
	double minDist = 1000;
	double candNodeStartNodeDist;

	for (const Node& candNode : nodeSet) {

		candNodeStartNodeDist = startNode.computeDist(candNode);

		if (candNodeStartNodeDist < minDist){
			minDist = candNodeStartNodeDist;
			bestNode = candNode;
		}
	}

	startNode.addToNodeList(bestNode);
	bestNode.addToNodeList(startNode);

	//nodeSet.push_back(startNode); // Adding to the total nodes the starting one

	//Goal Node

	minDist = 1000;

	for (const Node& candNode : nodeSet) {

		candNodeStartNodeDist = goalNode.computeDist(candNode);

		if (candNodeStartNodeDist < minDist) {
			minDist = candNodeStartNodeDist;
			bestNode = candNode;
		}
	}

	goalNode.addToNodeList(bestNode);
	bestNode.addToNodeList(goalNode);
	goalNode.setPrev(bestNode);
	//nodeSet.push_back(goalNode); // Adding to the total nodes the staring one


	// find the path that connect start to goal

	if (!constructPath())
		return 0;

	return 1;
}

bool Prm::constructPath() {

	std::vector<Node*> exploredNodes = { };

	std::list<Node*> nodeQueue = {&startNode};
	Node* pActualNode = nodeQueue.front();
	nodeQueue.pop_front();
	exploredNodes.push_back(&startNode);

	COORD consideredNodePosition = pActualNode->getPosition();
	COORD finalPosition = goalNode.getPosition();
	
	std::list<Node*> nodeList;
	COORD eachNodePosition;

	while (!sameCoordinates(consideredNodePosition, finalPosition)) {

		nodeList = pActualNode->getNodeList();

		for (Node* eachNode : nodeList) {

			eachNodePosition = eachNode->getPosition();

			if (!alreadyPresent(eachNodePosition.X, eachNodePosition.Y, exploredNodes)){

				nodeQueue.push_front(eachNode);

				pActualNode->setNext(*eachNode);
				eachNode->setPrev(*pActualNode);

			}
		}
		if (nodeQueue.size() < 1)
			return 0;

		pActualNode = nodeQueue.front();
		nodeQueue.pop_front(); // and removing it
		exploredNodes.push_back(pActualNode);
		consideredNodePosition = pActualNode->getPosition();

	}

	// Reverse the path
	COORD initialPosition = startNode.getPosition();

	pActualNode->deleteNext();  // Sets the pointer of the last node to NULL

	Node* pNodePrew = pActualNode;
	Node* pNodeAct;

	while (!sameCoordinates(pNodePrew->getPosition(), initialPosition)) {

		pNodeAct = pNodePrew->getPrew();

		pNodeAct->setNext(*pNodePrew);

		pNodePrew = pNodeAct;

	}

	return 1;
}

bool alreadyPresent(int x, int y, std::vector<Node>& nodeSet) {
	COORD eachPosition;

	for (Node eachNode : nodeSet) {

		eachPosition = eachNode.getPosition();

		if (eachPosition.X == x && eachPosition.Y == y)
			return 1;
	}

	return 0;
}

bool alreadyPresent(int x, int y, std::vector<Node*>& pNodeSet) {
	COORD eachPosition;

	for (Node* eachNode : pNodeSet) {

		eachPosition = eachNode->getPosition();

		if (eachPosition.X == x && eachPosition.Y == y)
			return 1;
	}

	return 0;
}

double distFunct(COORD candPosition, COORD newNodePosition) {

	return sqrt(pow(candPosition.X - newNodePosition.X, 2) + pow(candPosition.Y - newNodePosition.Y, 2));

}

bool sameCoordinates(COORD coord_1, COORD coord_2) {

	if (coord_1.X == coord_2.X && coord_1.Y == coord_2.Y)
		return 1;

	else
		return 0;

}











