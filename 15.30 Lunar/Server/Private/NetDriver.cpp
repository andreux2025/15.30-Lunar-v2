#include "../public/NetDriver.h"


void NetDriver::TickFlushHook(UNetDriver* NetDriver)
{
    static bool bBusStarted = false;
    static bool bRestarting = false;
    float TimeSeconds = UGameplayStatics::GetTimeSeconds(UWorld::GetWorld());

    AFortGameStateAthena* GameState = Cast<AFortGameStateAthena>(UWorld::GetWorld()->GameState);
    AFortGameModeAthena* GameMode = Cast<AFortGameModeAthena>(UWorld::GetWorld()->AuthorityGameMode);
    if (!GameState || !GameMode)
        return TickFlush(NetDriver);

    if (NetDriver->ClientConnections.Num() >= 1)
    {
        UReplicationDriver* ReplicationDriver = NetDriver->ReplicationDriver;
        if (!ReplicationDriver)
            return TickFlush(NetDriver);

        ServerReplicateActors(ReplicationDriver);
    }

    if (GameState->GamePhase > EAthenaGamePhase::Aircraft)
    {
        static std::map<int, bool> ProcessedCases;
        int AliveCount = GameMode->AlivePlayers.Num();

        if (ProcessedCases.find(AliveCount) == ProcessedCases.end())
        {
            std::map<int, int> PlacementPoints = {
                {25, 60}, {15, 30}, {5, 30}, {1, 60}
            };

            auto it = PlacementPoints.find(AliveCount);
            if (it != PlacementPoints.end())
            {
                for (auto& Player : GameMode->AlivePlayers)
                {
                    Player->ClientReportTournamentPlacementPointsScored(AliveCount, it->second);
                }
                ProcessedCases[AliveCount] = true;
            }
        }
    }

    //static auto AccoladeId_018_First_Landing = StaticFindObject<UFortAccoladeItemDefinition>(
    //    "/Game/Athena/Items/Accolades/AccoladeId_018_First_Landing.AccoladeId_018_First_Landing");
    //static auto AccoladeId_032_DiscoverLandmark = StaticFindObject<UFortAccoladeItemDefinition>(
    //    "/Game/Athena/Items/Accolades/AccoladeId_032_DiscoverLandmark.AccoladeId_032_DiscoverLandmark");
    //static auto AccoladeId_033_DiscoverPOI = StaticFindObject<UFortAccoladeItemDefinition>(
    //    "/Game/Athena/Items/Accolades/AccoladeId_033_DiscoverPOI.AccoladeId_033_DiscoverPOI");

    //    if (GameMode->MatchState == MatchState::InProgress)
    //    {
    //        static bool bFirstLandingGiven = false;

    //        for (AFortPlayerControllerAthena* Player : GameMode->AlivePlayers)
    //        {
    //            if (!bFirstLandingGiven && !Player->IsInAircraft())
    //            {
    //                //FVector Vel = Player->Pawn->GetVelocity();//Crash here
    //               // if (Vel.Z == 0.f)
    //               // {
    //                    bFirstLandingGiven = true;

    //                    XP_Accolades::GiveAccolade(Player, AccoladeId_018_First_Landing, nullptr, EXPEventPriorityType::NearReticle);
    //                    XP_Accolades::GiveAccolade(Player, AccoladeId_032_DiscoverLandmark, nullptr, EXPEventPriorityType::NearReticle);
    //                    XP_Accolades::GiveAccolade(Player, AccoladeId_033_DiscoverPOI, nullptr, EXPEventPriorityType::NearReticle);
    //                //}
    //            }
    //        }
    //    }


    if (NetDriver && (GameMode->MatchState == MatchState::InProgress))
    {
        if ((GetAsyncKeyState(VK_F5) & 1 || GameState->WarmupCountdownEndTime - TimeSeconds <= 0) && !bBusStarted)
        {
            UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"startaircraft", nullptr);
            bBusStarted = true;
        }
    }
    else if (NetDriver && GameMode->MatchState == MatchState::WaitingPostMatch && !bRestarting)
    {
        bRestarting = true;
    }

    return TickFlush(NetDriver);
}

void NetDriver::NetDriverHooks()
{
    ServerReplicateActors = decltype(ServerReplicateActors)(InSDKUtils::GetImageBase() + 0x176c2e0);

    MH_CreateHook((LPVOID)(InSDKUtils::GetImageBase() + 0x4ff0070), NetDriver::TickFlushHook, (PVOID*)&NetDriver::TickFlush);
}