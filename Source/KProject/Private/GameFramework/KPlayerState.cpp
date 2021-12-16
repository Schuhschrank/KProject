// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/KPlayerState.h"
#include "Net/UnrealNetwork.h"

void AKPlayerState::OnRep_PlayerRole()
{
	PlayerRoleChanged.Broadcast(PlayerRole);
}

void AKPlayerState::SetPlayerRole(EPlayerRole NewRole)
{
	PlayerRole = NewRole;
	OnRep_PlayerRole();
}

void AKPlayerState::Reset()
{
	Super::Reset();

	SetPlayerRole(EPlayerRole::NotParticipating);
	bIsGhost = false;
}

void AKPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKPlayerState, PlayerRole);

	// DOREPLIFETIME_CONDITION(APawn, RemoteViewPitch, COND_SkipOwner);
}

