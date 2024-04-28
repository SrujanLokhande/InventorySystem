// Copyright Srujan Lokhande @2024


#include "UserInterface/MainMenuWidget.h"
#include "InventorySystem/InventorySystemCharacter.h"

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
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	// Cast Operation to item drag drop
	// Endure player is valid
	// call drop item on player
}
