#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryGridDataStruct.generated.h"

USTRUCT()
struct FGridSizeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 GridColumns;
	
	UPROPERTY(EditAnywhere)
	int32 GridRows;

	UPROPERTY(EditAnywhere)
	float GridTileSize;	
};

USTRUCT()
struct FGridData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "GridData")
	FGridSizeData GridSizeData;
};
