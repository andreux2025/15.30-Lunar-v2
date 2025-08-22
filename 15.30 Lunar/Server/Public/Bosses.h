#pragma once
#include "../../framework.h"


void SpawnNpcs() {
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
	GlobalAISpawnerList = list;

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