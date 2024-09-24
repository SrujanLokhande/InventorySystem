// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tablet.generated.h"

class UMainMenuWidget;
class UInventoryWidget;
class UItemBase;
class UDragItemVisual;
class UWidgetComponent;

UCLASS()
class INVENTORYSYSTEM_API ATablet : public AActor
{
	GENERATED_BODY()

public:
	
	ATablet();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TabletMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* InventoryWidgetComponent;

};
