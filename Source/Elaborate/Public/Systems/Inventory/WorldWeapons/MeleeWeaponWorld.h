// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Inventory/WorldItem.h"
#include "Systems/Interfaces/CombatWeaponInterface.h"

#include "MeleeWeaponWorld.generated.h"


class UBoxComponent;
class UCharacterCombatComponent;
/**
 * 
 */
UCLASS()
class ELABORATE_API AMeleeWeaponWorld : public AWorldItem, public ICombatWeaponInterface
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UBoxComponent* AttackHitBox;

	UCharacterCombatComponent* CombatComponent;
	
public:
	AMeleeWeaponWorld();

	virtual void Attack() override;

	virtual void HeavyAttack() override;

protected:
	virtual void BeginPlay() override;


private:
	void OnAttackBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
};
