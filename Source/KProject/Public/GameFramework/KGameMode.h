// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerRole.h"
#include "KGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogKGameMode, Log, All);

/**
 * 
 */
UCLASS()
class KPROJECT_API AKGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	AKGameMode(const FObjectInitializer& ObjectInitializer);

	/////////////////////////////////////////////////////////////////////////
	// Player roles

protected:

	/* E.g., how many innocents are in the game? (Looks at every player each call.) */
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
		virtual int32 GetRoleCount(EPlayerRole InRole) const;

	/* E.g., how many living innocents are in the game? (Looks at every player each call.) */
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
		virtual int32 GetAliveRoleCount(EPlayerRole InRole) const;

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
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
	    virtual void HandlePlayerDeath(AController* DeadPlayer);

	/////////////////////////////////////////////////////////////////////////
	// Match State

protected:

	UFUNCTION(BlueprintImplementableEvent)
		void OnHandleMatchIsWaitingToStart();

	UFUNCTION(BlueprintImplementableEvent)
		void OnHandleMatchHasStarted();

	UFUNCTION(BlueprintImplementableEvent)
		void OnHandleMatchHasEnded();

	UFUNCTION(BlueprintImplementableEvent)
		void OnHandleMatchAborted();

	UFUNCTION(BlueprintImplementableEvent)
		void OnRestartGame();

	/////////////////////////////////////////////////////////////////////////
	// Match end

protected:
	
	/* Call this before EndMatch(). Called by ShouldEndMatch(). */
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
	    virtual void SetEndMatchInfo(const FText& InEndMatchReason, bool bInHaveTraitorsWon);

	/* If true it will call SetEndMatchInfo() accordingly. */
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
	    virtual bool ShouldEndMatch();

public:

	/* Call this function instead of EndMatch() unless SetEndMatchInfo() was called manually. */
	UFUNCTION(BlueprintCallable, Category = "KGameMode")
	    virtual void NewEndMatch(const FText& InEndMatchReason, bool bInHaveTraitorsWon);

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
