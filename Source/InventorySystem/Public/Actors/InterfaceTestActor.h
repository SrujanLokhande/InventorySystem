// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "InterfaceTestActor.generated.h"

UCLASS()
class INVENTORYSYSTEM_API AInterfaceTestActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInterfaceTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
