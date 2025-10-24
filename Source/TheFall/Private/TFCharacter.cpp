#include "TFCharacter.h"
#include "Components/StatlineComponent.h"

ATFCharacter::ATFCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
	Statline->SetMovementComponentRef(GetCharacterMovement());
}

void ATFCharacter::BeginPlay()
{
	Super::BeginPlay();
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

void ATFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
