// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Tablet.h"

#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UserInterface/MainMenuWidget.h"
#include "UserInterface/Inventory/DragItemVisual.h"
#include "UserInterface/Inventory/InventoryWidget.h"

// Sets default values
ATablet::ATablet()
{
	PrimaryActorTick.bCanEverTick = true;
	
	TabletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tablet Mesh"));
	SetRootComponent(TabletMesh);
	TabletMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

	InventoryWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Tablet Widget"));
	InventoryWidgetComponent->SetupAttachment(RootComponent);
	InventoryWidgetComponent->SetDrawAtDesiredSize(true);
	InventoryWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	InventoryWidgetComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}



