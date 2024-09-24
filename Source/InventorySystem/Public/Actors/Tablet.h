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

// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
// 	UWidgetComponent* DragVisualComponent;
//
// 	UFUNCTION(BlueprintCallable, Category = "Inventory")
// 	void StartItemDrag(UItemBase* ItemToDrag);	
//
// 	UFUNCTION(BlueprintCallable, Category = "Inventory")
// 	void EndItemDrag();
//
// protected:	
// 	virtual void BeginPlay() override;
//
// 	virtual void Tick(float DeltaSeconds) override;
//
// 	
// private:
// 	
// 	UPROPERTY()
// 	UDragItemVisual* DragVisualClass;
//
// 	UPROPERTY()
// 	UInventoryWidget* InventoryWidgetClass;
//
// 	UPROPERTY()
// 	UMainMenuWidget* MainMenuWidgetClass;
// 	
// 	void UpdateDragVisualPosition() const;

};
