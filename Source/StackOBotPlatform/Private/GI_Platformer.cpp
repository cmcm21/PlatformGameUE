// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Platformer.h"

void UGI_Platformer::SetGameoverState(const GameoverState& newState)
{
	gameoverState = newState;
}

GameoverState& UGI_Platformer::GetGameoverState()
{
	// TODO: insert return statement here
	return gameoverState;
}
