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
	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; }
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UInventoryTooltip> ToolTipClass;

	UPROPERTY(VisibleAnywhere ,Category = "Inventory Slot")
	UItemBase* ItemReference;

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
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
};


