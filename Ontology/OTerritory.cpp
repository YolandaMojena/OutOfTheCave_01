// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OTerritory.h"

OTerritory::OTerritory()
{
}

OTerritory::~OTerritory()
{
}

OTerritory::OTerritory(FVector position, float radius) {
	_position = position;
	_radius = radius;
}

FVector OTerritory::GetPosition() {
	return _position;
}
float OTerritory::GetRadius() {
	return _radius;
}

void OTerritory::SetPosition(FVector value) {
	_position = value;
}
void OTerritory::SetRadius(float value) {
	_radius = value;
}