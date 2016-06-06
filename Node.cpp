// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Node.h"
#include "Graph.h"
#include "Ontology/OEntity.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OEdification.h"
#include "Ontology/OGrabbable.h"

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

void Node::PopulateBlackboard(UOEntity* entityA, UOEntity* entityB, UOOwnable* ownable, UOEdification* edification, UOGrabbable* grabbable, FVector* position) {

	nBlackboard.entityA = entityA;
	nBlackboard.entityB = entityB;
	nBlackboard.ownable = ownable;
	nBlackboard.edification = edification;
	nBlackboard.grabbable = grabbable;
	nBlackboard.position = position;
}

void Node::SetIconPath(string path) {
	_iconPath = path;
}