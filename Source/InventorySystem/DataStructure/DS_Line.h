#pragma once
#include "CoreMinimal.h"
#include "DS_Line.generated.h"

// the columns and rows for the Inventory grid Lines
USTRUCT()
struct FLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector2D Start;
	
	UPROPERTY(EditAnywhere)
	FVector2D End;
	
};
