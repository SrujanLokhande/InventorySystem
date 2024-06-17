// Copyright Srujan Lokhande @2024


#include "UserInterface/MainMenuWidget.h"

#include "Components/InventoryComponent.h"
#include "InventorySystem/InventorySystemCharacter.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryWidget.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"


void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		if(InventoryWidget && InventoryComponentRef)
		{
			InventoryWidget->InitializeGrid(InventoryComponentRef->GetGridTileSize());
		}
	}, 0.2f, false);
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerCharacter = Cast<AInventorySystemCharacter>(GetOwningPlayerPawn());
	InventoryComponentRef = PlayerCharacter->GetInventory();
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

