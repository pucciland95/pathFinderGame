#include <Windows.h>
#include <list>
#include <vector>

#pragma once
class Node
{
private:

	Node* next;
	Node* prew;

	std::list<Node*> nodeList;

	COORD position;

public:

	Node() {};

	void initialize(int x, int y);

	void addToNodeList(Node& newNode);

	std::list<Node*> getNodeList() { return nodeList; }

	Node* getNext() { return next; }

	Node* getPrew() { return prew; };

	COORD getPosition();

	void setNext(Node& succ) { next = &succ; };

	void setPrev(Node& prec) { prew = &prec; };

	void deleteNext() { next = NULL; }

	void deletePrew() { prew = NULL; }

	double computeDist(Node node);

};

