// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/OEntity.h"
#include "OCivilian.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OUTOFTHECAVE_01_API UOCivilian : public UOEntity
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
		FString currentIconPath;

	// Allows to load a texture from a given path
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LoadTextureFromPath", Keywords = "Load texture from path"), Category = Game)
	static UTexture2D* LoadTextureFromPath(const FString& Path);

private:

	const FString _DEFAULT_PATH = "Texture2D'/Game/Icons/";

};
