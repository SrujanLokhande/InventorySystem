// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventorySystemPlayerController.generated.h"

UCLASS()
class INVENTORYSYSTEM_API AInventorySystemPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInventorySystemPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
