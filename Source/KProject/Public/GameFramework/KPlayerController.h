// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KPROJECT_API AKPlayerController : public APlayerController
{
	GENERATED_BODY()

	void OnRep_Pawn() override;
	void SetPawn(APawn* InPawn) override;
};
