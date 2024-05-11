// Copyright Srujan Lokhande @2024
#include "UserInterface/Inventory/InventoryWidget.h"

#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "InventorySystem/InventorySystemCharacter.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"


void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacterRef = Cast<AInventorySystemCharacter>(GetOwningPlayerPawn());
	if(PlayerCharacterRef)
	{
		InventoryComponentRef = PlayerCharacterRef->GetInventory();
		if(InventoryComponentRef)
		{
			// this is the method to call non dynamic delegate
			InventoryComponentRef->OnInventoryUpdated.AddUObject(this, &ThisClass::RefreshInventory);
			SetInfoText();
		}
	}
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	// detecting if the drop operation is happening on the same panel i.e on the inventory panel
	if(ItemDragDrop->SourceInventory && InventoryComponentRef)
	{
		// if it is, htne we return true which will prevent to drop the item on the panel
		return true;
	}
	return false;
}


void UInventoryWidget::RefreshInventory()
{
	if(InventoryComponentRef && InventorySlotClass)
	{
		// to get the most updated inventory we clear the previous inventory
		InventoryWrapBox->ClearChildren();

		// iterating through all of the Inventory items in the array and creates a slot of each item in the inventory Panel
		for (UItemBase* const& InventoryItem : InventoryComponentRef->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot =  CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			// this creates and adds the ItemSlot in the Inventory Panel
			InventoryWrapBox->AddChildToWrapBox(ItemSlot);
		}

		SetInfoText();
	}
}

void UInventoryWidget::SetInfoText() const
{
	// doing this to have the float value for the slots
	const FString WeightInfoValue {
	FString::SanitizeFloat(InventoryComponentRef->GetInventoryTotalWeight()) + "/" +
	FString::SanitizeFloat(InventoryComponentRef->GetWeightCapacity())};

	const FString CapacityInfoValue {
		FString::FromInt(InventoryComponentRef->GetInventoryContents().Num()) + "/" +
		FString::FromInt(InventoryComponentRef->GetSlotsCapacity())};

	TXT_WeightInfo->SetText(FText::FromString(WeightInfoValue));
	TXT_CapacityInfo->SetText(FText::FromString(CapacityInfoValue));

}


