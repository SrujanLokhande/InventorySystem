// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractionInterface.h"
#include "InterfaceTestActor.generated.h"

UCLASS()
class INVENTORYSYSTEM_API AInterfaceTestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInterfaceTestActor();

protected:

	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* ActorMesh;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact() override;
};
