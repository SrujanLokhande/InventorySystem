// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventorySystem/DataStructure/ItemDataStructs.h"
#include "ItemBase.generated.h"

class AInventorySystemCharacter;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UItemBase : public UObject
{
	GENERATED_BODY()

public:

	// Owner of the inventory
	// UPROPERTY()
	// UInventoryComponent* OwningInventory;
	
	// the items will be stored in a stack so I gave UMin and UMax
	UPROPERTY(VisibleAnywhere, Category = "ItemData", meta = (UIMin=1, UIMax=50))
	int32 ItemQuantity;

	// ALl of the properties from the Data Struct
	UPROPERTY(EditAnywhere, Category = "ItemData")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	EItemRarity ItemRarity;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FITemStats ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemAssetData AssetData;

	// Constructor
	UItemBase();

	// creating a copy of the item
	UItemBase* CreateItemCopy() const;

	// making it forceinline creates a copy of this function wherever it is called to reduce function calling times and optimizing
	// Returns the the weight of the whole stack of items
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return ItemQuantity * NumericData.ItemWeight ; }

	// returns the weight of individual items
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.ItemWeight ; }

	// returns if the item stack is full
	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsItemFullStack() const { return ItemQuantity == NumericData.maxStackSize ; }

	// Setter for the item quantity
	UFUNCTION(Category = "Item")
	void SetItemQuantity(const int32 NewQuantity);

	// Function to use the inventory item
	UFUNCTION(Category = "Item")
	virtual void Use(AInventorySystemCharacter* Character);

protected:

	// overloading the == operator to compare the keys inside TArray for array of the items
	bool operator==(const FName& OtherID) const
	{
		return ID == OtherID;
	}
};
