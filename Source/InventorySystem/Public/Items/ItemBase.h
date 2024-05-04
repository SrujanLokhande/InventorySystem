// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventorySystem/DataStructure/ItemDataStructs.h"
#include "ItemBase.generated.h"

class AInventorySystemCharacter;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UItemBase : public UObject
{
	GENERATED_BODY()

public:

	// Owner of the inventory
	UPROPERTY()
	UInventoryComponent* OwningInventory;	
	
	UPROPERTY(VisibleAnywhere, Category = "ItemData")
	int32 ItemQuantity;

	// ALl of the properties from the Data Struct
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FITemStats ItemStatistics;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
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
		return this->ID == OtherID;
	}
};
