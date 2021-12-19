// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/KEnums.h"
#include "KGameMode.generated.h"

USTRUCT(BlueprintType)
struct FRoleCounts
{
	GENERATED_BODY()

public:

	FRoleCounts() : NotParticipating(0), Traitors(0), Innocents(0), Detectives(0) {}

	UPROPERTY(BlueprintReadOnly)
		int32 NotParticipating;

	UPROPERTY(BlueprintReadOnly)
		int32 Traitors;

	UPROPERTY(BlueprintReadOnly)
		int32 Innocents;

	UPROPERTY(BlueprintReadOnly)
		int32 Detectives;
};

DECLARE_LOG_CATEGORY_EXTERN(LogKGameMode, Log, All);

/**
 * 
 */
UCLASS(Abstract, CustomConstructor)
class KPROJECT_API AKGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	AKGameMode(/* const FObjectInitializer& ObjectInitializer */);


protected:

	/** 
	 * Returns a list of currently existing controllers.
	 * Check validity for each item of the array before dereferencing.
	 */
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
		TArray<AController*> GetAllPlayers() const;


	/////////////////////////////////////////////////////////////////////////
	// Player roles

protected:

	/* E.g., how many innocents are in the game? (Looks at every player each call.) */
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
		virtual int32 GetRoleCount(EPlayerRole InRole) const;

	/* E.g., how many living innocents are in the game? (Looks at every player each call.) */
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
		virtual int32 GetAliveRoleCount(EPlayerRole InRole) const;

	/* Returns the counts for all player roles. If bAlive is true only living players will be counted. */
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
		virtual FRoleCounts GetRoleCounts(bool bAlive) const;

	UFUNCTION(BlueprintCallable, Category = "KGameMode")
		virtual void SetPlayerRole(AController* Player, EPlayerRole NewRole);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "KGameMode")
		virtual EPlayerRole GetPlayerRole(AController* Player) const;

	/* Assign a role to each player in the game. */
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
		virtual void DistributeRoles(int32 NumTraitors = 1, int32 NumDetectives = 0);

	/////////////////////////////////////////////////////////////////////////
	// Player participation

	/* Calls RestartPlayer() on all players. */
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
	    virtual void RestartAllPlayers();

	UFUNCTION(BlueprintCallable, Category = "KGameMode")
		virtual void MakePlayerSpectate(AController* NewSpectator);

	/////////////////////////////////////////////////////////////////////////
	// Player deaths

private:

	/* If false players will respawn after death or be invulnerable. */
	bool bArePlayersMortal;

protected:

	/* If false players will respawn after death or be invulnerable. */
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
	    virtual void SetArePlayersMortal(bool bAreMortal) { bArePlayersMortal = bAreMortal; }
	
	/* If false players will respawn after death or be invulnerable. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "KGameMode")
	    virtual bool ArePlayersMortal() const { return bArePlayersMortal; }

public:

	/**
	 * Do not call this to kill the player. Health and determination of death is handled elsewhere.
	 * Call this after player is known to be dead.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "KGameMode")
	    void HandlePlayerDeath(AController* DeadPlayer);

	/////////////////////////////////////////////////////////////////////////
	// Match end

protected:
	
	/* Call this before EndMatch(). Called by ShouldEndMatch(). */
	//UFUNCTION(BlueprintCallable, Category = "KGameMode")
	//    virtual void SetEndMatchInfo(const FText& InEndMatchReason, bool bInHaveTraitorsWon);

	/* If true it will call SetEndMatchInfo() accordingly. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "KGameMode")
	    bool ShouldEndMatch();

public:

	/* Call this function instead of EndMatch() unless SetEndMatchInfo() was called manually. */
	//UFUNCTION(BlueprintCallable, Category = "KGameMode")
	//    virtual void NewEndMatch(const FText& InEndMatchReason, bool bInHaveTraitorsWon);

    //////////////////////////////////////////////////////////////////////
	// Inherited stuff

public:

	/* GameMode interface */
	virtual void RestartGame() override;

protected:

	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual void HandleMatchAborted() override;
	/* End GameMode interface */

	/* GameModeBase interface */ /* End GameModeBase interface */

public:

	/* Actor interface */
	virtual void Tick(float DeltaSeconds) override;
	virtual void Reset() override;
	/* End Actor interface */

};
