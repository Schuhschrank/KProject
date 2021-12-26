


#include "GameFramework/KGameInstance.h"
#include "OnlineSessionClient.h"
#include "OnlineSubsystemUtils.h"

void UKGameInstance::Init()
{
	Super::Init();

	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete);
}

TSubclassOf<UOnlineSession> UKGameInstance::GetOnlineSessionClass()
{
	return UOnlineSessionClient::StaticClass();
}

void UKGameInstance::ReturnToMainMenu()
{
	Super::ReturnToMainMenu();
}

bool UKGameInstance::JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult)
{
	auto SessionInt = Online::GetSessionInterface();
	if (SessionInt.IsValid())
	{
		OnJoinSessionCompleteDelegateHandle = SessionInt->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

		return SessionInt->JoinSession(0, NAME_GameSession, SearchResult);
	}
	return false;
}

void UKGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogOnlineSession, Display, TEXT("Joining session completed for session with name %s with result %s."), *SessionName.ToString(), *LexToString(Result));

	auto SessionInt = Online::GetSessionInterface();
	if (SessionInt.IsValid())
	{
		SessionInt->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	}

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (!ClientTravelToSession(0, SessionName))
		{
			UE_LOG(LogOnlineSession, Warning, TEXT("Not able to travel to session."));
		}
	}
}
