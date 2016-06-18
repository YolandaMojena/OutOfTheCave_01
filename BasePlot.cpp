// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/OEntity.h"
#include "BasePlot.h"

BasePlot::BasePlot(UOEntity* plotEntity) {

	this->_plotEntity = plotEntity;
}

BasePlot::BasePlot() {}

BasePlot::~BasePlot() {}


void BasePlot::PrintSentence() {

	// Print on screen
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, _sentence);
}

void BasePlot::AddInvolvedInPlot(UOEntity * entity)
{
	_involvedInPlot.push_back(entity);
}

void BasePlot::SavePlotToFile(const FString path, const FString fileName)
{
	Utilities::SaveStringToFile(_sentence, Utilities::SavePath, Utilities::PlotFile);
}

void BasePlot::AbortPlot(const FString path, const FString fileName) {

	Utilities::SaveStringToFile(_identifier + " plot aborted.\n\n\n", Utilities::SavePath, Utilities::PlotFile);
}


Graph BasePlot::GetGraph() {
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

bool BasePlot::GetPlotIsValid() {
	return _isPlotValid;
}
