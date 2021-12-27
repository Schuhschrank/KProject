#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "KGameInstance.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogKGameInstance, Log, All);

/**
 * Handles session creation and joining, joining accepted invites and disconnecting when returning to menu.
 */
UCLASS()
class KPROJECT_API UKGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	/** Returns true if delegate will be fired, false otherwise (if there was an error). */
	UFUNCTION(BlueprintCallable)
	bool CreateSession();

protected:

	/** If successful open a level with the listen option. */
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveOnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

private:

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);;

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FOnFindFriendSessionCompleteDelegate OnFindFriendSessionCompleteDelegate;
	FDelegateHandle OnFindFriendSessionCompleteDelegateHandle;
	void OnFindFriendSessionComplete(int32 LocalUserNum, bool bWasSuccessful, const TArray<FOnlineSessionSearchResult>& SearchResult);

	// GameInstance interface begin

public:

	virtual void Init() override;

	/** Returns true if delegate will be fired, false otherwise (if there was an error). */
	virtual bool JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult);

	virtual TSubclassOf<UOnlineSession> GetOnlineSessionClass() override;

	/**
	 * Calls HandleDisconnect on either the OnlineSession if it exists or the engine, to cause a travel back to the default map. The instance must have a world.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void ReturnToMainMenu() override;

	// GameInstance interface end
	
};
