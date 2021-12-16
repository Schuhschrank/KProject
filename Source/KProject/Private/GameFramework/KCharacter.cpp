// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/KCharacter.h"

// Sets default values
AKCharacter::AKCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight",   this, &ThisClass::MoveRight);
		PlayerInputComponent->BindAxis("LookUp",      this, &ThisClass::LookUp);
		PlayerInputComponent->BindAxis("LookRight",   this, &ThisClass::LookRight);

		PlayerInputComponent->BindAction("Jump", IE_Pressed,  this, &ThisClass::Jump);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &ThisClass::StopJumping);
		PlayerInputComponent->BindAction("Use",  IE_Pressed,  this, &ThisClass::Use);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input functions

void AKCharacter::MoveForward(float Value)
{
	if (Value != 0)
	{
		FVector MovementDirection = GetControlRotation().Vector();
		MovementDirection.SetComponentForAxis(EAxis::Z, 0);
		MovementDirection.Normalize();
		AddMovementInput(MovementDirection, Value);
	}
}

void AKCharacter::MoveRight(float Value)
{
	if (Value != 0)
	{
		FVector MovementDirection = GetControlRotation().Vector();
		MovementDirection = MovementDirection.RotateAngleAxis(90, FVector::UpVector);
		MovementDirection.SetComponentForAxis(EAxis::Z, 0);
		MovementDirection.Normalize();
		AddMovementInput(MovementDirection, Value);
	}
}

void AKCharacter::LookUp(float Value)
{
	if (Value != 0)
	{
		AddControllerPitchInput(Value);
	}
}

void AKCharacter::LookRight(float Value)
{
	if (Value != 0)
	{
		AddControllerYawInput(Value);
	}
}

void AKCharacter::Use()
{

}
