#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StatlineComponent.generated.h"

UENUM(BlueprintType)
enum class ECoreStat : uint8
{
	CS_HEALTH UMETA(DisplayName="Health"),
	CS_STAMINA UMETA(DisplayName="Stamina"),
	CS_HUNGER UMETA(DisplayName="Hunger"),
	CS_THIRST UMETA(DisplayName="Thirst"),
};

USTRUCT(BlueprintType)
struct FCoreStat
{
	GENERATED_USTRUCT_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float Current = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float Max = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float PerSecondTick = 1;

public:
	FCoreStat() {};
	FCoreStat(const float& Current, const float& Max, const float& PerSecondTick)
		: Current(Current), Max(Max), PerSecondTick(PerSecondTick) {}
	
	void TickStat(const float& DeltaTime)
	{
		Current = FMath::Clamp(Current + (PerSecondTick * DeltaTime), 0, Max);
	}

	void Adjust(const float& Amount)
	{
		Current = FMath::Clamp(Current + Amount, 0, Max);
	}

	float Percentile() const
	{
		return Current / Max;
	}

	void AdjustTick(const float& NewTick)
	{
		PerSecondTick = NewTick;
	}
	float GetCurrent() const { return Current; }
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEFALL_API UStatlineComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UCharacterMovementComponent* OwningCharacterMovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FCoreStat Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FCoreStat Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FCoreStat Hunger = FCoreStat(100.0f, 100.0f, -0.125f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FCoreStat Thirst = FCoreStat(100.0f, 100.0f, -0.25f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsSprinting = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float SprintCostMultiplier = 2.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float JumpCost = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float WalkSpeed = 125.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float SprintSpeed = 450.0f;

	void TickStats(const float& DeltaTime);
	void TickStatStamina(const float& DeltaTime);
	bool IsValidSprinting() const;

protected:
	virtual void BeginPlay() override;

public:
	UStatlineComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Blueprintable)
	void SetMovementComponentRef(UCharacterMovementComponent* Component);

	UFUNCTION(BlueprintCallable)
	float GetStatPercentile(const ECoreStat Stat) const;

	UFUNCTION(BlueprintCallable)
	bool CanSprint() const;
	
	UFUNCTION(BlueprintCallable)
	void SetSprinting(const bool& IsSprinting);

	UFUNCTION(BlueprintCallable)
	bool CanJump();

	UFUNCTION(BlueprintCallable)
	void HasJumped();
};
