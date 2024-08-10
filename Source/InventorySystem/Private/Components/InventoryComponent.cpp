// Copyright Srujan Lokhande @2024

#include "Components/InventoryComponent.h"

#include "InventorySystem/DataStructure/DS_Tile.h"
#include "InventorySystem/DataStructure/InventoryGridDataStruct.h"
#include "Items/ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeGridData();
	InitializeInventoryArray();	
}

void UInventoryComponent::InitializeGridData()
{
	if(!GridDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grid Data Table Not set"));
		return;
	}

	static const FString Context("Inventory Grid Data Table");
	FGridData* InventoryGridData = GridDataTable->FindRow<FGridData>(FName(TEXT("GridData")), Context);

	if(InventoryGridData)
	{
		GridColumns = InventoryGridData->GridSizeData.GridColumns;
		GridRows = InventoryGridData->GridSizeData.GridRows;
		GridTileSize = InventoryGridData->GridSizeData.GridTileSize;
	}
}

void UInventoryComponent::InitializeInventoryArray()
{
	int32 ArraySize = GridColumns * GridRows;
	UE_LOG(LogTemp, Warning, TEXT("Initializing InventoryContents. GridColumns: %d, GridRows: %d, ArraySize: %d"), GridColumns, GridRows, ArraySize);
	InventoryContents.SetNum(ArraySize);
	UE_LOG(LogTemp, Warning, TEXT("InventoryContents size: %d"), InventoryContents.Num());
}

// this functions just checks if the current item pointer is already in the array or not
UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if(ItemIn)
	{
		if(InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if(ItemIn)
	{
		// checks the item pointer type and returns the item type
		// FindByKey finds the item using the overloaded operator inside the ItemBase class to find the item using the ItemID
		if(const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			// dereferencing it because we have a double pointer layer 
			return *Result;
		}
	}
	return nullptr;
}

// this uses a lambda function, which is a function that is temporarily generated on the run and the results are used
UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	// predicate means to have check for a condition
	// here the condition is that if the InventoryItem ID is same as the Item in the world and it is not fully stacked
	// then return the Result
	if(const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByPredicate([&ItemIn]
		(const UItemBase* InventoryItem)
		{
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsItemFullStack();
		
		}
	))
	{
		return *Result;
	}
	return nullptr;

	
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
	if(WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.maxStackSize - StackableItem->ItemQuantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}


void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->ItemQuantity);
	ItemIn->SetItemQuantity(ItemIn->ItemQuantity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, int32 AmountToSplit)
{
	if(InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		// splitting the exisitng item is just removing the number of items  that we need to split and then
		// add the items again as a new stack
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	// checks if the input item has a valid weight
	if(FMath::IsNearlyZero(ItemIn->GetItemSingleWeight()) || ItemIn->GetItemSingleWeight() < 0)
	{
		// because it is a static function we can call it directly using class name without having
		// to create an instance of it
		return FItemAddResult::AddedNone(FText::Format
			(FText::FromString("Could not add {0} to inventory. Item has invalid weight value"), ItemIn->TextData.Name));
	}

	// if the item has more weight than the inventory capacity
	if(InventoryTotalWeight + ItemIn->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format
			(FText::FromString("Could not add {0} to inventory. Item would overflow weight value"), ItemIn->TextData.Name));
	}

	// adding 1 more item would make the inventory to overflow
	if(InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format
			(FText::FromString("Could not add {0} to inventory. Inventory is full"), ItemIn->TextData.Name));
	}

	AddNewItem(ItemIn, RequestedAddAmount);
	return FItemAddResult::AddedAll(RequestedAddAmount ,FText::Format
			(FText::FromString("Added {0} {1} to the inventory"), RequestedAddAmount ,ItemIn->TextData.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	if(RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount;

	// checking the Item In if we already have it in our stack or not, using the lambda function and item ID
	UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

	// iterating over the Existing items and distributing items over an existing stack
	while (ExistingItemStack)
	{
		// calculate how many of the exisiting item would be needed to make a full stack
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);

		// calculate how many AmountToMakeFullStack can be carried based on the weight capacity
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		// the if and the else if returns when a partial amount is added
		if(WeightLimitAddAmount > 0)
		{
			// adjust the exisiting item quantity in the inventory total weight
			ExistingItemStack->SetItemQuantity(ExistingItemStack->ItemQuantity + WeightLimitAddAmount);
			InventoryTotalWeight += ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount;

			// adjust the count to be distributed
			AmountToDistribute -= WeightLimitAddAmount;

			// the amount of items left on the ground
			ItemIn->SetItemQuantity(AmountToDistribute);

			// if max weight capacity is reached no need to run the loop
			if(InventoryTotalWeight >= InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		
		else if(WeightLimitAddAmount <= 0)
		{
			// if there are still some stacks to distribute
			if(AmountToDistribute != RequestedAddAmount)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}

			return 0;
		}

		// if we have already distributed every item across exisiting stacks
		// this returns if the whole amount of item from ground is added
		if(AmountToDistribute <= 0)
		{
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		// checks if there is still any partial stacks left for that item
		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	// no more incomplete stacks is found, so check if we can create a new stack
	if(InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		// attempt to add as many from the item that can fit in the inventory capacity
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);

		if(WeightLimitAddAmount > 0)
		{
			// if there is still more item to distribute but weight limit is reached
			// add as many as we can
			if(WeightLimitAddAmount < AmountToDistribute)
			{
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetItemQuantity(AmountToDistribute);

				// creating a copy because if we are adding the same item to a stack, the memory address will point
				// to the same item in the inventory as well as on the ground
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);

				return RequestedAddAmount - AmountToDistribute;
			}

			// not creating a copy here because we will be picking up the whole item stack from gorund
			AddNewItem(ItemIn, WeightLimitAddAmount);
			return RequestedAddAmount;
		}
	}

	OnInventoryUpdated.Broadcast();
	return RequestedAddAmount - AmountToDistribute;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem) 
{
	if(GetOwner())
	{
		const int32 InitialRequestedAmount = InputItem->ItemQuantity;

		// if the item is non stackable
		if(!InputItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem, InitialRequestedAmount);
		}

		// if stackable
		const int32 StackableAmountAdded = HandleStackableItems(InputItem ,InitialRequestedAmount);

		if(StackableAmountAdded == InitialRequestedAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAmount ,FText::Format
			(FText::FromString("Added {0} {1} to the inventory"), InitialRequestedAmount ,InputItem->TextData.Name));
		}

		if(StackableAmountAdded < InitialRequestedAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded ,FText::Format
			(FText::FromString("Partial Amount of {0} added to the inventory. Number Added : {1}"),InputItem->TextData.Name, StackableAmountAdded));
		}

		if(StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format
			(FText::FromString("Could not add {0} to inventory. Inventory is full"), InputItem->TextData.Name));
		}
	}

	check(false);
	return FItemAddResult::AddedNone(FText::FromString("Get Owner inside the InventoryComponent not valid"));
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString("Adding New Item"));
	UItemBase* NewItem;

	// if the item is from the world, or is already a copy
	if(Item->bIsCopy || Item->bIsPickup)
	{		
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	// if the item is inside the inventory and we want to split it
	// the item is not from the world, but from somewhere else
	else
	{		
		NewItem = Item->CreateItemCopy();
	}
	NewItem->OwningInventory = this;
	NewItem->ItemQuantity = AmountToAdd;	


	// for the grid inventory, not the inventory array
	if(TryAddItem(NewItem))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Try Add Item is Succesffull"));
		InventoryTotalWeight += NewItem->GetItemStackWeight();	
		OnInventoryUpdated.Broadcast();		
	}
	GEngine->AddOnScreenDebugMessage(-2, 5, FColor::Red, TEXT("Try Add Item is not Succesffull"));
	
	// InventoryContents.Add(NewItem);

}

// Checks if there is space available for the new item in the existing item stack
// and if available adds the item to the specified index
bool UInventoryComponent::TryAddItem(UItemBase* InItem)
{
	if (!IsValid(InItem))
	{
		GEngine->AddOnScreenDebugMessage(-3, 5, FColor::Red, TEXT("Try Add Item Item is NotValid"));
		return false;
	}
	GEngine->AddOnScreenDebugMessage(-20, 5, FColor::Red, TEXT("Passed the valid check"));

	for (int32 i = 0; i <= InventoryContents.Num(); ++i)
	{
		if (bIsRoomAvailable(InItem, i))
		{
			AddItemAt(InItem, i);
			GEngine->AddOnScreenDebugMessage(-4, 5, FColor::Red, TEXT("Try Add Item inside the actual function is running"));
			return true;
		}
	}

	return false;	
}


// returns if there is room available at a specified index and its corresponding tile 
bool UInventoryComponent::bIsRoomAvailable(UItemBase* ItemObject, int32 TopLeftIndex)
{
	bool Result = true;

	ForEachIndex(ItemObject, TopLeftIndex, [this, &Result](const FTile& Tile) {
		if (!IsTileValid(Tile))
		{
			GEngine->AddOnScreenDebugMessage(-5, 5, FColor::Red, TEXT("bRoomAvailable Tile is not valid"));
			Result = false;
			return;
		}

		int32 Index = TileToIndex(Tile);
		
		bool Valid;
		UItemBase* ExistingItem;
		GetItemAtIndex(Index, Valid, ExistingItem);

		if (Valid && ExistingItem)
		{
			GEngine->AddOnScreenDebugMessage(-6, 5, FColor::Red, TEXT("bRoomAvailable, either not valid or already an item is there"));
			Result = false;
		}
	});

	return Result;
}

// actually adds the item at the specified index translated from the tile 
void UInventoryComponent::AddItemAt(UItemBase* ItemObject, int32 TopLeftIndex)
{
	//InventoryContents.Add(InItem);	
	// InventoryTotalWeight += InItem->GetItemStackWeight();	
	// OnInventoryUpdated.Broadcast();

	// if (!ItemObject) return;
	// GEngine->AddOnScreenDebugMessage(-7, 5, FColor::Red, TEXT("Inside Add Item At"));
	//
	// ForEachIndex(ItemObject, TopLeftIndex, [this, ItemObject](const FTile& Tile)
	// {
	// 	int32 Index = TileToIndex(Tile);
	//
	// 	InventoryContents[Index] = ItemObject;
	// });
	//
	// bIsDirty = true;

	if (!ItemObject)
	{
		UE_LOG(LogTemp, Error, TEXT("AddItemAt: ItemObject is null"));
		return;
	}
    
	UE_LOG(LogTemp, Warning, TEXT("AddItemAt: Starting to add item at TopLeftIndex: %d"), TopLeftIndex);
	UE_LOG(LogTemp, Warning, TEXT("Current InventoryContents size: %d"), InventoryContents.Num());
	UE_LOG(LogTemp, Warning, TEXT("GridColumns: %d, GridRows: %d"), GridColumns, GridRows);

	FIntPoint ItemDimensions = ItemObject->GetItemDimensions();
	UE_LOG(LogTemp, Warning, TEXT("Item Dimensions: %s"), *ItemDimensions.ToString());

	FTile TopLeftTile = IndexToTile(TopLeftIndex);
	UE_LOG(LogTemp, Warning, TEXT("TopLeftTile: X=%d, Y=%d"), TopLeftTile.X, TopLeftTile.Y);

	// Check if the item fits within the inventory bounds
	if (TopLeftTile.X + ItemDimensions.X > GridColumns || TopLeftTile.Y + ItemDimensions.Y > GridRows)
	{
		UE_LOG(LogTemp, Error, TEXT("AddItemAt: Item does not fit in inventory at specified position"));
		return;
	}

	// Use the ForEachIndex function
	ForEachIndex(ItemObject, TopLeftIndex, [this, ItemObject](const FTile& Tile) {
		int32 Index = TileToIndex(Tile);
        
		UE_LOG(LogTemp, Warning, TEXT("Processing Tile: X=%d, Y=%d, Calculated Index: %d"), Tile.X, Tile.Y, Index);

		// Add bounds check
		if (Index >= 0 && Index < InventoryContents.Num())
		{
			InventoryContents[Index] = ItemObject;
			UE_LOG(LogTemp, Warning, TEXT("Successfully added item at index: %d"), Index);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AddItemAt: Index out of bounds. Index: %d, Array Size: %d"), Index, InventoryContents.Num());
		}
	});

	// Set IsDirty to true after the loop
	bIsDirty = true;
}

FTile UInventoryComponent::IndexToTile(int32 Index) const
{
	if (GridColumns <= 0)
	{
		// Handle error case, perhaps log a warning
		UE_LOG(LogTemp, Warning, TEXT("Grid Columns very small in the IndexToTile Function"));
		return FTile(0, 0);
	}

	FTile Tile;
	Tile.X = Index % GridColumns;
	Tile.Y = Index / GridColumns;

	return Tile;
}

int32 UInventoryComponent::TileToIndex(const FTile& Tile) const
{	
	UE_LOG(LogTemp, Warning, TEXT("TileToIndex: Input Tile: X=%d, Y=%d"), Tile.X, Tile.Y);
    
	if (Tile.X < 0 || Tile.X >= GridColumns || Tile.Y < 0 || Tile.Y >= GridRows)
	{
		UE_LOG(LogTemp, Error, TEXT("TileToIndex: Tile out of bounds. Tile: (%d, %d), Grid: %d x %d"), Tile.X, Tile.Y, GridColumns, GridRows);
		return INDEX_NONE;
	}
    
	int32 Index = Tile.X + (Tile.Y * GridColumns);
	UE_LOG(LogTemp, Warning, TEXT("TileToIndex: Calculated Index: %d"), Index);
	return Index;
}

void UInventoryComponent::ForEachIndex(UItemBase* ItemObject, int32 TopLeftIndex,
                                       TFunction<void(const FTile&)> LoopBody)
{	
	if(!ItemObject) return;
	GEngine->AddOnScreenDebugMessage(-10, 5, FColor::Red, TEXT("Inside foreachindex"));

	FIntPoint ItemDimensions = ItemObject->GetItemDimensions();
	FTile TopLeftTile = IndexToTile(TopLeftIndex);

	int32 LastIndexX = TopLeftTile.X + (ItemDimensions.X - 1);
	int32 LastIndexY = TopLeftTile.Y + (ItemDimensions.Y - 1);

	for (int32 X = TopLeftTile.X; X <= LastIndexX; ++X)
	{
		for (int32 Y = TopLeftTile.Y; Y <= LastIndexY; ++Y)
		{
			FTile CurrentTile{X, Y};

			// this is the lambda function that takes the tile and iterated over the loop again
			// For each tile the item would occupy, it creates an FTile struct.
			// It then calls the LoopBody lambda function, passing the CurrentTile as an argument.
			LoopBody(CurrentTile);
			GEngine->AddOnScreenDebugMessage(-11, 5, FColor::Red, TEXT("Inside foreach looping through the body"));
		}
	}
}

bool UInventoryComponent::IsTileValid(const FTile& Tile) const
{
	bool bIsXValid = (Tile.X >= 0) && (Tile.X < GridColumns);
	bool bIsYValid = (Tile.Y >= 0) && (Tile.Y < GridRows);

	return bIsXValid && bIsYValid;
}

void UInventoryComponent::GetItemAtIndex(int32 Index, bool& Valid, UItemBase*& ItemObject)
{
	GEngine->AddOnScreenDebugMessage(-12, 5, FColor::Red, TEXT("Inside GetItemATIndex"));
	if(InventoryContents.IsValidIndex(Index))
	{
		GEngine->AddOnScreenDebugMessage(-13, 5, FColor::Red, TEXT("Index is valid"));
		Valid = true;
		ItemObject = InventoryContents[Index];
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-14, 5, FColor::Red, TEXT("Index is not valid"));
		Valid = false;
		ItemObject = nullptr;
	}
}
