// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Graph.h"

using namespace std;

Graph::Graph() {}

Graph::~Graph() {}

void Graph::AddNode(Node* n)
{
	if (graph.find(n->nodeName) == graph.end()) {
		graph.insert({ n->nodeName, n});
		_size++;
		cout << "\nNode added";
	}
	else cout << "\nNode already exists";
}


void Graph::AddEdge(string from, string to, string cond)
{
	if (EdgeExists(from, to) < 0) {
		graph[from]->neighbors.push_back(Node::Arc(from, to, cond));
		cout << "\nEdge added";
	}
	else {
		cout << "\nEdge already exists";
	}
}


int Graph::EdgeExists(string from, string to)
{
	for (int i = 0; i < graph[from]->neighbors.size(); i++) {
		if (graph[from]->neighbors[i].adj == to)
			return i;
	}
	return -1;
}

void Graph::RemoveNode(string id)
{
	if (graph.find(id) == graph.end()) {
		graph.erase(id);
		_size--;
		cout << "\nNode deleted";
	}
}

void Graph::RemoveEdge(string from, string to)
{
	int index = EdgeExists(from, to);
	if (index >= 0) {
		graph[from]->neighbors.erase(graph[from]->neighbors.begin() + index);
	}
}

void Graph::PrintPlot(string start)
{
	graph[start]->SetIsVisited(true);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("I AM A PLOT!"));
	deepSearchPrint(start);
}

void Graph::deepSearchPrint(string start) {

	cout << "\nNode: " + graph[start]->nodeName;
	cout << "\n";

	for (int i = 0; i < graph[start]->neighbors.size(); i++) {

		cout << "Carries to: ";
		cout << graph[start]->neighbors[i].adj;
		cout << "\n";
	}

	cout << "\n";

	for (int i = 0; i < graph[start]->neighbors.size(); i++) {

		if (!graph[graph[start]->neighbors[i].adj]->GetIsVisited()) {
			graph[graph[start]->neighbors[i].adj]->SetIsVisited(true);
			deepSearchPrint(graph[start]->neighbors[i].adj);
		}
	}
}
