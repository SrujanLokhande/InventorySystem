// Copyright Srujan Lokhande @2024
#include "UserInterface/Inventory/InventoryWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"
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
		FVector2D DropPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
		if(CanPlaceItemAt(ItemDragDrop->SourceItem, DropPosition))
		{
			UInventoryItemSlot* ItemSlot = GetItemSlotForItem(ItemDragDrop->SourceItem);
			if(ItemSlot)
			{
				FVector2D SnappedPosition(
					FMath::FloorToInt(DropPosition.X / TileSize) * TileSize,
					FMath::FloorToInt(DropPosition.Y / TileSize) * TileSize);

				UpdateItemPosition(ItemSlot, SnappedPosition);
				ItemSlot->SetVisibility(ESlateVisibility::Visible);

				// if it is, then we return true which will prevent to drop the item on the panel
				return true;				
			}			
		}
		return true;
	}
	return false;
}

//using the Canvas Panel
void UInventoryWidget::RefreshInventory()
{
	if(!InventoryComponentRef || !InventorySlotClass || !GridCanvasPanel) return;

	GridCanvasPanel->ClearChildren();

	int32 GridColumns = InventoryComponentRef->GetGridColumns();
	//int32 Index = 0;
	int32 GridRows = InventoryComponentRef->GetGridRows();

	// Create a 2D array to represent the grid
	TArray<TArray<bool>> OccupiedGrid;
	OccupiedGrid.SetNum(GridRows);

	for(int32 i = 0; i < GridRows; ++i)
	{
		OccupiedGrid[i].SetNum(GridColumns);
		OccupiedGrid[i].Init(false, GridColumns);
	}
	
	for(UItemBase*& Item : InventoryComponentRef->GetInventoryContents())
	{
		UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
		if(ItemSlot)
		{
			ItemSlot->SetItemReference(Item);
			ItemSlot->SetTileSize(TileSize);
			FIntPoint ItemDimensions = Item->GetItemDimensions();
			
			FVector2D SlotSize(ItemDimensions.X * TileSize, ItemDimensions.Y * TileSize);

			// Find the next available position for the item
			FIntPoint Position = FindNextAvailablePosition(OccupiedGrid, ItemDimensions);

			if(Position.X != -1 && Position.Y != -1)
			{
				UCanvasPanelSlot* CanvasPanelSlot = GridCanvasPanel->AddChildToCanvas(ItemSlot);
				if(CanvasPanelSlot)
				{
					CanvasPanelSlot->SetSize(SlotSize);
					FVector2D PositionPixels(Position.X * TileSize, Position.Y * TileSize);
					CanvasPanelSlot->SetPosition(PositionPixels);

					// Mark the tile occupied
					for (int32 Y = Position.Y; Y < Position.Y + ItemDimensions.Y; ++Y)
					{
						for (int32 X = Position.X; X < Position.X + ItemDimensions.X; ++X)
						{
							if(Y < GridRows && X < GridColumns)
							{
								OccupiedGrid[Y][X] = true;
							}
						}
					}
				}
			}
		}
		
	}
	SetInfoText();
	UpdateSlotMap();
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

UInventoryItemSlot* UInventoryWidget::GetItemSlotForItem(UItemBase* ItemBase)
{
	if(ItemToSlotMap.Contains(ItemBase))
	{
		return ItemToSlotMap[ItemBase];
	}
	return nullptr;
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

FIntPoint UInventoryWidget::FindNextAvailablePosition(const TArray<TArray<bool>>& OccupiedGrid,
	const FIntPoint& ItemDimensions)
{
	int32 GridRows = OccupiedGrid.Num();
	int32 GridColumns = OccupiedGrid[0].Num();

	for (int32 Y = 0; Y < GridRows; ++Y)
	{
		for (int32 X = 0; X < GridColumns; ++X)
		{
			if(IsSpaceAvailable(OccupiedGrid, X, Y, ItemDimensions))
			{
				return FIntPoint(X, Y);
			}
		}
	}
	return FIntPoint(-1, -1);
}

bool UInventoryWidget::IsSpaceAvailable(const TArray<TArray<bool>>& OccupiedGrid, int32 StartX, int32 StartY,
	const FIntPoint& ItemDimesions)
{
	int32 GridRows = OccupiedGrid.Num();
	int32 GridColumns = OccupiedGrid[0].Num();

	for (int32 Y = StartY; Y < StartY + ItemDimesions.Y; ++Y)
	{
		for (int32 X = StartX; X < StartX + ItemDimesions.X; ++X)
		{
			if(Y >= GridRows || X >= GridColumns || OccupiedGrid[Y][X])
			{
				return false;
			}
		}
	}
	return true;
}

void UInventoryWidget::UpdateSlotMap()
{
	ItemToSlotMap.Empty();

	for (UWidget* Child : GridCanvasPanel->GetAllChildren())
	{
		UInventoryItemSlot* ItemSlot = Cast<UInventoryItemSlot>(Child);
		if(ItemSlot && ItemSlot->GetItemReference())
		{
			ItemToSlotMap.Add(ItemSlot->GetItemReference(), ItemSlot);
		}
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

bool UInventoryWidget::CanPlaceItemAt(UItemBase* Item, const FVector2D& DropPosition)
{
	if (!Item || !InventoryComponentRef) return false;

	FIntPoint GridPosition(FMath::FloorToInt(DropPosition.X / TileSize), FMath::FloorToInt(DropPosition.Y / TileSize));
	FIntPoint ItemDimensions = Item->GetItemDimensions();

	for (int32 Y = GridPosition.Y; Y < GridPosition.Y + ItemDimensions.Y; ++ Y)
	{
		for (int32 X = GridPosition.X; X < GridPosition.X + ItemDimensions.X; ++X)
		{
			if(X < 0 || Y < 0 || X >= InventoryComponentRef->GetGridColumns() || Y >= InventoryComponentRef->GetGridRows())
			{
				return false;
			}

			// checks if the tile is occupied by another item
			for (const auto& Pair : ItemToSlotMap)
			{
				UItemBase* OtherItem = Pair.Key;
				UInventoryItemSlot* OtherItemSlot = Pair.Value;

				if(OtherItem == Item) continue;

				UCanvasPanelSlot* OtherCanvasPanelSlot = Cast<UCanvasPanelSlot>(OtherItemSlot->Slot);
				if(OtherCanvasPanelSlot)
				{
					FVector2D OtherPosition = OtherCanvasPanelSlot->GetPosition();
					FIntPoint OtherGridPosition(FMath::FloorToInt(OtherPosition.X / TileSize), FMath::FloorToInt(OtherPosition.Y / TileSize));
					FIntPoint OtherDimesions = OtherItem->GetItemDimensions();

					if(X >= OtherGridPosition.X && X < OtherGridPosition.X + OtherDimesions.X &&
						Y >= OtherGridPosition.Y && Y < OtherGridPosition.Y + OtherDimesions.Y)
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}

void UInventoryWidget::UpdateItemPosition(UInventoryItemSlot* ItemSlot, const FVector2D& NewPosition)
{
	if(!ItemSlot) return;

	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(ItemSlot->Slot);
	if(CanvasPanelSlot)
	{
		CanvasPanelSlot->SetPosition(NewPosition);
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