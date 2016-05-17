// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Location.h"

Location::Location(FVector location)
{
	_location = location;
}

Location::~Location()
{
}

FVector Location::GetLocation()
{
	return _location;
}
