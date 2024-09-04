// Copyright Srujan Lokhande @2024


#include "UserInterface/Inventory/InventoryTooltip.h"

#include "Components/TextBlock.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	// switch (ItemBeingHovered->ItemRarity) {
	// 	case EItemRarity::Common:
	// 		TXT_It
	// 		break;
	// case EItemRarity::Rare:
	// 	break;
	// case EItemRarity::Epic:
	// 	break;
	// case EItemRarity::Legendary:
	// 	break;
	// default: ;
	// }

	if(ItemBeingHovered)
	{
		switch (ItemBeingHovered->ItemType)
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

		TXT_ItemName->SetText(ItemBeingHovered->TextData.Name);
		TXT_DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.DamageValue));
		TXT_UsageText->SetText(ItemBeingHovered->TextData.UsageText);
		TXT_ItemDescription->SetText(ItemBeingHovered->TextData.Description);
		TXT_StackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));

		const FString WeightInfo =
			{"Weight : " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight())};

		TXT_StackWeight->SetText(FText::FromString(WeightInfo));

		if(ItemBeingHovered->NumericData.bIsStackable)
		{
			const FString StackInfo =
				{"Max Stack Size : " + FString::FromInt(ItemBeingHovered->NumericData.maxStackSize)};
		
			TXT_MaxStackSize->SetText(FText::FromString(StackInfo));
		}
		else
		{
			TXT_MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
