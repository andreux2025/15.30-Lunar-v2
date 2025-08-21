#pragma once
#include <vector>
#include <string>
#include <string>        
#include <vector>        
#include <random>        
#include "FortInventory.h"
std::vector<std::string> Assault_rifle = {
    "/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03" // scar legendary
    "/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"// scar epic
};

std::vector<std::string> Shotgun = {
    "/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03", // ok Legendary Pump
    "/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03", // Epic Pump
    "/Game/Athena/Items/Weapons/WID_Shotgun_Swing_Athena_VR.WID_Shotgun_Swing_Athena_VR" // Lever Action Shotgun Epic
    "/Game/Athena/Items/Weapons/WID_Shotgun_Swing_Athena_SR.WID_Shotgun_Swing_Athena_SR" // Lever Action Shotgun Legendarty
    "/Game/Athena/Items/Weapons/WID_Shotgun_Charge_Athena_SR_Ore_T03.WID_Shotgun_Charge_Athena_SR_Ore_T03" // Charge shotgun epic
    "/Game/Athena/Items/Weapons/WID_Shotgun_Charge_Athena_VR_Ore_T03.WID_Shotgun_Charge_Athena_VR_Ore_T03" // Charge shotgun legendary
};

//std::vector<std::string> Mixed = {
//    "/Game/Athena/Items/Weapons/WID_Sniper_Auto_Suppressed_Scope_Athena_VR.WID_Sniper_Auto_Suppressed_Scope_Athena_VR",
//    "/Game/Athena/Items/Weapons/WID_Pistol_Donut.WID_Pistol_Donut",
//    "/Game/Athena/Items/Weapons/Boss/WID_Boss_Adventure_GH.WID_Boss_Adventure_GH",
//    "/Game/Athena/Items/Weapons/WID_Pistol_BurstFireSMG_Athena_R_Ore_T03.WID_Pistol_BurstFireSMG_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/Seasonal/WID_Sniper_Valentine_Athena_VR_Ore_T03.WID_Sniper_Valentine_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_DualPistol_Suppresseed_Athena_VR_T01.WID_DualPistol_Suppresseed_Athena_VR_T01",
//    "/Game/Athena/Items/Weapons/WID_DualPistol_Suppresseed_Athena_R_T01.WID_DualPistol_Suppresseed_Athena_R_T01",
//    "/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavy_Athena_R_Ore_T03.WID_Pistol_AutoHeavy_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_VR_Ore_T03.WID_Launcher_Rocket_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_SR_Ore_T03.WID_Launcher_Rocket_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_R_Ore_T03.WID_Launcher_Rocket_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/Seasonal/WID_Launcher_Pumpkin_Athena_VR_Ore_T03.WID_Launcher_Pumpkin_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/Seasonal/WID_Launcher_Pumpkin_Athena_SR_Ore_T03.WID_Launcher_Pumpkin_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/Seasonal/WID_Launcher_Pumpkin_Athena_R_Ore_T03.WID_Launcher_Pumpkin_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_VR_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_SR_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_Flintlock_Athena_C.WID_Pistol_Flintlock_Athena_C",
//    "/Game/Athena/Items/Weapons/WID_DualPistol_SemiAuto_Athena_VR_Ore_T03.WID_DualPistol_SemiAuto_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Assault_LMG_Athena_VR_Ore_T03.WID_Assault_LMG_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Assault_LMG_Athena_SR_Ore_T03.WID_Assault_LMG_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Assault_LMG_Athena_R_Ore_T03.WID_Assault_LMG_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_VR_Ore_T03.WID_Pistol_Scavenger_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_R_Ore_T03.WID_Pistol_Scavenger_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_SR_Ore_T03.WID_Pistol_Scavenger_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_VR_Ore_T03.WID_Sniper_Standard_Scope_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_VeryRare_Ore_T03.WID_Sniper_Standard_Scope_Athena_VeryRare_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_SuperRare_Ore_T03.WID_Sniper_Standard_Scope_Athena_SuperRare_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_SR_Ore_T03.WID_Sniper_Standard_Scope_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_VR_Ore_T03.WID_Pistol_SemiAuto_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_Suppressed_Scope_Athena_VR_Ore_T03.WID_Sniper_Suppressed_Scope_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_Suppressed_Scope_Athena_SR_Ore_T03.WID_Sniper_Suppressed_Scope_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_Suppressed_Scope_Athena_R_Ore_T03.WID_Sniper_Suppressed_Scope_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_SR_Ore_T03.WID_Pistol_SemiAuto_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Assault_AutoDrum_Athena_R_Ore_T03.WID_Assault_AutoDrum_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Suppressed_Athena_SR_Ore_T03.WID_Pistol_SemiAuto_Suppressed_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Launcher_Military_Athena_VR_Ore_T03.WID_Launcher_Military_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Launcher_Military_Athena_SR_Ore_T03.WID_Launcher_Military_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Athena_SMG_VR.WID_Athena_SMG_VR",
//    "/Game/Athena/Items/Weapons/WID_Athena_SMG_SR.WID_Athena_SMG_SR",
//    "/Game/Athena/Items/Gameplay/Boss/AGID_Boss_Tina.AGID_Boss_Tina",
//    "/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_VR_Ore_T03.WID_Launcher_Grenade_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_Revolver_SingleAction_Athena_VR.WID_Pistol_Revolver_SingleAction_Athena_VR",
//    "/Game/Athena/Items/Weapons/WID_Pistol_Revolver_SingleAction_Athena_R.WID_Pistol_Revolver_SingleAction_Athena_R",
//    "/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_SR_Ore_T03.WID_Launcher_Grenade_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_R_Ore_T03.WID_Launcher_Grenade_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_DualPistol_SemiAuto_Athena_SR_Ore_T03.WID_DualPistol_SemiAuto_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_C_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_C_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_Auto_Suppressed_Scope_Athena_SR.WID_Sniper_Auto_Suppressed_Scope_Athena_SR",
//    "/Game/Athena/Items/Weapons/Boss/WID_Boss_Hos_MG.WID_Boss_Hos_MG",
//    "/Game/Athena/Items/Weapons/WID_Sniper_Auto_Suppressed_Scope_Athena_R.WID_Sniper_Auto_Suppressed_Scope_Athena_R",
//    "/Game/Athena/Items/Weapons/LTM/WID_Hook_Gun_Slide.WID_Hook_Gun_Slide",
//    "/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_VR_Ore_T03.WID_Sniper_Heavy_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_SR_Ore_T03.WID_Sniper_Heavy_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_R_Ore_T03.WID_Sniper_Heavy_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_HandCannon_Athena_VR_Ore_T03.WID_Pistol_HandCannon_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_HandCannon_Athena_SR_Ore_T03.WID_Pistol_HandCannon_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_GrenadeLauncher_Prox_Athena_VR.WID_GrenadeLauncher_Prox_Athena_VR",
//    "/Game/Athena/Items/Weapons/WID_Pistol_Scoped_Athena_VR_Ore_T03.WID_Pistol_Scoped_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_Scoped_Athena_SR_Ore_T03.WID_Pistol_Scoped_Athena_SR_Ore_T03"
//    "/Game/Athena/Items/Weapons/WID_GrenadeLauncher_Prox_Athena_SR.WID_GrenadeLauncher_Prox_Athena_SR",
//    "/Game/Athena/Items/Weapons/WID_Sniper_NoScope_Athena_R_Ore_T03.WID_Sniper_NoScope_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/Boss/WID_Boss_Midas.WID_Boss_Midas",
//    "/Game/Athena/Items/Weapons/WID_Pistol_Standard_Athena_VR.WID_Pistol_Standard_Athena_VR",
//    "/Game/Athena/Items/Weapons/WID_Pistol_Standard_Athena_SR.WID_Pistol_Standard_Athena_SR",
//    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_R_Ore_T03.WID_Pistol_SemiAuto_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_RapidFireSMG_Athena_R_Ore_T03.WID_Pistol_RapidFireSMG_Athena_R_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_RapidFireSMG_Athena_VR_Ore_T03.WID_Pistol_RapidFireSMG_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_RapidFireSMG_Athena_SR_Ore_T03.WID_Pistol_RapidFireSMG_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_VR_Ore_T03.WID_Pistol_SixShooter_Athena_VR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_Sixshooter_Athena_SR_Ore_T03.WID_Pistol_SixShooter_Athena_SR_Ore_T03",
//    "/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_R_Ore_T03.WID_Pistol_SixShooter_Athena_R_Ore_T03",
//};

std::vector<std::string> Consumables = {
    "/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields",
    "/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall",
    "/Game/Athena/Items/Consumables/AppleSun/WID_Athena_AppleSun.WID_Athena_AppleSun",
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
    for (int i = 0; i < 2; ++i)
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

    /*if (UFortItemDefinition* RandomDef = LoadWeapon(Mixed))
    {
        FortInventory::GiveItem(PC, RandomDef, 1, GetClipSize(RandomDef));

        if (auto* RangedDef = (UFortWeaponRangedItemDefinition*)RandomDef)
        {
            UFortWorldItemDefinition* AmmoDef = RangedDef->GetAmmoWorldItemDefinition_BP();
            if (AmmoDef)
                FortInventory::GiveItem(PC, AmmoDef, 120, 0);
        }
    }*/

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