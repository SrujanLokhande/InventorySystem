// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Tablet.h"

#include "Components/WidgetComponent.h"


// Sets default values
ATablet::ATablet()
{
	PrimaryActorTick.bCanEverTick = false;
	TabletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tablet Mesh"));
	SetRootComponent(TabletMesh);

	TabletMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

	InventoryWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Tablet Widget"));
	InventoryWidget->SetupAttachment(RootComponent);
	InventoryWidget->SetDrawAtDesiredSize(true);
	InventoryWidget->SetWidgetSpace(EWidgetSpace::World);
	InventoryWidget->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

// Called when the game starts or when spawned
void ATablet::BeginPlay()
{
	Super::BeginPlay();
	
}


