// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSomethingHappened);

/**
 * 
 */
UCLASS()
class KPROJECT_API AKPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/* Triggered by OnRep_PlayerState(). Only triggered on clients. */
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="KPlayerController")
		FSomethingHappened OnRepPlayerState;

	UFUNCTION(BlueprintCallable, Category = "KPlayerController")
		virtual void SetMouseSensitivity(const float NewSensitivity);

	//////////////////////////////////////////////////////////////////////
	// Inherited stuff

public:

	/* Controller interface */
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Pawn() override;
	virtual void SetPawn(APawn* InPawn) override;
	/* End Controller interface */
};
