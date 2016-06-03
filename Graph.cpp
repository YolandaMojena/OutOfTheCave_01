// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Graph.h"

using namespace std;

Graph::Graph() {}

Graph::~Graph() {}

Graph::Graph(Node* n) {
	firstNode = n;
	_lastNode = n;
}

void Graph::AddNode(Node* n) {
	n->SetGraph(this);
	if (firstNode == nullptr)
		firstNode = n;
	else
		_lastNode->nextNodes.push_back(n);
	
	_lastNode = n;
}

void Graph::AddNodeInNewBranch(Node* n, int depth) {
	n->SetGraph(this);

	Node* currentNode = firstNode;

	for (int i = 0; i < depth; i++)
		currentNode = currentNode->nextNodes[currentNode->nextNodes.size() - 1];

	currentNode->nextNodes.push_back(n);

	_lastNode = n;
}

void Graph::ExecuteTask() {
	firstNode->ExecuteTask();
}

void Graph::TaskCompleted(bool completedOk) {
	if(completedOk && firstNode != _lastNode)
		firstNode = firstNode->nextNodes[0]; //BRANCH!!!
	else
		EndGraph();
}

void Graph::EndGraph() {}
