#include "Game/TFGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Game/TFSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"


UTFGameInstance::UTFGameInstance()
{

}

void UTFGameInstance::CreateSaveSlot()
{
	SaveGameObject = Cast<UTFSaveGame>(UGameplayStatics::CreateSaveGameObject(UTFSaveGame::StaticClass()));
}

void UTFGameInstance::GatherActorData()
{
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<USaveActorInterface>())
		{
			continue;
		}

		ISaveActorInterface* Inter = Cast<ISaveActorInterface>(Actor);

		if (Inter == nullptr) continue;

		FGuid SAI = Inter->GetActorSaveID_Implementation();
		if (!SAI.IsValid())
		{
			continue;
		}

		FSaveActorData SAD = Inter->GetSaveData_Implementation();

		FMemoryWriter MemWriter(SAD.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);

		SaveableActorData.Add(SAI, SAD);
	}
}

void UTFGameInstance::LoadGame()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveGameName, 0))
	{
		//TODO: Add loggin and error message about missing save game
		return;
	}
	SaveableActorData.Empty();
	SaveGameObject = Cast<UTFSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameName, 0));
	SaveableActorData = SaveGameObject->GetSaveActorData();

	for (TTuple<FGuid, FSaveActorData> SAD : SaveableActorData)
	{
		if (SAD.Value.WasSpawned)
		{
			AActor* SpawnedActor = GetWorld()->SpawnActor(SAD.Value.ActorClass->StaticClass(), &SAD.Value.ActorTransform);
			ISaveActorInterface* Inter = Cast<ISaveActorInterface>(SpawnedActor);

			if (Inter == nullptr) continue;
			
			//Set Actor GUID
		}
	}

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<USaveActorInterface>())
		{
			continue;
		}

		ISaveActorInterface* Inter = Cast<ISaveActorInterface>(Actor);

		if (Inter == nullptr) continue;

		FGuid SAI = Inter->GetActorSaveID_Implementation();

		if (!SaveableActorData.Find(SAI)) continue;

		FSaveActorData SAD = SaveableActorData[SAI];
		Actor->SetActorTransform(SAD.ActorTransform);

		FMemoryReader MemReader(SAD.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);

		/*
			Add Adicional Logic here for custom data
		*/
	}
}

void UTFGameInstance::AddActorData(const FGuid ActorID, FSaveActorData ActorData)
{
	SaveableActorData.Add(ActorID, ActorData);
}

FSaveActorData UTFGameInstance::GetActorData(const FGuid& ActorID)
{
	return SaveableActorData[ActorID];
}

void UTFGameInstance::DEV_SaveGame()
{
	if (SaveGameObject == nullptr)
	{
		CreateSaveSlot();
	}
	GatherActorData();
	SaveGameObject->SetSaveActorData(SaveableActorData);
	UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameName, 0);
}

void UTFGameInstance::DEV_LoadGame()
{
	LoadGame();
}
