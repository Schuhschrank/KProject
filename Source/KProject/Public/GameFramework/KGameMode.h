// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "KGameMode.generated.h"

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	NotParticipating,
	Traitor,
	Innocent,
	Detective,
};

/**
 * 
 */
UCLASS()
class KPROJECT_API AKGameMode : public AGameMode
{
	GENERATED_BODY()

	AKGameMode(const FObjectInitializer& ObjectInitializer);

protected:

	UFUNCTION(BlueprintCallable, Category = "KGame")
	virtual void RestartAllPlayers();

	UFUNCTION(BlueprintCallable, Category = "KGame")
	virtual void DistributeRoles(int32 NumTraitors = 1, int32 NumDetectives = 0);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "KGame")
	TArray<int32> RoleCounts;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "KGame")
	TArray<int32> AliveRoleCounts;

	UFUNCTION(BlueprintCallable, Category = "KGame")
	void SetPlayerRole(AController* Player, EPlayerRole NewRole);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "KGame")
	EPlayerRole GetPlayerRole(AController* Player);

private:

	bool bArePlayersMortal;

protected:

	UFUNCTION(BlueprintCallable, Category = "KGame")
	virtual void SetMortality(bool bAreMortal) { bArePlayersMortal = bAreMortal; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "KGame")
	virtual bool GetMortality() { return bArePlayersMortal; }

public:

	UFUNCTION(BlueprintCallable, Category = "KGame")
	virtual void HandlePlayerDeath(AController* DeadPlayer);

protected:

	UFUNCTION(BlueprintCallable, Category = "KGame")
	void MakePlayerSpectate(AController* NewSpectator);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "KGame")
	FText EndMatchReason;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "KGame")
	bool bHaveTraitorsWon;

	/* If true it will set EndMatchReason and bHaveTraitorsWon accordingly. */
	UFUNCTION(BlueprintCallable, Category = "KGame")
	bool ShouldEndMatch();

public:

	UFUNCTION(BlueprintCallable, Category = "KGame")
	virtual void MyEndMatch(const FText& InEndMatchReason, bool bInHaveTraitorsWon);

    //////////////////////////////////////////////////////////////////////
	// Inherited stuff

public:

	/* GameMode interface */
	virtual void AbortMatch() override;
	virtual void RestartGame() override;

protected:

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	/* End GameMode interface */

	/* GameModeBase interface */ /* End GameModeBase interface */

public:

	/* Actor interface */
	virtual void Tick(float DeltaSeconds) override;
	virtual void Reset() override;
	/* End Actor interface */

};
