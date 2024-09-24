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

	// DragVisualComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Drag Visual Widget"));
	// DragVisualComponent->SetupAttachment(RootComponent);
	// //DragVisualComponent->SetVisibility(false);
	// DragVisualComponent->SetDrawAtDesiredSize(true);
	// DragVisualComponent->SetWidgetSpace(EWidgetSpace::World);
	// DragVisualComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

// Called when the game starts or when spawned
// void ATablet::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	if (InventoryWidgetComponent)
// 	{
// 		MainMenuWidgetClass = Cast<UMainMenuWidget>(InventoryWidgetComponent->GetUserWidgetObject());
// 		if (MainMenuWidgetClass)
// 		{
// 			InventoryWidgetClass = MainMenuWidgetClass->GetInventoryWidgetClass();			
// 			InventoryWidgetClass->SetTabletReference(this);
// 			UE_LOG(LogTemp, Log, TEXT("Tablet: InventoryWidget found and TabletReference set"));
// 		}
// 		else
// 		{
// 			UE_LOG(LogTemp, Error, TEXT("Tablet: Failed to cast UserWidget to InventoryWidget"));
// 		}	
// 	}
//
// 	if (DragVisualComponent)
// 	{
// 		DragVisualClass = Cast<UDragItemVisual>(DragVisualComponent->GetUserWidgetObject());
// 	}
// }
//
// void ATablet::Tick(float DeltaSeconds)
// {
// 	Super::Tick(DeltaSeconds);
//
// 	if (DragVisualComponent->IsVisible())
// 	{
// 		UpdateDragVisualPosition();
// 	}
// }
//
// void ATablet::UpdateDragVisualPosition() const
// {
// 	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
// 	{
// 		if (FVector2D MousePosition; PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
// 		{
// 			FVector WorldDirection;
// 			if (FVector WorldPosition; UGameplayStatics::DeprojectScreenToWorld(PlayerController, MousePosition, WorldPosition, WorldDirection))
// 			{
// 				FVector Start = WorldPosition;
// 				FVector End = WorldPosition + WorldDirection * 1000.0f;
//
// 				FHitResult HitResult;
// 				FCollisionQueryParams QueryParams;
// 				QueryParams.AddIgnoredActor(this);
//
// 				if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
// 				{
// 					DragVisualComponent->SetWorldLocation(HitResult.Location);
// 					DragVisualComponent->SetWorldRotation(HitResult.Normal.Rotation());
// 				}
// 			}
// 		}
// 	}
// }
//
// void ATablet::StartItemDrag(UItemBase* ItemToDrag)
// {
// 	if (DragVisualClass && ItemToDrag)
// 	{
// 		DragVisualClass->SetItemReference(ItemToDrag);
// 		DragVisualComponent->SetVisibility(true);
// 		
// 	}
// }
//
// void ATablet::EndItemDrag()
// {
// 	if (DragVisualComponent)
// 	{
// 		DragVisualComponent->SetVisibility(false);
// 	}
// }



