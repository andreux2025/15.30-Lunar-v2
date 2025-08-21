#include "../Public/Player.h"
#include "../Public/Abilities.h"
#include "../Public/FortInventory.h"
#include "../Public/xp.h"
#include "../../framework.h"

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

void Player::OnPawnAISpawnedHook(AActor* Controller, AFortPlayerPawnAthena* Pawn)
{
	if (Pawn->Controller->Class == ABP_PhoebePlayerController_C::StaticClass())
	{
		if (Pawn)
		{
			OnPawnAISpawnedHookOG(Controller, Pawn);
			ABP_PhoebePlayerController_C* BotPC = (ABP_PhoebePlayerController_C*)Pawn->Controller;
			if (BotPC)
			{
			//	BotPC->RunBehaviorTree(BotPC->BehaviorTree);

				static auto Name1 = UKismetStringLibrary::Conv_StringToName(TEXT("AIEvaluator_Global_GamePhaseStep"));
				static auto Name2 = UKismetStringLibrary::Conv_StringToName(TEXT("AIEvaluator_Global_GamePhase"));
				//BotPC->Blackboard->SetValueAsEnum(Name1, (uint8)EAthenaGamePhaseStep::Warmup);
			//	BotPC->Blackboard->SetValueAsEnum(Name2, (uint8)EAthenaGamePhase::Warmup);

				auto BotPlayerState = (AFortPlayerStateAthena*)Pawn->PlayerState;
				for (size_t i = 0; i < EmoteItemDefs.size(); i++)
				{
					BotPC->CosmeticLoadoutBC.Dances.Add(EmoteItemDefs.at(i));
				}
				BotPC->CosmeticLoadoutBC.Character = CharacterItemDefs.at(UKismetMathLibrary::RandomIntegerInRange(0, CharacterItemDefs.size() - 1));
				while (!BotPC->CosmeticLoadoutBC.Character)
				{
					BotPC->CosmeticLoadoutBC.Character = CharacterItemDefs.at(UKismetMathLibrary::RandomIntegerInRange(0, CharacterItemDefs.size() - 1));
				}
				if (BotPC->CosmeticLoadoutBC.Character)
				{
					if (BotPC->CosmeticLoadoutBC.Character->HeroDefinition)
					{
						for (int i = 0; i < BotPC->CosmeticLoadoutBC.Character->HeroDefinition->Specializations.Num(); i++)
						{
							auto SpecStr = UKismetStringLibrary::Conv_NameToString(BotPC->CosmeticLoadoutBC.Character->HeroDefinition->Specializations[i].ObjectID.AssetPathName);
							UFortHeroSpecialization* Spec = StaticLoadObject<UFortHeroSpecialization>(SpecStr.ToString());
							if (Spec)
							{
								for (int j = 0; j < Spec->CharacterParts.Num(); j++)
								{
									auto PartStr = UKismetStringLibrary::Conv_NameToString(Spec->CharacterParts[j].ObjectID.AssetPathName);
									UCustomCharacterPart* CharacterPart = StaticLoadObject<UCustomCharacterPart>(PartStr.ToString());
									if (CharacterPart)
									{
										BotPlayerState->CharacterData.Parts[(uintptr_t)CharacterPart->CharacterPartType] = CharacterPart;
									}
									PartStr.Free();
								}
							}
							SpecStr.Free();
						}
					}
				}
				BotPlayerState->OnRep_CharacterData();


				if (!BotPC->Inventory)
					BotPC->Inventory = SpawnActor<AFortInventory>({}, {}, BotPC);

				for (auto& Item : ((AFortGameModeAthena*)UWorld::GetWorld()->AuthorityGameMode)->StartingItems)
				{
					UFortWorldItem* Bro = Cast<UFortWorldItem>(Item.Item->CreateTemporaryItemInstanceBP(Item.Count, 0));
					Bro->OwnerInventory = BotPC->Inventory;
					FFortItemEntry& Entry = Bro->ItemEntry;
					BotPC->Inventory->Inventory.ReplicatedEntries.Add(Entry);
					BotPC->Inventory->Inventory.ItemInstances.Add(Bro);
					BotPC->Inventory->Inventory.MarkItemDirty(Entry);
					BotPC->Inventory->HandleInventoryLocalUpdate();
				}
				//FItemAndCount bro{};
				//bro.Count = 9999;
				//bro.Item = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight");
				//BotPC->StartupInventory->Items.Add(bro);
				//FItemAndCount bro2{};
				//bro2.Count = 9999;
				//bro2.Item = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium");
				//BotPC->StartupInventory->Items.Add(bro2);
				for (auto& Item : BotPC->StartupInventory->Items)
				{
					if (!Item.Item)
						continue;
					UFortWorldItem* Bro = Cast<UFortWorldItem>(Item.Item->CreateTemporaryItemInstanceBP(Item.Count, 0));
					Bro->OwnerInventory = BotPC->Inventory;
					FFortItemEntry& Entry = Bro->ItemEntry;
					Entry.LoadedAmmo = 9999;
					BotPC->Inventory->Inventory.ReplicatedEntries.Add(Entry);
					BotPC->Inventory->Inventory.ItemInstances.Add(Bro);
					BotPC->Inventory->Inventory.MarkItemDirty(Entry);
					BotPC->Inventory->HandleInventoryLocalUpdate();
					if (auto WeaponDef = Cast<UFortWeaponMeleeItemDefinition>(Entry.ItemDefinition))
					{
						BotPC->PendingEquipWeapon = Bro;
						Pawn->EquipWeaponDefinition(WeaponDef, Entry.ItemGuid, Entry.TrackerGuid);
					}
				}
			}

		}
		return;
	}


	OnPawnAISpawnedHookOG(Controller, Pawn);

	void (*NotExecRunBehaviorTree)(AAIController * Controller, UBehaviorTree * BehaviorTree) = decltype(NotExecRunBehaviorTree)(__int64(GetModuleHandleW(0)) + 0x5880C30);
	//UBehaviorTree* BehaviorTree = (UBehaviorTree*)(__int64(Controller) + 0xA80); // BehaviorTree Offset
	//UBehaviorTree* BehaviorTree = StaticLoadObject<UBehaviorTree>("/Game/Athena/AI/NPCs/Base/BehaviorTree/BT_NPC.BT_NPC");

	auto PC = (AFortAthenaAIBotController*)Pawn->Controller;
	auto PlayerState = (AFortPlayerStateAthena*)Pawn->PlayerState;


	if (PC->CosmeticLoadoutBC.Character)
	{
		if (PC->CosmeticLoadoutBC.Character->HeroDefinition)
		{
			for (int i = 0; i < PC->CosmeticLoadoutBC.Character->HeroDefinition->Specializations.Num(); i++)
			{
				auto SpecStr = UKismetStringLibrary::Conv_NameToString(PC->CosmeticLoadoutBC.Character->HeroDefinition->Specializations[i].ObjectID.AssetPathName);
				UFortHeroSpecialization* Spec = StaticLoadObject<UFortHeroSpecialization>(SpecStr.ToString());
				if (Spec)
				{
					for (int j = 0; j < Spec->CharacterParts.Num(); j++)
					{
						auto PartStr = UKismetStringLibrary::Conv_NameToString(Spec->CharacterParts[j].ObjectID.AssetPathName);
						UCustomCharacterPart* CharacterPart = StaticLoadObject<UCustomCharacterPart>(PartStr.ToString());
						if (CharacterPart)
						{
							PlayerState->CharacterData.Parts[(uintptr_t)CharacterPart->CharacterPartType] = CharacterPart;
						}
						PartStr.Free();
					}
				}
				SpecStr.Free();
			}
		}
	}
	PlayerState->OnRep_CharacterData();



	//NotExecRunBehaviorTree((AFortAthenaAIBotController*)Pawn->Controller, PC->BehaviorTree);


	//((AFortAthenaAIBotController*)Pawn->Controller)->RunBehaviorTree(((AFortAthenaAIBotController*)Pawn->Controller)->BehaviorTree);
	auto BlackboardComp = ((AFortAthenaAIBotController*)Pawn->Controller)->Blackboard;
	static auto name1 = UKismetStringLibrary::Conv_StringToName(L"AIEvaluator_Global_GamePhaseStep");
	static auto name1b = UKismetStringLibrary::Conv_StringToName(L"AIEvaluator_Global_GamePhase");
	//BlackboardComp->SetValueAsEnum(name1, 7);
//	BlackboardComp->SetValueAsEnum(name1b, (uint8)EAthenaGamePhase::SafeZones);

	UFortNPCConversationParticipantComponent* bruh = (UFortNPCConversationParticipantComponent*)Pawn->GetComponentByClass(UFortNPCConversationParticipantComponent::StaticClass());

	if (!bruh)
		return;

	bruh->BotControllerOwner = PC;
	bruh->PlayerPawnOwner = Pawn;

	int MaxServices = bruh->MaxServices;
	bruh->bCanStartConversation = true;
	bruh->OnRep_CanStartConversation();

	printf("bruh->AffiliationManager %s\n", bruh->AffiliationManager->GetName().c_str());
	printf("MaxServices: %d\n", MaxServices);

	auto& SalesMap = *(TMap<FName, FNPCSaleInventoryRow*>*)(__int64(bruh->SalesInventory) + 0x0030);
	auto& QuestMap = *(TMap<FName, FNPCQuestRow*>*)(__int64(bruh->Quests) + 0x0030);
	auto& ServicesMap = *(TMap<FName, FNPCDynamicServiceRow*>*)(__int64(bruh->Services) + 0x0030);

	bool HasBuy = false;
	bool HasBounty = false;

	if (bruh->SalesInventory)
	{
		for (auto& Pair : SalesMap)
		{
			if (Pair.Value()->NPC.TagName.ComparisonIndex == bruh->CharacterData->GameplayTag.TagName.ComparisonIndex)
			{
				bruh->SupportedSales.Add(*Pair.Value());
			}
		}
	}

	if (bruh->Quests)
	{
		for (auto& Pair : QuestMap)
		{
			if (!Pair.Value()->Quest.ObjectID.AssetPathName.ComparisonIndex)
				continue;
			if (Pair.Value()->NPC.TagName.ComparisonIndex == bruh->CharacterData->GameplayTag.TagName.ComparisonIndex)
			{
				bruh->SupportedQuests.Add(Pair.Value()->Quest);
			}
		}
	}

	if (bruh->Services)
	{
		for (auto& Pair : ServicesMap)
		{
			if (Pair.Value()->Chance <= 0)
				continue;
			if (Pair.Value()->NPC.TagName.ComparisonIndex == bruh->CharacterData->GameplayTag.TagName.ComparisonIndex)
			{
				bool IsBounty = Pair.Value()->ServiceTag.TagName.ToString().contains("Bounty");
				bool IsBuy = Pair.Value()->ServiceTag.TagName.ToString().contains("Sell");
				if (IsBuy && HasBuy)
					continue;
				if (IsBounty && HasBounty)
					continue;
				if (IsBounty)
					HasBounty = true;
				if (IsBuy)
					HasBuy = true;
				bruh->SupportedServices.GameplayTags.Add(Pair.Value()->ServiceTag);
			}
		}
	}
	while (bruh->SupportedServices.GameplayTags.Num() > MaxServices)
	{
		for (auto& Pair : ServicesMap)
		{
			if (Pair.Value()->Chance <= 0)
				continue;
			if (bruh->SupportedServices.GameplayTags.Num() <= MaxServices)
				break;
			if (Pair.Value()->NPC.TagName.ComparisonIndex == bruh->CharacterData->GameplayTag.TagName.ComparisonIndex)
			{
				if (!UKismetMathLibrary::RandomBoolWithWeight(Pair.Value()->Chance))
				{
					for (size_t i = 0; i < bruh->SupportedServices.GameplayTags.Num(); i++)
					{
						if (bruh->SupportedServices.GameplayTags[i].TagName.ComparisonIndex == Pair.Value()->ServiceTag.TagName.ComparisonIndex)
						{
							bruh->SupportedServices.GameplayTags.Remove(i);
							break;
						}
					}
				}
			}
		}
	}

	FItemAndCount bro{};
	bro.Count = 9999;
	bro.Item = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight");
	PC->StartupInventory->Items.Add(bro);
	FItemAndCount bro2{};
	bro2.Count = 9999;
	bro2.Item = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium");
	PC->StartupInventory->Items.Add(bro2);

	for (auto& Item : PC->StartupInventory->Items)
	{
		UFortWorldItem* Bro = Cast<UFortWorldItem>(Item.Item->CreateTemporaryItemInstanceBP(Item.Count, 0));
		Bro->OwnerInventory = PC->Inventory;
		FFortItemEntry& Entry = Bro->ItemEntry;
		Entry.LoadedAmmo = 9999;
		PC->Inventory->Inventory.ReplicatedEntries.Add(Entry);
		PC->Inventory->Inventory.ItemInstances.Add(Bro);
		PC->Inventory->Inventory.MarkItemDirty(Entry);
		PC->Inventory->HandleInventoryLocalUpdate();
		if (auto WeaponDef = Cast<UFortWeaponRangedItemDefinition>(Entry.ItemDefinition))
		{
			PC->PendingEquipWeapon = Bro;
		}
	}
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
	if (!First && !UKismetStringLibrary::Conv_NameToString(Cast<AFortGameStateAthena>(GameMode->GameState)->CurrentPlaylistInfo.BasePlaylist->PlaylistName).ToString().contains("Showdown"))
	{
		First = true;

		auto SpawnerData = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Deadfire/BP_AIBotSpawnerData_Deadfire.BP_AIBotSpawnerData_Deadfire_C");
		auto SpawnerData2 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Guide/BP_AIBotSpawnerData_Guide.BP_AIBotSpawnerData_Guide_C");
		auto SpawnerData3 = StaticLoadObject<UClass>("/CosmosGameplay/AI/NPCs/Cosmos/AISpawnerData/BP_AIBotSpawnerData_Cosmos.BP_AIBotSpawnerData_Cosmos_C");
		auto SpawnerData4 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Gladiator/BP_AIBotSpawnerData_Gladiator.BP_AIBotSpawnerData_Gladiator_C");
		auto SpawnerData5 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Outlaw/BP_AIBotSpawnerData_Outlaw.BP_AIBotSpawnerData_Outlaw_C");
		auto SpawnerData6 = StaticLoadObject<UClass>("/NightmareGameplay/AI/NPCs/Nightmare/AISpawnerData/BP_AIBotSpawnerData_Nightmare.BP_AIBotSpawnerData_Nightmare_C");
		auto SpawnerData7 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Dummy/BP_AIBotSpawnerData_Dummy.BP_AIBotSpawnerData_Dummy_C");
		auto SpawnerData8 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Splode/BP_AIBotSpawnerData_Splode.BP_AIBotSpawnerData_Splode_C");
		auto SpawnerData9 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Remedy/BP_AIBotSpawnerData_Remedy.BP_AIBotSpawnerData_Remedy_C");
		auto SpawnerData10 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Doggo/BP_AIBotSpawnerData_Doggo.BP_AIBotSpawnerData_Doggo_C");
		auto SpawnerData11 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Fishstick/BP_AIBotSpawnerData_Fishstick.BP_AIBotSpawnerData_Fishstick_C");
		auto SpawnerData12 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Blaze/BP_AIBotSpawnerData_Blaze.BP_AIBotSpawnerData_Blaze_C");
		auto SpawnerData13 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Bullseye/BP_AIBotSpawnerData_Bullseye.BP_AIBotSpawnerData_Bullseye_C");
		auto SpawnerData14 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Burnout/BP_AIBotSpawnerData_Burnout.BP_AIBotSpawnerData_Burnout_C");
		auto SpawnerData15 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/TomatoHead/BP_AIBotSpawnerData_TomatoHead.BP_AIBotSpawnerData_TomatoHead_C");
		auto SpawnerData16 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Ragnarok/BP_AIBotSpawnerData_Ragnarok.BP_AIBotSpawnerData_Ragnarok_C");
		auto SpawnerData17 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Outcast/BP_AIBotSpawnerData_Outcast.BP_AIBotSpawnerData_Outcast_C");
		auto SpawnerData18 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Bigfoot/BP_AIBotSpawnerData_Bigfoot.BP_AIBotSpawnerData_Bigfoot_C");
		auto SpawnerData19 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/BeefBoss/BP_AIBotSpawnerData_BeefBoss.BP_AIBotSpawnerData_BeefBoss_C");
		auto SpawnerData20 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Kyle/BP_AIBotSpawnerData_Kyle.BP_AIBotSpawnerData_Kyle_C");
		auto SpawnerData21 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/BigChuggus/BP_AIBotSpawnerData_BigChuggus.BP_AIBotSpawnerData_BigChuggus_C");
		auto SpawnerData22 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Bandolier/BP_AIBotSpawnerData_Bandolier.BP_AIBotSpawnerData_Bandolier_C");
		auto SpawnerData23 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/RuckusH/BP_AIBotSpawnerData_NPC_RuckusH.BP_AIBotSpawnerData_NPC_RuckusH_C");
		auto SpawnerData24 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/FutureSamurai/BP_AIBotSpawnerData_FutureSamurai.BP_AIBotSpawnerData_FutureSamurai_C");
		auto SpawnerData25 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Longshot/BP_AIBotSpawnerData_Longshot.BP_AIBotSpawnerData_Longshot_C");
		auto SpawnerData26 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Turk/BP_AIBotSpawnerData_Turk.BP_AIBotSpawnerData_Turk_C");
		auto SpawnerData27 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Cole/BP_AIBotSpawnerData_Cole.BP_AIBotSpawnerData_Cole_C");
		auto SpawnerData28 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/TheReaper/BP_AIBotSpawnerData_TheReaper.BP_AIBotSpawnerData_TheReaper_C");
		auto SpawnerData29 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Triggerfish/BP_AIBotSpawnerData_Triggerfish.BP_AIBotSpawnerData_Triggerfish_C");
		auto SpawnerData30 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Grimbles/BP_AIBotSpawnerData_Grimbles.BP_AIBotSpawnerData_Grimbles_C");
		auto SpawnerData31 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Sleuth/BP_AIBotSpawnerData_Sleuth.BP_AIBotSpawnerData_Sleuth_C");
		auto SpawnerData32 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Bushranger/BP_AIBotSpawnerData_Bushranger.BP_AIBotSpawnerData_Bushranger_C");
		auto SpawnerData33 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/BunkerJonesy/BP_AIBotSpawnerData_BunkerJonesy.BP_AIBotSpawnerData_BunkerJonesy_C");
		auto SpawnerData34 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Kit/BP_AIBotSpawnerData_Kit.BP_AIBotSpawnerData_Kit_C");
		auto SpawnerData35 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Shapeshifter/BP_AIBotSpawnerData_Shapeshifter.BP_AIBotSpawnerData_Shapeshifter_C");
		auto SpawnerData36 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Brutus/BP_AIBotSpawnerData_Brutus.BP_AIBotSpawnerData_Brutus_C");
		auto SpawnerData37 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/WeaponsExpert/BP_AIBotSpawnerData_WeaponsExpert.BP_AIBotSpawnerData_WeaponsExpert_C");
		auto SpawnerData38 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/FarmerSteel/BP_AIBotSpawnerData_FarmerSteel.BP_AIBotSpawnerData_FarmerSteel_C");
		auto SpawnerData39 = StaticLoadObject<UClass>("/BattlepassS15/AI/NPCs/Sunflower/BP_AIBotSpawnerData_Sunflower.BP_AIBotSpawnerData_Sunflower_C");
		auto list = ((UFortAthenaAIBotSpawnerData*)SpawnerData)->CreateComponentListFromClass(SpawnerData, UWorld::GetWorld());
		auto list2 = ((UFortAthenaAIBotSpawnerData*)SpawnerData2)->GetDefaultObj()->CreateComponentListFromClass(SpawnerData2, UWorld::GetWorld());
		auto list3 = ((UFortAthenaAIBotSpawnerData*)SpawnerData3)->GetDefaultObj()->CreateComponentListFromClass(SpawnerData3, UWorld::GetWorld());
		auto list4 = ((UFortAthenaAIBotSpawnerData*)SpawnerData4)->CreateComponentListFromClass(SpawnerData4, UWorld::GetWorld());
		auto list5 = ((UFortAthenaAIBotSpawnerData*)SpawnerData5)->CreateComponentListFromClass(SpawnerData5, UWorld::GetWorld());
		auto list6 = ((UFortAthenaAIBotSpawnerData*)SpawnerData6)->CreateComponentListFromClass(SpawnerData6, UWorld::GetWorld());
		auto list7 = ((UFortAthenaAIBotSpawnerData*)SpawnerData7)->CreateComponentListFromClass(SpawnerData7, UWorld::GetWorld());
		auto list8 = ((UFortAthenaAIBotSpawnerData*)SpawnerData8)->CreateComponentListFromClass(SpawnerData8, UWorld::GetWorld());
		auto list9 = ((UFortAthenaAIBotSpawnerData*)SpawnerData9)->CreateComponentListFromClass(SpawnerData9, UWorld::GetWorld());
		auto list10 = ((UFortAthenaAIBotSpawnerData*)SpawnerData10)->CreateComponentListFromClass(SpawnerData10, UWorld::GetWorld());
		auto list11 = ((UFortAthenaAIBotSpawnerData*)SpawnerData11)->CreateComponentListFromClass(SpawnerData11, UWorld::GetWorld());
		auto list12 = ((UFortAthenaAIBotSpawnerData*)SpawnerData12)->CreateComponentListFromClass(SpawnerData12, UWorld::GetWorld());
		auto list13 = ((UFortAthenaAIBotSpawnerData*)SpawnerData13)->CreateComponentListFromClass(SpawnerData13, UWorld::GetWorld());
		auto list14 = ((UFortAthenaAIBotSpawnerData*)SpawnerData14)->CreateComponentListFromClass(SpawnerData14, UWorld::GetWorld());
		auto list15 = ((UFortAthenaAIBotSpawnerData*)SpawnerData15)->CreateComponentListFromClass(SpawnerData15, UWorld::GetWorld());
		auto list16 = ((UFortAthenaAIBotSpawnerData*)SpawnerData16)->CreateComponentListFromClass(SpawnerData16, UWorld::GetWorld());
		auto list17 = ((UFortAthenaAIBotSpawnerData*)SpawnerData17)->CreateComponentListFromClass(SpawnerData17, UWorld::GetWorld());
		auto list18 = ((UFortAthenaAIBotSpawnerData*)SpawnerData18)->CreateComponentListFromClass(SpawnerData18, UWorld::GetWorld());
		auto list19 = ((UFortAthenaAIBotSpawnerData*)SpawnerData19)->CreateComponentListFromClass(SpawnerData19, UWorld::GetWorld());
		auto list20 = ((UFortAthenaAIBotSpawnerData*)SpawnerData20)->CreateComponentListFromClass(SpawnerData20, UWorld::GetWorld());
		auto list21 = ((UFortAthenaAIBotSpawnerData*)SpawnerData21)->CreateComponentListFromClass(SpawnerData21, UWorld::GetWorld());
		auto list22 = ((UFortAthenaAIBotSpawnerData*)SpawnerData22)->CreateComponentListFromClass(SpawnerData22, UWorld::GetWorld());
		auto list23 = ((UFortAthenaAIBotSpawnerData*)SpawnerData23)->CreateComponentListFromClass(SpawnerData23, UWorld::GetWorld());
		auto list24 = ((UFortAthenaAIBotSpawnerData*)SpawnerData24)->CreateComponentListFromClass(SpawnerData24, UWorld::GetWorld());
		auto list25 = ((UFortAthenaAIBotSpawnerData*)SpawnerData25)->CreateComponentListFromClass(SpawnerData25, UWorld::GetWorld());
		auto list26 = ((UFortAthenaAIBotSpawnerData*)SpawnerData26)->CreateComponentListFromClass(SpawnerData26, UWorld::GetWorld());
		auto list27 = ((UFortAthenaAIBotSpawnerData*)SpawnerData27)->CreateComponentListFromClass(SpawnerData27, UWorld::GetWorld());
		auto list28 = ((UFortAthenaAIBotSpawnerData*)SpawnerData28)->CreateComponentListFromClass(SpawnerData28, UWorld::GetWorld());
		auto list29 = ((UFortAthenaAIBotSpawnerData*)SpawnerData29)->CreateComponentListFromClass(SpawnerData29, UWorld::GetWorld());
		auto list30 = ((UFortAthenaAIBotSpawnerData*)SpawnerData30)->CreateComponentListFromClass(SpawnerData30, UWorld::GetWorld());
		auto list31 = ((UFortAthenaAIBotSpawnerData*)SpawnerData31)->CreateComponentListFromClass(SpawnerData31, UWorld::GetWorld());
		auto list32 = ((UFortAthenaAIBotSpawnerData*)SpawnerData32)->CreateComponentListFromClass(SpawnerData32, UWorld::GetWorld());
		auto list33 = ((UFortAthenaAIBotSpawnerData*)SpawnerData33)->CreateComponentListFromClass(SpawnerData33, UWorld::GetWorld());
		auto list34 = ((UFortAthenaAIBotSpawnerData*)SpawnerData34)->CreateComponentListFromClass(SpawnerData34, UWorld::GetWorld());
		auto list35 = ((UFortAthenaAIBotSpawnerData*)SpawnerData35)->CreateComponentListFromClass(SpawnerData35, UWorld::GetWorld());
		auto list36 = ((UFortAthenaAIBotSpawnerData*)SpawnerData36)->CreateComponentListFromClass(SpawnerData36, UWorld::GetWorld());
		auto list37 = ((UFortAthenaAIBotSpawnerData*)SpawnerData37)->CreateComponentListFromClass(SpawnerData37, UWorld::GetWorld());
		auto list38 = ((UFortAthenaAIBotSpawnerData*)SpawnerData38)->CreateComponentListFromClass(SpawnerData38, UWorld::GetWorld());
		auto list39 = ((UFortAthenaAIBotSpawnerData*)SpawnerData39)->CreateComponentListFromClass(SpawnerData39, UWorld::GetWorld());

		FTransform Transform{};
		Transform.Translation = FVector{ 19972, 25561, -376 };
		Transform.Rotation = FQuat();
		Transform.Scale3D = FVector{ 1,1,1 };

		FTransform Transform2{};
		Transform2.Translation = FVector{ -37632, 10752, 4992 };
		Transform2.Rotation = FQuat();
		Transform2.Scale3D = FVector{ 1,1,1 };

		FTransform Transform3{};
		Transform3.Translation = FVector{ 5632, 54016, -1536 };
		Transform3.Rotation = FQuat();
		Transform3.Scale3D = FVector{ 1,1,1 };

		FTransform Transform4{};
		Transform4.Translation = FVector{ 28160, 42752 ,-1152 };
		Transform4.Rotation = FQuat();
		Transform4.Scale3D = FVector{ 1,1,1 };

		FTransform Transform5{};
		Transform5.Translation = FVector{ -7935, 14592 ,-2304 };
		Transform5.Rotation = FQuat();
		Transform5.Scale3D = FVector{ 1,1,1 };

		FTransform Transform6{};
		Transform6.Translation = FVector{ 102656, -31744, -1152 };
		Transform6.Rotation = FQuat();
		Transform6.Scale3D = FVector{ 1,1,1 };

		FTransform Transform7{};
		Transform7.Translation = FVector{ 65024, -32512, 100 };
		Transform7.Rotation = FQuat();
		Transform7.Scale3D = FVector{ 1,1,1 };

		FTransform Transform8{};
		Transform8.Translation = FVector{ 112640 - 4351 - 3072 };
		Transform8.Rotation = FQuat();
		Transform8.Scale3D = FVector{ 1,1,1 };

		FTransform Transform9{};
		Transform9.Translation = FVector{ 83968, -256 ,1152 };
		Transform9.Rotation = FQuat();
		Transform9.Scale3D = FVector{ 1,1,1 };

		FTransform Transform10{};
		Transform10.Translation = FVector{ 69375, -10496, -2304 };
		Transform10.Rotation = FQuat();
		Transform10.Scale3D = FVector{ 1,1,1 };

		FTransform Transform11{};
		Transform11.Translation = FVector{ 104448 ,26880 ,-1920 };
		Transform11.Rotation = FQuat();
		Transform11.Scale3D = FVector{ 1,1,1 };

		FTransform Transform12{};
		Transform12.Translation = FVector{ 107776, 74240, -1160 };
		Transform12.Rotation = FQuat();
		Transform12.Scale3D = FVector{ 1,1,1 };

		FTransform Transform13{};
		Transform13.Translation = FVector{ 83200, 93440, -1536 };
		Transform13.Rotation = FQuat();
		Transform13.Scale3D = FVector{ 1,1,1 };

		FTransform Transform14{};
		Transform14.Translation = FVector{ 65536, 84736, -2304 };
		Transform14.Rotation = FQuat();
		Transform14.Scale3D = FVector{ 1,1,1 };

		FTransform Transform15{};
		Transform15.Translation = FVector{ 51712, 54528, -768 };
		Transform15.Rotation = FQuat();
		Transform15.Scale3D = FVector{ 1,1,1 };

		FTransform Transform16{};
		Transform16.Translation = FVector{ -13926 ,-96267 ,-384 };
		Transform16.Rotation = FQuat();
		Transform16.Scale3D = FVector{ 1,1,1 };

		FTransform Transform17{};
		Transform17.Translation = FVector{ -768 ,-55296 ,-1920 };
		Transform17.Rotation = FQuat();
		Transform17.Scale3D = FVector{ 1,1,1 };

		FTransform Transform18{};
		Transform18.Translation = FVector{ -21950, -55905, -2672 };
		Transform18.Rotation = FQuat();
		Transform18.Scale3D = FVector{ 1,1,1 };

		FTransform Transform19{};
		Transform19.Translation = FVector{ -38411, -70157, -2679 };
		Transform19.Rotation = FQuat();
		Transform19.Scale3D = FVector{ 1,1,1 };

		FTransform Transform20{};
		Transform20.Translation = FVector{ -33536, -24576, -2304 };
		Transform20.Rotation = FQuat();
		Transform20.Scale3D = FVector{ 1,1,1 };

		FTransform Transform21{};
		Transform21.Translation = FVector{ -63488, -43264, -3072 };
		Transform21.Rotation = FQuat();
		Transform21.Scale3D = FVector{ 1,1,1 };

		FTransform Transform22{};
		Transform22.Translation = FVector{ -87552, -29952, -3840 };
		Transform22.Rotation = FQuat();
		Transform22.Scale3D = FVector{ 1,1,1 };

		FTransform Transform23{};
		Transform23.Translation = FVector{ -71680, -12031, -3456 };
		Transform23.Rotation = FQuat();
		Transform23.Scale3D = FVector{ 1,1,1 };

		FTransform Transform24{};
		Transform24.Translation = FVector{ -88126, 32251, 1552 };
		Transform24.Rotation = FQuat();
		Transform24.Scale3D = FVector{ 1,1,1 };

		FTransform Transform25{};
		Transform25.Translation = FVector{ -87296, 44032, 6144 };
		Transform25.Rotation = FQuat();
		Transform25.Scale3D = FVector{ 1,1,1 };

		FTransform Transform26{};
		Transform26.Translation = FVector{ -64512, 30976, 384 };
		Transform26.Rotation = FQuat();
		Transform26.Scale3D = FVector{ 1,1,1 };

		FTransform Transform27{};
		Transform27.Translation = FVector{ -22528, 92416, 7296 };
		Transform27.Rotation = FQuat();
		Transform27.Scale3D = FVector{ 1,1,1 };

		FTransform Transform28{};
		Transform28.Translation = FVector{ 19968, -114432, -1920 };
		Transform28.Rotation = FQuat();
		Transform28.Scale3D = FVector{ 1,1,1 };

		FTransform Transform29{};
		Transform29.Translation = FVector{ 32768, -112384, -3840 };
		Transform29.Rotation = FQuat();
		Transform29.Scale3D = FVector{ 1,1,1 };

		FTransform Transform30{};
		Transform30.Translation = FVector{ 50176, 108800, -768 };
		Transform30.Rotation = FQuat();
		Transform30.Scale3D = FVector{ 1,1,1 };

		FTransform Transform31{};
		Transform31.Translation = FVector{ 33536, -73727, -3456 };
		Transform31.Rotation = FQuat();
		Transform31.Scale3D = FVector{ 1,1,1 };

		FTransform Transform32{};
		Transform32.Translation = FVector{ 18944, -35584, 0 };
		Transform32.Rotation = FQuat();
		Transform32.Scale3D = FVector{ 1,1,1 };

		FTransform Transform33{};
		Transform33.Translation = FVector{ -125696, 69119, -1920 };
		Transform33.Rotation = FQuat();
		Transform33.Scale3D = FVector{ 1,1,1 };

		FTransform Transform34{};
		Transform34.Translation = FVector{ -77824, 79104, 4992 };
		Transform34.Rotation = FQuat();
		Transform34.Scale3D = FVector{ 1,1,1 };

		FTransform Transform35{};
		Transform35.Translation = FVector{ -89088, 97024, 3456 };
		Transform35.Rotation = FQuat();
		Transform35.Scale3D = FVector{ 1,1,1 };

		FTransform Transform36{};
		Transform36.Translation = FVector{ 3326, 113663, -3072 };
		Transform36.Rotation = FQuat();
		Transform36.Scale3D = FVector{ 1,1,1 };

		FTransform Transform37{};
		Transform37.Translation = FVector{ 18686, 121342, -2304 };
		Transform37.Rotation = FQuat();
		Transform37.Scale3D = FVector{ 1,1,1 };

		FTransform Transform38{};
		Transform38.Translation = FVector{ 42752, 67072, -1152 };
		Transform38.Rotation = FQuat();
		Transform38.Scale3D = FVector{ 1,1,1 };

		FTransform Transform39{};
		Transform39.Translation = FVector{ 59648, 55296, -768 };
		Transform39.Rotation = FQuat();
		Transform39.Scale3D = FVector{ 1,1,1 };
		GlobalList = list;
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list, Transform);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list2, Transform2);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list3, Transform3);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list4, Transform4);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list5, Transform5);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list6, Transform6);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list7, Transform7);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list8, Transform8);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list9, Transform9);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list10, Transform10);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list11, Transform11);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list12, Transform12);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list13, Transform13);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list14, Transform14);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list15, Transform15);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list16, Transform16);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list17, Transform17);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list18, Transform18);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list19, Transform19);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list20, Transform20);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list21, Transform21);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list22, Transform22);
		//((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list23, Transform23);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list24, Transform24);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list25, Transform25);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list26, Transform26);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list27, Transform27);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list28, Transform28);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list29, Transform29);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list30, Transform30);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list31, Transform31);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list32, Transform32);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list33, Transform33);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list34, Transform34);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list35, Transform35);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list36, Transform36);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list37, Transform37);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list38, Transform38);
		((UAthenaAISystem*)UWorld::GetWorld()->AISystem)->AISpawner->RequestSpawn(list39, Transform39);
	}
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
		FortInventory::RemoveItem(PlayerController, ItemEntry->ItemGuid, 0);
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


void Player::ServerSetInAircraft(AFortPlayerState* PlayerState, bool bNewInAircraft) {
	if (!PlayerState)
		return;

	auto PlayerController = Cast<AFortPlayerControllerAthena>(PlayerState->GetOwner());
	if (!PlayerController)
		return;

	FortInventory::RemoveAllDroppableItems(PlayerController);

	return ServerSetInAircraftOG(PlayerState, bNewInAircraft);
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

//void Player::ServerPlayEmoteItemHook(AFortPlayerController* PlayerController, UFortItemDefinition* EmoteAsset, float RandomEmoteNumber)
//{
//	if (!PlayerController || !EmoteAsset || !PlayerController->MyFortPawn)
//		return;
//	auto ASC = ((AFortPlayerStateAthena*)PlayerController->PlayerState)->AbilitySystemComponent;
//
//	FGameplayAbilitySpec NewSpec;
//	UClass* Ability = nullptr;
//	 if (auto Emote = EmoteAsset->Cast<UAthenaDanceItemDefinition>())
//	{
//		auto DA = Emote->CustomDanceAbility.Get();
//		Ability = DA ? DA : UGAB_Emote_Generic_C::StaticClass();
//		PlayerController->MyFortPawn->bMovingEmote = Emote->bMovingEmote;
//		PlayerController->MyFortPawn->bMovingEmoteForwardOnly = Emote->bMoveForwardOnly;
//		PlayerController->MyFortPawn->EmoteWalkSpeed = Emote->WalkForwardSpeed;
//	}
//
//	if (Ability)
//	{
//		FGameplayAbilitySpec Spec{};
//		FGameplayAbilitySpec* (*FGameplayAbilitySpecCtor)(FGameplayAbilitySpec * Spec, UGameplayAbility * Ability, int Level, int InputID, UObject * SourceObject) = decltype(FGameplayAbilitySpecCtor)(__int64(InSDKUtils::GetImageBase() + 0xa4c210));
//		FGameplayAbilitySpecCtor(&Spec, (UGameplayAbility*)Ability->DefaultObject, 1, -1, EmoteAsset);
//		FGameplayAbilitySpecHandle(*GiveAbilityAndActivateOnce)(UAbilitySystemComponent * ASC, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec, __int64) = decltype(GiveAbilityAndActivateOnce)(__int64(InSDKUtils::GetImageBase() + 0xa6fc20));
//		GiveAbilityAndActivateOnce(((AFortPlayerStateAthena*)PlayerController->PlayerState)->AbilitySystemComponent, &Spec.Handle, Spec, 0);
//	}
//
//}

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
	MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x1FE49B0), Player::OnPawnAISpawnedHook, (PVOID*)&Player::OnPawnAISpawnedHookOG);
	MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x33ee900), Player::ClientOnPawnDied, (LPVOID*)(&Player::ClientOnPawnDiedOG));
	MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x2c6cc80), Player::GetPlayerViewPoint, (PVOID*)&Player::GetPlayerViewPointOG);


}