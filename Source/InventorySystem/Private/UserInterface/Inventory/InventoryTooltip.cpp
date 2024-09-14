// Copyright Srujan Lokhande @2024


#include "UserInterface/Inventory/InventoryTooltip.h"

#include "Components/TextBlock.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	// const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();
	//
	// if(ItemBeingHovered)
	// {
	// 	switch (ItemBeingHovered->ItemType)
	// 	{
	// 	case EItemType::Potions:
	// 		TXT_Itemtype->SetText(FText::FromString("Potions"));
	// 		GEngine->AddOnScreenDebugMessage(-199, 5.0f, FColor::Red, TEXT("Inside EItem::Potions"));
	// 		TXT_DamageValue->SetVisibility(ESlateVisibility::Collapsed);			
	// 		break;
	// 	case EItemType::Herbs:
	// 		TXT_Itemtype->SetText(FText::FromString("Herbs"));
	// 		GEngine->AddOnScreenDebugMessage(-169, 5.0f, FColor::Red, TEXT("Inside EItem::Herbs"));
	// 		TXT_DamageValue->SetVisibility(ESlateVisibility::Collapsed);	
	// 		break;
	// 	case EItemType::Weapons:
	// 		TXT_Itemtype->SetText(FText::FromString("Weapons"));
	// 		break;
	// 	case EItemType::QuestItems:
	// 		TXT_Itemtype->SetText(FText::FromString("QuestItems"));
	// 		TXT_DamageValue->SetVisibility(ESlateVisibility::Collapsed);
	// 		break;
	// 	case EItemType::Mundane:
	// 		TXT_Itemtype->SetText(FText::FromString("Mundane Item"));
	// 		TXT_DamageValue->SetVisibility(ESlateVisibility::Collapsed);		
	// 		TXT_UsageText->SetVisibility(ESlateVisibility::Collapsed);
	// 		break;
	// 	
	// 	default: ;		
	// 	}
	//
	// 	TXT_ItemName->SetText(ItemBeingHovered->TextData.Name);
	// 	TXT_DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.DamageValue));
	// 	TXT_UsageText->SetText(ItemBeingHovered->TextData.UsageText);
	// 	TXT_ItemDescription->SetText(ItemBeingHovered->TextData.Description);
	// 	TXT_StackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));
	//
	// 	const FString WeightInfo =
	// 		{"Weight : " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight())};
	//
	// 	TXT_StackWeight->SetText(FText::FromString(WeightInfo));
	//
	// 	if(ItemBeingHovered->NumericData.bIsStackable)
	// 	{
	// 		const FString StackInfo =
	// 			{"Max Stack Size : " + FString::FromInt(ItemBeingHovered->NumericData.maxStackSize)};
	// 	
	// 		TXT_MaxStackSize->SetText(FText::FromString(StackInfo));
	// 	}
	// 	else
	// 	{
	// 		TXT_MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	// 	}
	// }
}

void UInventoryTooltip::UpdateToolTipContents(const UItemBase* ItemToDisplay)
{
	if(!ItemToDisplay) return;
	
	switch (ItemToDisplay->ItemType)
	{
	case EItemType::Potions:
		TXT_Itemtype->SetText(FText::FromString("Potions"));
		TXT_DamageValue->SetVisibility(ESlateVisibility::Collapsed);			
		break;
	case EItemType::Herbs:
		TXT_Itemtype->SetText(FText::FromString("Herbs"));
		TXT_DamageValue->SetVisibility(ESlateVisibility::Collapsed);	
		break;
	case EItemType::Weapons:
		TXT_Itemtype->SetText(FText::FromString("Weapons"));
		break;
	case EItemType::QuestItems:
		TXT_Itemtype->SetText(FText::FromString("QuestItems"));
		TXT_DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Mundane:
		TXT_Itemtype->SetText(FText::FromString("Mundane Item"));
		TXT_DamageValue->SetVisibility(ESlateVisibility::Collapsed);		
		TXT_UsageText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	
	default: ;		
	}

	TXT_ItemName->SetText(ItemToDisplay->TextData.Name);
	TXT_DamageValue->SetText(FText::AsNumber(ItemToDisplay->ItemStatistics.DamageValue));
	TXT_UsageText->SetText(ItemToDisplay->TextData.UsageText);
	TXT_ItemDescription->SetText(ItemToDisplay->TextData.Description);
	TXT_StackWeight->SetText(FText::AsNumber(ItemToDisplay->GetItemStackWeight()));

	const FString WeightInfo =
		{"Weight : " + FString::SanitizeFloat(ItemToDisplay->GetItemStackWeight())};

	TXT_StackWeight->SetText(FText::FromString(WeightInfo));

	if(ItemToDisplay->NumericData.bIsStackable)
	{
		const FString StackInfo =
			{"Max Stack Size : " + FString::FromInt(ItemToDisplay->NumericData.maxStackSize)};
	
		TXT_MaxStackSize->SetText(FText::FromString(StackInfo));
	}
	else
	{
		TXT_MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}
