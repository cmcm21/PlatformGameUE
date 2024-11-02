// Fill out your copyright notice in the Description page of Project Settings.


#include "FileManagement.h"
#include "HAL/PlatformFileManager.h"
#include "Internationalization/Text.h"

#define LOCTEXT_NAMESPACE "FileManagementTextLoc"

FString UFileManagement::ReadFile(FString filePath, bool& bOutSuccess, FText& outInfoMessage)
{
	FString returningString = "";
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*filePath))
	{
		bOutSuccess = false;
		outInfoMessage = FText::Format(LOCTEXT("ReadFileFailed", "Read file content failed - Does file exists? : {0}"), FText::FromString(filePath));
		return "";
	}

	if (!FFileHelper::LoadFileToString(returningString, *filePath))
	{
		bOutSuccess = false;
		outInfoMessage =
			FText::Format(LOCTEXT("ReadFileSuccess", "Read file content failed - Was not able to read file: {0}"), FText::FromString(filePath));

		return "";
	}

	bOutSuccess = true;
	outInfoMessage = FText(LOCTEXT("ReadFileCompleted", "File read compleated!!"));
	return returningString;
}

void UFileManagement::WriteFile(FString filePath, FString fileContent, bool& bOutSuccess, FText& outInfoMessage)
{
	if (!FFileHelper::SaveStringToFile(fileContent, *filePath))
	{
		bOutSuccess = false;
		outInfoMessage =
			FText::Format(LOCTEXT("WriteFileFailed", "Write file content failed - Was not able to write file: {0}"), FText::FromString(filePath));
	}

	bOutSuccess = true;
	outInfoMessage = FText(LOCTEXT("WriteFie", "Write file success"));
}

void UFileManagement::FileExist(FString filePath, bool& bOutSuccess, FText& outInfoMessage)
{
	bOutSuccess = FPlatformFileManager::Get().GetPlatformFile().FileExists(*filePath);
	if (bOutSuccess)
		outInfoMessage = FText(LOCTEXT("FileExist", "File exists"));
	else
		outInfoMessage = FText(LOCTEXT("FileDoesntExit", "File Doesn't exist"));
}
#undef LOCTEXT_NAMESPACE
