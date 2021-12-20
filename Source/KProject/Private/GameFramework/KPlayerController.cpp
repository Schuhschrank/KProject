// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/KPlayerController.h"
#include "GameFramework/PlayerInput.h"

void AKPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (!GetPawn() && GetLocalRole() == ROLE_Authority)
	{
		ChangeState(NAME_Spectating);
	}
}

void AKPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (!GetPawn() && GetLocalRole() != ROLE_Authority)
	{
		ChangeState(NAME_Spectating);
	}
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
