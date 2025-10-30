// Fill out your copyright notice in the Description page of Project Settings.

#include "TFUtils.h"

TArray<FString> ChopString(FString Source, char split)
{
	TArray<FString> Ret;
	FString LSide, RSide;

	while (Source.Split(&split, &LSide, &RSide))
	{
		Ret.Add(LSide);
		Source = RSide;
		LSide.Empty();
	}
	if (!RSide.IsEmpty())
	{
		Ret.Add(RSide);
	}

	return Ret;
}