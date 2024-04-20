// Copyright Srujan Lokhande @2024


#include "InventorySystem/Public/Items/ItemBase.h"

UItemBase::UItemBase()
{
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

	return ItemCopy;
}

void UItemBase::SetItemQuantity(const int32 NewQuantity)
{
	if(NewQuantity != ItemQuantity)
	{
		ItemQuantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.maxStackSize : 1);

		// if(OwningInventory)
		// {
		// 	if(Quantitiy <= 0)
		// 	{
		// 		OwningInventory->RemoveItem(this);
		// 	}
		// }
	}
}

void UItemBase::Use(AInventorySystemCharacter* Character)
{
}
