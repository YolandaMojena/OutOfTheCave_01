// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/OEntity.h"
#include "BasePlot.h"

BasePlot::BasePlot(UOEntity* plotEntity) {

	this->plotEntity = plotEntity;
}

BasePlot::BasePlot() {}

BasePlot::~BasePlot() {}

void BasePlot::BuildGraph() {

}

void BasePlot::ExecutePlot() {

	PrintSentence();

	/*_currentNode->SetTarget(this);
	_currentNode->ExecuteActions(this, deltaTime);

	if (_currentNode->NodeCompleted(this))
	{
		if (_currentNode->neighbors.size() > 0) {

			for (int i = 0; i < _currentNode->neighbors.size(); i++) {

				if (_currentNode->neighbors[i].condition == "") { // MUST CHECK ARC CONDITION
					_currentNode = _plotGraph.graph[_currentNode->neighbors[i].adj];
					break;
				}
			}
		}

		else {
			_plotCompleted = true;
		}
	}*/
}

void BasePlot::PrintSentence() {

	// Print on screen
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, _sentence.c_str());
}

void BasePlot::AddInvolvedInPlot(UOEntity * entity)
{
	_involvedInPlot.push_back(entity);
}

Graph* BasePlot::GetGraph() {
	return _plotGraph;
}

