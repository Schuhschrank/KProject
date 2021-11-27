// Fill out your copyright notice in the Description page of Project Settings.


#include "Testing/TPlayerController.h"

//void ATPlayerController::ClientReset_Implementation()
//{
//	ResetCameraMode();
//	SetViewTarget(this);
//
//	bPlayerIsWaiting = (PlayerState == nullptr) || !PlayerState->IsOnlyASpectator();
//	ChangeState(NAME_Spectating);
//}

void ATPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (!GetPawn() && GetLocalRole() == ROLE_Authority)
	{
		ChangeState(NAME_Spectating);
	}
}

void ATPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (!GetPawn() && GetLocalRole() != ROLE_Authority)
	{
		ChangeState(NAME_Spectating);
	}
}

