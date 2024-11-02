// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileManagement.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOTPLATFORM_API UFileManagement : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	* Open file and read its content *
	* @param filePath
	* @param boutSucess			if the action was success or not
	* @param outInfoMessage		More information about the action's result

	*	@return the string content of your file
	*/
	UFUNCTION(BlueprintCallable, Category = "File Management")
	static FString ReadFile(FString filePath, bool& bOutSuccess, FText& outInfoMessage);

	/**
	* Open a file and write content
	* @param filePath
	* @param bOutSuccess		if the action was success or not
	* @param outInfoMessage		More information about the action's result
	**/
	UFUNCTION(BlueprintCallable, Category = "File Management")
	static void WriteFile(FString filePath, FString fileContent, bool& bOutSuccess, FText& outInfoMessage);
	
	UFUNCTION(BlueprintCallable, Category="File Management")
	static void FileExist(FString filePath, bool& bOutSuccess, FText& outInfoMessage);
};
