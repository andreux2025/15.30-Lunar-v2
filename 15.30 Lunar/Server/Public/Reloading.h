
#pragma once
#include "../../Framework.h"


class Weapon
{
public:
    inline static __int64 (*OnReloadOG)(AFortWeapon* Weapon, int RemoveCount);

    static __int64 OnReload(AFortWeapon* Weapon, int RemoveCount);

    static void WeaponHook();
};
