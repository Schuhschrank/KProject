// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KPROJECT_API ATPlayerController : public APlayerController
{
	GENERATED_BODY()

	// void ClientReset_Implementation() override;

	void SetPawn(APawn* InPawn) override;

	void OnRep_Pawn() override;
};
