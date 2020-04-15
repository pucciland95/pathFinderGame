#pragma once

#include <vector>
#include"Node.h"


class Prm{

private:

	std::vector<Node> nodeSet;
	Node startNode;
	Node goalNode;

	int maxRep;

	int mapWidth;
	int mapHeight;

public:

	Prm() {};

	void initialize(Node& start, Node& end, int maxRep, int mapWidth, int mapHeigth);

	bool findPath();

	int getMaxRep() { return maxRep; }

	Node* getStartPosition() { return &startNode; }

	std::vector<Node>& getNodeSet() { return nodeSet; }


	bool constructPath();

	void findNearestNodes();

};

