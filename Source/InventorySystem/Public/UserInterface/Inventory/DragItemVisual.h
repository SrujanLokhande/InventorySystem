// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/ItemBase.h"
#include "DragItemVisual.generated.h"

/**
 * 
 */
class UBorder;
class UImage;
class UTextBlock;
UCLASS()
class INVENTORYSYSTEM_API UDragItemVisual : public UUserWidget
{
	GENERATED_BODY()

public:	

	// for the square shaped Item Image inside the inventory panel
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta=(BindWidget))
	UBorder* ItemBorder;
	
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta=(BindWidget))
	UImage* IMG_ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta=(BindWidget))
	UTextBlock* TXT_ItemQuantity;	
};
