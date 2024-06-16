// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/DataStructure/DS_Line.h"
#include "InventoryWidget.generated.h"

class UCanvasPanel;
class UGridPanel;
class UBorder;
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

	// UPROPERTY(meta=(BindWidget))
	// UGridPanel* InventoryGridPanel;

	// Canvas panel to display the inventory grid
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* GridCanvasPanel;

	// Inventory Grid Border
	UPROPERTY(meta=(BindWidget))
	UBorder* GridBorder;
	
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

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
protected:

	UPROPERTY()
	TArray<FLine> Lines;

	UPROPERTY()
	double TileSize;

	// setting the info text to the UI
	void SetInfoText() const;

	// constructor
	virtual void NativeOnInitialized() override;

	// when something is dropped on the UI
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

	UFUNCTION()
	void CreateLineSegment();

	UFUNCTION()
	void SetGridBorderSize();
	

};
