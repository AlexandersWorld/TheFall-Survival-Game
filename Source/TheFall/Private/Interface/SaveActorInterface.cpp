// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/SaveActorInterface.h"

// Add default functionality here for any ISaveActorInterface functions that are not pure virtual.

FGuid ISaveActorInterface::GetActorSaveID_Implementation()
{
	FGuid Ret = FGuid();
	Ret.Invalidate();
	return Ret;
}

void ISaveActorInterface::SetActorGUID_Implementation(const FGuid& NewGuid)
{
}

FSaveActorData ISaveActorInterface::GetSaveData_Implementation()
{
	return FSaveActorData();
}

FSaveComponentData ISaveActorInterface::GetComponentSaveData_Implementation()
{
	return FSaveComponentData();
}

void ISaveActorInterface::SetComponentSaveData_Implementation(FSaveComponentData Data)
{
}
