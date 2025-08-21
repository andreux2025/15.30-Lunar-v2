#include "../Public/Player.h"
#include "../Public/Abilities.h"
#include "../Public/FortInventory.h"
#include "../Public/xp.h"
#include "../../framework.h"
#include "../Public/LateGame.h"

void UpdatePlayerNumpber(int NumberOfPlayer)
{
	if (NumberOfPlayer > Globals::IPlayerNumber) {
		Globals::IPlayerNumber = NumberOfPlayer;

		HttpClient client;

		// Launcher
		client.getAsync("http://backend-services-lunar.xyz:3551/api/games/edit?id=" + Globals::uuid + "&player=" + std::to_string(Globals::IPlayerNumber));
	}
}

void Player::ServerAcknowledgePossessionHook(AFortPlayerControllerAthena* PlayerController, APawn* P)
{
	if (!PlayerController || !PlayerController->MyFortPawn)
		return;

	AFortGameStateAthena* GameState = Cast<AFortGameStateAthena>(UWorld::GetWorld()->GameState);
	AFortGameModeAthena* GameMode = Cast<AFortGameModeAthena>(UWorld::GetWorld()->AuthorityGameMode);
	if (!GameState || !GameMode)
		return;

	for (auto& StartingItem : GameMode->StartingItems)
	{
		FortInventory::GiveItem(PlayerController, StartingItem.Item, StartingItem.Count, 0, false);
	}
	FortInventory::GiveItem(PlayerController, PlayerController->CosmeticLoadoutPC.Pickaxe->WeaponDefinition, 1, 0, false);

	if (Globals::bLateGame)
	{
		if (PlayerController->MyFortPawn)
		{
			PlayerController->MyFortPawn->SetHealth(100);
			PlayerController->MyFortPawn->SetShield(100);
		}
	}

	static bool bFirst = false;
	if (!bFirst)
	{
		GameState->WarmupCountdownEndTime = UGameplayStatics::GetTimeSeconds(UWorld::GetWorld()) + 120;
		GameMode->WarmupCountdownDuration = 120;
		GameState->WarmupCountdownStartTime = UGameplayStatics::GetTimeSeconds(UWorld::GetWorld());
		GameMode->WarmupCountdownDuration = 120;

		bFirst = true;
	}

	PlayerController->AcknowledgedPawn = P;
}


void Player::ServerLoadingScreenDroppedHook(AFortPlayerControllerAthena* PlayerController)
{
	if (!PlayerController || !PlayerController->PlayerState)
		return;

	AFortPlayerStateAthena* PlayerState = Cast<AFortPlayerStateAthena>(PlayerController->PlayerState);
	if (!PlayerState) return;

	AFortGameStateAthena* GameState = Cast<AFortGameStateAthena>(UWorld::GetWorld()->GameState);
	if (!GameState) return;

	UFortPlaylistAthena* Playlist = GetPlaylist();
	if (!Playlist) return;

	if (PlayerState->AbilitySystemComponent)
		Abilities::GiveDefaultAbilitySet(PlayerState->AbilitySystemComponent);
	else
		printf("PlayerState->AbilitySystemComponent is null!");


	UFortKismetLibrary::UpdatePlayerCustomCharacterPartsVisualization(PlayerState);

	PlayerController->XPComponent->bRegisteredWithQuestManager = true;
	PlayerController->XPComponent->OnRep_bRegisteredWithQuestManager();

	static bool First = false;

	auto test = (UFortControllerComponent_InventoryService*)PlayerController->GetComponentByClass(UFortControllerComponent_InventoryService::StaticClass());

	if (test)
	{
		static auto GoldDef = test->GetDefaultGlobalCurrencyItemDefinition();
		FortInventory::GiveItem(PlayerController, GoldDef, test->GlobalCurrencyData.Currency.Count);
	}
	auto GameMode = Cast<AFortGameModeAthena>(UWorld::GetWorld()->AuthorityGameMode);
	
	auto InvServiceComp = ((UFortControllerComponent_InventoryService*)PlayerController->GetComponentByClass(UFortControllerComponent_InventoryService::StaticClass()));
	FortInventory::GiveItem(PlayerController, InvServiceComp->GetDefaultGlobalCurrencyItemDefinition(), 5000);



	auto QuestManager = PlayerController->GetQuestManager(ESubGame::Athena);
	if (!QuestManager)
		return;

	if (Globals::bLateGame)
	{
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		std::vector<UFortItemDefinition*> Rifles = {
			StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03"),
			StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03")
		};

		std::vector<UFortItemDefinition*> Shotguns = {
			StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03"),
			StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03"),
			StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03")
		};

		std::vector<UFortItemDefinition*> SniperRifles = {
			//StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/LTM/WID_Sniper_NoScope_Athena_SR_Ore_T03.WID_Sniper_NoScope_Athena_SR_Ore_T03"),
			StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_SR_Ore_T03.WID_Sniper_Heavy_Athena_SR_Ore_T03"),
			StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03")
		};

		std::vector<UFortItemDefinition*> Movility = {
			StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Shockwave_Athena_UR_Ore_T03.WID_Launcher_Shockwave_Athena_UR_Ore_T03"),
			//	StaticFindObject<UFortItemDefinition>("/HighTower/Items/Soy/AGID_HighTower_Soy_Boarding.AGID_HighTower_Soy_Boarding"),
				//StaticFindObject<UFortItemDefinition>("/HighTower/Items/Grape/BrambleShield/CoreBR/WID_HighTower_Grape_BrambleShield_CoreBR.WID_HighTower_Grape_BrambleShield_CoreBR")
		};

		std::vector<UFortItemDefinition*> Consumables = {
			StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Flopper/Effective/WID_Athena_Flopper_Effective.WID_Athena_Flopper_Effective"),
			StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall")
		};

		UFortItemDefinition* Wood = StaticFindObject<UFortItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");
		UFortItemDefinition* Stone = StaticFindObject<UFortItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
		UFortItemDefinition* Metal = StaticFindObject<UFortItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");

		UFortItemDefinition* Shells = StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells");
		UFortItemDefinition* Medium = StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium");
		UFortItemDefinition* Light = StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight");
		UFortItemDefinition* Heavy = StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy");
		UFortItemDefinition* RocketAmmo = StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AmmoDataRockets.AmmoDataRockets");

		UFortItemDefinition* ChosenRifle = Rifles[std::rand() % Rifles.size()];
		UFortItemDefinition* ChosenShotgun = Shotguns[std::rand() % Shotguns.size()];
		UFortItemDefinition* ChosenConsumable = Consumables[std::rand() % Consumables.size()];
		UFortItemDefinition* ChosenMovility = Movility[std::rand() % Movility.size()];
		UFortItemDefinition* ChosenSniperRifles = SniperRifles[std::rand() % SniperRifles.size()];

		int ConsumableCount = 3;
		if (ChosenConsumable == StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall"))
		{
			ConsumableCount = 6;
		}

		FortInventory::GiveItem(PlayerController, ChosenRifle, 1, 30, false);
		FortInventory::GiveItem(PlayerController, ChosenShotgun, 1, 5, false);
		FortInventory::GiveItem(PlayerController, Wood, 800, 0, true);
		FortInventory::GiveItem(PlayerController, Stone, 750, 0, true);
		FortInventory::GiveItem(PlayerController, Metal, 750, 0, true);
		FortInventory::GiveItem(PlayerController, Shells, 70, 0, true);
		FortInventory::GiveItem(PlayerController, Medium, 300, 0, true);
		FortInventory::GiveItem(PlayerController, Light, 300, 0, true);
		FortInventory::GiveItem(PlayerController, Heavy, 85, 0, true);
		FortInventory::GiveItem(PlayerController, RocketAmmo, 85, 0, true);
		FortInventory::GiveItem(PlayerController, ChosenConsumable, ConsumableCount, 0, true);
		FortInventory::GiveItem(PlayerController, ChosenMovility, 1, 6, true);
		FortInventory::GiveItem(PlayerController, ChosenSniperRifles, 1, 1, true);

	}



	return ServerLoadingScreenDropped(PlayerController);
}

void Player::ServerCreateBuildingActor(AFortPlayerControllerAthena* PlayerController, FCreateBuildingActorData& CreateBuildingData) {
	if (!PlayerController || PlayerController->IsInAircraft())
		return;

	auto BuildingClass = PlayerController->BroadcastRemoteClientInfo->RemoteBuildableClass.Get();
	if (!BuildingClass) {
		std::cout << "BuildingClass is null" << std::endl;
		return;
	}

	TArray<ABuildingSMActor*> ExistingBuildings;
	char BuildRestrictionFlag;

	if (CantBuild(UWorld::GetWorld(), BuildingClass, CreateBuildingData.BuildLoc, CreateBuildingData.BuildRot, CreateBuildingData.bMirrored, &ExistingBuildings, &BuildRestrictionFlag)) {
		return;
	}

	auto NewBuilding = BuildingSpawnActor<ABuildingSMActor>(CreateBuildingData.BuildLoc, CreateBuildingData.BuildRot, BuildingClass);
	if (!NewBuilding) {
		std::cout << "Failed to spawn NewBuilding" << std::endl;
		return;
	}

	NewBuilding->bPlayerPlaced = true;

	auto PlayerState = Cast<AFortPlayerStateAthena>(PlayerController->PlayerState);
	NewBuilding->TeamIndex = PlayerState->TeamIndex;
	NewBuilding->Team = static_cast<EFortTeam>(PlayerState->TeamIndex);
	NewBuilding->OnRep_Team();
	NewBuilding->InitializeKismetSpawnedBuildingActor(NewBuilding, PlayerController, true);

	for (auto& Building : ExistingBuildings) {
		Building->K2_DestroyActor();
	}
	ExistingBuildings.Free();

	auto ItemDefinition = UFortKismetLibrary::K2_GetResourceItemDefinition(NewBuilding->ResourceType);
	auto ItemEntry = FortInventory::FindItemEntry(PlayerController, ItemDefinition);

	if (ItemEntry && ItemEntry->Count >= 10) {
		FortInventory::RemoveItem(PlayerController, ItemEntry->ItemGuid, 10);
	}
	else {
		std::cout << "Not enough resources or ItemEntry is null." << std::endl;
		NewBuilding->K2_DestroyActor();
	}
}

void Player::ServerEndEditingBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActorToEdit)
{
	if (!BuildingActorToEdit || !PC->MyFortPawn || BuildingActorToEdit->bDestroyed == 1 || BuildingActorToEdit->EditingPlayer != PC->PlayerState)
		return;
	BuildingActorToEdit->SetNetDormancy(ENetDormancy::DORM_DormantAll);
	BuildingActorToEdit->EditingPlayer = nullptr;
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ItemInstances.Num(); i++)
	{
		auto Item = PC->WorldInventory->Inventory.ItemInstances[i];
		if (Item->GetItemDefinitionBP()->IsA(UFortEditToolItemDefinition::StaticClass()))
		{
			PC->MyFortPawn->EquipWeaponDefinition((UFortWeaponItemDefinition*)Item->GetItemDefinitionBP(), Item->GetItemGuid(), Item->GetTrackerGuid());
			break;
		}
	}
	if (!PC->MyFortPawn->CurrentWeapon || !PC->MyFortPawn->CurrentWeapon->WeaponData || !PC->MyFortPawn->CurrentWeapon->IsA(AFortWeap_EditingTool::StaticClass()))
		return;

	AFortWeap_EditingTool* EditTool = (AFortWeap_EditingTool*)PC->MyFortPawn->CurrentWeapon;

	EditTool->EditActor = nullptr;
	EditTool->OnRep_EditActor();
}

static ABuildingSMActor* (*ReplaceBuildingActor)(ABuildingSMActor* BuildingSMActor, unsigned int a2, UObject* a3, unsigned int a4, int a5, bool bMirrored, AFortPlayerControllerAthena* PC) = decltype(ReplaceBuildingActor)(__int64(GetModuleHandleW(0)) + 0x2429a70);

void Player::ServerEditBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, uint8 RotationIterations, bool bMirrored)
{
	if (!BuildingActorToEdit || BuildingActorToEdit->EditingPlayer != PC->PlayerState || !NewBuildingClass.Get() || BuildingActorToEdit->bDestroyed == 1)
		return;

	BuildingActorToEdit->SetNetDormancy(ENetDormancy::DORM_DormantAll);
	BuildingActorToEdit->EditingPlayer = nullptr;
	ABuildingSMActor* NewActor = ReplaceBuildingActor(BuildingActorToEdit, 1, NewBuildingClass.Get(), 0, RotationIterations, bMirrored, PC);
	if (NewActor)
		NewActor->bPlayerPlaced = true;
}

void Player::ServerBeginEditingBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActorToEdit)
{
	if (!BuildingActorToEdit || !BuildingActorToEdit->bPlayerPlaced || !PC->MyFortPawn)
		return;

	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
	BuildingActorToEdit->SetNetDormancy(ENetDormancy::DORM_Awake);
	BuildingActorToEdit->EditingPlayer = PlayerState;
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ItemInstances.Num(); i++)
	{
		auto Item = PC->WorldInventory->Inventory.ItemInstances[i];
		if (Item->GetItemDefinitionBP()->IsA(UFortEditToolItemDefinition::StaticClass()))
		{
			PC->MyFortPawn->EquipWeaponDefinition((UFortWeaponItemDefinition*)Item->GetItemDefinitionBP(), Item->GetItemGuid(), FGuid());
			break;
		}
	}
	if (!PC->MyFortPawn->CurrentWeapon || !PC->MyFortPawn->CurrentWeapon->WeaponData || !PC->MyFortPawn->CurrentWeapon->IsA(AFortWeap_EditingTool::StaticClass()))
		return;

	AFortWeap_EditingTool* EditTool = (AFortWeap_EditingTool*)PC->MyFortPawn->CurrentWeapon;
	EditTool->EditActor = BuildingActorToEdit;
	EditTool->OnRep_EditActor();
}

void Player::ServerAttemptAircraftJumpHook(const UFortControllerComponent_Aircraft* ControllerComponent,
	const FRotator& ClientRotation)
{
	if (!ControllerComponent)
		return;


	AFortPlayerController* PlayerController = Cast<AFortPlayerController>(ControllerComponent->GetOwner());
	if (!PlayerController || !PlayerController->IsInAircraft()) return;

	AFortGameModeAthena* GameMode = Cast<AFortGameModeAthena>(UWorld::GetWorld()->AuthorityGameMode);
	if (!GameMode) return;

	GameMode->RestartPlayer(PlayerController);
	PlayerController->ClientSetRotation(ClientRotation, true);
}

void Player::ServerExecuteInventoryItem(AFortPlayerController* PC, FGuid Guid)
{
	if (!PC || !PC->MyFortPawn || PC->IsInAircraft())
		return;
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (!PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition)
			continue;
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == Guid)
		{
			UFortWeaponItemDefinition* WeaponDef = Cast<UFortWeaponItemDefinition>(PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition);
			if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortGadgetItemDefinition::StaticClass()))
			{
				printf("gadget: %s\n", PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition->GetName().c_str());
				FString WStr = UKismetStringLibrary::Conv_NameToString(Cast<UFortGadgetItemDefinition>(PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition)->WeaponItemDefinition.ObjectID.AssetPathName);
				WeaponDef = StaticLoadObject<UFortWeaponItemDefinition>(WStr.ToString());
				WStr.Free();
			}
			if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortDecoItemDefinition::StaticClass()))
			{
				PC->MyFortPawn->PickUpActor(nullptr, (UFortDecoItemDefinition*)PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition);
				PC->MyFortPawn->CurrentWeapon->ItemEntryGuid = Guid;

				if (PC->MyFortPawn->CurrentWeapon->IsA(AFortDecoTool_ContextTrap::StaticClass()))
				{
					reinterpret_cast<AFortDecoTool_ContextTrap*>(PC->MyFortPawn->CurrentWeapon)->ContextTrapItemDefinition = (UFortContextTrapItemDefinition*)PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition;
				}
			}
			if (!WeaponDef)
				return;
			PC->MyFortPawn->EquipWeaponDefinition(WeaponDef, Guid, PC->WorldInventory->Inventory.ReplicatedEntries[i].TrackerGuid);
			break;
		}
	}
}

inline void Player::ServerHandlePickup(
	AFortPlayerPawnAthena* Pawn,
	AFortPickup* Pickup,
	float InFlyTime,
	const FVector& InStartDirection,
	bool bPlayPickupSound)
{
	if (!Pickup || !Pawn || !Pawn->Controller || Pickup->bPickedUp)
		return;

	auto* PC = static_cast<AFortPlayerControllerAthena*>(Pawn->Controller);
	if (!PC || !PC->WorldInventory)
		return;

	FFortItemEntry& PickupEntry = Pickup->PrimaryPickupItemEntry;
	UFortItemDefinition* PickupItemDef = PickupEntry.ItemDefinition;
	if (!PickupItemDef) return;

	const bool bStackable = PickupItemDef->IsStackable();
	const float MaxStackSize = GetMaxStack(PickupItemDef);

	const bool bIsAmmo = PickupItemDef->IsA(UFortAmmoItemDefinition::StaticClass());
	const bool bIsRes = PickupItemDef->IsA(UFortResourceItemDefinition::StaticClass());
	const bool bIsTrap = PickupItemDef->IsA(UFortTrapItemDefinition::StaticClass());
	const bool bFreeStack = (bIsAmmo || bIsRes || bIsTrap);

	auto ReplicatePickupTaken = [&]()
		{
			Pickup->PickupLocationData.bPlayPickupSound = true;
			Pickup->PickupLocationData.FlyTime = 0.3f;
			Pickup->PickupLocationData.ItemOwner = Pawn;
			Pickup->PickupLocationData.PickupGuid = Pickup->PrimaryPickupItemEntry.ItemGuid;
			Pickup->PickupLocationData.PickupTarget = Pawn;
			Pickup->OnRep_PickupLocationData();

			Pickup->bPickedUp = true;
			Pickup->OnRep_bPickedUp();
		};

	if (bFreeStack)
	{
		bool stacked = false;

		for (int32 i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); ++i)
		{
			FFortItemEntry& Entry = PC->WorldInventory->Inventory.ReplicatedEntries[i];
			if (Entry.ItemDefinition == PickupItemDef)
			{
				Entry.Count += PickupEntry.Count;
				PC->WorldInventory->Inventory.MarkItemDirty(Entry);
				stacked = true;
				break;
			}
		}

		if (!stacked)
		{
			GiveStack(PC, PickupItemDef, PickupEntry.Count, false, 0, true);
		}

		ReplicatePickupTaken();
		return;
	}

	if (IsInventoryFull(PC))
	{
		if (Pawn->CurrentWeapon && Pawn->CurrentWeapon->WeaponData &&
			!Pawn->CurrentWeapon->WeaponData->IsA(UFortWeaponMeleeItemDefinition::StaticClass()))
		{
			if (bStackable)
			{
				bool stacked = false;
				for (int32 i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); ++i)
				{
					FFortItemEntry& Entry = PC->WorldInventory->Inventory.ReplicatedEntries[i];
					if (Entry.ItemDefinition == PickupItemDef)
					{
						if (Entry.Count < MaxStackSize)
						{
							int freeSpace = (int)MaxStackSize - Entry.Count;
							int toAdd = (PickupEntry.Count <= freeSpace) ? PickupEntry.Count : freeSpace;
							Entry.Count += toAdd;
							PC->WorldInventory->Inventory.MarkItemDirty(Entry);

							int remaining = PickupEntry.Count - toAdd;
							if (remaining > 0)
								SpawnStack((APlayerPawn_Athena_C*)PC->Pawn, PickupItemDef, remaining);

							stacked = true;
						}
						break;
					}
				}
				if (!stacked)
				{
					const FGuid currentGuid = Pawn->CurrentWeapon->GetInventoryGUID();
					for (int32 i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); ++i)
					{
						if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == currentGuid)
						{
							PC->ServerAttemptInventoryDrop(currentGuid, PC->WorldInventory->Inventory.ReplicatedEntries[i].Count, false);
							break;
						}
					}
					GiveStack(PC, PickupItemDef, PickupEntry.Count, false, 0, true);
				}

				ReplicatePickupTaken();
				return;
			}
			else
			{
				const FGuid currentGuid = Pawn->CurrentWeapon->GetInventoryGUID();
				for (int32 i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); ++i)
				{
					if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == currentGuid)
					{
						PC->ServerAttemptInventoryDrop(currentGuid, PC->WorldInventory->Inventory.ReplicatedEntries[i].Count, false);
						break;
					}
				}
				FortInventory::GiveItem(PC, PickupItemDef, PickupEntry.Count, PickupEntry.LoadedAmmo);
				ReplicatePickupTaken();
				return;
			}
		}
	}

	if (!IsInventoryFull(PC))
	{
		if (bStackable)
		{
			bool stacked = false;
			for (int32 i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); ++i)
			{
				FFortItemEntry& Entry = PC->WorldInventory->Inventory.ReplicatedEntries[i];
				if (Entry.ItemDefinition == PickupItemDef)
				{
					if (Entry.Count < MaxStackSize)
					{
						int freeSpace = (int)MaxStackSize - Entry.Count;
						int toAdd = (PickupEntry.Count <= freeSpace) ? PickupEntry.Count : freeSpace;
						Entry.Count += toAdd;
						PC->WorldInventory->Inventory.MarkItemDirty(Entry);

						int remaining = PickupEntry.Count - toAdd;
						if (remaining > 0)
							SpawnStack((APlayerPawn_Athena_C*)PC->Pawn, PickupItemDef, remaining);

						stacked = true;
					}
					break;
				}
			}
			if (!stacked)
			{
				GiveStack(PC, PickupItemDef, PickupEntry.Count, false, 0, true);
			}
		}
		else
		{
			FortInventory::GiveItem(PC, PickupItemDef, PickupEntry.Count, PickupEntry.LoadedAmmo);
		}
	}

	ReplicatePickupTaken();
}
__int64 Player::OnDamageServer(ABuildingSMActor* Actor, float Damage, FGameplayTagContainer DamageTags, FVector Momentum, FHitResult HitInfo, AFortPlayerControllerAthena* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle EffectContext)
{
	if (!Actor || !Actor->IsA(ABuildingSMActor::StaticClass()) || !InstigatedBy || !InstigatedBy->IsA(AFortPlayerControllerAthena::StaticClass()) || !DamageCauser->IsA(AFortWeapon::StaticClass()) || !((AFortWeapon*)DamageCauser)->WeaponData->IsA(UFortWeaponMeleeItemDefinition::StaticClass()) || Actor->bPlayerPlaced)
		return OnDamageServerOG(Actor, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);

	auto Def = UFortKismetLibrary::K2_GetResourceItemDefinition(Actor->ResourceType);

	if (Def)
	{
		auto& BuildingResourceAmountOverride = Actor->BuildingResourceAmountOverride;
		if (!BuildingResourceAmountOverride.RowName.ComparisonIndex)
			return OnDamageServerOG(Actor, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
		auto GameState = (AFortGameStateAthena*)UWorld::GetWorld()->GameState;

		FString CurveTableAssetPath = UKismetStringLibrary::Conv_NameToString(GameState->CurrentPlaylistInfo.BasePlaylist->ResourceRates.ObjectID.AssetPathName);
		static auto CurveTable = StaticLoadObject<UCurveTable>(CurveTableAssetPath.ToString());
		CurveTableAssetPath.Free();
		if (!CurveTable)
			CurveTable = StaticLoadObject<UCurveTable>("/Game/Athena/Balance/DataTables/AthenaResourceRates.AthenaResourceRates");

		float Average = 1;
		EEvaluateCurveTableResult OutCurveTable;
		UDataTableFunctionLibrary::EvaluateCurveTableRow(CurveTable, BuildingResourceAmountOverride.RowName, 0.f, &OutCurveTable, &Average, FString());
		float FinalResourceCount = round(Average / (Actor->GetMaxHealth() / Damage));

		if (FinalResourceCount > 0)
		{
			InstigatedBy->ClientReportDamagedResourceBuilding(Actor, Actor->ResourceType, FinalResourceCount, false, Damage == 100.f);
			GiveItemStack(InstigatedBy, Def, FinalResourceCount, 0);
		}
	}

	return OnDamageServerOG(Actor, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
}

typedef void(*OnCapsuleBeginOverlapOG2_t)(AFortPlayerPawn*, FFrame&, void*);
inline OnCapsuleBeginOverlapOG2_t OnCapsuleBeginOverlapOG2 = nullptr;
void /*Player::*/OnCapsuleBeginOverlap(AFortPlayerPawn* PlayerPawn, FFrame& Stack, void* Ret)
{
	UPrimitiveComponent* OverlappedComp;
	AActor* OtherActor;
	UPrimitiveComponent* OtherComp;
	int32 OtherBodyIndex;
	bool bFromSweep;
	FHitResult SweepResult;

	Stack.StepCompiledIn(&OverlappedComp);
	Stack.StepCompiledIn(&OtherActor);
	Stack.StepCompiledIn(&OtherComp);
	Stack.StepCompiledIn(&OtherBodyIndex);
	Stack.StepCompiledIn(&bFromSweep);
	Stack.StepCompiledIn(&SweepResult);

	if (PlayerPawn->IsDead())
	{
		return;
	}

	Stack.Code += Stack.Code != nullptr;

	AFortPlayerController* PlayerController = Cast<AFortPlayerController>(PlayerPawn->Controller);
	if (!PlayerController) return;

	AFortPickup* Pickup = Cast<AFortPickup>(OtherActor);
	if (!Pickup) return;

	if (!PlayerPawn->bIsDBNO ||
		!PlayerPawn->bIsSkydiving || !PlayerPawn)
	{
		if (Pickup->bPickedUp || !Pickup->bWeaponsCanBeAutoPickups)
			return;

		if (!Pickup->bServerStoppedSimulation && (Pickup->PawnWhoDroppedPickup == PlayerPawn))
			return;

		if (Pickup->PawnWhoDroppedPickup == PlayerPawn)
		{
			return;
		}

		UFortWorldItemDefinition* WorldItemDefinition = Cast<UFortWorldItemDefinition>(Pickup->PrimaryPickupItemEntry.ItemDefinition);
		if (!WorldItemDefinition) return;

		if (WorldItemDefinition->IsA(UFortAmmoItemDefinition::StaticClass()) || WorldItemDefinition->IsA(UFortResourceItemDefinition::StaticClass()))
		{
			int32 ItemQuantity = UFortKismetLibrary::K2_GetItemQuantityOnPlayer(PlayerController, WorldItemDefinition);

			for (int32 i = 0; i < PlayerPawn->QueuedAutoPickups.Num(); i++)//++i
			{
				AFortPickup* QueuedAutoPickup = PlayerPawn->QueuedAutoPickups[i];
				if (!QueuedAutoPickup) continue;

				UFortWorldItemDefinition* QueuedWorldItemDefinition = Cast<UFortWorldItemDefinition>(QueuedAutoPickup->PrimaryPickupItemEntry.ItemDefinition);
				if (!QueuedWorldItemDefinition) continue;

				if (QueuedWorldItemDefinition != WorldItemDefinition)
					continue;

				ItemQuantity += QueuedAutoPickup->PrimaryPickupItemEntry.Count;
			}

			int32 MaxStackSize = EvaluateScalableFloat(Pickup->PrimaryPickupItemEntry.ItemDefinition->MaxStackSize);

			if (ItemQuantity >= MaxStackSize && !WorldItemDefinition->bAllowMultipleStacks)
				return;

			PlayerPawn->QueuedAutoPickups.Add(Pickup);

			float InFlyTime = 0.4;
			PlayerPawn->ServerHandlePickup(Pickup, InFlyTime, FVector(), true);
		}
	}
}

void Player::ServerAttemptInventoryDrop(AFortPlayerControllerAthena* PC, FGuid ItemGuid, int Count, bool bTrash)
{
	if (Count < 1)
		return;
	if (!PC->WorldInventory)
		return;

	FFortItemEntry* Entry = FortInventory::FindEntry22(PC, ItemGuid);
	if (Entry->Count < Count)
		return;
	static auto petrolpickupclass = StaticLoadObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/Prototype/WID_Launcher_Petrol.WID_Launcher_Petrol");
	if (Entry->ItemDefinition == petrolpickupclass)
	{
		std::cout << Entry->ItemDefinition->GetFullName() << std::endl;
		FTransform Transform{};
		Transform.Translation = PC->Pawn->K2_GetActorLocation();
		Transform.Rotation = FQuat();
		Transform.Scale3D = FVector(1, 1, 1);
		UGameplayStatics::FinishSpawningActor(UGameplayStatics::BeginDeferredActorSpawnFromClass(UWorld::GetWorld(), ABGA_Petrol_Pickup_C::StaticClass(), Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, nullptr), Transform);
		FortInventory::RemoveItem(PC, ItemGuid, Count);
	}
	else {

		SpawnPickup222222(PC->Pawn->K2_GetActorLocation(), Entry->ItemDefinition, EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::Unset, Count, Entry->LoadedAmmo);
		FortInventory::RemoveItem(PC, ItemGuid, Count);
	}
}


namespace Siphon
{
	void ApplySiphonEffect(AFortPlayerState* PlayerState)
	{
		if (PlayerState)
		{
			UFortAbilitySystemComponent* AbilitySystemComponent = PlayerState->AbilitySystemComponent;

			if (AbilitySystemComponent)
			{
				FGameplayTag GameplayTag = FGameplayTag();
				GameplayTag.TagName = UKismetStringLibrary::Conv_StringToName(L"GameplayCue.Shield.PotionConsumed");

				AbilitySystemComponent->NetMulticast_InvokeGameplayCueAdded(GameplayTag, FPredictionKey(), FGameplayEffectContextHandle());
				AbilitySystemComponent->NetMulticast_InvokeGameplayCueExecuted(GameplayTag, FPredictionKey(), FGameplayEffectContextHandle());
			}
		}
	}

	void GiveSiphonBonus(AFortPlayerController* PlayerController, AFortPawn* Pawn, bool bGiveBuildingResource = true, bool bHealPlayer = true)
	{
		if (PlayerController)
		{
			if (bGiveBuildingResource)
			{
				UFortKismetLibrary::K2_GiveBuildingResource(PlayerController, EFortResourceType::Wood, 50);
				UFortKismetLibrary::K2_GiveBuildingResource(PlayerController, EFortResourceType::Stone, 50);
				UFortKismetLibrary::K2_GiveBuildingResource(PlayerController, EFortResourceType::Metal, 50);
			}

			if (bHealPlayer && Pawn)
			{
				float MaxHealth = Pawn->GetMaxHealth();
				float MaxShield = Pawn->GetMaxShield();

				float Health = Pawn->GetHealth();
				float Shield = Pawn->GetShield();

				float SiphonAmount = 200.0f;
				float RemainingSiphonAmount = SiphonAmount;

				if (Health < MaxHealth)
				{
					float NewHealth = std::clamp(Health + SiphonAmount, 0.0f, MaxHealth);

					Pawn->SetHealth(NewHealth);

					RemainingSiphonAmount -= (NewHealth - Health);
				}

				if (RemainingSiphonAmount > 0.0f)
				{
					float NewShield = std::clamp(Shield + RemainingSiphonAmount, 0.0f, MaxShield);

					Pawn->SetShield(NewShield);
				}
			}

			AFortPlayerState* PlayerState = Cast<AFortPlayerState>(PlayerController->PlayerState);

			if (PlayerState)
			{
				GiveSiphonBonus(PlayerController, Pawn);
				ApplySiphonEffect(PlayerState);
			}
		}
	}

}

void Player::ServerPlayEmoteItemHook(AFortPlayerController* PlayerController, UFortItemDefinition* EmoteAsset, float RandomEmoteNumber)
{
	if (!PlayerController || !EmoteAsset || !PlayerController->MyFortPawn)
		return;
	auto ASC = ((AFortPlayerStateAthena*)PlayerController->PlayerState)->AbilitySystemComponent;

	FGameplayAbilitySpec NewSpec;
	UClass* Ability = nullptr;
	 if (auto Emote = EmoteAsset->Cast<UAthenaDanceItemDefinition>())
	{
		auto DA = Emote->CustomDanceAbility.Get();
		Ability = DA ? DA : UGAB_Emote_Generic_C::StaticClass();
		PlayerController->MyFortPawn->bMovingEmote = Emote->bMovingEmote;
		PlayerController->MyFortPawn->bMovingEmoteForwardOnly = Emote->bMoveForwardOnly;
		PlayerController->MyFortPawn->EmoteWalkSpeed = Emote->WalkForwardSpeed;
	}

	if (Ability)
	{
		FGameplayAbilitySpec Spec{};
		FGameplayAbilitySpec* (*FGameplayAbilitySpecCtor)(FGameplayAbilitySpec * Spec, UGameplayAbility * Ability, int Level, int InputID, UObject * SourceObject) = decltype(FGameplayAbilitySpecCtor)(__int64(InSDKUtils::GetImageBase() + 0xa4c210));
		FGameplayAbilitySpecCtor(&Spec, (UGameplayAbility*)Ability->DefaultObject, 1, -1, EmoteAsset);
		FGameplayAbilitySpecHandle(*GiveAbilityAndActivateOnce)(UAbilitySystemComponent * ASC, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec, __int64) = decltype(GiveAbilityAndActivateOnce)(__int64(InSDKUtils::GetImageBase() + 0xa6fc20));
		GiveAbilityAndActivateOnce(((AFortPlayerStateAthena*)PlayerController->PlayerState)->AbilitySystemComponent, &Spec.Handle, Spec, 0);
	}

}

void shutdownAfterDelay(int seconds) {
	std::thread([seconds]() {
		std::this_thread::sleep_for(std::chrono::seconds(seconds));
		std::exit(0);
		}).detach();
}
void ApplySiphonEffect(AFortPlayerState* PlayerState)
{
	if (PlayerState)
	{
		UFortAbilitySystemComponent* AbilitySystemComponent = PlayerState->AbilitySystemComponent;

		if (AbilitySystemComponent)
		{
			FGameplayTag GameplayTag = FGameplayTag();
			GameplayTag.TagName = UKismetStringLibrary::Conv_StringToName(L"GameplayCue.Shield.PotionConsumed");

			AbilitySystemComponent->NetMulticast_InvokeGameplayCueAdded(GameplayTag, FPredictionKey(), FGameplayEffectContextHandle());
			AbilitySystemComponent->NetMulticast_InvokeGameplayCueExecuted(GameplayTag, FPredictionKey(), FGameplayEffectContextHandle());
		}
	}
}
void GiveSiphonBonus(AFortPlayerController* PlayerController, AFortPawn* Pawn, bool bGiveBuildingResource = true, bool bHealPlayer = true)
{
	if (PlayerController)
	{
		if (bGiveBuildingResource)
		{
			UFortKismetLibrary::K2_GiveBuildingResource(PlayerController, EFortResourceType::Wood, 50);
			UFortKismetLibrary::K2_GiveBuildingResource(PlayerController, EFortResourceType::Stone, 50);
			UFortKismetLibrary::K2_GiveBuildingResource(PlayerController, EFortResourceType::Metal, 50);
		}

		if (bHealPlayer && Pawn)
		{
			float MaxHealth = Pawn->GetMaxHealth();
			float MaxShield = Pawn->GetMaxShield();

			float Health = Pawn->GetHealth();
			float Shield = Pawn->GetShield();

			float SiphonAmount = 50.0f;
			float RemainingSiphonAmount = SiphonAmount;

			if (Health < MaxHealth)
			{
				float NewHealth = std::clamp(Health + SiphonAmount, 0.0f, MaxHealth);

				Pawn->SetHealth(NewHealth);

				RemainingSiphonAmount -= (NewHealth - Health);
			}

			if (RemainingSiphonAmount > 0.0f)
			{
				float NewShield = std::clamp(Shield + RemainingSiphonAmount, 0.0f, MaxShield);

				Pawn->SetShield(NewShield);
			}
		}

		AFortPlayerState* PlayerState = Cast<AFortPlayerState>(PlayerController->PlayerState);

		if (PlayerState)
		{
			ApplySiphonEffect(PlayerState);
		}
	}
}
static void (*RemoveFromAlivePlayers)(UObject* GameMode, UObject* PlayerController, APlayerState* PlayerState, APawn* FinisherPawn, UFortWeaponItemDefinition* FinishingWeapon, uint8_t DeathCause, char a7) = decltype(RemoveFromAlivePlayers)(InSDKUtils::GetImageBase() + 0x2132e50);
void Player::ClientOnPawnDied(AFortPlayerControllerZone* PlayerControllerZone, const FFortPlayerDeathReport& DeathReport)
{

	if (!PlayerControllerZone)
		return;

	AFortPlayerPawnAthena* PlayerPawnAthena = Cast<AFortPlayerPawnAthena>(PlayerControllerZone->MyFortPawn);
	AFortPlayerStateAthena* PlayerStateAthena = Cast<AFortPlayerStateAthena>(PlayerControllerZone->PlayerState);

	if (!PlayerPawnAthena || !PlayerStateAthena)
		return;

	AFortGameModeAthena* GameModeAthena = Cast<AFortGameModeAthena>(GetGameMode());
	AFortGameStateAthena* GameStateAthena = Cast<AFortGameStateAthena>(GameModeAthena->GameState);

	if (!GameModeAthena || !GameStateAthena)
		return;

	AFortPlayerStateAthena* KillerPlayerState = Cast<AFortPlayerStateAthena>(DeathReport.KillerPlayerState);
	AFortPlayerPawnAthena* KillerPawn = Cast<AFortPlayerPawnAthena>(DeathReport.KillerPawn);
	AFortPlayerControllerAthena* KillerPlayerControllerAthena = (KillerPawn ? Cast<AFortPlayerControllerAthena>(KillerPawn->Controller) : nullptr);

	AActor* DamageCauser = DeathReport.DamageCauser;

	FGameplayTagContainer TagContainer = PlayerPawnAthena ? *(FGameplayTagContainer*)(__int64(PlayerPawnAthena) + 0x16b0) : FGameplayTagContainer();

	float Distance = KillerPawn ? KillerPawn->GetDistanceTo(PlayerPawnAthena) : 0;

	EDeathCause DeathCause = AFortPlayerStateAthena::ToDeathCause(TagContainer, PlayerPawnAthena->bIsDBNO);

	FDeathInfo& DeathInfo = PlayerStateAthena->DeathInfo;
	DeathInfo.FinisherOrDowner = KillerPlayerState ? KillerPlayerState : PlayerStateAthena;
	DeathInfo.bDBNO = PlayerPawnAthena->bIsDBNO;
	DeathInfo.DeathCause = DeathCause;
	DeathInfo.Distance = (DeathCause == EDeathCause::FallDamage) ? PlayerPawnAthena->LastFallDistance : Distance;
	DeathInfo.DeathLocation = PlayerPawnAthena->K2_GetActorLocation();
	DeathInfo.bInitialized = true;

	PlayerStateAthena->PawnDeathLocation = DeathInfo.DeathLocation;

	PlayerStateAthena->OnRep_DeathInfo();

	AFortPlayerControllerAthena* PlayerControllerAthena = Cast<AFortPlayerControllerAthena>(PlayerControllerZone);

	if (PlayerControllerAthena)
	{
		if (KillerPlayerState && PlayerStateAthena != KillerPlayerState)
		{
			KillerPlayerState->KillScore++;
			KillerPlayerState->OnRep_Kills();
			KillerPlayerState->ClientReportKill(PlayerStateAthena);

			auto TeamKills = KillerPlayerState->TeamKillScore;
			KillerPlayerState->ClientReportTeamKill(TeamKills);

			for (auto Member : KillerPlayerState->PlayerTeam->TeamMembers)
			{
				auto MemberPS = (AFortPlayerStateAthena*)Member->PlayerState;
				MemberPS->TeamKillScore++;
				MemberPS->OnRep_TeamKillScore();
				MemberPS->ClientReportTeamKill(MemberPS->TeamKillScore);

				auto KillerPC = Cast<AFortPlayerControllerAthena>(KillerPlayerState->GetOwner());
				if (KillerPC)
				{
					KillerPC->ClientReportTournamentPlacementPointsScored(-1, 20);
				}

				HttpClient client;
				//Kill (Arena)
				client.getAsync("http://backend-services-lunar.xyz:76/api/v1/aio/" + MemberPS->GetPlayerName().ToString() + "/Elimination/100/50/84059365-25d6-486f-81f3-04b306828c35");
			}

			if (Globals::bEnableScoringSystem)
			{
				KillerPlayerState->Score = KillerPlayerState->KillScore;
				KillerPlayerState->TeamScore = KillerPlayerState->KillScore;
				KillerPlayerState->TeamScorePlacement = 1;
				KillerPlayerState->TotalPlayerScore = KillerPlayerState->KillScore;
				GameStateAthena->CurrentHighScore = KillerPlayerState->KillScore;
				GameStateAthena->WinningScore = KillerPlayerState->KillScore;
				GameStateAthena->WinningPlayerState = KillerPlayerState;

				KillerPlayerState->OnRep_Score();
				KillerPlayerState->OnRep_TeamScore();
				KillerPlayerState->OnRep_TeamScorePlacement();
				KillerPlayerState->OnRep_TotalPlayerScore();
				GameStateAthena->OnRep_CurrentHighScore();
				GameStateAthena->OnRep_WinningScore();
				GameStateAthena->OnRep_WinningPlayerState();
			}

			if (PlayerControllerZone->MyFortPawn && ((KillerPlayerState && KillerPlayerState->Place != 1) || PlayerStateAthena->Place != 1))
			{
				static auto AccoladeId_012_Elimination = StaticFindObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_012_Elimination.AccoladeId_012_Elimination");
				static auto AccoladeId_014_Elimination_Bronze = StaticFindObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_014_Elimination_Bronze.AccoladeId_014_Elimination_Bronze");
				static auto AccoladeId_015_Elimination_Silver = StaticFindObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_015_Elimination_Silver.AccoladeId_015_Elimination_Silver");
				static auto AccoladeId_016_Elimination_Gold = StaticFindObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeId_016_Elimination_Gold.AccoladeId_016_Elimination_Gold");

				switch (KillerPlayerState->KillScore)
				{
				case 1:
					XP_Accolades::GiveAccolade((AFortPlayerControllerAthena*)KillerPlayerState->Owner, AccoladeId_014_Elimination_Bronze, nullptr, EXPEventPriorityType::NearReticle);
					break;
				case 4:
					XP_Accolades::GiveAccolade((AFortPlayerControllerAthena*)KillerPlayerState->Owner, AccoladeId_015_Elimination_Silver, nullptr, EXPEventPriorityType::NearReticle);
					break;
				case 8:
					XP_Accolades::GiveAccolade((AFortPlayerControllerAthena*)KillerPlayerState->Owner, AccoladeId_016_Elimination_Gold, nullptr, EXPEventPriorityType::NearReticle);
					break;
				default:
					XP_Accolades::GiveAccolade((AFortPlayerControllerAthena*)KillerPlayerState->Owner, AccoladeId_012_Elimination, nullptr, EXPEventPriorityType::NearReticle);
					break;
				}
			}

			float Distance = DeathInfo.Distance / 100.0f;

			static auto AccoladeId_DistanceShot = StaticFindObject<UFortAccoladeItemDefinition>(
				"/Game/Athena/Items/Accolades/AccoladeId_DistanceShot.AccoladeId_DistanceShot");
			static auto AccoladeId_051_LongShot = StaticFindObject<UFortAccoladeItemDefinition>(
				"/Game/Athena/Items/Accolades/AccoladeId_051_LongShot.AccoladeId_051_LongShot");
			static auto AccoladeId_052_LudicrousShot = StaticFindObject<UFortAccoladeItemDefinition>(
				"/Game/Athena/Items/Accolades/AccoladeId_052_LudicrousShot.AccoladeId_052_LudicrousShot");
			static auto AccoladeId_053_ImpossibleShot = StaticFindObject<UFortAccoladeItemDefinition>(
				"/Game/Athena/Items/Accolades/AccoladeId_053_ImpossibleShot.AccoladeId_053_ImpossibleShot");

			if (Distance >= 100.0f && Distance < 150.0f)
			{
				XP_Accolades::GiveAccolade((AFortPlayerControllerAthena*)KillerPlayerState->Owner, AccoladeId_DistanceShot, nullptr, EXPEventPriorityType::NearReticle);
			}
			else if (Distance >= 150.0f && Distance < 200.0f)
			{
				XP_Accolades::GiveAccolade((AFortPlayerControllerAthena*)KillerPlayerState->Owner, AccoladeId_051_LongShot, nullptr, EXPEventPriorityType::NearReticle);
			}
			else if (Distance >= 200.0f && Distance < 250.0f)
			{
				XP_Accolades::GiveAccolade((AFortPlayerControllerAthena*)KillerPlayerState->Owner, AccoladeId_052_LudicrousShot, nullptr, EXPEventPriorityType::NearReticle);
			}
			else if (Distance >= 250.0f)
			{
				XP_Accolades::GiveAccolade((AFortPlayerControllerAthena*)KillerPlayerState->Owner, AccoladeId_053_ImpossibleShot, nullptr, EXPEventPriorityType::NearReticle);
			}


			for (int32 i = 0; i < GameStateAthena->Teams.Num(); i++)
			{
				AFortTeamInfo* TeamInfo = GameStateAthena->Teams[i];
				if (!TeamInfo) continue;

				if (TeamInfo->Team != KillerPlayerState->TeamIndex)
					continue;

				for (int32 j = 0; j < TeamInfo->TeamMembers.Num(); j++)
				{
					AFortPlayerControllerAthena* TeamMember = Cast<AFortPlayerControllerAthena>(TeamInfo->TeamMembers[j]);
					if (!TeamMember) continue;

					AFortPlayerStateAthena* TeamMemberPlayerState = Cast<AFortPlayerStateAthena>(TeamMember->PlayerState);
					if (!TeamMemberPlayerState) continue;

					TeamMemberPlayerState->TeamKillScore++;
					TeamMemberPlayerState->OnRep_TeamKillScore();

					TeamMemberPlayerState->ClientReportTeamKill(TeamMemberPlayerState->TeamKillScore);
				}
				break;
			}

			AFortPlayerControllerAthena* KillerPC = (AFortPlayerControllerAthena*)KillerPlayerState->GetOwner();
			if (Globals::bArensal && KillerPC)
			{
				AFortAthenaMutator_GG* Mutator = Cast<AFortAthenaMutator_GG>(GameStateAthena->GetMutatorByClass(GameModeAthena, AFortAthenaMutator_GG::StaticClass()));

				if (Mutator && KillerPC->WorldInventory)
				{
					int32 Kills = KillerPlayerState->KillScore;

					if (Mutator->WeaponEntries.IsValidIndex(Kills - 1))
					{
						FortInventory::RemoveItem22(KillerPC, Mutator->WeaponEntries[Kills - 1].Weapon, true);
					}

					if (Mutator->WeaponEntries.IsValidIndex(Kills))
					{
						//FortInventory::GiveItem(KillerPC, Mutator->WeaponEntries[Kills].Weapon, 1, GetAmmoForDef(Mutator->WeaponEntries[Kills].Weapon));
					}

					if (Kills >= Mutator->ScoreToWin)
					{
						KillerPlayerState->Place = 1;
						KillerPlayerState->OnRep_Place();
						GameModeAthena->EndMatch();
					}
				}
			}

			AFortPlayerControllerAthena* KillerPlayerController = Cast<AFortPlayerControllerAthena>(KillerPlayerState->Owner);

			if (KillerPlayerController)
				GiveSiphonBonus(KillerPlayerController, KillerPawn);
		}

		if (!GameStateAthena->IsRespawningAllowed(PlayerStateAthena) && !PlayerPawnAthena->bIsDBNO)
		{
			AFortPlayerStateAthena* CorrectKillerPlayerState = (KillerPlayerState && KillerPlayerState == PlayerStateAthena) ? nullptr : KillerPlayerState;
			UFortWeaponItemDefinition* KillerWeaponItemDefinition = nullptr;

			if (DamageCauser)
			{
				AFortProjectileBase* ProjectileBase = Cast<AFortProjectileBase>(DamageCauser);
				AFortWeapon* Weapon = Cast<AFortWeapon>(DamageCauser);

				if (ProjectileBase)
				{
					AFortWeapon* ProjectileBaseWeapon = Cast<AFortWeapon>(ProjectileBase->Owner);
					if (ProjectileBaseWeapon)
						KillerWeaponItemDefinition = ProjectileBaseWeapon->WeaponData;
				}
				else if (Weapon)
					KillerWeaponItemDefinition = Weapon->WeaponData;
			}

			if (PlayerControllerZone->WorldInventory)
			{
				for (auto ItemInstance : PlayerControllerZone->WorldInventory->Inventory.ItemInstances)
				{
					if (ItemInstance && ItemInstance->CanBeDropped())
					{
						const auto& ItemEntry = ItemInstance->ItemEntry;
						SpawnPickup222222(
							ItemEntry.ItemDefinition,
							ItemEntry.Count,
							ItemEntry.LoadedAmmo,
							PlayerPawnAthena->K2_GetActorLocation(),
							EFortPickupSourceTypeFlag::Player,
							EFortPickupSpawnSource::PlayerElimination
						);
					}
				}
			}


			bool bMatchEnded = GameModeAthena->HasMatchEnded();

			RemoveFromAlivePlayers(UWorld::GetWorld()->AuthorityGameMode, PlayerControllerAthena, CorrectKillerPlayerState, DeathReport.KillerPawn, DeathReport.KillerWeapon, (uint8)DeathCause, 0);
			auto GameMode = (AFortGameModeAthena*)UWorld::GetWorld()->AuthorityGameMode;
			int TotalAlive = GameMode->AlivePlayers.Num() + GameMode->AliveBots.Num();

			if (TotalAlive == 50)
			{
				static auto BronzeAccolade = StaticLoadObject<UFortAccoladeItemDefinition>(
					"/Game/Athena/Items/Accolades/AccoladeId_026_Survival_Default_Bronze.AccoladeId_026_Survival_Default_Bronze");

				for (AFortPlayerControllerAthena* PC : GameMode->AlivePlayers)
				{
					XP_Accolades::GiveAccolade(PC, BronzeAccolade, nullptr, EXPEventPriorityType::NearReticle);
				}
			}
			else if (TotalAlive == 25)
			{
				static auto SilverAccolade = StaticLoadObject<UFortAccoladeItemDefinition>(
					"/Game/Athena/Items/Accolades/AccoladeId_027_Survival_Default_Silver.AccoladeId_027_Survival_Default_Silver");

				for (AFortPlayerControllerAthena* PC : GameMode->AlivePlayers)
				{
					XP_Accolades::GiveAccolade(PC, SilverAccolade, nullptr, EXPEventPriorityType::NearReticle);
				}
			}
			else if (TotalAlive == 10)
			{
				static auto GoldAccolade = StaticLoadObject<UFortAccoladeItemDefinition>(
					"/Game/Athena/Items/Accolades/AccoladeId_028_Survival_Default_Gold.AccoladeId_028_Survival_Default_Gold");

				for (AFortPlayerControllerAthena* PC : GameMode->AlivePlayers)
				{
					XP_Accolades::GiveAccolade(PC, GoldAccolade, nullptr, EXPEventPriorityType::NearReticle);
				}
			}
			if (GameStateAthena->GamePhase > EAthenaGamePhase::Warmup)
			{
				auto SendMatchReport = [&](AFortPlayerControllerAthena* MatchPlayerControllerAthena) -> void
					{
						UAthenaPlayerMatchReport* PlayerMatchReport = MatchPlayerControllerAthena->MatchReport;
						AFortPlayerStateAthena* MatchPlayerStateAthena = Cast<AFortPlayerStateAthena>(MatchPlayerControllerAthena->PlayerState);

						if (PlayerMatchReport && MatchPlayerStateAthena)
						{
							if (PlayerMatchReport->bHasTeamStats)
							{
								FAthenaMatchTeamStats& TeamStats = PlayerMatchReport->TeamStats;
								TeamStats.Place = GameStateAthena->TeamsLeft;
								TeamStats.TotalPlayers = GameStateAthena->TotalPlayers;

								MatchPlayerControllerAthena->ClientSendTeamStatsForPlayer(TeamStats);
							}

							if (PlayerMatchReport->bHasMatchStats)
							{
								FAthenaMatchStats& MatchStats = PlayerMatchReport->MatchStats;
								//MatchStats
								MatchPlayerControllerAthena->ClientSendMatchStatsForPlayer(MatchStats);

								//Beta: Teams MatchStats NunTested
								FAthenaMatchTeamStats KTeamStats = PlayerControllerAthena->MatchReport->TeamStats;
								KTeamStats.Place = KillerPlayerState->Place;
								KTeamStats.TotalPlayers = GameStateAthena->TotalPlayers;
								MatchPlayerControllerAthena->ClientSendTeamStatsForPlayer(KTeamStats);
							}

							if (PlayerMatchReport->bHasRewards)
							{
								FAthenaRewardResult& EndOfMatchResults = PlayerMatchReport->EndOfMatchResults;
								EndOfMatchResults.LevelsGained = 5;
								EndOfMatchResults.BookLevelsGained = 10;
								EndOfMatchResults.TotalSeasonXpGained = 15;
								EndOfMatchResults.TotalBookXpGained = 20;
								EndOfMatchResults.PrePenaltySeasonXpGained = 25;

								MatchPlayerControllerAthena->ClientSendEndBattleRoyaleMatchForPlayer(true, EndOfMatchResults);
							}

							MatchPlayerStateAthena->Place = GameStateAthena->TeamsLeft;
							MatchPlayerStateAthena->OnRep_Place();
						}
					};


				for (int32 i = 0; i < GameStateAthena->Teams.Num(); i++)
				{
					AFortTeamInfo* TeamInfo = GameStateAthena->Teams[i];
					if (!TeamInfo) continue;

					if (TeamInfo->Team != PlayerStateAthena->TeamIndex)
						continue;

					bool bIsTeamAlive = false;
					for (int32 j = 0; j < TeamInfo->TeamMembers.Num(); j++)
					{
						AFortPlayerControllerAthena* TeamMember = Cast<AFortPlayerControllerAthena>(TeamInfo->TeamMembers[j]);
						if (!TeamMember || (TeamMember == PlayerControllerAthena)) continue;

						AFortPlayerPawn* TeamMemberPlayerPawn = Cast<AFortPlayerPawn>(TeamMember->MyFortPawn);
						if (!TeamMemberPlayerPawn || TeamMemberPlayerPawn->bIsDying) continue;

						bIsTeamAlive = true;
						break;
					}

					if (!bIsTeamAlive)
					{
						for (int32 j = 0; j < TeamInfo->TeamMembers.Num(); j++)
						{
							AFortPlayerControllerAthena* TeamMember = Cast<AFortPlayerControllerAthena>(TeamInfo->TeamMembers[j]);
							if (!TeamMember) continue;

							SendMatchReport(TeamMember);
						}
					}
					break;
				}

				if (GameStateAthena->TeamsLeft == 1 && KillerPlayerState && !bMatchEnded)
				{
					for (int32 i = 0; i < GameStateAthena->Teams.Num(); i++)
					{
						AFortTeamInfo* TeamInfo = GameStateAthena->Teams[i];
						if (!TeamInfo) continue;

						if (TeamInfo->Team != KillerPlayerState->TeamIndex)
							continue;

						for (int32 j = 0; j < TeamInfo->TeamMembers.Num(); j++)
						{
							AFortPlayerControllerAthena* TeamMember = Cast<AFortPlayerControllerAthena>(TeamInfo->TeamMembers[j]);
							if (!TeamMember) continue;

							AFortPlayerStateAthena* TeamMemberPlayerState = Cast<AFortPlayerStateAthena>(TeamMember->PlayerState);
							if (!TeamMemberPlayerState) continue;

							TeamMemberPlayerState->Place = GameStateAthena->TeamsLeft;
							TeamMemberPlayerState->OnRep_Place();

							TeamMember->ClientNotifyWon(KillerPawn, KillerWeaponItemDefinition, DeathCause);
							TeamMember->ClientNotifyTeamWon(KillerPawn, KillerWeaponItemDefinition, DeathCause);
							//TeamMember->ClientReportTournamentPlacementPointsScored(1, 60); is on tickflush.h
							//Win Arena points 
							HttpClient client;
							client.getAsync("http://backend-services-lunar.xyz:76/api/v1/aio/" + TeamMember->GetName() + "/Win/250/100/84059365-25d6-486f-81f3-04b306828c35");
							client.getAsync("http://backend-services-lunar.xyz:90/api/v1/rewards/managehype/" + TeamMember->GetName() + "/Win/84059365-25d6-486f-81f3-04b306828c35");
							client.getAsync("http://backend-services-lunar.xyz:3551/api/games/delete?id=" + Globals::uuid);
							SendMatchReport(TeamMember);

							shutdownAfterDelay(10);
						}
						break;
					}
				}
			}
		}
	}

	/*if (DeathReport.KillerPawn)
	{
		auto KillerPC = Cast<AFortPlayerControllerAthena>(DeathReport.KillerPawn->Controller);
		OnKilled(PlayerControllerAthena, KillerPC, DeathReport.KillerWeapon);
	}*/

	ClientOnPawnDiedOG(PlayerControllerZone, DeathReport);
}

void Player::GetPlayerViewPoint(APlayerController* PC, FVector& outLocation, FRotator& outRotation)
{
	auto PCViewTarget = PC->GetViewTarget();
	if (PC->StateName.ComparisonIndex == Globals::SpectatingName.ComparisonIndex)
	{
		outLocation = PC->LastSpectatorSyncLocation;
		outRotation = PC->LastSpectatorSyncRotation;
	}
	else if (PCViewTarget)
	{
		outLocation = PCViewTarget->K2_GetActorLocation();
		outRotation = PC->GetControlRotation();
	}
	else
	{
		GetPlayerViewPointOG(PC, outLocation, outRotation);
	}
}

void Player::ServerSetInAircraft(AFortPlayerStateAthena* PlayerState, bool bNewInAircraft)
{

	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)PlayerState->Owner;

	/*BattleBus = GetGameState()->GetAircraft(0);*/

	if (PC && PC->WorldInventory)
	{
		for (int i = PC->WorldInventory->Inventory.ReplicatedEntries.Num() - 1; i >= 0; i--)
		{
			if (((UFortWorldItemDefinition*)PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition)->bCanBeDropped && !GivenLootPlayers.Contains(PC) && Globals::bLateGame)
			{
				int Count = PC->WorldInventory->Inventory.ReplicatedEntries[i].Count;
				FortInventory::RemoveItem(PC, PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid, Count);
			}
			else if (((UFortWorldItemDefinition*)PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition)->bCanBeDropped && !Globals::bLateGame)
			{
				int Count = PC->WorldInventory->Inventory.ReplicatedEntries[i].Count;
				FortInventory::RemoveItem(PC, PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid, Count);
			}
		}
	}
	if (Globals::bLateGame)
	{ 
		if (!GivenLootPlayers.Contains(PC))
		{
			GiveLoadout(PC);
			GivenLootPlayers.Add(PC);
		}
	}

	return OrginalServerSetInAircraft(PlayerState, bNewInAircraft);
}


void Player::PlayerHooks()
{
	CantBuild = decltype(CantBuild)(InSDKUtils::GetImageBase() + 0x278d0d0);

	SwapVFTs(AFortPlayerControllerAthena::StaticClass()->DefaultObject, 0x111, Player::ServerAcknowledgePossessionHook, nullptr);

	SwapVFTs(AFortPlayerControllerAthena::StaticClass()->DefaultObject, 0x270, Player::ServerLoadingScreenDroppedHook, (PVOID*)&Player::ServerLoadingScreenDropped);

	SwapVFTs(UFortControllerComponent_Aircraft::StaticClass()->DefaultObject, 0x94, Player::ServerAttemptAircraftJumpHook, nullptr);

	SwapVFTs(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x211, Player::ServerExecuteInventoryItem, nullptr);

	MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x3049130), Player::OnDamageServer, (PVOID*)&Player::OnDamageServerOG);

	SwapVFTs(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x234, Player::ServerCreateBuildingActor, nullptr);
	SwapVFTs(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x239, Player::ServerEndEditingBuildingActor, nullptr);
	SwapVFTs(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x23B, Player::ServerBeginEditingBuildingActor, nullptr);
	SwapVFTs(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x236, Player::ServerEditBuildingActor, nullptr);
	SwapVFTs(APlayerPawn_Athena_C::StaticClass()->DefaultObject, 0x1FF, Player::ServerHandlePickup, (LPVOID*)&Player::ServerHandlePickupOG);
	SwapVFTs(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x221, Player::ServerAttemptInventoryDrop, nullptr);
	MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x3400620), OnCapsuleBeginOverlap, (void**)&OnCapsuleBeginOverlapOG2);
	/*MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x1FE49B0), Player::OnPawnAISpawnedHook, (PVOID*)&Player::OnPawnAISpawnedHookOG);*/
	MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x33ee900), Player::ClientOnPawnDied, (LPVOID*)(&Player::ClientOnPawnDiedOG));
	MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x2c6cc80), Player::GetPlayerViewPoint, (PVOID*)&Player::GetPlayerViewPointOG);
	HookVTable(AFortPlayerStateAthena::StaticClass()->DefaultObject, 0xFD, ServerSetInAircraft, (PVOID*)&OrginalServerSetInAircraft);
	SwapVFTs(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x1CD, Player::ServerPlayEmoteItemHook, nullptr);
}
