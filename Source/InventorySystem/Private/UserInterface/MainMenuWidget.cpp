// Copyright Srujan Lokhande @2024


#include "UserInterface/MainMenuWidget.h"
#include "InventorySystem/InventorySystemCharacter.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerCharacter = Cast<AInventorySystemCharacter>(GetOwningPlayerPawn());
}

bool UMainMenuWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if(PlayerCharacter && ItemDragDrop->SourceItem)
	{
		PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->ItemQuantity);
		return true;
	}
	return false;
}
