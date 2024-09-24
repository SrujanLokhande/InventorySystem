// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryTooltip.generated.h"

class UItemBase;
class UTextBlock;
class UInventoryItemSlot;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryTooltip : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	UInventoryItemSlot* InventorySlotBeingHovered;

	// this is all gonna show when we hover over an item
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TXT_ItemName;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TXT_Itemtype;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TXT_DamageValue;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TXT_UsageText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TXT_ItemDescription;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TXT_MaxStackSize;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TXT_StackWeight;

	virtual void NativeConstruct() override;		
};
