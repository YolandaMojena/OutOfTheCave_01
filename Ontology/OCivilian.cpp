// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OCivilian.h"

UTexture2D* UOCivilian::LoadTextureFromPath(const FString & Path)
{
	if (Path.IsEmpty()) return NULL;
	return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(Path)));
}


