// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseEnemy.h"
#include "Characters/CharacterCombatComponent.h"
#include "Components/CapsuleComponent.h"

#include "Elaborate.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	CharacterCombatComponent = CreateDefaultSubobject<UCharacterCombatComponent>(TEXT("CombatComp"));
	
	UCapsuleComponent* Capsule = GetCapsuleComponent(); 
	Capsule->SetCollisionResponseToChannel(ECC_Attackable, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UCharacterCombatComponent& ABaseEnemy::GetCombatComponent()
{
	return *CharacterCombatComponent;
}

