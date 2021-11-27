// Fill out your copyright notice in the Description page of Project Settings.


#include "Testing/TGameModeBase.h"
#include "GameFramework/PlayerController.h"

#include "GameFramework/GameNetworkManager.h"
#include "Matinee/MatineeActor.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/World.h"
#include "Misc/CommandLine.h"
#include "UObject/Package.h"
#include "Misc/PackageName.h"
#include "Net/OnlineEngineInterface.h"
#include "GameFramework/GameStateBase.h"
#include "PhysicsEngine/BodyInstance.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/NetConnection.h"
#include "Engine/ChildConnection.h"
#include "Engine/PlayerStartPIE.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"

void ATGameModeBase::ResetPlayer(AController* PlayerToBeReset)
{
	if (PlayerToBeReset)
	{
		PlayerToBeReset->Reset();

		APlayerController* PC = Cast<APlayerController>(PlayerToBeReset);
		if (PC)
		{
			PC->ClientReset();
		}
	}
}

void ATGameModeBase::ResetPlayerReversed(AController* PlayerToBeReset)
{
	if (PlayerToBeReset)
	{
		APlayerController* PC = Cast<APlayerController>(PlayerToBeReset);
		if (PC)
		{
			PC->ClientReset();
		}

		PlayerToBeReset->Reset();
	}
}

void ATGameModeBase::ResetPlayerServer(AController* PlayerToBeReset)
{
	if (PlayerToBeReset)
	{
		PlayerToBeReset->Reset();
	}
}

void ATGameModeBase::ResetPlayerClient(AController* PlayerToBeReset)
{
	if (PlayerToBeReset)
	{
		APlayerController* PC = Cast<APlayerController>(PlayerToBeReset);
		if (PC)
		{
			if (PC->PlayerState)
			{
				PC->PlayerState->SetIsOnlyASpectator(true);
			}
			PC->ClientReset();
		}
	}
}

void ATGameModeBase::ResetGame()
{
	Reset();
}

void ATGameModeBase::ResetLevel()
{
	UE_LOG(LogGameMode, Verbose, TEXT("Reset %s"), *GetName());

	// Reset ALL controllers first
	for (FConstControllerIterator Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
	{
		AController* Controller = Iterator->Get();
		Controller->Reset();

		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (PlayerController)
		{
			PlayerController->ClientReset();
		}
	}

	// Reset all actors (except controllers, the GameMode, and any other actors specified by ShouldReset())
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* A = *It;
		if (A && !A->IsPendingKill() && A != this && !A->IsA<AController>() && ShouldReset(A))
		{
			A->Reset();
		}
	}

	// Reset the GameMode
	Reset();

	// Notify the level script that the level has been reset
	ALevelScriptActor* LevelScript = GetWorld()->GetLevelScriptActor();
	if (LevelScript)
	{
		LevelScript->LevelReset();
	}
}
