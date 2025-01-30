// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CharacterCombatComponent.generated.h"


class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELABORATE_API UCharacterCombatComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStats | Health")
    float Health = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStats | Stamina")
    float Stamina = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStats | Health")
    float MaxHealth = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStats | Stamina")
    float MaxStamina = 100.f;

	/** Attack cool down in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoolDown")
	float AttackCooldown = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float BaseMeleeDMG = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float BaseRangedDMG = 10.f;

	/** How much more DMG is inflicted when heavily attacking. In range 1.0...1.5. Defaults to 1.1f. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float HeavyAttackDMGMultiplier = 1.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Damage | Crit")
	float CritRate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Damage | Crit")
	float CritDMG = 0.f;

	/** In range 0.0...1.0 where 0.0 is no DMG taken */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defence")
	float DefenceMultiplier = 1.0;

	// Character states and during combat
	bool bIsAttacking = false;

	bool bIsAttackOnCoolDown = false;

	/** Is set by the currently equipped weapon of the component's owner. Defaults to 20. */
	float HeavyAttackStaminaConsuption = 20.f;


	// Other References
	UCharacterMovementComponent* MovementComponent;


public:	
	UCharacterCombatComponent();

	void PerformMeleeAttack(bool bIsHeavy);

	void PerformRangedAttack();

	/** Applies DMG to the owner of the component with opt. reduction */
	void TakeDamage(float IncomingDamage);

	void ConsumeStamina(float StaminaPoints);

	void RegenerateHealth(float HealthPoints);

	void RegenerateStamina(float StaminaPoints);

	void Die();

protected:
	virtual void BeginPlay() override;

	/** Based on the information from CharacterMovementComponent */
	bool CanAttack();
private:

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
