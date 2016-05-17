// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "VenturLocation.h"

VenturLocation::VenturLocation(string locationName, FVector worldPoint)
{
	this->locationName = locationName;
	this->worldPoint = worldPoint;
}


VenturLocation::~VenturLocation()
{
}
