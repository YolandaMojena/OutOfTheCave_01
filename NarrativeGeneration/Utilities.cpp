// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Utilities.h"

Utilities::Utilities()
{
}

Utilities::~Utilities()
{
}

bool Utilities::SaveStringToFile(const FString & String, const FString SaveDirectory, const FString FileName, bool AllowOverwriting)
{

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	// CreateDirectoryTree returns true if the destination
	// directory existed prior to call or has been created
	// during the call.
	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		// Get absolute file path
		FString AbsoluteFilePath = SaveDirectory + "/" + FileName;

		// Allow overwriting or file doesn't already exist
		if (AllowOverwriting || !PlatformFile.FileExists(*AbsoluteFilePath))
		{
			FFileHelper::SaveStringToFile(String, *AbsoluteFilePath);
			return true;
		}
	}
	return false;
}
