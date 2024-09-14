// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InventorySystemHUD.generated.h"

struct FInteractableData;
class UInteractionWidget;
class UMainMenuWidget;

UCLASS()
class INVENTORYSYSTEM_API AInventorySystemHUD : public AHUD
{
	GENERATED_BODY()

public:	

	//=============================================================================
	// PROPERTIES
	//=============================================================================

	// Main Menu Class
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> MainMenuClass;

	// Interaction Widget Class
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible;

	//=============================================================================
	// FUNCTIONS
	//=============================================================================

	// Sets default values for this actor's properties
	AInventorySystemHUD();

	// void DisplayMenu();
	// void HideMenu();
	// void ToggleMenu();	
	
	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

protected:
	//=============================================================================
	// PROPERTIES
	//=============================================================================

	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;
	
	//=============================================================================
	// FUNCTIONS
	//=============================================================================

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
