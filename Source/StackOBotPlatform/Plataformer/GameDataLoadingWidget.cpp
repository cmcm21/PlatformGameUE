// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDataLoadingWidget.h"
#include "Components/ListView.h"
#include "StackOBotPlatform/GameModeData.h"
#include "GameDataLoadingEntry.h"

#define LOCTEXT_NAMESPACE "LoadGameDataLocText"

void UGameDataLoadingWidget::LoadGameData(TMap<FString, FSaveSlotMetaData> data, FText& feedbackMessage)
{
	if (listEntryTemplate == nullptr)
	{
		feedbackMessage = FText(LOCTEXT("LoadGameDataFailed", "Load Game Data entry failed - listEntry not assigned"));
		return;
	}

	TArray<FString> dataKeys;
	data.GetKeys(dataKeys);

	if (data.Num() == 0)
	{
		feedbackMessage = FText(LOCTEXT("NoGameDataSaved", "No game data saved yet"));
		return;
	}

	for (FString key : dataKeys)
	{
		UGameDataLoadingEntry *entry = Cast<UGameDataLoadingEntry>(CreateWidget(GetWorld(), listEntryTemplate));
		if (entry)
		{
			loadList->AddItem(entry);
			FSaveSlotMetaData slotData = data[key];
			entry->SetData(slotData);
		}
		else
			feedbackMessage = FText(LOCTEXT("CreationGameFailed", "Creation of game load entry failed"));
	}
}

#undef LOCTEXT_NAMESPACE
