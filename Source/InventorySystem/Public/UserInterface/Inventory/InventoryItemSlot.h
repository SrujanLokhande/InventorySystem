// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/ItemBase.h"
#include "InventoryItemSlot.generated.h"

class UInventoryItemSlot;
class UImage;
class UBorder;
class UInventoryTooltip;
class UDragItemVisual;
class UItemBase;
class UTextBlock;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSlotMouseEvent, UInventoryItemSlot*, ItemSlot);
UCLASS()
class INVENTORYSYSTEM_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	FORCEINLINE void SetItemReference(UItemBase* ItemIn) { ItemReference = ItemIn; }
	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; }

	UPROPERTY(BlueprintAssignable, Category = "ItemSlotMouseEvents")
	FOnItemSlotMouseEvent OnMouseEnterDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "ItemSlotMouseEvents")
	FOnItemSlotMouseEvent OnMouseLeaveDelegate;	
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UInventoryTooltip> ToolTipClass;

	UPROPERTY(VisibleAnywhere ,Category = "Inventory Slot")
	UItemBase* ItemReference;

	UPROPERTY()
	UInventoryTooltip* ToolTipUI;

	// for the square shaped Item Image inside the inventory panel
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta=(BindWidget))
	UBorder* ItemBorder;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta=(BindWidget))
	UImage* IMG_ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta=(BindWidget))
	UTextBlock* TXT_ItemQuantity;	

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	//void UpdateToolTipPosition(const FPointerEvent& InMouseEvent);
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
	
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	// virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	// 	UDragDropOperation* InOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	//FVector2D LastMousePosition;
};


