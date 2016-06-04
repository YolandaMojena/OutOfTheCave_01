// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Node.h"
#include "Graph.h"

Node::Node()  {}


Node::~Node() {}

void Node::SetGraph(Graph* g) {
	_graph = g;
}

void Node::RewriteNode(vector<Node*> nodes) {
	vector<Node*> childs = nextNodes;

	nextNodes = nodes;

	for (auto n : nextNodes) {
		n->nextNodes = childs;
	}
}


void Node::ExecuteTask() {

}
void Node::TaskCompleted(bool completedOk) {
	_graph->TaskCompleted(completedOk);
}