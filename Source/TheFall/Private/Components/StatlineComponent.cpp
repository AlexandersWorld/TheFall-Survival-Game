#include "Components/StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UStatlineComponent::TickStats(const float& DeltaTime)
{
	Health.TickStat(DeltaTime);
	TickStatStamina(DeltaTime);
	Hunger.TickStat(DeltaTime);
	Thirst.TickStat(DeltaTime);
}

void UStatlineComponent::TickStatStamina(const float& DeltaTime)
{
	if (bIsSprinting && IsValidSprinting())
	{
		Stamina.TickStat(0 - (DeltaTime * SprintCostMultiplier));

		if (Stamina.GetCurrent() <= 0.0f)
		{
			SetSprinting(false);
		}
		return;
	}
	Stamina.TickStat(DeltaTime);
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
	OwningCharacterMovementComponent->MaxWalkSpeed = IsSprinting ? SprintSpeed : WalkSpeed;
}

bool UStatlineComponent::CanJump()
{
	return Stamina.GetCurrent() >= JumpCost;
}

void UStatlineComponent::HasJumped()
{
	Stamina.Adjust(0 - JumpCost);
}
