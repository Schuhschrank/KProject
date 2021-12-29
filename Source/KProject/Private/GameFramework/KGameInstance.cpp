#include "GameFramework/KGameInstance.h"
#include "OnlineSessionClient.h"
#include "OnlineSubsystemUtils.h"

DEFINE_LOG_CATEGORY(LogKGameInstance);

void UKGameInstance::Init()
{
	Super::Init();

	OnJoinSessionCompleteDelegate       =       FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete);
	OnCreateSessionCompleteDelegate     =     FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete);
	OnFindFriendSessionCompleteDelegate = FOnFindFriendSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnFindFriendSessionComplete);
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
	UE_LOG(LogKGameInstance, Display, TEXT("Joining session completed for session with name %s with result %s."), *SessionName.ToString(), LexToString(Result));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		FString::Printf(TEXT("Joining session completed for session with name %s with result %s."), *SessionName.ToString(), LexToString(Result)));

	auto SessionInt = Online::GetSessionInterface();
	if (SessionInt.IsValid())
	{
		SessionInt->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	}

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (!ClientTravelToSession(0, SessionName))
		{
			UE_LOG(LogKGameInstance, Warning, TEXT("Not able to travel to session."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Not able to travel to session."));
		}
	}
}

bool UKGameInstance::CreateSession(int InNumPublicConnections, bool bInAllowInvites, bool bInAllowJoinViaPresence, bool bInUseLobbiesVoiceChatIfAvailable, int Filter)
{
	auto SessionInt = Online::GetSessionInterface();
	if (SessionInt.IsValid())
	{
		OnCreateSessionCompleteDelegateHandle = SessionInt->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

		FOnlineSessionSettings Settings;
		Settings.bAllowInvites = bInAllowInvites;
		Settings.bAllowJoinInProgress = true;
		Settings.bAllowJoinViaPresence = bInAllowJoinViaPresence;
		Settings.bShouldAdvertise = true;		// true otherwise cannot invite friends
		Settings.bUseLobbiesIfAvailable = true; // true to set up lobby
		Settings.bUsesPresence = true;			// true to set up lobby
		Settings.NumPublicConnections = InNumPublicConnections;

		Settings.bUseLobbiesVoiceChatIfAvailable = bInUseLobbiesVoiceChatIfAvailable;

		// Temporary setting for filtering
		Settings.Set(SETTING_CUSTOMSEARCHINT1, Filter, EOnlineDataAdvertisementType::ViaOnlineService);

		return SessionInt->CreateSession(0, NAME_GameSession, Settings);
	}
	return false;
}

void UKGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogKGameInstance, Display, TEXT("Creating session completed for session with name %s with result %s."), *SessionName.ToString(), bWasSuccessful ? TEXT("true") : TEXT("false"));

	auto SessionInt = Online::GetSessionInterface();
	if (SessionInt.IsValid())
	{
		SessionInt->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	}

	ReceiveOnCreateSessionComplete(SessionName, bWasSuccessful);
}

void UKGameInstance::OnFindFriendSessionComplete(int32 LocalUserNum, bool bWasSuccessful, const TArray<FOnlineSessionSearchResult>& SearchResult)
{
	UE_LOG(LogKGameInstance, Display, TEXT("OnFindFriendSessionComplete with result %s."), bWasSuccessful ? TEXT("true") : TEXT("false"));

	if (bWasSuccessful && SearchResult.IsValidIndex(0) && SearchResult[0].IsValid())
	{
		if (!JoinSession(GetLocalPlayerByIndex(LocalUserNum), SearchResult[0]))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("OnFindFriendSessionComplete: Joining session failed."));
		}
	}
}
