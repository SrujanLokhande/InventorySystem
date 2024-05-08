// Copyright Srujan Lokhande @2024


#include "UserInterface/Inventory/InventoryTooltip.h"

#include "Components/TextBlock.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	switch (ItemBeingHovered->ItemType)
	{
	case EItemType::Potions:
		TXT_Itemtype->SetText(FText::FromString("Potions"));
		TXT_DamageValue->SetVisibility(ESlateVisibility::Collapsed);		
			break;
	case EItemType::Herbs:
		break;
	case EItemType::Weapons:
		break;
	case EItemType::QuestItems:
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

	if(ItemBeingHovered->NumericData.bIsStackable)
	{
		TXT_StackSizeText->SetText(FText::AsNumber(ItemBeingHovered->NumericData.maxStackSize));
	}
	else
	{
		TXT_StackSizeText->SetVisibility(ESlateVisibility::Collapsed);
	}
}
