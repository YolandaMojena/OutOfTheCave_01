// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/OEntity.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "BasePlot.h"

BasePlot::BasePlot(UOEntity* plotEntity) {

	_plotEntity = plotEntity;
}

BasePlot::BasePlot() {}

BasePlot::~BasePlot() {}


void BasePlot::PrintSentence(APlotGenerator* plotGenerator, FString motivationName, TypeOfAmbition ambition) {

	// Print on screen
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, _sentence);
	plotGenerator->SetCurrentPlotString(FString(*(_sentence)));

	if(motivationName == "" && ambition == TypeOfAmbition::noAmbition)
		plotGenerator->SetCurrentPlotTypeColor(FLinearColor::Red);

	else if (motivationName != "" && ambition == TypeOfAmbition::noAmbition) {
		plotGenerator->SetCurrentPlotTypeColor(FLinearColor::Black);
	}
	else if(ambition!=TypeOfAmbition::noAmbition)
		plotGenerator->SetCurrentPlotTypeColor(FLinearColor::White);

}

void BasePlot::AddInvolvedInPlot(UOEntity * entity)
{
	_involvedInPlot.Add(entity);
	//entity->SetMainPlotEntity(_plotEntity);
}

void BasePlot::DeleteFromInvolved(UOEntity* entity) {

	if (_involvedInPlot.Contains(entity)) {
		_involvedInPlot.Remove(entity);
	}
}

void BasePlot::SavePlotToFile(const FString path, const FString fileName)
{
	Utilities::SaveStringToFile(_sentence, Utilities::SavePath, Utilities::PlotFile);
}

void BasePlot::AbortPlot(const FString path, const FString fileName) {

	Utilities::SaveStringToFile(_identifier + " plot aborted.\n\n", Utilities::SavePath, Utilities::PlotFile);
}


Graph BasePlot::GetGraph() {
	return _plotGraph;
}
Graph* BasePlot::GetGraphPointer() {
	return &_plotGraph;
}

UOEntity* BasePlot::GetMainEntity() {
	return _plotEntity;
}

TArray<UOEntity*> BasePlot::GetInvolvedInPlot() {
	return _involvedInPlot;
}

bool BasePlot::GetIsExclusive() {
	return _isExclusive;
}

TypeOfAmbition BasePlot::GetPlotTypeOfAmbition() {
	return _ambition;
}

FString BasePlot::GetSentence() {
	return FString(*(_sentence));
}

FString BasePlot::GetMotivationRace() {
	return _motivationRace;
}

FString BasePlot::GetMotivationName() {
	return _motivationName;
}

TypeOfAmbition BasePlot::GetAmbition() {
	return _ambition;
}

void BasePlot::ChangeMainEntity(UOEntity* entity) {
	_plotEntity = entity;
}