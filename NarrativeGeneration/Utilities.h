// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runtime/Core/Public/Misc/CoreMisc.h"

/**
 * 
 */
class OUTOFTHECAVE_01_API Utilities
{
public:
	Utilities();
	~Utilities();

	// Write the FString to a file.
	static bool SaveStringToFile(const FString& String, const FString SaveDirectory, const FString Filename);

	// Read file as array of FStrings
	static TArray<FString> ReadFileToVector(const FString SaveDirectory, const FString Filename);

	// Verify directory
	static bool VerifyOrCreateDirectory(const FString& TestDir);

	


};
