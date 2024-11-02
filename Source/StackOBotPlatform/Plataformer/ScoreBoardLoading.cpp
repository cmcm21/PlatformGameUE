// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreBoardLoading.h"
#include "Components/ListView.h"
#include "StackOBotPlatform/GameModeData.h"
#include "ScoreListEntry.h"

void UScoreBoardLoading::LoadScoreBoard(TMap<FString, FScoreDataStruct> data, FString &feedbackMessage)
{
	if (listEntryTemplate == nullptr) 
	{
		feedbackMessage = FString::Printf(TEXT("Load score board entries faield - listEntry not assigned"));
		return;
	}	

	TArray<FString> dataKeys;
	data.GetKeys(dataKeys);

	if (dataKeys.Num() == 0)
	{
		feedbackMessage = FString::Printf(TEXT("Load score board entries failed - data is empty"));
		return;
	}
	int32 listNumber = 1;
	for (FString key : dataKeys)
	{
		UScoreListEntry* listItem = Cast<UScoreListEntry>(CreateWidget(GetWorld(), listEntryTemplate));
		if (listItem)
		{
			scoresList->AddItem(listItem);
			auto gameplayStruct = data[key];
			listItem->SetData(listNumber,gameplayStruct);
			listNumber++;
		}
		else
			feedbackMessage = FString::Printf(TEXT("Creation of list entry failed"));
	}
}
