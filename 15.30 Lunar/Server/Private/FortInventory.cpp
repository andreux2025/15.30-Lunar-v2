#include "../Public/FortInventory.h"

void FortInventory::GiveItem(AFortPlayerController* PlayerController, UFortItemDefinition* ItemDefintion, int Count, int LoadedAmmo, bool bStack, bool bShowToast) {
	auto Item = Cast<UFortWorldItem>(ItemDefintion->CreateTemporaryItemInstanceBP(Count, 1));
	if (!bStack) {
		Item->OwnerInventory = PlayerController->WorldInventory;
		Item->SetOwningControllerForTemporaryItem(PlayerController);

		Item->ItemEntry.LoadedAmmo = LoadedAmmo;
		Item->ItemEntry.Count = Count;
		Item->ItemEntry.ItemDefinition = ItemDefintion;

		if (bShowToast) {
			FFortItemEntryStateValue StateValue{};
			StateValue.IntValue = 1;
			StateValue.StateType = EFortItemEntryState::ShouldShowItemToast;
			Item->ItemEntry.StateValues.Add(StateValue);
		}

		PlayerController->WorldInventory->Inventory.ItemInstances.Add(Item);
		PlayerController->WorldInventory->Inventory.ReplicatedEntries.Add(Item->ItemEntry);
		PlayerController->WorldInventory->Inventory.MarkItemDirty(Item->ItemEntry);
		PlayerController->WorldInventory->HandleInventoryLocalUpdate();
	}
	else {
		bool bFound = false;

		for (auto& ItemEntry : PlayerController->WorldInventory->Inventory.ReplicatedEntries) {
			if (ItemEntry.ItemDefinition == ItemDefintion) {
				bFound = true;

				ItemEntry.Count += Count;

				if (bShowToast) {
					FFortItemEntryStateValue StateValue{};
					StateValue.IntValue = 1;
					StateValue.StateType = EFortItemEntryState::ShouldShowItemToast;
					Item->ItemEntry.StateValues.Add(StateValue);
				}

				for (int i = 0; i < PlayerController->WorldInventory->Inventory.ReplicatedEntries.Num(); i++) {
					PlayerController->WorldInventory->Inventory.ReplicatedEntries[i].LoadedAmmo = ItemEntry.LoadedAmmo;
					PlayerController->WorldInventory->Inventory.ReplicatedEntries[i].Count = ItemEntry.Count;
					break;
				}

				PlayerController->WorldInventory->Inventory.MarkItemDirty(ItemEntry);
				PlayerController->WorldInventory->HandleInventoryLocalUpdate();
			}
		}

		if (!bFound) {
			GiveItem(PlayerController, ItemDefintion, Count, LoadedAmmo, false, bShowToast);
		}
	}
}

FFortItemEntry* FortInventory::FindItemEntry(AFortPlayerController* PlayerController, UFortItemDefinition* ItemDefinition) {
	for (auto& ItemEntry : PlayerController->WorldInventory->Inventory.ReplicatedEntries) {
		if (ItemEntry.ItemDefinition == ItemDefinition) {
			return &ItemEntry;
		}
	}

	return nullptr;
}

FFortItemEntry* FortInventory::FindItemEntry(AFortPlayerController* PlayerController, FGuid* Guid) {
	for (auto& ItemEntry : PlayerController->WorldInventory->Inventory.ReplicatedEntries) {
		if (ItemEntry.ItemGuid == *Guid) {
			return &ItemEntry;
		}
	}

	return nullptr;
}

void FortInventory::RemoveItem(AFortPlayerController* PlayerController, FGuid Guid, int Count) {
	if (!PlayerController || !PlayerController->WorldInventory) return;

	auto& Inventory = PlayerController->WorldInventory->Inventory;
	auto& ReplicatedEntries = Inventory.ReplicatedEntries;

	bool ItemRemoved = false;

	for (int32 i = 0; i < ReplicatedEntries.Num(); ++i) {
		auto& Entry = ReplicatedEntries[i];

		if (Entry.ItemGuid == Guid) {
			Entry.Count = (Entry.Count > Count) ? (Entry.Count - Count) : 0;

			Inventory.MarkItemDirty(Entry);
			PlayerController->WorldInventory->HandleInventoryLocalUpdate();

			if (Entry.Count == 0) {
				ReplicatedEntries.Remove(i);
				ItemRemoved = true;
			}
			else {
				for (auto& OtherEntry : ReplicatedEntries) {
					OtherEntry.LoadedAmmo = Entry.LoadedAmmo;
				}
			}
			break;
		}
	}

	if (ItemRemoved) {
		for (int32 i = 0; i < Inventory.ItemInstances.Num(); ++i) {
			if (Inventory.ItemInstances[i]->GetItemGuid() == Guid) {
				Inventory.ItemInstances.Remove(i);
				break;
			}
		}

		Inventory.MarkArrayDirty();
	}

	PlayerController->WorldInventory->bRequiresLocalUpdate = true;
	PlayerController->WorldInventory->HandleInventoryLocalUpdate();
}

void FortInventory::RemoveAllDroppableItems(AFortPlayerControllerAthena* PlayerController) {
	if (!PlayerController)
		return;

	for (size_t i = 0; i < PlayerController->WorldInventory->Inventory.ItemInstances.Num(); i++) {
		if (PlayerController->WorldInventory->Inventory.ItemInstances[i]->CanBeDropped()) {
			PlayerController->WorldInventory->Inventory.ItemInstances.Remove(i);
			PlayerController->WorldInventory->Inventory.ReplicatedEntries.Remove(i);
			PlayerController->WorldInventory->Inventory.MarkArrayDirty();
		}
	}
}

void FortInventory::UpdateInventory(AFortPlayerController* PC, FFortItemEntry& Entry)
{
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ItemInstances.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.ItemGuid == Entry.ItemGuid)
		{
			PC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry = Entry;
			break;
		}
	}
}

void FortInventory::GiveItemStack(AFortPlayerController* PC, UFortItemDefinition* Def, int Count, int LoadedAmmo)
{
	EEvaluateCurveTableResult Result;
	float OutXY = 0;
	UDataTableFunctionLibrary::EvaluateCurveTableRow(Def->MaxStackSize.Curve.CurveTable, Def->MaxStackSize.Curve.RowName, 0, &Result, &OutXY, FString());
	if (!Def->MaxStackSize.Curve.CurveTable || OutXY <= 0)
		OutXY = Def->MaxStackSize.Value;;
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
			if (PC->WorldInventory->Inventory.ReplicatedEntries[i].StateValues[0].IntValue)
				PC->WorldInventory->Inventory.ReplicatedEntries[i].StateValues[0].IntValue = false;
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


EFortQuickBars FortInventory::GetQuickbar(UFortItemDefinition* ItemDef)
{
	if (!ItemDef) return EFortQuickBars::Primary;

	return (ItemDef->IsA(UFortWeaponMeleeItemDefinition::StaticClass()) ||
		ItemDef->IsA(UFortResourceItemDefinition::StaticClass()) ||
		ItemDef->IsA(UFortAmmoItemDefinition::StaticClass()) ||
		ItemDef->IsA(UFortTrapItemDefinition::StaticClass()) ||
		ItemDef->IsA(UFortBuildingItemDefinition::StaticClass()) ||
		ItemDef->IsA(UFortEditToolItemDefinition::StaticClass()))
		? EFortQuickBars::Secondary
		: EFortQuickBars::Primary;
}

void FortInventory::RemoveItem22(AFortPlayerController* PC, UFortItemDefinition* Def, int Count)
{
	bool Remove = false;
	FGuid guid;
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		auto& Entry = PC->WorldInventory->Inventory.ReplicatedEntries[i];
		if (Entry.ItemDefinition == Def)
		{
			Entry.Count -= Count;
			if (Entry.Count <= 0)
			{
				//if (Def->IsA(UFortGadgetItemDefinition::StaticClass())) reinterpret_cast<bool(*)(UFortGadgetItemDefinition*, IFortInventoryOwnerInterface*, UFortItem*)>(__int64(GetModuleHandleW(0)) + 0x2AF2EA0)((UFortGadgetItemDefinition*)Def, reinterpret_cast<IFortInventoryOwnerInterface * (*)(AFortPlayerController*, UClass*)>(__int64(GetModuleHandleW(0)) + 0x3AD9490)(PC, IFortInventoryOwnerInterface::StaticClass()), PC->WorldInventory->Inventory.ItemInstances[i]);
				PC->WorldInventory->Inventory.ReplicatedEntries[i].StateValues.Free();
				PC->WorldInventory->Inventory.ReplicatedEntries.Remove(i);
				Remove = true;
				guid = Entry.ItemGuid;
			}
			else
			{
				PC->WorldInventory->Inventory.MarkItemDirty(PC->WorldInventory->Inventory.ReplicatedEntries[i]);
				FortInventory::UpdateInventory(PC, Entry);
				PC->WorldInventory->HandleInventoryLocalUpdate();
				return;
			}
			break;
		}
	}

	if (Remove)
	{
		for (size_t i = 0; i < PC->WorldInventory->Inventory.ItemInstances.Num(); i++)
		{
			if (PC->WorldInventory->Inventory.ItemInstances[i]->GetItemGuid() == guid)
			{
				PC->WorldInventory->Inventory.ItemInstances.Remove(i);
				break;
			}
		}
	}

	PC->WorldInventory->Inventory.MarkArrayDirty();
	PC->WorldInventory->HandleInventoryLocalUpdate();
}

void FortInventory::TriggerInventoryUpdate(AFortPlayerController* PC, FFortItemEntry* Entry)
{
	AFortInventory* Inventory;
	if (auto Bot = Cast<AFortAthenaAIBotController>(PC)) {
		Inventory = Bot->Inventory;
	}
	else {
		Inventory = PC->WorldInventory;
	}
	if (!PC) return;
	Inventory->bRequiresLocalUpdate = true;
	Inventory->HandleInventoryLocalUpdate();

	return Entry ? Inventory->Inventory.MarkItemDirty(*Entry) : Inventory->Inventory.MarkArrayDirty();
}

FFortItemEntry* FortInventory::FindEntry22(AFortPlayerController* PC, FGuid Guid)
{
	for (auto& Entry : PC->WorldInventory->Inventory.ReplicatedEntries)
	{
		if (Entry.ItemGuid == Guid)
		{
			return &Entry;
		}
	}
	return nullptr;
}

FFortItemEntry* FortInventory::FindEntry22(AFortPlayerController* PC, UFortItemDefinition* Def)
{
	for (auto& Entry : PC->WorldInventory->Inventory.ReplicatedEntries)
	{
		if (Entry.ItemDefinition == Def)
		{
			return &Entry;
		}
	}
	return nullptr;
}


void FortInventory::UpdateLoadedAmmo(AFortPlayerController* PC, AFortWeapon* Weapon)
{
	for (int32 i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == Weapon->ItemEntryGuid)
		{
			PC->WorldInventory->Inventory.ReplicatedEntries[i].LoadedAmmo = Weapon->AmmoCount;
			FortInventory::UpdateInventory((AFortPlayerControllerAthena*)PC, PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			PC->WorldInventory->Inventory.MarkItemDirty(PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			break;
		}
	}
}

void FortInventory::UpdateLoadedAmmo(AFortPlayerController* PC, AFortWeapon* Weapon, int AmountToAdd)
{
	for (int32 i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == Weapon->ItemEntryGuid)
		{
			PC->WorldInventory->Inventory.ReplicatedEntries[i].LoadedAmmo += AmountToAdd;
			FortInventory::UpdateInventory((AFortPlayerControllerAthena*)PC, PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			PC->WorldInventory->Inventory.MarkItemDirty(PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			break;
		}
	}
}