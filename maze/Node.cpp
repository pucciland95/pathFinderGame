#include "Node.h"
#include <Windows.h>
#include <math.h>


void Node::initialize(int x, int y) {
	position.X = x;
	position.Y = y;
}

COORD Node::getPosition(){
	return position;
}

void Node::addToNodeList(Node& newNode){
	nodeList.push_front(&newNode);
}

double Node::computeDist(Node node) {

	COORD coordNode = node.getPosition();

	double distance = sqrt(pow(coordNode.X - position.X ,2) - pow(coordNode.Y - position.Y, 2));

	return distance;
}
