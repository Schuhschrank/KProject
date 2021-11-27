// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KPROJECT_API ATGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ResetPlayer(AController* PlayerToBeReset);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ResetPlayerReversed(AController* PlayerToBeReset);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ResetPlayerServer(AController* PlayerToBeReset);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ResetPlayerClient(AController* PlayerToBeReset);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ResetGame();

	void ResetLevel() override;
};
