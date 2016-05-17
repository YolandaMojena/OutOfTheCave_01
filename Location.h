// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Locations will be associated with specific places or areas
//_location will always be the center

class OUTOFTHECAVE_01_API Location
{
public:
	Location(FVector location);
	~Location();

	FVector GetLocation();

private:
	FVector _location;
};
