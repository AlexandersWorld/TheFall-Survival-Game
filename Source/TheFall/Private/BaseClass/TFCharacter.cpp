#include "BaseClass/TFCharacter.h"
#include "Components/StatlineComponent.h"

ATFCharacter::ATFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
	Statline->SetMovementComponentRef(GetCharacterMovement());

	SaveActorID = FGuid::NewGuid();
}

void ATFCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!SaveActorID.IsValid())
	{
		SaveActorID = FGuid::NewGuid();
	}
}

bool ATFCharacter::CanJump() const
{
	return Statline->CanJump();
}

void ATFCharacter::HasJumped()
{
	Statline->HasJumped();
	ACharacter::Jump();
}

bool ATFCharacter::CanSprint() const
{
	return Statline->CanSprint();
}

void ATFCharacter::SetSprinting(const bool& IsSprinting)
{
	Statline->SetSprinting(IsSprinting);
}

void ATFCharacter::SetSneaking(const bool& IsSneaking)
{
	Statline->SetSneaking(IsSneaking);
}

void ATFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FGuid ATFCharacter::GetActorSaveID_Implementation()
{
	return SaveActorID;
}

FSaveActorData ATFCharacter::GetSaveData_Implementation()
{
	FSaveActorData Ret;

	Ret.ActorClass = this->GetClass();
	Ret.ActorTransform = this->GetTransform();
	Ret.WasSpawned = this->WasSpawned;

	return Ret;
}
