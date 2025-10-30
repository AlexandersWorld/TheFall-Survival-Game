#include "Components/StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TFUtils.h"

void UStatlineComponent::TickStats(const float& DeltaTime)
{
	TickStatStamina(DeltaTime);
	TickStatHunger(DeltaTime);
	TickStatThirst(DeltaTime);
	if (Thirst.GetCurrent() <= 0.0 || Hunger.GetCurrent() <= 0.0)
	{
		return;
	}
	Health.TickStat(DeltaTime);
}

void UStatlineComponent::TickStatStamina(const float& DeltaTime)
{

	if (CurrentStaminaExhaustion > 0.0f)
	{
		CurrentStaminaExhaustion -= DeltaTime;
		return;
	}

	if (bIsSprinting && IsValidSprinting())
	{
		Stamina.TickStat(0 - (DeltaTime * SprintCostMultiplier));

		if (Stamina.GetCurrent() <= 0.0f)
		{
			SetSprinting(false);
			CurrentStaminaExhaustion = SecondsForStaminaExhaustion;
		}
		return;
	}

	Stamina.TickStat(DeltaTime);
}

void UStatlineComponent::TickStatThirst(const float& DeltaTime)
{
	if (Thirst.GetCurrent() <= 0.0)
	{
		Health.Adjust(0 - abs(DehydrationHealthDamagePerSecond * DeltaTime));
		return;
	}

	Thirst.TickStat(DeltaTime);
}

void UStatlineComponent::TickStatHunger(const float& DeltaTime)
{
	if (Hunger.GetCurrent() <= 0.0)
	{
		Health.Adjust(0 - abs(StarvingHealthdamagePerSecond * DeltaTime));
		return;
	}

	Hunger.TickStat(DeltaTime);
}

bool UStatlineComponent::IsValidSprinting() const
{
	return OwningCharacterMovementComponent->Velocity.Length() > WalkSpeed
		&& !OwningCharacterMovementComponent->IsFalling();
}

UStatlineComponent::UStatlineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatlineComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningCharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
}

void UStatlineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TickType != ELevelTick::LEVELTICK_PauseTick)
	{
		TickStats(DeltaTime);
	}
}

void UStatlineComponent::SetMovementComponentRef(UCharacterMovementComponent* Component)
{
	OwningCharacterMovementComponent = Component;
}

float UStatlineComponent::GetStatPercentile(const ECoreStat Stat) const
{
	switch (Stat)
	{
	case ECoreStat::CS_HEALTH:
		return Health.Percentile();
	case ECoreStat::CS_STAMINA:
		return Stamina.Percentile();
	case ECoreStat::CS_HUNGER:
		return Hunger.Percentile();
	case ECoreStat::CS_THIRST:
		return Thirst.Percentile();
	default:
		break;
	}
	return -1;
}

bool UStatlineComponent::CanSprint() const
{
	return Stamina.GetCurrent() > 0.0f;
}

void UStatlineComponent::SetSprinting(const bool& IsSprinting)
{
	bIsSprinting = IsSprinting;
	if (bIsSneaking && !bIsSprinting)
	{
		return;
	}
	bIsSneaking = false;
	OwningCharacterMovementComponent->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
}

void UStatlineComponent::SetSneaking(const bool& IsSneaking)
{
	bIsSneaking = IsSneaking;
	if (bIsSprinting && !bIsSneaking)
	{
		return;
	}
	bIsSprinting = false;
	OwningCharacterMovementComponent->MaxWalkSpeed = bIsSneaking ? SneakSpeed : WalkSpeed;
}

bool UStatlineComponent::CanJump()
{
	return Stamina.GetCurrent() >= JumpCost && OwningCharacterMovementComponent->IsMovingOnGround();
}

void UStatlineComponent::HasJumped()
{
	Stamina.Adjust(0 - JumpCost);
}

FSaveComponentData UStatlineComponent::GetComponentSaveData_Implementation()
{
	FSaveComponentData Ret;

	Ret.ComponentClass = this->GetClass();
	Ret.RawData.Add(Health.GetSaveString());
	Ret.RawData.Add(Stamina.GetSaveString());
	Ret.RawData.Add(Hunger.GetSaveString());
	Ret.RawData.Add(Thirst.GetSaveString());

	return Ret;
}

void UStatlineComponent::SetComponentSaveData_Implementation(FSaveComponentData Data)
{
	TArray<FString> Parts;

	for (int i = 0; i < Data.RawData.Num(); i++)
	{
		Parts.Empty();
		Parts = ChopString(Data.RawData[i], '|');

		switch (i)
		{
		case 0: 
			Health.UpdateFromSaveString(Parts);
			break;
		case 1:
			Stamina.UpdateFromSaveString(Parts);
			break;
		case 2:
			Hunger.UpdateFromSaveString(Parts);
			break;
		case 3:
			Thirst.UpdateFromSaveString(Parts);
			break;
		default:
			break;
		}
	}
}
