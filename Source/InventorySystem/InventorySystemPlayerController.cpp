// Copyright Srujan Lokhande @2024


#include "InventorySystemPlayerController.h"


// Sets default values
AInventorySystemPlayerController::AInventorySystemPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInventorySystemPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventorySystemPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

