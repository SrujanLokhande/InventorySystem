// Copyright Srujan Lokhande @2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UItemBase;
// this enum class is for deciding the states for the item pickup
// if the players inventory is full then NoItemAdded, 
// if only some quantity from the world is picked up then PartialAmountAdded
UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No Item Picked Up"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial Amount of Items Picked Up"),
	IAR_AllItemAdded UMETA(DisplayName = "All Items Picked Up")
};

// the struct which uses the Enum to decide the state of the item
USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() : ActualAmountAdded(0), OperationResult(EItemAddResult::IAR_NoItemAdded), ResultMessage(FText::GetEmpty()) {}

	// the actual amount of item added to the inventory after all the checks
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	// declaring the state of the item
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;

	// displaying the message on the UI regarding how many items picked up and all
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	// static functions dont need to have create instances for that class and can be called directly
	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	}
	
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	}
	
	static FItemAddResult AddedAll(const int32 AmountAdded ,const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	//=============================================================================
	// PROPERTIES
	//=============================================================================
	
	// whenever we add or adjust the inventory items we will use this delegate to broadcast the update
	FOnInventoryUpdated OnInventoryUpdated;
	
	//=============================================================================
	// FUNCTIONS
	//=============================================================================	
	UInventoryComponent();

	// compare the item in the world with the one in the memory array
	// these functions are only for searching and returning so thats why they are const
	UFUNCTION(Category = "Inventory")
	UItemBase* FindMatchingItem(UItemBase* ItemIn) const;
	
	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextItemByID(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextPartialStack(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemBase* InputItem);

	// to completely remove the item from inventory
	UFUNCTION(Category = "Inventory")
	void RemoveSingleInstanceOfItem(UItemBase* ItemToRemove);

	// to drop partial amount of item
	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove);

	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(UItemBase* ItemIn, int32 AmountToSplit);

	// Getter functions
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetInventoryTotalWeight() {return InventoryTotalWeight; }

	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() {return InventoryWeightCapacity;}

	UFUNCTION(Category = "Inventory")
	FORCEINLINE int32 GetSlotsCapacity() {return InventorySlotsCapacity;}

	// needed the refernce to the items from inventory to draw on the UI
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const {return InventoryContents;}

	// Setter Functions
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) {InventorySlotsCapacity = NewSlotsCapacity;}

	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) {InventoryWeightCapacity = NewWeightCapacity;}

protected:

	//=============================================================================
	// PROPERTIES
	//=============================================================================
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryTotalWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 InventorySlotsCapacity;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float InventoryWeightCapacity;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents;
	
	//=============================================================================
	// FUNCTIONS
	//=============================================================================
	virtual void BeginPlay() override;

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleNonStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount);

	UFUNCTION(Category = "Inventory")
	int32 HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount);

	UFUNCTION(Category = "Inventory")
	int32 CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount);

	UFUNCTION(Category = "Inventory")
	int32 CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount);

	// to actually add new items
	UFUNCTION(Category = "Inventory")
	void AddNewItem(UItemBase* Item, const int32 AmountToAdd);
};
