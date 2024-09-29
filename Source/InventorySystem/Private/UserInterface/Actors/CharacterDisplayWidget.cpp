// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Actors/CharacterDisplayWidget.h"

#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"
#include "InventorySystem/InventorySystemCharacter.h"
#include "InventorySystem/DataStructure/ItemDataStructs.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"

void UCharacterDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AInventorySystemCharacter>(GetOwningPlayerPawn());
}

bool UCharacterDisplayWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);	

	if(PlayerCharacter && ItemDragDrop->SourceItem)
	{
		if (ItemDragDrop->SourceItem->ItemType == EItemType::Weapons)
		{		
			PlayerCharacter->EquipWeapon(ItemDragDrop->SourceItem);
			return true;			
		}
	}
	return false;

	
}
