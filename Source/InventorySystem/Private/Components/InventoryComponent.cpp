// Copyright Srujan Lokhande @2024

#include "Components/InventoryComponent.h"

#include "Items/ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	
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
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
}


void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem) const
{
}





