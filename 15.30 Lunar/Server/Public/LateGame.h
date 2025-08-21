#pragma once
#include <vector>
#include <string>
#include <string>        
#include <vector>        
#include <random>        
#include "FortInventory.h"
std::vector<std::string> Assault_rifle = {
    "/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03", // scar legendary
    "/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03",// scar epic
    "/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_UR_Ore_T03.WID_Assault_SemiAuto_Athena_UR_Ore_T03",
    "/Game/Athena/Items/Weapons/Boss/WID_Boss_Midas.WID_Boss_Midas",// Jules gun
};

std::vector<std::string> Shotgun = {
    "/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03", // Legendary Pump
    "/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03", // Epic Pump
    "/Game/Athena/Items/Weapons/WID_Shotgun_Swing_Athena_VR.WID_Shotgun_Swing_Athena_VR", // Lever Action Shotgun Epic
    "/Game/Athena/Items/Weapons/WID_Shotgun_Swing_Athena_SR.WID_Shotgun_Swing_Athena_SR", // Lever Action Shotgun Legendary
 //   "/Game/Athena/Items/Weapons/WID_Shotgun_Charge_Athena_SR_Ore_T03.WID_Shotgun_Charge_Athena_SR_Ore_T03", // Charge Shotgun Epic
  //  "/Game/Athena/Items/Weapons/WID_Shotgun_Charge_Athena_VR_Ore_T03.WID_Shotgun_Charge_Athena_VR_Ore_T03",  // Charge Shotgun Legendary
};

std::vector<std::string> Snipers = {
    "/Game/Athena/Items/Weapons/LTM/WID_Sniper_NoScope_Athena_SR_Ore_T03.WID_Sniper_NoScope_Athena_SR_Ore_T03", //Hunting rifle Legendary
    "/Game/Athena/Items/Weapons/LTM/WID_Sniper_NoScope_Athena_VR_Ore_T03.WID_Sniper_NoScope_Athena_VR_Ore_T03",//Hunting rifle Epic
    "/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_SR_Ore_T03.WID_Sniper_Heavy_Athena_SR_Ore_T03", //heavy legendary
    "/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03",//Bolt Action Sniper Rifle
};


std::vector<std::string> Consumables = {
    "/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields",
    "/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall",
    "/Game/Athena/Items/Consumables/Flopper/Effective/WID_Athena_Flopper_Effective.WID_Athena_Flopper_Effective",
};

std::vector<std::string> Traps = {
    "/Game/Athena/Items/Traps/TID_Context_BouncePad_Athena.TID_Context_BouncePad_Athena",
  //  "/Game/Athena/Items/Traps/TID_ContextTrap_Athena.TID_ContextTrap_Athena",
    "/Game/Items/Traps/Floor/TID_Floor_Player_Launch_Pad_Athena.TID_Floor_Player_Launch_Pad_Athena",
};


inline int GetClipSize(UFortItemDefinition* ItemDef) {
    if (auto RangedDef = Cast<UFortWeaponRangedItemDefinition>(ItemDef)) {
        auto DataTable = RangedDef->WeaponStatHandle.DataTable;
        auto RowName = RangedDef->WeaponStatHandle.RowName;

        if (DataTable && RowName.ComparisonIndex) {
            auto& RowMap = *(TMap<FName, FFortRangedWeaponStats*>*)(__int64(DataTable) + 0x30);

            for (auto& Pair : RowMap) {
                FName CurrentRowName = Pair.Key();
                FFortRangedWeaponStats* PackageData = Pair.Value();

                if (CurrentRowName == RowName && PackageData) {
                    return PackageData->ClipSize;
                }
            }
        }
    }

    return 0;
}

inline std::string GetRandomWeapon(const std::vector<std::string>& list) {
    if (list.empty()) return "";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, list.size() - 1);
    return list[dist(gen)];
}

inline UFortItemDefinition* LoadWeapon(const std::vector<std::string>& Pool)// fixes issue where deff is somehow null
{
    for (int i = 0; i < 6; ++i)
    {
        std::string WeaponPath = GetRandomWeapon(Pool);
        UFortItemDefinition* Def = StaticLoadObject<UFortItemDefinition>(WeaponPath);
        if (Def)
            return Def;
    }
    return nullptr;
}

inline void __fastcall GiveLoadout(AFortPlayerController* PC)
{
    if (UFortItemDefinition* AssaultRifleDef = LoadWeapon(Assault_rifle))
    {
        FortInventory::GiveItem(PC, AssaultRifleDef, 1, GetClipSize(AssaultRifleDef));

        if (auto* RangedDef = (UFortWeaponRangedItemDefinition*)AssaultRifleDef)
        {
            UFortWorldItemDefinition* AmmoDef = RangedDef->GetAmmoWorldItemDefinition_BP();
            if (AmmoDef)
                FortInventory::GiveItem(PC, AmmoDef, 200, 0);
        }
    }

    if (UFortItemDefinition* ShotGunDef = LoadWeapon(Shotgun))
    {
        FortInventory::GiveItem(PC, ShotGunDef, 1, GetClipSize(ShotGunDef));

        if (auto* RangedDef = (UFortWeaponRangedItemDefinition*)ShotGunDef)
        {
            UFortWorldItemDefinition* AmmoDef = RangedDef->GetAmmoWorldItemDefinition_BP();
            if (AmmoDef)
                FortInventory::GiveItem(PC, AmmoDef, 120, 0);
        }
    }

    if (UFortItemDefinition* RandomDef = LoadWeapon(Snipers))
    {
        FortInventory::GiveItem(PC, RandomDef, 1, GetClipSize(RandomDef));

        if (auto* RangedDef = (UFortWeaponRangedItemDefinition*)RandomDef)
        {
            UFortWorldItemDefinition* AmmoDef = RangedDef->GetAmmoWorldItemDefinition_BP();
            if (AmmoDef)
                FortInventory::GiveItem(PC, AmmoDef, 120, 0);
        }
    }

    if (auto Consumable1Def = LoadWeapon(Consumables))
        FortInventory::GiveItemStack(PC, Consumable1Def, 3, 0);

    if (auto Consumable2Def = LoadWeapon(Consumables))
        FortInventory::GiveItemStack(PC, Consumable2Def, 3, 0);

    if (auto TrapDef = LoadWeapon(Traps))
        FortInventory::GiveItem(PC, TrapDef, 3, 0);

    static UFortItemDefinition* WoodDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");
    FortInventory::GiveItem(PC, WoodDef, 500, 0);

    static UFortItemDefinition* StoneDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
    FortInventory::GiveItem(PC, StoneDef, 500, 0);

    static UFortItemDefinition* MetalDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");
    FortInventory::GiveItem(PC, MetalDef, 500, 0);

}