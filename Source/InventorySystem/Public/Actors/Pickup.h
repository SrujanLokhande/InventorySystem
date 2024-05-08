// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "Pickup.generated.h"

struct FInteractableData;
class UItemBase;
class UDataTable;
class UStaticMeshComponent;

UCLASS()
class INVENTORYSYSTEM_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:	
	
	// Sets default values for this actor's properties
	APickup();

	// take the items from the data table during begin play
	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, int32 ItemQuantity);

	// to initialize the items from the data table to be ready for dropping
	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);	

	// getter for the Item
	FORCEINLINE UItemBase* GetItemData() const { return ItemReference;}

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupStaticMesh;

	// The data table with all of the info we give to a specific actor
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	UDataTable* ItemDataTable;

	// the ID of the item which can be referenced with the ItemID in the data table to get a specific item
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	FName DesiredID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 PickupItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;
	
	virtual void BeginPlay() override;

	virtual void Interact(AInventorySystemCharacter* PlayerCharacter) override;
	void UpdateInteractableData();
	void TakePickup(const AInventorySystemCharacter* PickupTaker);

#if WITH_EDITOR
	// to change the mesh of the placed objects and not be needing to explicitly go and change the mesh and the item_id for
	// all of the item instances inside editor
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};


