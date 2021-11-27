// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/KGameMode.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/KPlayerState.h"
#include "GameFramework/KPlayerController.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/SpectatorPawn.h"

AKGameMode::AKGameMode(const FObjectInitializer& ObjectInitializer)
	: AGameMode(ObjectInitializer)
{
	// GameModeBase
	PlayerControllerClass = AKPlayerController::StaticClass();
	PlayerStateClass = AKPlayerState::StaticClass();
	bStartPlayersAsSpectators = true;

	// GameMode
	bDelayedStart = true;

	// KGameMode
	RoleCounts = { 0, 0, 0, 0 };
	AliveRoleCounts = { 0, 0, 0, 0 };
	bArePlayersMortal = false;
	bHaveTraitorsWon = false;
	EndMatchReason = FText::GetEmpty();
}

void AKGameMode::HandleMatchHasStarted()
{
	DistributeRoles();
	SetMortality(true);
	Super::HandleMatchHasStarted();
	RestartAllPlayers();
}

bool AKGameMode::ShouldEndMatch()
{
	if (!IsMatchInProgress())
	{
		return false;
	}

	if (AliveRoleCounts[(int32)EPlayerRole::Innocent]
		+ AliveRoleCounts[(int32)EPlayerRole::Detective] <= 0)
	{
		EndMatchReason = FText::FromString(FString("All Innocent and Detectives have died."));
		bHaveTraitorsWon = true;
		return true;
	}
	else if (AliveRoleCounts[(int32)EPlayerRole::Traitor] <= 0)
	{
		EndMatchReason = FText::FromString(FString("All Traitors have died."));
		bHaveTraitorsWon = false;
		return true;
	}

	return false;
}

void AKGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::RestartGame);
}

void AKGameMode::MyEndMatch(const FText& InEndMatchReason, bool bInHaveTraitorsWon)
{
	EndMatchReason = InEndMatchReason;
	bHaveTraitorsWon = bInHaveTraitorsWon;

	Super::EndMatch();
}

void AKGameMode::AbortMatch()
{
	EndMatchReason = FText::FromString(FString("The Match has been aborted."));
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::RestartGame);
}

void AKGameMode::RestartGame()
{
	if (GameSession->CanRestartGame())
	{
		if (GetMatchState() == MatchState::LeavingMap)
		{
			return;
		}

		// GetWorld()->ServerTravel("?Restart", GetTravelType());
		ResetLevel();
	}
}

void AKGameMode::RestartAllPlayers()
{
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		RestartPlayer(It->Get());
	}
}

void AKGameMode::DistributeRoles(int32 NumTraitors, int32 NumDetectives)
{
	int32 TotalPlayers = GetWorld()->GetNumControllers();
	TArray<EPlayerRole> RoleList;
	RoleList.Reserve(TotalPlayers);

	for (int32 i = 0; i < NumTraitors; ++i)
	{
		RoleList.Add(EPlayerRole::Traitor);
	}
	for (int32 i = 0; i < NumDetectives; ++i)
	{
		RoleList.Add(EPlayerRole::Detective);
	}
	for (int32 i = 0; i < TotalPlayers - NumTraitors - NumDetectives; ++i)
	{
		RoleList.Add(EPlayerRole::Innocent);
	}

	// Shuffle list for randomness

	for (int32 i = 0; i < TotalPlayers; ++i)
	{
		int32 RandomIndex = FMath::RandRange(i, TotalPlayers - 1);
		if (RandomIndex > i)
		{
			RoleList.Swap(i, RandomIndex);
		}
	}

	{
		int32 ListIndex = 0;
		for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
		{
			SetPlayerRole(It->Get(), RoleList[ListIndex++]);
		}
	}

	RoleCounts[(int32)EPlayerRole::NotParticipating] = 0;
	RoleCounts[(int32)EPlayerRole::Traitor] = NumTraitors;
	RoleCounts[(int32)EPlayerRole::Innocent] = TotalPlayers - NumTraitors - NumDetectives;
	RoleCounts[(int32)EPlayerRole::Detective] = NumDetectives;

	AliveRoleCounts[(int32)EPlayerRole::NotParticipating] = 0;
	AliveRoleCounts[(int32)EPlayerRole::Traitor] = NumTraitors;
	AliveRoleCounts[(int32)EPlayerRole::Innocent] = TotalPlayers - NumTraitors - NumDetectives;
	AliveRoleCounts[(int32)EPlayerRole::Detective] = NumDetectives;
}

void AKGameMode::HandlePlayerDeath(AController* DeadPlayer)
{
	if (!DeadPlayer)
	{
		return;
	}

	if (!GetMortality())
	{
		RestartPlayer(DeadPlayer);
		return;
	}

	{
		APlayerController* PC = Cast<APlayerController>(DeadPlayer);
		if (PC && PC->GetSpectatorPawn())
		{
			// Player is already dead
			return;
		}
	}

	MakePlayerSpectate(DeadPlayer);

	--AliveRoleCounts[(int32)GetPlayerRole(DeadPlayer)];

	if (ShouldEndMatch())
	{
		EndMatch();
	}
}

void AKGameMode::MakePlayerSpectate(AController* NewSpectator)
{
	if (NewSpectator)
	{
		APlayerController* PC = Cast<APlayerController>(NewSpectator);

		if (PC && !PC->IsInState(NAME_Spectating))
		{	
			// GetPawn() gets unpossessed
			PC->ChangeState(NAME_Spectating);
			PC->ClientGotoState(NAME_Spectating);
		}
	}
}

void AKGameMode::SetPlayerRole(AController* Player, EPlayerRole NewRole)
{
	if (Player && Player->PlayerState)
	{
		AKPlayerState* PlayerState = Cast<AKPlayerState>(Player->PlayerState);
		if (PlayerState)
		{
			PlayerState->PlayerRole = NewRole;
		}
	}
}

EPlayerRole AKGameMode::GetPlayerRole(AController* Player)
{
	if (Player && Player->PlayerState)
	{
		AKPlayerState* PlayerState = Cast<AKPlayerState>(Player->PlayerState);
		if (PlayerState)
		{
			return PlayerState->PlayerRole;
		}
	}
	return EPlayerRole::NotParticipating;
}

void AKGameMode::Tick(float DeltaSeconds)
{
	AGameModeBase::Tick(DeltaSeconds);
}

void AKGameMode::Reset()
{
	Super::Reset();

	bHaveTraitorsWon = false;
	EndMatchReason = FText::GetEmpty();
	SetMortality(false);

	SetMatchState(MatchState::WaitingToStart);
}
