// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Systems/Interfaces/InteractableInterface.h"
#include "WorldItem.generated.h"


class UBaseItem;
class USphereComponent;

/**
 * Actor that represents an Item on a level
 */
UCLASS()
class ELABORATE_API AWorldItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultSceneComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBillboardComponent* Billboard = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* TraceSphere = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Defaults", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBaseItem> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Defaults", meta = (AllowPrivateAccess = "true"))
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Defaults", meta = (AllowPrivateAccess = "true"))
	float DespawnTime = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PreInitializeComponents() override;

	void SetDespawnTimer();

	void InitialiseMeshBasedOnClass();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool Interact(UInventoryComponent* Inventory) override;

	void SetItemClass(TSubclassOf<UBaseItem> NewItemClass);

	inline TSubclassOf<UBaseItem> GetItemClass() { return ItemClass; };

};
