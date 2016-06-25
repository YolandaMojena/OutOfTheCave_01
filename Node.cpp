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


void Node::RewriteNode(vector<Node*> nodes) {
	vector<Node*> childs = nextNodes;

	nextNodes = nodes;

	for (auto n : nextNodes) {
		n->nextNodes = childs;
	}
}


void Node::SetNodeType(NodeType n) {
	_nodeType = n;
}
NodeType Node::GetNodeType() {
	return _nodeType;
}

/*void Node::PopulateBlackboard(UOEntity* entityA, UOEntity* entityB, UOOwnable* ownable, UOEdification* edification, UOGrabbable* grabbable) {

	nBlackboard.entityA = entityA;
	nBlackboard.entityB = entityB;
	nBlackboard.ownable = ownable;
	nBlackboard.edification = edification;
	nBlackboard.grabbable = grabbable;
}

void Node::PopulateBlackboard(FVector position, UOEntity* entityA, UOEntity* entityB, UOOwnable* ownable, UOEdification* edification, UOGrabbable* grabbable) {

	nBlackboard.entityA = entityA;
	nBlackboard.entityB = entityB;
	nBlackboard.ownable = ownable;
	nBlackboard.edification = edification;
	nBlackboard.grabbable = grabbable;
	nBlackboard.position = position;
}*/

void Node::SetEntity(UOEntity* e) {
	nBlackboard.entity = e;
}
void Node::SetActor(AActor* a) {
	nBlackboard.actor = a;
}
void Node::SetOwnable(UOOwnable* o) {
	nBlackboard.ownable = o;
}
void Node::SetAffordableUse(OntologicFunctions::AffordableUse au) {
	nBlackboard.affordableUse = au;
}
void Node::SetEdification(UOEdification* edf) {
	nBlackboard.edification = edf;
}
void Node::SetPosition(FVector v) {
	nBlackboard.position = v;
}
void Node::SetDaytime(float d) {
	nBlackboard.daytime = d;
}
void Node::SetAsBranch() {
	nBlackboard.branch = true;
}
void Node::SetHighPriority(bool isHP) {
	nBlackboard.isHighPriority = true;
}
