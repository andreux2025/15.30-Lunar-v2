#include "../Public/GameMode.h"
#include "../Public/xp.h"
#include <array>


void GameMode::HandleNewSafeZonePhaseHook(AFortGameModeAthena* GameMode, int32 ZoneIndex)
{
	AFortGameStateAthena* GameState = Cast<AFortGameStateAthena>(GameMode->GameState);
	if (!GameState) return HandleNewSafeZonePhase(GameMode, ZoneIndex);

	AFortAthenaMapInfo* MapInfo = GameState->MapInfo;
	if (!MapInfo) return HandleNewSafeZonePhase(GameMode, ZoneIndex);

	auto& WaitTimes = MapInfo->SafeZoneDefinition.WaitTimes();
	auto& Durations = MapInfo->SafeZoneDefinition.Durations();

	static constexpr std::array<float, 8> LateGameDurations{
		0.f,
		65.f,
		60.f,
		50.f,
		45.f,
		35.f,
		30.f,
		40.f,
	};

	static constexpr std::array<float, 8> LateGameHoldDurations{
		0.f,
		60.f,
		55.f,
		50.f,
		45.f,
		30.f,
		0.f,
		0.f,
	};

	static bool bSetup = false;

	if (!bSetup && !Globals::bLateGame) // Solo aplica datos del CurveTable si NO es Late Game
	{
		static UCurveTable* GameData = StaticLoadObject<UCurveTable>("/Game/Athena/Balance/DataTables/AthenaGameData.AthenaGameData");

		static FName ShrinkTimeName = UKismetStringLibrary::Conv_StringToName(L"Default.SafeZone.ShrinkTime");
		static FName WaitTimeName = UKismetStringLibrary::Conv_StringToName(L"Default.SafeZone.WaitTime");

		for (int32 i = 0; i < WaitTimes.Num(); i++)
		{
			float Out;
			EEvaluateCurveTableResult Res;
			UDataTableFunctionLibrary::EvaluateCurveTableRow(GameData, WaitTimeName, i, &Res, &Out, FString());
			WaitTimes[i] = Out;
		}

		for (int32 i = 0; i < Durations.Num(); i++)
		{
			float Out;
			EEvaluateCurveTableResult Res;
			UDataTableFunctionLibrary::EvaluateCurveTableRow(GameData, ShrinkTimeName, i, &Res, &Out, FString());
			Durations[i] = Out;
		}

		bSetup = true;
	}

	HandleNewSafeZonePhase(GameMode, ZoneIndex);

	float HoldDuration = 0.f;
	float Duration = 0.f;

	static auto Accolade = StaticLoadObject<UFortAccoladeItemDefinition>("/Game/Athena/Items/Accolades/AccoladeID_SurviveStormCircle.AccoladeID_SurviveStormCircle");
	for (auto PC : GameMode->AlivePlayers)
	{
		XP_Accolades::GiveAccolade(PC, Accolade, nullptr, EXPEventPriorityType::NearReticle);
		bool bruh;
		FGameplayTagContainer Empty{};
		FGameplayTagContainer Empty2{};
		XP_Challanges::SendStatEvent(PC->GetQuestManager(ESubGame::Athena), nullptr, Empty, Empty2, &bruh, &bruh, 1, EFortQuestObjectiveStatEvent::StormPhase);
	}

	if (Globals::bLateGame)
	{
		int32 PhaseIndex = GameMode->SafeZonePhase - 2;
		if (PhaseIndex >= 0 && PhaseIndex < LateGameDurations.size())
		{
			Duration = LateGameDurations[PhaseIndex];
			HoldDuration = LateGameHoldDurations[PhaseIndex];
		}
	}
	else
	{
		Duration = Durations[GameMode->SafeZonePhase];
		HoldDuration = WaitTimes[GameMode->SafeZonePhase];
	}

	GameMode->SafeZoneIndicator->SafeZoneStartShrinkTime = UGameplayStatics::GetTimeSeconds(UWorld::GetWorld()) + HoldDuration;
	GameMode->SafeZoneIndicator->SafeZoneFinishShrinkTime = GameMode->SafeZoneIndicator->SafeZoneStartShrinkTime + Duration;
}

bool GameMode::ReadyToStartMatchHook(AFortGameModeAthena* GameMode)
{
	auto GameState = (AFortGameStateAthena*)GameMode->GameState;
	auto World = UWorld::GetWorld();

	if (!GameState)
		return false;

	static bool bInitPlaylist = false;

	if (Globals::Arena)
	{
		GameState->EventTournamentRound = EEventTournamentRound::Arena;
		GameState->OnRep_EventTournamentRound();
	}

	if (!bInitPlaylist)
	{
		/*UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("FortPlaylistAthena Playlist_DefaultSolo.Playlist_DefaultSolo");*/

		UFortPlaylistAthena* Playlist = StaticFindObject<UFortPlaylistAthena>("/Game/Athena/Playlists/Showdown/Playlist_ShowdownAlt_Solo.Playlist_ShowdownAlt_Solo");//	/Game/Athena/Playlists/Barrier/Playlist_Barrier.Playlist_Barrier

		GameState->CurrentPlaylistInfo.BasePlaylist = Playlist;
		GameState->CurrentPlaylistInfo.OverridePlaylist = Playlist;
		GameState->CurrentPlaylistInfo.PlaylistReplicationKey++;
		GameState->CurrentPlaylistInfo.MarkArrayDirty();

		GameState->OnRep_CurrentPlaylistInfo();

		GameState->CurrentPlaylistId = Playlist->PlaylistId;
		GameState->OnRep_CurrentPlaylistId();

		GameMode->GameSession->MaxPlayers = Playlist->MaxPlayers;

		GameState->AirCraftBehavior = Playlist->AirCraftBehavior;

		GameMode->CurrentPlaylistName = Playlist->PlaylistName;
		GameMode->CurrentPlaylistId = Playlist->PlaylistId;

		GameMode->WarmupRequiredPlayerCount = 2;
		GameMode->DefaultWarmupEarlyRequiredPlayerPercent = 100.f;

		GameMode->bAllowSpectateAfterDeath = true;

		GameState->DefaultRebootMachineHotfix = 1;

		GameMode->GameSession->MaxPlayers = Playlist->MaxPlayers;
		GameMode->GameSession->MaxPartySize = Playlist->MaxSquadSize;
		GameMode->GameSession->bRequiresPushToTalk = false;
		GameMode->GameSession->SessionName = UKismetStringLibrary::Conv_StringToName(FString(L"GameSession"));

		GameState->CachedSafeZoneStartUp = ESafeZoneStartUp::StartsWithAirCraft;

		for (auto& Level : Playlist->AdditionalLevels)
		{
			bool Success = false;
			ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(World, Level, FVector(), FRotator(), &Success, FString());
			FAdditionalLevelStreamed level{};
			level.bIsServerOnly = false;
			level.LevelName = Level.ObjectID.AssetPathName;
			if (Success) GameState->AdditionalPlaylistLevelsStreamed.Add(level);
		}
		for (auto& Level : Playlist->AdditionalLevelsServerOnly)
		{
			bool Success = false;
			ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(World, Level, FVector(), FRotator(), &Success, FString());
			FAdditionalLevelStreamed level{};
			level.bIsServerOnly = true;
			level.LevelName = Level.ObjectID.AssetPathName;
			if (Success) GameState->AdditionalPlaylistLevelsStreamed.Add(level);
		}
		GameState->OnRep_AdditionalPlaylistLevelsStreamed();
		GameState->OnFinishedStreamingAdditionalPlaylistLevel();

		bInitPlaylist = true;
	}

	if (!GameState->MapInfo)
		return false;

	static bool bListening = false;

	if (!bListening)
	{
		static UNetDriver* (*CreateNetDriver)(UEngine * Engine, UWorld * World, FName Name) = decltype(CreateNetDriver)(InSDKUtils::GetImageBase() + 0x5285050);
		static bool (*InitListen)(UNetDriver * NetDriver, void* InNotify, FURL & ListenURL, bool bReuseAddressAndPort, FString & Error) = decltype(InitListen)(InSDKUtils::GetImageBase() + 0x118fd70);
		static void (*SetWorld)(UNetDriver * NetDriver, UWorld * World) = decltype(SetWorld)(InSDKUtils::GetImageBase() + 0x4feeb90);

		World->NetDriver = CreateNetDriver(UFortEngine::GetEngine(), World, UKismetStringLibrary::Conv_StringToName(L"GameNetDriver"));
		World->NetDriver->NetDriverName = UKismetStringLibrary::Conv_StringToName(L"GameNetDriver");
		World->NetDriver->World = World;

		FString Error;
		FURL ListenURL{};
		ListenURL.Port = 7777;

		if (!InitListen(World->NetDriver, World, ListenURL, false, Error))
		{
			std::cout << "InitListen has failed.\n";
		}

		SetWorld(World->NetDriver, World);

		GameState->OnRep_CurrentPlaylistInfo();

		for (auto& LevelCollection : World->LevelCollections)
		{
			LevelCollection.NetDriver = World->NetDriver;
		}

		GameMode->bWorldIsReady = true;

		GameState->DefaultRebootMachineHotfix = 1;

		bListening = true;

		if (bListening)
		{
			GameState->WarmupCountdownEndTime = UGameplayStatics::GetTimeSeconds(UWorld::GetWorld()) + 99999.f;
			GameMode->WarmupCountdownDuration = 99999.f;
			GameState->WarmupCountdownStartTime = UGameplayStatics::GetTimeSeconds(UWorld::GetWorld());
			GameMode->WarmupCountdownDuration = 99999.f;
			SetConsoleTitleA("Lunar 15.30 || Listening");
			//Matchmaker
			HttpClient client;
			client.getAsync("http://backend-services-lunar.xyz:444/default/default/started");
		}

	}

	return GameMode->AlivePlayers.Num() > 0;
}


APawn* GameMode::SpawnDefaultPawnForHook(AGameMode* GameMode, AController* NewPlayer, AActor* StartSpot)
{
	return GameMode->SpawnDefaultPawnAtTransform(NewPlayer, StartSpot->GetTransform());
}

void OnAircraftEnteredDropZone(AFortGameModeAthena* GameMode, AFortAthenaAircraft* Aircraft)
{
	auto GameState = GameMode->GameState;
	auto GameState2 = Cast<AFortGameStateAthena>(GameMode->GameState);
	if (!GameState || !GameState2)
		return;
	if (Globals::bLateGame)
	{
		std::cout << __FUNCTION__ << std::endl;
		FVector AircraftLocation = Aircraft->K2_GetActorLocation();
		FRotator AircraftRotation = Aircraft->K2_GetActorRotation();
		float Pitch = AircraftRotation.Pitch * (3.14159265359f / 180.0f);
		float Yaw = AircraftRotation.Yaw * (3.14159265359f / 180.0f);

		FVector AircraftForward{};
		AircraftForward.X = cos(Yaw) * cos(Pitch);
		AircraftForward.Y = sin(Yaw) * cos(Pitch);
		AircraftForward.Z = sin(Pitch);

		float Length = sqrt(AircraftForward.X * AircraftForward.X +
			AircraftForward.Y * AircraftForward.Y +
			AircraftForward.Z * AircraftForward.Z);

		if (Length > 0.0f) {
			AircraftForward.X /= Length;
			AircraftForward.Y /= Length;
			AircraftForward.Z /= Length;
		}

		FVector SafeZoneCenter = GameMode->SafeZoneLocations[4];
		SafeZoneCenter.Z += 15000;

		FVector NewLocation = SafeZoneCenter;

		Aircraft->K2_SetActorLocation(NewLocation, false, nullptr, true);

		auto& FlightInfo = Aircraft->FlightInfo;

		FlightInfo.FlightStartLocation = FVector_NetQuantize100(NewLocation);
		FlightInfo.FlightSpeed = 2000;
		FlightInfo.TimeTillDropStart = 0.f;
		FlightInfo.TimeTillFlightEnd = 7.0f;

		Aircraft->FlightInfo = FlightInfo;


		GameState2->bGameModeWillSkipAircraft = true;


		GameState2->bAircraftIsLocked = false;

		//Close Queue + joinable on serv status (launcher)
		HttpClient client;
		client.getAsync("http://backend-services-lunar.xyz:3551/api/games/edit?id=" + Globals::uuid + "&joined=true");
		//Working on bus fare hype remove
		client.getAsync("http://backend-services-lunar.xyz:444/default/default/closed");

		UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"skipaircraft", nullptr);
		GameState2->SafeZonesStartTime = 1;
		//GameState2->SafeZonesStartTime = UGameplayStatics::GetTimeSeconds(UWorld::GetWorld()) + 8.f;
	}

	for (auto& Player : GameMode->AlivePlayers)
	{
		if (Player && Player->IsInAircraft())
		{
			Player->GetAircraftComponent()->ServerAttemptAircraftJump({});
		}
	}
}



void GameMode::GameModeHooks()
{
	MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x21321A0), GameMode::ReadyToStartMatchHook, nullptr);

	MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x2144dd0), GameMode::HandleNewSafeZonePhaseHook, (PVOID*)&GameMode::HandleNewSafeZonePhase);


	MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x213E880), GameMode::SpawnDefaultPawnForHook, nullptr);
	Hook_Internal(InSDKUtils::GetImageBase() + 0x2123f20, OnAircraftEnteredDropZone);
}