// Copyright Srujan Lokhande @2024

#include "UserInterface/Inventory/InventoryItemSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/DragItemVisual.h"
#include "UserInterface/Inventory/InventoryTooltip.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"


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

	// for dragging the item actor 
	if(DragItemVisualClass)
	{
		const TObjectPtr<UDragItemVisual> DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);

		FVector2D DragVisualSize = CalculateDragVisualSize();
		if(UCanvasPanelSlot* DragVisualSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(DragVisual->GetRootWidget()))
		{
			DragVisualSlot->SetSize(DragVisualSize);
		}

		if(USizeBox* DragVisualSizeBox = Cast<USizeBox>(DragVisual->GetWidgetFromName("ItemSize")))
		{
			DragVisualSizeBox->SetWidthOverride(DragVisualSize.X);
			DragVisualSizeBox->SetHeightOverride(DragVisualSize.Y);
		}
		if(UCanvasPanelSlot* ImageSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(DragVisual->IMG_ItemIcon))
		{
			ImageSlot->SetSize(DragVisualSize);
		}
		
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

FVector2D UInventoryItemSlot::CalculateDragVisualSize() const
{
	if(ItemReference)
	{
		FIntPoint ItemDimensions = ItemReference->GetItemDimensions();
		return FVector2D(ItemDimensions.X * TileSize, ItemDimensions.Y * TileSize);
	}
	return FVector2D::ZeroVector;
}


