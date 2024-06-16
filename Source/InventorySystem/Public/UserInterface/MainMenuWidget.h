// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


class UInventoryWidget;
class AInventorySystemCharacter;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
	AInventorySystemCharacter* PlayerCharacter;

	UFUNCTION()
	void SetTileSize(float InTileSize);

protected:
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	// when we do drag and drop for the inventory item
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

	UPROPERTY(meta=(BindWidget))
	UInventoryWidget* InventoryWidget;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, meta=(ExposeOnSpawn))
	float TileSize = 0.0f;
};
