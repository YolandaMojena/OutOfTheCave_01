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

	void AddNode(Node* n);
	void AddEdge(string from, string to, string cond);
	int EdgeExists(string from, string to); //returns index
	void RemoveNode(string id);
	void RemoveEdge(string from, string to);
	void PrintPlot(string start);

	unordered_map<string, Node*> graph;

private:

	void deepSearchPrint(string start);
	int _size;
};
