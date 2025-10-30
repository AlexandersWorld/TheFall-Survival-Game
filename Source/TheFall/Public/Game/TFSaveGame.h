#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Interface/SaveActorInterface.h"

#include "TFSaveGame.generated.h"


UCLASS()
class THEFALL_API UTFSaveGame : public USaveGame
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<FGuid, FSaveActorData> SaveableActorData;
	UPROPERTY()
	FName CurrentlyLoadedLevel = "NONE";
	UPROPERTY()
	FSaveActorData  PlayerData;

public:
	void SetSaveActorData(TMap<FGuid, FSaveActorData> Data);
	TMap<FGuid, FSaveActorData> GetSaveActorData();
	void SetCurrentLevel(const FName Level);
	FName GetCurrentLevel();
	void SetPlayerData(FSaveActorData Data);
	FSaveActorData GetPlayerData();
};
