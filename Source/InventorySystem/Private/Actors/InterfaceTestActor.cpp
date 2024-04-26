// Copyright Srujan Lokhande @2024


#include "Actors/InterfaceTestActor.h"
// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{	
	PrimaryActorTick.bCanEverTick = true;

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(ActorMesh);
}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInterfaceTestActor::BeginFocus()
{
	if(ActorMesh)
	{
		// this creates a layer on the top of the mesh and applies the material
		// which we provide on the top of the mesh
		// setting it true displays it
		// using this to create the white highlighting material to highlight the mesh
		ActorMesh->SetRenderCustomDepth(true);
	}
}

void AInterfaceTestActor::EndFocus()
{
	if(ActorMesh)
	{
		ActorMesh->SetRenderCustomDepth(false);
	}
}

void AInterfaceTestActor::BeginInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling Begin Interact on Test Actor"));
}

void AInterfaceTestActor::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling End Interact on Test Actor"));
}

void AInterfaceTestActor::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling Interact on Test Actor"));
}

