// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

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

	// to make the grid lines of the widget
	UFUNCTION()
	void InitializeGrid(float InTileSize);	

	// UPROPERTY(meta=(BindWidget))
	// UWrapBox* InventoryWrapBox;

	UPROPERTY(meta=(BindWidget))
	UGridPanel* InventoryGridPanel;
	
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

protected:

	// setting the info text to the UI
	void SetInfoText() const;

	// constructor
	virtual void NativeOnInitialized() override;

	// when something is dropped on the UI
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

private:

	UPROPERTY()
	float TileSize;

};
