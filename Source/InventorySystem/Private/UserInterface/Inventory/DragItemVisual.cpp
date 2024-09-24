// Copyright Srujan Lokhande @2024


#include "UserInterface/Inventory/DragItemVisual.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// void UDragItemVisual::NativeConstruct()
// {
// 	Super::NativeConstruct();
//
// 	SetVisibility(ESlateVisibility::HitTestInvisible);
// 	SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
// }
//
// void UDragItemVisual::SetItemReference(UItemBase* ItemIn) const
// {
// 	if (ItemIn && IMG_ItemIcon)
// 	{
// 		IMG_ItemIcon->SetBrushFromTexture(ItemIn->AssetData.Icon);
//         
// 		if (ItemBorder)
// 		{
// 			// Set border color based on item rarity
// 			switch (ItemIn->ItemRarity)
// 			{
// 			case EItemRarity::Common:
// 				ItemBorder->SetBrushColor(FLinearColor::White);
// 				break;
// 			case EItemRarity::Rare:
// 				ItemBorder->SetBrushColor(FLinearColor::Blue);
// 				break;
// 			case EItemRarity::Epic:
// 				ItemBorder->SetBrushColor(FLinearColor::Red);
// 				break;
// 			case EItemRarity::Legendary:
// 				ItemBorder->SetBrushColor(FLinearColor::Yellow);
// 				break;
// 			default:
// 				ItemBorder->SetBrushColor(FLinearColor::White);
// 			}
// 		}
//
// 		if (TXT_ItemQuantity)
// 		{
// 			if (ItemIn->NumericData.bIsStackable)
// 			{
// 				TXT_ItemQuantity->SetText(FText::AsNumber(ItemIn->ItemQuantity));
// 				TXT_ItemQuantity->SetVisibility(ESlateVisibility::Visible);
// 			}
// 			else
// 			{
// 				TXT_ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
// 			}
// 		}
// 	}
// }


