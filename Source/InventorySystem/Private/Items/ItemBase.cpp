// Copyright Srujan Lokhande @2024


#include "InventorySystem/Public/Items/ItemBase.h"
#include "Components/InventoryComponent.h"


UItemBase::UItemBase(): bIsCopy(false), bIsPickup(false)
{
}

void UItemBase::ResetItemFlags() 
{
	bIsCopy = false;
	bIsPickup = false;
}


UItemBase* UItemBase::CreateItemCopy() const
{
	// creating a new copy
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());
	ItemCopy->ID = this->ID;
	ItemCopy->ItemQuantity = this->ItemQuantity;
	ItemCopy->ItemRarity = this->ItemRarity;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->TextData = this->TextData;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UItemBase::SetItemQuantity(const int32 NewQuantity)
{
	if(NewQuantity != ItemQuantity)
	{
		ItemQuantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.maxStackSize : 1);

		if(OwningInventory)
		{
			if(ItemQuantity <= 0)
			{
				OwningInventory->RemoveSingleInstanceOfItem(this);
			}
		}
	}
}

void UItemBase::Use(AInventorySystemCharacter* Character)
{
}
