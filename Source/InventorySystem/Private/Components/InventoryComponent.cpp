// Copyright Srujan Lokhande @2024

#include "Components/InventoryComponent.h"
#include "Items/ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Columns = 6.0f;
	Rows = 6.0f;
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

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}





