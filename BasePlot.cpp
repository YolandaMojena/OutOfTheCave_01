// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/OEntity.h"
#include "BasePlot.h"

BasePlot::BasePlot(UOEntity* plotEntity) {

	this->_plotEntity = plotEntity;
}

BasePlot::BasePlot() {}

BasePlot::~BasePlot() {}


void BasePlot::ExecutePlot() {
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

UOEntity* BasePlot::GetMainEntity() {
	return _plotEntity;
}

vector<UOEntity*> BasePlot::GetInvolvedInPlot() {
	return _involvedInPlot;
}

bool BasePlot::GetIsExclusive() {
	return _isExclusive;
}

