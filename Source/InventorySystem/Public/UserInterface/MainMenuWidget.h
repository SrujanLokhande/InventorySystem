// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


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

protected:
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	// when we do drag and drop for the inventory item
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
};