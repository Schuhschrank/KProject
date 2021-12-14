// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "KOnlineSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnUpdateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnStartSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnEndSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnDestroySessionComplete, bool, Successful);
// DECLARE_MULTICAST_DELEGATE_TwoParams(FCSOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCSOnFindSessionsComplete, int32, NumResults, bool, Successful);
// DECLARE_MULTICAST_DELEGATE_OneParam(FCSOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnJoinSessionComplete, int32, Result);

/**
 * 
 */
UCLASS()
class KPROJECT_API UKOnlineSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	UKOnlineSubsystem();

	UFUNCTION(BlueprintCallable)
	void CreateSession(int32 NumPublicConnections, bool IsLANMatch);

	UFUNCTION(BlueprintCallable)
	void UpdateSession();

	UFUNCTION(BlueprintCallable)
	void StartSession();

	UFUNCTION(BlueprintCallable)
	void EndSession();

	UFUNCTION(BlueprintCallable)
	void DestroySession();

	UFUNCTION(BlueprintCallable)
	void FindSessions(int32 MaxSearchResults, bool IsLANQuery);

	UFUNCTION(BlueprintCallable)
	void JoinGameSession(/*const FOnlineSessionSearchResult& SessionResult*/ int32 Index);

	UPROPERTY(BlueprintAssignable)
	FCSOnCreateSessionComplete OnCreateSessionCompleteEvent;

	UPROPERTY(BlueprintAssignable)
	FCSOnUpdateSessionComplete OnUpdateSessionCompleteEvent;

	UPROPERTY(BlueprintAssignable)
	FCSOnStartSessionComplete OnStartSessionCompleteEvent;
	
	UPROPERTY(BlueprintAssignable)
	FCSOnEndSessionComplete OnEndSessionCompleteEvent;

	UPROPERTY(BlueprintAssignable)
	FCSOnDestroySessionComplete OnDestroySessionCompleteEvent;

	UPROPERTY(BlueprintAssignable)
	FCSOnFindSessionsComplete OnFindSessionsCompleteEvent;

	UPROPERTY(BlueprintAssignable)
	FCSOnJoinSessionComplete OnJoinGameSessionCompleteEvent;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetSearchResultOwningUserName(int32 Index);

protected:

	void OnCreateSessionCompleted(FName SessionName, bool Successful);
	void OnUpdateSessionCompleted(FName SessionName, bool Successful);
	void OnStartSessionCompleted(FName SessionName, bool Successful);
	void OnEndSessionCompleted(FName SessionName, bool Successful);
	void OnDestroySessionCompleted(FName SessionName, bool Successful);
	void OnFindSessionsCompleted(bool Successful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	// void OnFindFriendSessionCompleted();

public:

	UFUNCTION(BlueprintCallable)
	bool TryTravelToCurrentSession();

private:

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDelegateHandle EndSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;

	// FOnFindFriendSessionCompleteDelegate OnFindFriendSessionCompleteDelegate;
};
