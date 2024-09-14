// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UItemBase;
class UInventoryTooltip;
class UGridPanel;
class UInventoryItemSlot;
class UInventoryComponent;
class AInventorySystemCharacter;
class UTextBlock;
class UWrapBox;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// force a refresh if we need to
	UFUNCTION()
	void RefreshInventory();

	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryWrapBox;

	UPROPERTY(meta=(BindWidget))
	UInventoryTooltip* ToolTip;

	// UPROPERTY(meta=(BindWidget))
	// UGridPanel* InventoryGridPanel;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TXT_WeightInfo;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TXT_CapacityInfo;

	UPROPERTY()
	AInventorySystemCharacter* PlayerCharacterRef;

	UPROPERTY()
	UInventoryComponent* InventoryComponentRef;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;

	void UpdateToolTip(UItemBase* ItemToDisplay);
	void HideToolTip();
	
	UFUNCTION()
	void OnItemSlotMouseEnter(UInventoryItemSlot* ItemSlot);

	UFUNCTION()
	void OnItemSlotMouseLeave(UInventoryItemSlot* ItemSlot);

protected:

	// setting the info text to the UI
	void SetInfoText() const;

	// constructor
	virtual void NativeOnInitialized() override;

	// when something is dropped on the UI
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

};
