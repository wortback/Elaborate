// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/WorldWeapons/MeleeWeaponWorld.h"
#include "Components/BoxComponent.h"
#include "Characters/BaseEnemy.h"
#include "Characters/CharacterCombatComponent.h"

#include "Elaborate.h"

AMeleeWeaponWorld::AMeleeWeaponWorld()
	: Super()
{
	AttackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitBox"));
	AttackHitBox->SetupAttachment(RootComponent);
}

void AMeleeWeaponWorld::Attack()
{
    UE_LOG(LogCombatSystem, Log, TEXT("Begin attack."));
	AttackHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackHitBox->SetVisibility(true);


	// TODO: Disable the collision

}

void AMeleeWeaponWorld::HeavyAttack()
{
    UE_LOG(LogCombatSystem, Log, TEXT("Begin heavy attack."));
    AttackHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    AttackHitBox->SetVisibility(true);

    // TODO: Disable the collision
}

void AMeleeWeaponWorld::BeginPlay()
{
	Super::BeginPlay();

	// Bind the overlap event
	if (AttackHitBox)
	{
		AttackHitBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponWorld::OnAttackBeginOverlap);
	}
}

void AMeleeWeaponWorld::OnAttackBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    // Check if the overlapped actor is an enemy and handle the attack
    if (OtherActor && OtherActor->IsA(ABaseEnemy::StaticClass()))
    {
        ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);
        if (Enemy)
        {
            UE_LOG(LogTemp, Log, TEXT("Weapon hit an enemy!"));
            Enemy->GetCombatComponent().TakeDamage(CombatComponent->CalculateTotalAttackDMG(true));
        }
    }
}