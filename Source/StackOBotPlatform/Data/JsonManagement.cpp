// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonManagement.h"
#include "FileManagement.h"
#include "Serialization/JsonSerializer.h" //Json
#include "JsonObjectConverter.h" // JsonUtilities
#include "StackOBotPlatform/GameModeData.h"

#define LOCTEXT_NAMESPACE "ReadWriteJsonLocText"

TSharedPtr<FJsonObject> UJsonManagement::ReadJson(FString jsonFilePath, bool& bOutSuccess, FText& outInfoMessage)
{
	FString jsonString = UFileManagement::ReadFile(jsonFilePath, bOutSuccess, outInfoMessage);
	if (!bOutSuccess)
		return nullptr;

	TSharedPtr<FJsonObject> jsonObject;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(jsonString), jsonObject))
	{
		bOutSuccess = false;
		outInfoMessage = 
			FText::Format(LOCTEXT("JsonGenericReadFailed", "Read json failed - Was not able to read json string: {0}"), FText::FromString(jsonString));
		return nullptr;
	}

	bOutSuccess = true;
	outInfoMessage = FText(LOCTEXT("JsonGenericReadSuccess", "Read Json success"));
	return jsonObject;
}

void UJsonManagement::WriteJson(FString jsonFilePath, TSharedPtr<FJsonObject> jsonObject, bool& bOutSuccess, FText& outInfoMessage)
{
	FString jsonString;
	if (!FJsonSerializer::Serialize(jsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&jsonString, 0)))
	{
		bOutSuccess = false;
		outInfoMessage = FText(LOCTEXT("JsonWriteFailed", "Write Json failed - serialize json object failed"));
		return;
	}

	UFileManagement::WriteFile(jsonFilePath, jsonString, bOutSuccess, outInfoMessage);
	if (!bOutSuccess)
		return;

	bOutSuccess = true;
	outInfoMessage = FText(LOCTEXT("JsonWriteSucess", "Write Json Success"));
}

#undef LOCTEXT_NAMESPACE
