// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KGameMode.h"
#include "KPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class KPROJECT_API AKPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Replicated)
	EPlayerRole PlayerRole;

	UPROPERTY(BlueprintReadOnly)
	bool bIsGhost;
	
	void Reset() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
