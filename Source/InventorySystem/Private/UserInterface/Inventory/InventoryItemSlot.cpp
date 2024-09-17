// Copyright Srujan Lokhande @2024

#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/DragItemVisual.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"



void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// if(ToolTipClass)
	// {
	// 	//ToolTipUI->InventorySlotBeingHovered = this;		
	// 	
	// 	// ToolTipUI = CreateWidget<UInventoryTooltip>(this, ToolTipClass);
	// 	// ToolTipUI->AddToViewport(10);
	// 	// ToolTipUI->SetVisibility(ESlateVisibility::Collapsed);
	// 	// SetToolTip(ToolTipUI);
	// }
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
				ItemBorder->SetBrushColor(FLinearColor::Red);
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
		IMG_ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon); 		

		if(ItemReference->NumericData.bIsStackable)
		{
			TXT_ItemQuantity->SetText(FText::AsNumber(ItemReference->ItemQuantity));
		}
		else
		{
			TXT_ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// if we are pressing LMB it detects the drag and returns it
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	// if we are not dragging or detecting any LMB
	// submenu on Right Click will happen here
	return Reply.Unhandled();
}


void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (!DragItemVisualClass || !ItemReference) return;

	// for dragging the item actor 
	if(DragItemVisualClass)
	{		
		
		const TObjectPtr<UDragItemVisual> DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);		
		
		DragVisual->IMG_ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());
		DragVisual->TXT_ItemQuantity->SetText(FText::AsNumber(ItemReference->ItemQuantity));
		
		// checking if the item is not stackable then dont show the item quantity
		ItemReference->NumericData.bIsStackable
		? DragVisual->TXT_ItemQuantity->SetText(FText::AsNumber(ItemReference->ItemQuantity))
		: DragVisual->TXT_ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
			

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;
		DragItemOperation->DefaultDragVisual = DragVisual;

		// to tell where to snap the cursor with the item visual
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation;
	}
}

void UInventoryItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	OnMouseEnterDelegate.Broadcast(this);
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	OnMouseLeaveDelegate.Broadcast(this);
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
