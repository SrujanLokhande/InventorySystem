#pragma once
#include "CoreMinimal.h"
#include "DS_Tile.generated.h"

USTRUCT()
struct FTile
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	int X;

	UPROPERTY(EditAnywhere)
	int Y;
};
