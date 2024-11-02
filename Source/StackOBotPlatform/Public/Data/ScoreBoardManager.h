// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScoreBoardManager.generated.h"

/**
 * 
 */
struct FScoreDataStruct;

UCLASS()
class STACKOBOTPLATFORM_API UScoreBoardManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/// <summary>
	///	Sort score board by time, where the lower time has the major priority 
	/// </summary>
	/// <param name="data"> all score board data</param>
	/// <returns> score board data sorted by time from lower time to higher time</returns>
	UFUNCTION(BlueprintCallable,Category="Score Management")
	static TMap<FString, FScoreDataStruct> SortByTime(TMap<FString, FScoreDataStruct> data);
	
	/// <summary>
	///	Verify that the new score has a better time than the scores sotored in data 
	/// </summary>
	/// <param name="data">all score board data</param>
	/// <param name="newScore"></param>
	/// <param name="bOutSuccess">out value, if the new score has better time true else false</param>
	/// <param name="feedback">out value</param>
	UFUNCTION(BlueprintCallable,Category="Score Management")
	static void IsInRange(TMap<FString, FScoreDataStruct> data, FScoreDataStruct newScore, bool& bOutSuccess, FText& feedback);
	/// <summary>
	///	Verify if the new score has better time with IsInTopTweenty 
	/// </summary>
	/// <param name="data"> all score board data </param>
	/// <param name="newScore"></param>
	/// <param name="bOutNewScore">out value,if new score saved true else false</param>
	/// <param name="feedback">out value</param>
	UFUNCTION(BlueprintCallable,Category="Score Management")
	static void SaveNewScore(TMap<FString, FScoreDataStruct> data, FScoreDataStruct newScore, bool& bOutNewScore, FText& feedback);

	/// <summary>
	/// Remove the lowest score and return the score board data without that element
	/// </summary>
	/// <param name="data"></param>
	/// <param name="bOutSuccess">out value, If the lowest score was removed without problems</param>
	/// <param name="feedback"> out value</param>
	/// <returns>Score board without the lowest score</returns>
	UFUNCTION(BlueprintCallable, Category="Score Management")
	static TMap<FString, FScoreDataStruct> RemoveLowestScore(TMap<FString, FScoreDataStruct> data, bool& bOutSuccess, FText& feedback);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="data"></param>
	/// <param name="bOutSuccess">out value, pass ref</param>
	/// <param name="feedback"> out value, pass ref</param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, Category="Score Management")
	static float GetLastScoreTime(TMap<FString, FScoreDataStruct> data, bool &bOutSuccess, FText& feedback);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="data"></param>
	/// <param name="bOutSuccess">out value</param>
	/// <param name="feedback">out value</param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, Category="Score Management")
	static FString GetLastScoreKey(TMap <FString, FScoreDataStruct> data, bool &bOutSuccess, FText& feedback);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="scoreBoard"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, Category="Score Management")
	static TMap<FString, FScoreDataStruct> TruncateScoreBoard(TMap<FString, FScoreDataStruct> scoreBoard);
	
private:
	static bool IsScoreDuplicated(TMap<FString, FScoreDataStruct> scoreBoard, FScoreDataStruct newScore);

};
