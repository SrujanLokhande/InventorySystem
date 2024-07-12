// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

/**
 * 
 */
class UItemBase;
class UInventoryComponent;
UCLASS()
class INVENTORYSYSTEM_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UItemBase* SourceItem;

	UPROPERTY()
	UInventoryComponent* SourceInventory;

	UPROPERTY()
	bool bWasDragged;
};
