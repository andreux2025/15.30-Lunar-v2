
#pragma once
#include "../../Framework.h"


class FortInventory {
private:
	FortInventory() = default;
public:
	static FFortItemEntry* FindItemEntry(AFortPlayerController* PlayerController, UFortItemDefinition* ItemDefinition);
	static FFortItemEntry* FindItemEntry(AFortPlayerController* PlayerController, FGuid* Guid);
	static void GiveItem(AFortPlayerController* PlayerController, UFortItemDefinition* ItemDefintion, int Count = 1, int LoadedAmmo = 0, bool bStack = false, bool bShowToast = true);
	static void RemoveItem(AFortPlayerController* PlayerController, FGuid Guid, int Count);
	static void RemoveAllDroppableItems(AFortPlayerControllerAthena* PlayerController);
	static void UpdateInventory(AFortPlayerController* PC, FFortItemEntry& Entry);
	static void GiveItemStack(AFortPlayerController* PC, UFortItemDefinition* Def, int Count = 1, int LoadedAmmo = 0);
	static EFortQuickBars GetQuickbar(UFortItemDefinition* ItemDef);
	static void RemoveItem22(AFortPlayerController* PC, UFortItemDefinition* Def, int Count);
	static FFortRangedWeaponStats* GetStats(UFortWeaponItemDefinition*);
	static void TriggerInventoryUpdate(AFortPlayerController* PC, FFortItemEntry* Entry);
	static UFortWorldItem* GiveItemLategame(AFortPlayerController*, UFortItemDefinition*, int = 1, int = 0, int = 0, bool = true, bool = true, int = 0);
	static UFortWorldItem* GiveItemLategame(AFortPlayerController*, FFortItemEntry, int = -1, bool = true, bool = true);
	static FFortItemEntry* FindEntry22(AFortPlayerController* PC, FGuid Guid);
	static FFortItemEntry* FindEntry22(AFortPlayerController* PC, UFortItemDefinition* Def);
	static void UpdateLoadedAmmo(AFortPlayerController* PC, AFortWeapon* Weapon);
	static void UpdateLoadedAmmo(AFortPlayerController* PC, AFortWeapon* Weapon, int AmountToAdd);
};


inline void UpdateStack(AFortPlayerController* PC, bool Update, FFortItemEntry* EntryToUpdate = nullptr)
{
	PC->WorldInventory->bRequiresLocalUpdate = true;
	PC->WorldInventory->HandleInventoryLocalUpdate();
	PC->HandleWorldInventoryLocalUpdate();
	PC->ClientForceUpdateQuickbar(EFortQuickBars::Primary);
	PC->ClientForceUpdateQuickbar(EFortQuickBars::Secondary);

	if (Update)
	{

		PC->WorldInventory->Inventory.MarkItemDirty(*EntryToUpdate);
	}
	else
	{
		PC->WorldInventory->Inventory.MarkArrayDirty();
	}
}

inline FFortItemEntry* GiveStack(AFortPlayerControllerAthena* PC, UFortItemDefinition* Def, int Count = 1, bool GiveLoadedAmmo = false, int LoadedAmmo = 0, bool Toast = false)
{
	UFortWorldItem* Item = (UFortWorldItem*)Def->CreateTemporaryItemInstanceBP(Count, 0);

	Item->SetOwningControllerForTemporaryItem(PC);
	Item->OwnerInventory = PC->WorldInventory;
	Item->ItemEntry.ItemDefinition = Def;
	Item->ItemEntry.Count = Count;


	if (GiveLoadedAmmo)
	{
		Item->ItemEntry.LoadedAmmo = LoadedAmmo;
	}
	Item->ItemEntry.ReplicationKey++;

	PC->WorldInventory->Inventory.ReplicatedEntries.Add(Item->ItemEntry);
	PC->WorldInventory->Inventory.ItemInstances.Add(Item);

	UpdateStack(PC, false);
	return &Item->ItemEntry;
}

inline void GiveItemStack(AFortPlayerController* PC, UFortItemDefinition* Def, int Count, int LoadedAmmo)
{
	/*int OutXY = Def->MaxStackSize;*/
	int OutXY = static_cast<int>(UFortScalableFloatUtils::GetValueAtLevel(Def->MaxStackSize, 1.0f));
	FFortItemEntry* Found = nullptr;
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition == Def)
		{
			Found = &PC->WorldInventory->Inventory.ReplicatedEntries[i];
			PC->WorldInventory->Inventory.ReplicatedEntries[i].Count += Count;
			if (PC->WorldInventory->Inventory.ReplicatedEntries[i].Count > OutXY)
			{
				PC->WorldInventory->Inventory.ReplicatedEntries[i].Count = OutXY;
			}

			PC->WorldInventory->Inventory.MarkItemDirty(PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			FortInventory::UpdateInventory(PC, PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			PC->WorldInventory->HandleInventoryLocalUpdate();
			return;
		}
	}

	if (!Found)
	{
		FortInventory::GiveItem(PC, Def, Count, LoadedAmmo);
	}
}

inline EFortQuickBars GetQuickBars(UFortItemDefinition* ItemDefinition)
{
	if (!ItemDefinition->IsA(UFortWeaponMeleeItemDefinition::StaticClass()) && !ItemDefinition->IsA(UFortEditToolItemDefinition::StaticClass()) &&
		!ItemDefinition->IsA(UFortBuildingItemDefinition::StaticClass()) && !ItemDefinition->IsA(UFortAmmoItemDefinition::StaticClass()) && !ItemDefinition->IsA(UFortResourceItemDefinition::StaticClass()) && !ItemDefinition->IsA(UFortTrapItemDefinition::StaticClass()) && !ItemDefinition->IsA(AFortWeaponRangedForVehicle::StaticClass()))
		return EFortQuickBars::Primary;

	return EFortQuickBars::Secondary;
}

inline bool IsPrimaryQuickbar(UFortItemDefinition* Def)
{
	return Def->IsA(UFortConsumableItemDefinition::StaticClass()) || Def->IsA(UFortWeaponRangedItemDefinition::StaticClass()) || Def->IsA(UFortGadgetItemDefinition::StaticClass());
}

inline bool IsInventoryFull(AFortPlayerController* PC)
{
	int ItemNb = 0;
	auto InstancesPtr = &PC->WorldInventory->Inventory.ItemInstances;
	for (int i = 0; i < InstancesPtr->Num(); i++)
	{
		if (InstancesPtr->operator[](i))
		{
			if (GetQuickBars(InstancesPtr->operator[](i)->ItemEntry.ItemDefinition) == EFortQuickBars::Primary)
			{
				ItemNb++;

				if (ItemNb >= 5)
				{
					break;
				}
			}
		}
	}

	return ItemNb >= 5;
}

inline int32 GetMaxStack(UFortItemDefinition* Def)
{
	return static_cast<int32>(UFortScalableFloatUtils::GetValueAtLevel(Def->MaxStackSize, 1.0f));
}