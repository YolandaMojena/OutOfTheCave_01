// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Utilities.h"

FString Utilities::SavePath = FPaths::GameDir() + "SavedFiles/";
FString Utilities::PlotFile = "PlotReport.txt";
FString Utilities::ReportFile = "ReportReport.txt";
FString Utilities::goblinPath = "OOTC_goblinNames.txt";
FString Utilities::femaleHumanPath = "OOTC_femaleNames.txt";
FString  Utilities::maleHumanPath = "OOTC_maleNames.txt";
float Utilities::GAMEDAY_LENGTH = 120.f;	//s/d
float Utilities::DAY_LENGTH = 24.f;		//h/d

Utilities::Utilities()
{
}

Utilities::~Utilities()
{
}

bool Utilities::SaveStringToFile(const FString & String, const FString SaveDirectory, const FString FileName)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		FString AbsoluteFilePath = SaveDirectory + "/" + FileName;
		IFileHandle* handle =  PlatformFile.OpenWrite(*AbsoluteFilePath, true, true);
		if (handle) {
			if(handle->Write((const uint8*)TCHAR_TO_ANSI(*String), String.Len()))
			handle->~IFileHandle();
			//PlatformFile.~IPlatformFile();
		}
		return true;
	}
	else GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, "Failed to write file");
	return false;
}

TArray<FString> Utilities::ReadFileToVector(const FString SaveDirectory, const FString FileName)
{
	TArray<FString> result = TArray<FString>();
	FString test = "";

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		FString AbsoluteFilePath = SaveDirectory + "/" + FileName;

		if (PlatformFile.FileExists(*AbsoluteFilePath)) {

			FFileHelper::LoadFileToString(test, *AbsoluteFilePath);
			test.ParseIntoArray(result, _T("\n"), true);
		}
		else GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, "Failed to load vector from file");
	}
	return result;
}

bool Utilities::VerifyOrCreateDirectory(const FString & TestDir)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Directory Exists?
	if (!PlatformFile.DirectoryExists(*TestDir))
	{
		PlatformFile.CreateDirectory(*TestDir);
		if (PlatformFile.DirectoryExists(*TestDir))
			return true;
		else return false;
	}
	return true;
}


