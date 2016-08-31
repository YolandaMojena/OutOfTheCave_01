// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <algorithm> 
#include <unordered_map>
#include "Node.h"

using namespace std;

class OUTOFTHECAVE_01_API Graph
{
public:

	Graph();
	~Graph();
	Graph(Node* n);

	void AddNode(Node* n);
	void AddNodeInNewBranch(Node* n, int depth);
	void AddInstantNode(Node* n);

	Node* firstNode;

	Node* Peek();
	bool IsLastNode();
	void SetNodeCompleted(bool completedOK);
	bool GetNodeCompleted();
	void NextNode();
	int GetGraphSize();

private:
	Node* _lastNode;
	int _size = 0;
};
