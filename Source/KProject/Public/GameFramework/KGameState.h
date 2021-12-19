// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "KGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEndMatchInfoChanged, FText, NewEndMatchReason, bool, bNewHaveTraitorsWon);

/**
 * 
 */
UCLASS()
class KPROJECT_API AKGameState : public AGameState
{
	GENERATED_BODY()

public:
	
//private:
//
//	UPROPERTY(VisibleAnywhere, Category = "KGameState")
//		FText EndMatchReason;
//
//	UPROPERTY(VisibleAnywhere, Category = "KGameState")
//		bool bHaveTraitorsWon;
//
//protected:
//
//	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "KGameState")
//		virtual void MulticastSetEndMatchInfo(const FText& InEndMatchReason, bool bInHaveTraitorsWon);
//
//public:
//
//	UPROPERTY(BlueprintAssignable)
//		FEndMatchInfoChanged EndMatchInfoChanged;
//
//	UFUNCTION(BlueprintCallable, Category = "KGameState")
//		virtual void SetEndMatchInfo(const FText& InEndMatchReason, bool bInHaveTraitorsWon);
//
//	UFUNCTION(BlueprintCallable, BlueprintPure)
//		FText GetEndMatchReason() const { return EndMatchReason; }
//
//	UFUNCTION(BlueprintCallable, BlueprintPure)
//		bool GetHaveTraitorsWon() const { return bHaveTraitorsWon; }

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "KGameState")
		void OnMatchStateChanged();

	//////////////////////////////////////////////////////////////////////
    // Inherited stuff

public:

	/* GameState interface */
	virtual void OnRep_MatchState() override;
	/* End GameState interface */

	/* Actor interface */
	// virtual void Tick(float DeltaSeconds) override;
	virtual void Reset() override;
	/* End Actor interface */

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
