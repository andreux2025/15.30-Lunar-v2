// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "Server/Public/GameMode.h"
#include "Server/Public/NetDriver.h"
#include "Server/Public/Player.h"
#include "Server/Public/Abilities.h"
#include "Server/Public/Looting.h"
#include "Server/Public/xp.h"
#include "Server/Public/Reloading.h"
DWORD WINAPI Main(LPVOID) {
    AllocConsole();
    FILE* File = nullptr;
    freopen_s(&File, "CONOUT$", "w+", stdout);
    Globals::uuid = UUIDGenerator::generate();
    MH_Initialize();
    SetConsoleTitleA("Lunar 15.30 | Waiting");
    HttpClient client;
    client.getAsync("http://backend-services-lunar.xyz:3551/api/games/create?id=" + Globals::uuid + "&game=LATEGAME ARENA&player=0&joined=false&region=EU-SERVER");
    Sleep(5);

    *(bool*)(InSDKUtils::GetImageBase() + 0x96BC1CC) = false;
    *(bool*)(InSDKUtils::GetImageBase() + 0x96BC1CD) = true;

    MiscChecks();
    NetDriver::NetDriverHooks();
    GameMode::GameModeHooks();
    Player::PlayerHooks();
    Abilities::Hooking();
    Looting::Hook();
    Weapon::WeaponHook();
    //XP_Challanges::InitXPHooks();

   Globals::SpectatingName = UKismetStringLibrary::Conv_StringToName(TEXT("Spectating"));
    UKismetSystemLibrary::GetDefaultObj()->ExecuteConsoleCommand(UWorld::GetWorld(), L"open Apollo_Terrain", nullptr);
    UWorld::GetWorld()->OwningGameInstance->LocalPlayers.Remove(0);

    MH_EnableHook(MH_ALL_HOOKS);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, const DWORD ulReasonForCall
)
{
    switch (ulReasonForCall)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, Main, nullptr, 0, nullptr);
        break;
    default:
        break;
    }
    return TRUE;
}