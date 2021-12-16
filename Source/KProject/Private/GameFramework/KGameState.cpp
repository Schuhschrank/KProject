// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/KGameState.h"
#include "Net/UnrealNetwork.h"

void AKGameState::OnRep_MatchState()
{
	Super::OnRep_MatchState();

	OnMatchStateChanged();
}

void AKGameState::SetEndMatchInfo(const FText& InEndMatchReason, bool bInHaveTraitorsWon)
{
	MulticastSetEndMatchInfo(InEndMatchReason, bInHaveTraitorsWon);
}

void AKGameState::MulticastSetEndMatchInfo_Implementation(const FText& InEndMatchReason, bool bInHaveTraitorsWon)
{
	EndMatchReason = InEndMatchReason;
	bHaveTraitorsWon = bInHaveTraitorsWon;
	EndMatchInfoChanged.Broadcast(EndMatchReason, bHaveTraitorsWon);
}

void AKGameState::Reset()
{
	Super::Reset();

	// EndMatchReason = FText::GetEmpty();
	// bHaveTraitorsWon = false;
}

void AKGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(AKGameState, EndMatchReason);
	// DOREPLIFETIME(AKGameState, bHaveTraitorsWon);

	// DOREPLIFETIME_CONDITION(APawn, RemoteViewPitch, COND_SkipOwner);
}
