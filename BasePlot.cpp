// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BasePlot.h"

BasePlot::BasePlot() {}

BasePlot::~BasePlot() {}

void BasePlot::BuildGraph(vector<Node*> nodes, vector<Node::Arc> arcs) {

	for (int i = 0; i < nodes.size(); i++) {
		plotGraph.AddNode(nodes[i]);
	}

	for (int i = 0; i < arcs.size(); i++) {
		plotGraph.AddEdge(arcs[i].parent, arcs[i].adj, arcs[i].condition);
	}
}

void BasePlot::ExecutePlot(float deltaTime) {

	currentNode->SetTarget(this);
	currentNode->ExecuteActions(this, deltaTime);

	if (currentNode->NodeCompleted(this))
	{
		if (currentNode->neighbors.size() > 0) {

			for (int i = 0; i < currentNode->neighbors.size(); i++) {

				if (currentNode->neighbors[i].condition == "") { // MUST CHECK ARC CONDITION
					currentNode = plotGraph.graph[currentNode->neighbors[i].adj];
					break;
				}
			}
		}

		else {
			plotCompleted = true;
		}
	}
}
