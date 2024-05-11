// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class AInventorySystemCharacter;
struct FInteractableData;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// The Owning player ref
	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	AInventorySystemCharacter* PlayerCharacterRef;

	void UpdateWidget(const FInteractableData* InteractableData) const;

protected:

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* TXT_Name;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* TXT_Action;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* TXT_Quantity;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* TXT_KeyPress;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UProgressBar* BAR_InteractionProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	float CurrentInteractionDuration;

	UFUNCTION(Category = "Interaction Widget | Interactable Data")
	float UpdateInteractionProgress();
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
};


