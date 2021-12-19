// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/KGameMode.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/KPlayerState.h"
#include "GameFramework/KPlayerController.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/KGameState.h"

DEFINE_LOG_CATEGORY(LogKGameMode);

constexpr auto StrInnocentsWon = "The Innocents have won! All Traitors are dead.";
constexpr auto StrTraitorsWon = "The Traitors have won! All Innocents and Detectives are dead.";
constexpr auto StrMatchAborted = "The Match has been aborted.";

AKGameMode::AKGameMode(/* const FObjectInitializer& ObjectInitializer */)
	: AGameMode(/* ObjectInitializer */)
{
	// GameModeBase
	PlayerControllerClass = AKPlayerController::StaticClass();
	PlayerStateClass = AKPlayerState::StaticClass();
	GameStateClass = AKGameState::StaticClass();
	bStartPlayersAsSpectators = true;

	// GameMode
	bDelayedStart = true;

	// KGameMode
	bArePlayersMortal = false;
}

void AKGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
}

void AKGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AKGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

void AKGameMode::HandleMatchAborted()
{
	Super::HandleMatchAborted();
}

//void AKGameMode::SetEndMatchInfo(const FText& InEndMatchReason, bool bInHaveTraitorsWon)
//{
//	auto GS = GetGameState<AKGameState>();
//	if (GS)
//	{
//		GS->SetEndMatchInfo(InEndMatchReason, bInHaveTraitorsWon);
//	}
//}

bool AKGameMode::ShouldEndMatch_Implementation()
{
	//if (!IsMatchInProgress())
	//{
	//	return false;
	//}

	//if ( GetAliveRoleCount(EPlayerRole::Innocent) + GetAliveRoleCount(EPlayerRole::Detective) <= 0)
	//{
	//	SetEndMatchInfo(FText::FromString(FString(StrTraitorsWon)), true);

	//	return true;
	//}
	//else if (GetAliveRoleCount(EPlayerRole::Traitor) /* AliveRoleCounts[(int32)EPlayerRole::Traitor] */ <= 0)
	//{
	//	SetEndMatchInfo(FText::FromString(FString(StrInnocentsWon)), false);

	//	return true;
	//}

	return false;
}

//void AKGameMode::NewEndMatch(const FText& InEndMatchReason, bool bInHaveTraitorsWon)
//{
//	SetEndMatchInfo(InEndMatchReason, bInHaveTraitorsWon);
//	EndMatch();
//}

void AKGameMode::RestartGame()
{
	UE_LOG(LogKGameMode, Display, TEXT("Begin restarting game."));

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

	UE_LOG(LogKGameMode, Display, TEXT("Restarted all players."));
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

	UE_LOG(LogKGameMode, Display, TEXT("Distributed roles."));
}

void AKGameMode::HandlePlayerDeath_Implementation(AController* DeadPlayer)
{
	if (!DeadPlayer)
	{
		return;
	}

	AKPlayerState* PS = Cast<AKPlayerState>(DeadPlayer->PlayerState);

	if (PS && (PS->IsGhost() || PS->GetPlayerRole() == EPlayerRole::NotParticipating))
	{
		// Player is already dead
		return;
	}

	if (!ArePlayersMortal())
	{
		RestartPlayer(DeadPlayer);
		return;
	}

	// Register player as dead

	if (PS)
	{
		PS->SetIsGhost(true);
	}

	MakePlayerSpectate(DeadPlayer);

	UE_LOG( LogKGameMode, Display, TEXT("Handled death of player %s."), (DeadPlayer->PlayerState) ? *( DeadPlayer->PlayerState->GetPlayerName() ) : TEXT("unknown") );

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

			UE_LOG( LogKGameMode, Display, TEXT("Made player %s a spectator."), ( PC->PlayerState ) ? *( PC->PlayerState->GetPlayerName() ) : TEXT("unknown") );
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
			PlayerState->SetPlayerRole(NewRole);

			UE_LOG( LogKGameMode, Display, TEXT("Assigned role %d to player %s."), (int32)NewRole, *( PlayerState->GetPlayerName() ) );
		}
	}
}

EPlayerRole AKGameMode::GetPlayerRole(AController* Player) const
{
	if (Player && Player->PlayerState)
	{
		AKPlayerState* PlayerState = Cast<AKPlayerState>(Player->PlayerState);
		if (PlayerState)
		{
			return PlayerState->GetPlayerRole();
		}
	}
	return EPlayerRole::NotParticipating;
}

int32 AKGameMode::GetRoleCount(EPlayerRole InRole) const
{
	int32 Total = 0;
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		AController* Controller = It->Get();
		if (Controller)
		{
			Total += GetPlayerRole(Controller) == InRole ? 1 : 0;
		}
	}
	return Total;
}

int32 AKGameMode::GetAliveRoleCount(EPlayerRole InRole) const
{
	int32 Total = 0;
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		AController* Controller = It->Get();
		if (Controller)
		{
			bool bPlayerIsGhost = true;
			if (auto PS = Controller->GetPlayerState<AKPlayerState>())
			{
				bPlayerIsGhost = PS->IsGhost();
			}
			Total += GetPlayerRole(Controller) == InRole && !bPlayerIsGhost ? 1 : 0;
		}
	}
	return Total;
}

FRoleCounts AKGameMode::GetRoleCounts(bool bAlive) const
{
	FRoleCounts RoleCounts;
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		AController* Controller = It->Get();
		if (Controller)
		{
			bool bPlayerIsGhost = true;
			if (auto PS = Controller->GetPlayerState<AKPlayerState>())
			{
				bPlayerIsGhost = PS->IsGhost();
			}
			if (!bPlayerIsGhost || !bAlive)
			{
				switch (GetPlayerRole(Controller))
				{
				case EPlayerRole::NotParticipating:
					RoleCounts.NotParticipating += 1;
					break;
				case EPlayerRole::Traitor:
					RoleCounts.Traitors += 1;
					break;
				case EPlayerRole::Innocent:
					RoleCounts.Innocents += 1;
					break;
				case EPlayerRole::Detective:
					RoleCounts.Detectives += 1;
					break;
				default:
					break;
				}
			}
		}
	}
	return RoleCounts;
}

TArray<AController*> AKGameMode::GetAllPlayers() const
{
	TArray<AController*> Result;
	Result.Reserve(GetWorld() ? GetWorld()->GetNumControllers() : 1);

	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		if (auto Controller = It->Get())
		{
			Result.Add(Controller);
		}
	}
	return Result;
}

void AKGameMode::Tick(float DeltaSeconds)
{
	AGameModeBase::Tick(DeltaSeconds);
}

void AKGameMode::Reset()
{
	Super::Reset();

	SetArePlayersMortal(false);

	SetMatchState(MatchState::WaitingToStart);
}
