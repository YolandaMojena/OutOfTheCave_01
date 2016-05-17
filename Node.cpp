// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Node.h"

Node::Arc::Arc(string from, string to, string cond) : parent(from), adj(to), condition(cond) {}

Node::Node() {}


Node::~Node() {}


Node::Node(string id, string iconPath)
{
	nodeName = id;
	this->iconPath = iconPath;
}

bool Node::GetIsVisited() {
	return _visited;
}

void Node::SetIsVisited(bool value) {
	_visited = value;
}
