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

void Node::SetItem(UItem* i) {
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Yellow, TEXT("Pon un print aquí"));
	nBlackboard.item = i;
}
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
	nBlackboard.position = v * FVector(1, 1, 0);
}
void Node::SetAnotherEntity(UOEntity* ae) {
	nBlackboard.anotherEntity = ae;
}
void Node::SetDaytime(float d) {
	nBlackboard.daytime = d;
}
void Node::SetBoolKey(bool b) {
	nBlackboard.boolKey = b;
}
void Node::SetFloatKey(float f) {
	nBlackboard.floatKey = f;
}
void Node::SetAsBranch() {
	nBlackboard.branch = true;
}
void Node::SetHighPriority(bool isHP) {
	nBlackboard.isHighPriority = true;
}
void Node::SetHighPriority() {
	nBlackboard.isHighPriority = true;
}