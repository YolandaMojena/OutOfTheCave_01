// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <string>

using namespace std;

class OUTOFTHECAVE_01_API VenturLocation
{
public:

	VenturLocation(string locationName, FVector worldPoint);
	~VenturLocation();
	
	FVector worldPoint;
	string locationName;
};
