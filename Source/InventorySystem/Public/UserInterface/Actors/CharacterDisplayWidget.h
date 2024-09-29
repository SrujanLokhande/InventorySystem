// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterDisplayWidget.generated.h"

class AInventorySystemCharacter;
class UImage;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UCharacterDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	
protected:
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

	UPROPERTY(meta=(BindWidget))
	UImage* CharacterImage;

	UPROPERTY()
	AInventorySystemCharacter* PlayerCharacter;


};
