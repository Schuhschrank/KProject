// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/KPlayerController.h"
#include "GameFramework/PlayerInput.h"

void AKPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnSetPawn();
}

void AKPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	OnSetPawn();
}

void AKPlayerController::OnSetPawn()
{
	if (!GetPawn())
	{
		ChangeState(NAME_Spectating);
	}
	SetNewPawn.Broadcast();
}

void AKPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnRepPlayerState.Broadcast();
}

void AKPlayerController::SetMouseSensitivity(const float NewSensitivity)
{
	if (PlayerInput)
	{
		PlayerInput->SetMouseSensitivity(NewSensitivity);
	}
}
