#include "../Public/Reloading.h"
#include "../Public/FortInventory.h"

__int64 Weapon::OnReload(AFortWeapon* Weapon, int RemoveCount)
{
	AFortGameModeAthena* GameMode = (AFortGameModeAthena*)UWorld::GetWorld()->AuthorityGameMode;

	auto GameState = (AFortGameStateAthena*)GameMode->GameState;

	auto Ret = OnReloadOG(Weapon, RemoveCount);
	auto WeaponDef = Weapon->WeaponData;
	if (!WeaponDef)
		return Ret;

	auto AmmoDef = WeaponDef->GetAmmoWorldItemDefinition_BP();
	if (!AmmoDef)
		return Ret;

	AFortPlayerPawnAthena* Pawn = (AFortPlayerPawnAthena*)Weapon->GetOwner();
	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Pawn->Controller;
	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
	if (!PC || !PC->Pawn || !PC->IsA(AFortPlayerControllerAthena::StaticClass()) || &PC->WorldInventory->Inventory == nullptr || GameState->GamePhase >= EAthenaGamePhase::EndGame)
		return Ret;

	if (PC->bInfiniteAmmo) {
		FortInventory::UpdateLoadedAmmo(PC, Weapon, RemoveCount);
		return Ret;
	}

	int AmmoCount = 0;
	FFortItemEntry* FoundEntry = nullptr;
	for (int32 i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		FFortItemEntry& Entry = PC->WorldInventory->Inventory.ReplicatedEntries[i];

		if (Entry.ItemDefinition == AmmoDef) {
			AmmoCount = Entry.Count;
			FoundEntry = &Entry;
			break;
		}
	}

	int AmmoToRemove = (RemoveCount < AmmoCount) ? RemoveCount : AmmoCount;

	if (AmmoToRemove > 0) {
		FortInventory::RemoveItem22(PC, AmmoDef, AmmoToRemove);
		FortInventory::UpdateLoadedAmmo(PC, Weapon, AmmoToRemove);
	}

	PC->WorldInventory->bRequiresLocalUpdate = true;
	//PC->WorldInventory->Inventory.MarkItemDirty();
	PC->WorldInventory->HandleInventoryLocalUpdate();

	return Ret;
}


void Weapon::WeaponHook()
{
	MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x2FCA040), OnReload, (LPVOID*)&OnReloadOG);

}