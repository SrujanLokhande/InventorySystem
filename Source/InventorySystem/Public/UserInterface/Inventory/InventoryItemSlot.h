// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/ItemBase.h"
#include "InventoryItemSlot.generated.h"

class UImage;
class UBorder;
class UInventoryTooltip;
class UDragItemVisual;
class UItemBase;
class UTextBlock;
/**
 * 
 */

UCLASS()
class INVENTORYSYSTEM_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	FORCEINLINE void SetItemReference(UItemBase* ItemIn) { ItemReference = ItemIn; }
	FORCEINLINE void SetTileSize(float InTileSize) { TileSize = InTileSize; }
	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; }
	FORCEINLINE int32 GetItemHeight() const { return ItemReference->NumericData.ItemHeight; }
	FORCEINLINE int32 GetItemWidth() const { return ItemReference->NumericData.ItemWidth; }

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UInventoryTooltip> ToolTipClass;

	UPROPERTY(VisibleAnywhere ,Category = "Inventory Slot")
	UItemBase* ItemReference;

	UPROPERTY(VisibleAnywhere ,Category = "Inventory Slot")
	float TileSize;

	// for the square shaped Item Image inside the inventory panel
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta=(BindWidget))
	UBorder* ItemBorder;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta=(BindWidget))
	UImage* IMG_ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta=(BindWidget))
	UTextBlock* TXT_ItemQuantity;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;

	UFUNCTION()
	FVector2D CalculateDragVisualSize() const;
};


