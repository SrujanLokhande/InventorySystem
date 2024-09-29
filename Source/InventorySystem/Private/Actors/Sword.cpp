// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Sword.h"


// Sets default values
ASword::ASword()
{	
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tablet Mesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}
