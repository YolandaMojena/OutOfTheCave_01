// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Graph.h"

using namespace std;

Graph::Graph() {
	firstNode = nullptr;
}

Graph::~Graph() {}

Graph::Graph(Node* n) {
	firstNode = n;
	_lastNode = n;
}

void Graph::AddNode(Node* n) {
	if (firstNode == nullptr)
		firstNode = n;
	else
		_lastNode->nextNodes.push_back(n);
	_lastNode = n;

	_size++;
}

void Graph::AddNodeInNewBranch(Node* n, int depth) {
	Node* currentNode = firstNode;

	for (int i = 0; i < depth; i++)
		currentNode = currentNode->nextNodes[currentNode->nextNodes.size() - 1];

	currentNode->nextNodes.push_back(n);

	_lastNode = n;
}

void Graph::AddInstantNode(Node* n) {

	Node* auxFirst = new Node(); // Node(*firstNode);
	auxFirst->SetNodeType(NodeType::numb);
	//auxFirst.nextNodes.clear();
	auxFirst->nextNodes.push_back(n);
	for (Node* nn : firstNode->nextNodes) {
		n->nextNodes.push_back(nn);
	}
	firstNode = auxFirst;


	/*if (firstNode == _lastNode) {
		_lastNode = n;
	}
	else {
		for (Node* nn : firstNode->nextNodes) {
			n->nextNodes.push_back(nn);
		}
		firstNode->nextNodes.clear();
	}
	firstNode->nextNodes.push_back(n);

	_size++;*/
}

void Graph::AddSplitSecondNode(Node* n) {
	n->nextNodes.push_back(firstNode);
	firstNode = n;

	_size++;
}

Node* Graph::Peek() {
	return firstNode;
}

bool Graph::IsLastNode() {
	return firstNode == _lastNode;
}

void Graph::NextNode() {
	firstNode = firstNode->nextNodes[0];
	_size--;
}

int Graph::GetGraphSize()
{
	return _size;
}
