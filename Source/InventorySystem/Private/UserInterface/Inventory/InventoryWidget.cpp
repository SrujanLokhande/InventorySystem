// Copyright Srujan Lokhande @2024
#include "UserInterface/Inventory/InventoryWidget.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
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
			// this is the method to call non-dynamic delegate
			InventoryComponentRef->OnInventoryUpdated.AddUObject(this, &ThisClass::RefreshInventory);
			SetInfoText();			
		}
	}
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	// detecting if the drop operation is happening on the same panel i.e. on the inventory panel
	if(ItemDragDrop->SourceInventory && InventoryComponentRef)
	{
		// if it is, then we return true which will prevent to drop the item on the panel
		return true;
	}
	return false;
}

// using the Wrap Box
// void UInventoryWidget::RefreshInventory()
// {
// 	if(InventoryComponentRef && InventorySlotClass)
// 	{
// 		// to get the most updated inventory we clear the previous inventory
// 		InventoryWrapBox->ClearChildren();
//
// 		// iterating through all the Inventory items in the array and creates a slot of each item in the inventory Panel
// 		for (UItemBase* const& InventoryItem : InventoryComponentRef->GetInventoryContents())
// 		{
// 			UInventoryItemSlot* ItemSlot =  CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
// 			ItemSlot->SetItemReference(InventoryItem);
//
// 			// this creates and adds the ItemSlot in the Inventory Panel
// 			InventoryWrapBox->AddChildToWrapBox(ItemSlot);
// 		}
//
// 		SetInfoText();
// 	}
// }

// using the GridPanel
// void UInventoryWidget::RefreshInventory()
// {
// 	if(InventoryComponentRef && InventorySlotClass)
// 	{
// 		// Clear the previous inventory
// 		InventoryGridPanel->ClearChildren();
//
// 		int32 Column = 0;
// 		int32 Row = 0;
//
// 		// Iterating through all the Inventory items in the array
// 		for (UItemBase* const& InventoryItem : InventoryComponentRef->GetInventoryContents())
// 		{
// 			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
// 			ItemSlot->SetItemReference(InventoryItem);
//
// 			// Create and add the ItemSlot to the Inventory Panel
// 			UGridSlot* GridSlot = InventoryGridPanel->AddChildToGrid(ItemSlot);
// 			GridSlot->SetRow(Row);
// 			GridSlot->SetColumn(Column);
//
// 			// Move to the next column and wrap to the next row if necessary
// 			++Column;
// 			if (constexpr int32 MaxColumns = 6; Column >= MaxColumns)
// 			{
// 				Column = 0;
// 				++Row;
// 			}
// 		}
//
// 		SetInfoText();
// 	}
// }

//using the Canvas Panel
void UInventoryWidget::RefreshInventory()
{
	if(InventoryComponentRef && InventorySlotClass)
	{		

		SetInfoText();
	}
}

void UInventoryWidget::InitializeGrid(float InTileSize)
{
	if(!InventoryComponentRef) return;
	TileSize = InTileSize;

	// Sets the Grid border Size
	SetGridBorderSize();

	// Creates the Grid line segment inside the Canvas Panel
	CreateLineSegment();
}

void UInventoryWidget::SetGridBorderSize()
{
	if(!GridBorder) return;
	if(UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder))
	{
		FVector2D NewSize;
		NewSize.X = InventoryComponentRef->GetGridColumns() * TileSize;
		NewSize.Y = InventoryComponentRef->GetGridRows() * TileSize;
		CanvasPanelSlot->SetSize(NewSize);
	}
}

void UInventoryWidget::CreateLineSegment()
{	
	int32 Columns = InventoryComponentRef->GetGridColumns();
	int32 Rows = InventoryComponentRef->GetGridRows();
	double TileSizeLocal = this->TileSize;

	// Create Vertical Lines
	for (int32 i = 0; i <= Columns; ++i)
	{
		double X = i * TileSizeLocal;

		FLine GridLine;
		GridLine.Start = FVector2D(X, 0.0);
		GridLine.End = FVector2D(X, Rows * TileSizeLocal);

		Lines.Add(GridLine);
	}

	// Create Horizontal Lines
	for (int32 i = 0; i <= Columns; ++i)
	{
		double Y = i * TileSizeLocal;

		FLine GridLine;
		GridLine.Start = FVector2D(0.0, Y);
		GridLine.End = FVector2D(Columns * TileSizeLocal, Y);

		Lines.Add(GridLine);
	}
	
}
// The function which lets us draw lines on the canvas panel
int32 UInventoryWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	LayerId =  Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);

	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	if(GridBorder)
	{
		const FGeometry& Geometry = GridBorder->GetCachedGeometry();
		FVector2D TopLeft = USlateBlueprintLibrary::GetLocalTopLeft(Geometry);
		FLinearColor LinearColor = FLinearColor(0.5f, 0.5, 0.05f, 0.5f);
		float Thickness = 1.0f;

		for(const FLine& Line : Lines)
		{
			FVector2D PositionA = Line.Start + TopLeft;
			FVector2D PositionB = Line.End + TopLeft;

			UWidgetBlueprintLibrary::DrawLine(Context, PositionA, PositionB, LinearColor, true, Thickness);
		}
	}
	return LayerId;
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


