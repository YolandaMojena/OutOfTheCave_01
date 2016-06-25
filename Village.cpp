// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Village.h"

AVillage::AVillage()
{}

AVillage::~AVillage()
{}

void AVillage::BeginPlay()
{
	Super::BeginPlay();

	_goblinNames = Utilities::ReadFileToVector(Utilities::SavePath, Utilities::goblinPath);
	_femaleHumanNames = Utilities::ReadFileToVector(Utilities::SavePath, Utilities::femaleHumanPath);
	_maleHumanNames = Utilities::ReadFileToVector(Utilities::SavePath, Utilities::maleHumanPath);
}

FString AVillage::AssignFemaleHumanName() {

	FString name;
	if (_femaleHumanNames.Num() > 0) {
		name = _femaleHumanNames[rand() % _femaleHumanNames.Num()];
		_femaleHumanNames.Remove(name);
	}
	return name;
}
FString AVillage::AssignMaleHumanName() {

	FString name;
	if (_maleHumanNames.Num() > 0) {
		name = _maleHumanNames[rand() % _maleHumanNames.Num()];
		_maleHumanNames.Remove(name);
	}
	return name;

}
FString AVillage::AssignGoblinName() {

	FString name;
	if (_goblinNames.Num() > 0) {
		name = _goblinNames[rand() % _goblinNames.Num()];
		_goblinNames.Remove(name);
	}
	return name;

}