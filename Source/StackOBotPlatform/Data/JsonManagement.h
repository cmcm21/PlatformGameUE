// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Containers/Map.h"
#include "Serialization/JsonSerializer.h"
#include "JsonManagement.generated.h"


class FJsonObject;
struct FScoreDataStruct;

/**
 *
 */
#define LOCTEXT_NAMESPACE "JsonNotifyLoctText"

UCLASS()
class STACKOBOTPLATFORM_API UJsonManagement : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template<typename T>
	static TMap<FString, T> DeserializeJsonToMap(FString filePath, bool& bOutSuccess, FText& OutInfoMessage);

	template<typename T>
	static void SerializeMapToJson(FString filePath, TMap<FString, T> data, bool& bOutSuccess, FText& outInfoMessage);

	template<typename T> 
	static void WriteStructToJsonFile(FString jsonFilePath, T Struct, bool& bOutSuccess, FText& outInfoMessage);

	template<typename T>
	static T ReadFromJsonFileToStruct(FString jsonFilePath, bool& bOutSuccess, FText& outInfoMessage);

	static TSharedPtr<FJsonObject> ReadJson(FString jsonFilePath, bool& bOutSuccess, FText& outInfoMessage);

	static void WriteJson(FString jsonFilePath, TSharedPtr<FJsonObject>, bool& bOutSuccess, FText& outInfoMessage);
};

template<typename T>
void UJsonManagement::WriteStructToJsonFile(FString jsonFilePath, T Struct, bool& bOutSuccess, FText& outInfoMessage)
{
	TSharedPtr<FJsonObject> jsonObject = FJsonObjectConverter::UStructToJsonObject(Struct);
	if (jsonObject == nullptr)
	{
		bOutSuccess = false;
		outInfoMessage = 
			FText::Format(LOCTEXT("JsonFailed", "Convert struct to json object failed, is struct correct?: {0}"), FText::FromString(jsonFilePath));
		return;
	}

	WriteJson(jsonFilePath, jsonObject, bOutSuccess, outInfoMessage);

	if (!bOutSuccess)
		return;

	bOutSuccess = true;
	outInfoMessage = FText(LOCTEXT("JsonSuccess", "Write Struct to json success"));
}

template<typename T>
T UJsonManagement::ReadFromJsonFileToStruct(FString jsonFilePath, bool& bOutSuccess, FText& outInfoMessage)
{
	TSharedPtr<FJsonObject> jsonObject = ReadJson(jsonFilePath, bOutSuccess, outInfoMessage);
	if (!bOutSuccess)
		return T();

	T testStruct;
	if (!FJsonObjectConverter::JsonObjectToUStruct(jsonObject.ToSharedRef(), &testStruct))
	{
		bOutSuccess = false;
		outInfoMessage = FText::Format(LOCTEXT("ReadJsonFailed", "Read from json faield - is the struct right?: {0} "), FText::FromString(jsonFilePath));
		return T();
	}

	bOutSuccess = true;
	outInfoMessage = FText(LOCTEXT("ReadJsonSuccess", "Read json to struct Success"));
	return testStruct;
}

template<typename T>
void UJsonManagement::SerializeMapToJson(FString filePath, TMap<FString, T> data, bool& bOutSuccess, FText& outInfoMessage)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	for (const TPair<FString, T>& Pair : data)
	{
		// Convert the value to a JSON object
		TSharedPtr<FJsonObject> ValueJsonObject = MakeShareable(new FJsonObject);
		if (FJsonObjectConverter::UStructToJsonObject(T::StaticStruct(), &Pair.Value, ValueJsonObject.ToSharedRef(), 0, 0))
		{
			// Convert the JSON object to a JSON value
			TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(ValueJsonObject.ToSharedRef()));

			// Set the JSON value in the main object
			JsonObject->SetField(Pair.Key, JsonValueObject);
		}
		// Handle the case where serialization fails for a specific type
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to serialize value for key %s"), *Pair.Key);
		}
	}

	FString JsonString;

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	UFileManagement::WriteFile(filePath, JsonString, bOutSuccess, outInfoMessage);
}


template<typename T>
TMap<FString, T> UJsonManagement::DeserializeJsonToMap(FString filePath, bool& bOutSuccess, FText& OutInfoMessage)
{
	TMap<FString, T> ResultMap;
	FString JsonString = UFileManagement::ReadFile(filePath, bOutSuccess, OutInfoMessage);

	if (!bOutSuccess)
	{
		return ResultMap;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		for (auto JsonEntry : JsonObject->Values)
		{
			FString Key = JsonEntry.Key;
			TSharedPtr<FJsonValue> JsonValue = JsonEntry.Value;

			if (JsonValue->Type == EJson::Object)
			{
				TSharedPtr<FJsonObject> ValueJsonObject = JsonValue->AsObject();
				T Value;

				if (FJsonObjectConverter::JsonObjectToUStruct(
						ValueJsonObject.ToSharedRef(), T::StaticStruct(), &Value, 0, 0))
				{
					ResultMap.Add(Key, Value);
				}
				else
				{
					OutInfoMessage = FText::Format(
						LOCTEXT("JsonReadKeyFailed", 
							"Failed to deserialize value for key {0}"),FText::FromString(Key));
				}
			}
			else
			{

				OutInfoMessage = FText::Format(
					LOCTEXT("UnexpectedJsonValue", 
						"Unexpected JSON value type for key {0}"),FText::FromString(Key));
			}
		}
	}
	else
	{
		OutInfoMessage = FText::Format(
			LOCTEXT("JsonDeserializeFailed", 
				"Failed to Deserialized Json from file {0}"),FText::FromString(filePath));
	}

	OutInfoMessage = FText(LOCTEXT("JsonDeserializeSuccess", "JsonFile read Successfully"));
	return ResultMap;
}

#undef LOCTEXT_NAMESPACE
