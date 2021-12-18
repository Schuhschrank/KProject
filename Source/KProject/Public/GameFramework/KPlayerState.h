// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
// #include "KGameMode.h"
#include "GameFramework/KEnums.h"
#include "KPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerRoleChanged, EPlayerRole, NewRole);

/**
 * 
 */
UCLASS()
class KPROJECT_API AKPlayerState : public APlayerState
{
	GENERATED_BODY()

	/////////////////////////////////////////////////////////////////////////
	// Player role

private:

	UPROPERTY(ReplicatedUsing=OnRep_PlayerRole)
	    EPlayerRole PlayerRole;

protected:

	UFUNCTION(BlueprintCallable, Category = "KPlayerState")
	    virtual void OnRep_PlayerRole();

public:

	UFUNCTION(BlueprintCallable, Category = "KPlayerState")
	    virtual void SetPlayerRole(EPlayerRole NewRole);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "KPlayerState")
		virtual EPlayerRole GetPlayerRole() const { return PlayerRole; }

	UPROPERTY(BlueprintAssignable, Category = "KPlayerState")
		FPlayerRoleChanged PlayerRoleChanged;

	/////////////////////////////////////////////////////////////////////////
	// Is ghost

private:

	bool bIsGhost;

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "KPlayerState")
		virtual bool IsGhost() const { return bIsGhost; }

	UFUNCTION(BlueprintCallable, Category = "KPlayerState")
		virtual void SetIsGhost(bool bInIsGhost) { bIsGhost = bInIsGhost; }

	//////////////////////////////////////////////////////////////////////
	// Inherited stuff

public:

	/* Actor interface */
	// virtual void Tick(float DeltaSeconds) override;
	virtual void Reset() override;
	/* End Actor interface */

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
