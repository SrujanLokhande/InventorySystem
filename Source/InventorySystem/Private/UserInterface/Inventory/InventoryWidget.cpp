// Copyright Srujan Lokhande @2024
#include "UserInterface/Inventory/InventoryWidget.h"

#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "InventorySystem/InventorySystemCharacter.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"


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
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}


void UInventoryWidget::RefreshInventory()
{
	if(InventoryComponentRef && InventorySlotClass)
	{
		// to get the most updated inventory we clear the previous inventory
		InventoryPanel->ClearChildren();

		// iterating through all of the Inventory items in the array and creates a slot of each item in the inventory Panel
		for (UItemBase* const& InventoryItem : InventoryComponentRef->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot =  CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			// this creates and adds the ItemSlot in the Inventory Panel
			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}
	}
}

void UInventoryWidget::SetInfoText() const
{
	WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
		InventoryComponentRef->GetInventoryTotalWeight(),
		InventoryComponentRef->GetWeightCapacity()));

	CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
		// number of items in the inventory array
	InventoryComponentRef->GetInventoryContents().Num(),
	InventoryComponentRef->GetSlotsCapacity()));
}


