

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "KGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class KPROJECT_API UKGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// UGameInstance interface begin

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

	// UGameInstance interface end
	
};
