#pragma once
#include "CoreMinimal.h"
#include "DS_Line.generated.h"

USTRUCT()
struct F_Line
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FVector2D Start;

	UPROPERTY(EditAnywhere)
	FVector2D End;
	
};
