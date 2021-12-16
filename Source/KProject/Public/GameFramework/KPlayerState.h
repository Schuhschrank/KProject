// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
// #include "KGameMode.h"
#include "GameFramework/PlayerRole.h"
#include "KPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerRoleChanged, EPlayerRole, NewRole);

/**
 * 
 */
UCLASS()
class KPROJECT_API AKPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PlayerRole)
	EPlayerRole PlayerRole;

	UFUNCTION()
	virtual void OnRep_PlayerRole();

public:

	void SetPlayerRole(EPlayerRole NewRole);

	EPlayerRole GetPlayerRole() const { return PlayerRole; }

	UPROPERTY(BlueprintAssignable)
	FPlayerRoleChanged PlayerRoleChanged;

	UPROPERTY(BlueprintReadOnly)
	bool bIsGhost;

	//////////////////////////////////////////////////////////////////////
	// Inherited stuff

public:

	/* Actor interface */
	// virtual void Tick(float DeltaSeconds) override;
	virtual void Reset() override;
	/* End Actor interface */

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
