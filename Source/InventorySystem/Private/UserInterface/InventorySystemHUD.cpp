// Copyright Srujan Lokhande @2024


#include "UserInterface/InventorySystemHUD.h"
#include "UserInterface/MainMenuWidget.h"
#include "UserInterface/Interaction/InteractionWidget.h"


// Sets default values
AInventorySystemHUD::AInventorySystemHUD()
{
	
}

// Called when the game starts or when spawned
void AInventorySystemHUD::BeginPlay()
{
	Super::BeginPlay();

	if(MainMenuClass)
	{
		// Here UMainMenu::StaticClass would fail because Static Class only gets the C++ version
		// but the MainMenuClass has the Blueprint version because it is designed in the editor
		MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);

		// the difference between hidden and collapsed is that making it hidden it will still have tasks running in bg
		// collapsed is more performant because it does not have tasks running in bg
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}


void AInventorySystemHUD::DisplayMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AInventorySystemHUD::HideMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AInventorySystemHUD::ShowInteractionWidget()
{
	if(InteractionWidget)
	{		
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AInventorySystemHUD::HideInteractionWidget()
{
	if(InteractionWidget)
	{		
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AInventorySystemHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if(InteractionWidget)
	{
		if(InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		// when you aim at a new interactable
		InteractionWidget->UpdateWidget(InteractableData);
	}
}




