// Copyright Srujan Lokhande @2024

#include "UserInterface/Inventory/InventoryItemSlot.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryTooltip.h"


void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if(ToolTipClass)
	{
		UInventoryTooltip* Tooltip = CreateWidget<UInventoryTooltip>(this, ToolTipClass);
		Tooltip->InventorySlotBeingHovered = this;
		SetToolTip(Tooltip);
	}
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// changing the color of the image border based on the rarity
	if(ItemReference)
	{
		switch (ItemReference->ItemRarity)
		{
		case EItemRarity::Common:
				ItemBorder->SetBrushColor(FLinearColor::White);
				break;
		case EItemRarity::Rare:
				ItemBorder->SetBrushColor(FLinearColor::Blue);
				break;
			case EItemRarity::Epic:
				ItemBorder->SetBrushColor(FLinearColor::Red);
				break;
			case EItemRarity::Legendary:
				ItemBorder->SetBrushColor(FLinearColor::Yellow);
				break;
		default: ;
		}

		// setting the item icon from datatable
		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

		if(ItemReference->NumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->ItemQuantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
