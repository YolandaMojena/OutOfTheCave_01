// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class OUTOFTHECAVE_01_API OTerritory
{
public:
	OTerritory(FVector position, float radius);
	OTerritory();
	~OTerritory();

	FVector GetPosition();
	float GetRadius();
	void SetPosition(FVector value);
	void SetRadius(float value);

private:
	FVector _position;
	float _radius;
};
